//
// Created by Martyn Klassen on 2020-05-20.
//

#ifndef ISMRMRD_COMPRESSION_TEMPLATES_H
#define ISMRMRD_COMPRESSION_TEMPLATES_H

#include <cassert>
#include <iostream>
#include <zfp.h>
#include <cstring>

namespace ISMRMRD {

namespace _private {

constexpr size_t zfp_block_size = 4;

size_t integerPower(size_t base, size_t exponent) {
    if (exponent == 0)
        return 1;

    size_t result = integerPower(base, exponent / 2);
    result *= result;

    if (exponent & 1)
        result *= base;

    return result;
}

// Overloaded methods for selecting encoder
ulong (*get_encoder(uint dims, int32 *))(zfp_stream *, const int32 *) {
    auto table = { zfp_encode_block_int32_1,
                   zfp_encode_block_int32_2,
                   zfp_encode_block_int32_3,
                   zfp_encode_block_int32_4 };
    return *(table.begin() + (dims - 1));
}

ulong (*get_encoder(uint dims, int64 *))(zfp_stream *, const int64 *) {
    auto table = { zfp_encode_block_int64_1,
                   zfp_encode_block_int64_2,
                   zfp_encode_block_int64_3,
                   zfp_encode_block_int64_4 };
    return *(table.begin() + (dims - 1));
}

// Overloaded functions for selecting decoded
ulong (*get_decoder(uint dims, int32 *))(zfp_stream *, int32 *) {
    auto table = { zfp_decode_block_int32_1,
                   zfp_decode_block_int32_2,
                   zfp_decode_block_int32_3,
                   zfp_decode_block_int32_4 };
    return *(table.begin() + (dims - 1));
}

ulong (*get_decoder(uint dims, int64 *))(zfp_stream *, int64 *) {
    auto table = { zfp_decode_block_int64_1,
                   zfp_decode_block_int64_2,
                   zfp_decode_block_int64_3,
                   zfp_decode_block_int64_4 };
    return *(table.begin() + (dims - 1));
}

// Templated version of promote and demote

template <typename D, typename S>
typename std::enable_if<std::is_signed<D>::value && std::is_integral<D>::value && std::is_integral<S>::value && (std::numeric_limits<D>::digits > std::numeric_limits<S>::digits), void>::type
promote(D *oblock, const S *iblock, uint dims) {
    // The number of powers of 2 between D and S
    // D digits is 31 or 63, D is signed
    unsigned int shift = std::numeric_limits<D>::digits - std::numeric_limits<S>::digits;

    // Total values within block are 4^dims
    uint count = 1u << (2u * dims);
    if (std::is_signed<S>::value) {
        // S digits is 7, 15, or 31
        // shift is 24, 16, or 32, but needs to be 23, 15, or 31
        shift--;

        while (count--)
            *oblock++ = static_cast<D>(*iblock++) << shift;
    } else {
        // S digits is 8, 16, or 32
        // shift is 23, 15, or 31
        D offset = 1u << static_cast<unsigned int>(std::numeric_limits<S>::digits - 1);
        while (count--)
            *oblock++ = (static_cast<D>(*iblock++) - offset) << shift;
    }
}

template <typename D, typename S>
typename std::enable_if<std::is_signed<S>::value && std::is_integral<D>::value && std::is_integral<S>::value && (std::numeric_limits<D>::digits < std::numeric_limits<S>::digits), void>::type
demote(D *oblock, const S *iblock, uint dims) {
    // The number of powers of 2 between S and D
    // S digits is always 31 or 63, S is signed
    unsigned int shift = std::numeric_limits<S>::digits - std::numeric_limits<D>::digits;

    // Total values within block are 4^dims
    uint count = 1u << (2u * dims);
    if (std::is_signed<D>::value) {
        // D digits is 7, 15, or 31
        // shift is 24, 16, or 32, but needs to be 23, 15, or 31
        shift--;

        S offset = 1lu << static_cast<unsigned int>(std::numeric_limits<D>::digits);
        while (count--) {
            S i = static_cast<S>(*iblock++ >> shift);
            *oblock++ = static_cast<D>(std::max(-offset, std::min(i, offset - 1)));
        }
    } else {
        // S digits is 8, 16, or 32
        // shift is 23, 15, or 31

        S offset = 1lu << static_cast<unsigned int>(std::numeric_limits<D>::digits - 1);
        while (count--) {
            S i = (*iblock++ >> shift) + offset;
            *oblock++ = static_cast<D>(std::max(S(0), std::min(i, offset * 2 - 1)));
        }
    }
}

template <typename T>
static void
pad_block(T *p, size_t n, size_t s) {

    // If no data provided then populate everything with zeros
    if (n < 1) {
        memset(p, 0, zfp_block_size * sizeof(T) * s);
        return;
    }
    
    size_t i = n;
    // Padding up to half of block size, just replicates last provided
    for (; i < (zfp_block_size - zfp_block_size / 2); i++)
        memcpy(p + i * s, p + (n - 1) * s, sizeof(T) * s);

    // Padding after half reflects first half
    for (; i < zfp_block_size; i++)
        memcpy(p + i * s, p + (zfp_block_size - i - 1) * s, sizeof(T) * s);
}

// Templated compression for integers
template <typename S, typename D>
typename std::enable_if<std::is_signed<D>::value && std::is_integral<D>::value && std::is_integral<S>::value && (std::numeric_limits<D>::digits > std::numeric_limits<S>::digits), size_t>::type
compress(zfp_stream *zfp, const zfp_field *field) {
    auto dims = static_cast<size_t>(zfp_field_dimensionality(field));

    auto pData = reinterpret_cast<const S *>(field->data);
    size_t blockSize = integerPower(zfp_block_size, dims);
    S iblock[blockSize];
    D oblock[blockSize];

    size_t l, k, j, i;
    size_t x, y, z, w;
    size_t nx = field->nx ? field->nx : 1;
    size_t ny = field->ny ? field->ny : 1;
    size_t nz = field->nz ? field->nz : 1;
    size_t nw = field->nw ? field->nw : 1;
    ssize_t sx = field->sx ? field->sx : 1;
    ssize_t sy = field->sy ? field->sy : (ssize_t)nx;
    ssize_t sz = field->sz ? field->sz : (ssize_t)(nx * ny);
    ssize_t sw = field->sw ? field->sw : (ssize_t)(nx * ny * nz);

    ulong (*encode)(zfp_stream *, const D *) = get_encoder(dims, oblock);

    for (w = 0; w < nw; w += zfp_block_size) {
        for (z = 0; z < nz; z += zfp_block_size) {
            for (y = 0; y < ny; y += zfp_block_size) {
                for (x = 0; x < nx; x += zfp_block_size) {

                    auto pBlock = pData + x * sx + y * sy + z * sz + w * sw;

                    auto bx = std::min(nx - x, zfp_block_size);
                    auto by = std::min(ny - y, zfp_block_size);
                    auto bz = std::min(nz - z, zfp_block_size);
                    auto bw = std::min(nw - w, zfp_block_size);

                    auto fx = bx;
                    auto fy = dims > 1 ? by : zfp_block_size;
                    auto fz = dims > 2 ? bz : zfp_block_size;
                    auto fw = dims > 3 ? bw : zfp_block_size;

                    // Get the block
                    for (l = 0; l < bw; l++) {
                        for (k = 0; k < bz; k++) {
                            for (j = 0; j < by; j++) {
                                for (i = 0; i < bx; i++) {
                                    iblock[((l * zfp_block_size + k) * zfp_block_size + j) * zfp_block_size + i] =
                                        pBlock[i * sx + j * sy + k * sz + l * sw];
                                }
                                pad_block(iblock + ((l * zfp_block_size + k) * zfp_block_size + j) * zfp_block_size, fx, 1);
                            }
                            pad_block(iblock + (l * zfp_block_size + k) * zfp_block_size * zfp_block_size, fy, zfp_block_size);
                        }
                        pad_block(iblock + l * zfp_block_size * zfp_block_size * zfp_block_size, fz, zfp_block_size * zfp_block_size);
                    }
                    pad_block(iblock, fw, zfp_block_size * zfp_block_size * zfp_block_size);

                    // Promote the block
                    promote(oblock, iblock, dims);

                    // Encode the block
                    encode(zfp, oblock);
                }
            }
        }
    }
    zfp_stream_flush(zfp);
    return zfp_stream_compressed_size(zfp);
}

// Templated decompression for integers
template <typename S, typename D>
typename std::enable_if<std::is_signed<D>::value && std::is_integral<D>::value && std::is_integral<S>::value && (std::numeric_limits<D>::digits > std::numeric_limits<S>::digits), size_t>::type
decompress(zfp_stream *zfp, zfp_field *field) {
    auto dims = static_cast<size_t>(zfp_field_dimensionality(field));

    S *pData = reinterpret_cast<S *>(field->data);
    size_t blockSize = integerPower(zfp_block_size, dims);
    D iblock[blockSize];
    S oblock[blockSize];

    size_t l, k, j, i;
    size_t x, y, z, w;
    size_t nx = field->nx ? field->nx : 1;
    size_t ny = field->ny ? field->ny : 1;
    size_t nz = field->nz ? field->nz : 1;
    size_t nw = field->nw ? field->nw : 1;
    ssize_t sx = field->sx ? field->sx : 1;
    ssize_t sy = field->sy ? field->sy : (ssize_t)(nx);
    ssize_t sz = field->sz ? field->sz : (ssize_t)(nx * ny);
    ssize_t sw = field->sw ? field->sw : (ssize_t)(nx * ny * nz);

    ulong (*decode)(zfp_stream *, D *) = get_decoder(dims, iblock);

    for (w = 0; w < nw; w += 4) {
        for (z = 0; z < nz; z += 4) {
            for (y = 0; y < ny; y += 4) {
                for (x = 0; x < nx; x += 4) {
                    // Decode the block
                    decode(zfp, iblock);

                    // Demote the block
                    demote(oblock, iblock, dims);

                    // Copy to the field
                    for (l = 0; l < std::min(nw - w, zfp_block_size); l++) {
                        for (k = 0; k < std::min(nz - z, zfp_block_size); k++) {
                            for (j = 0; j < std::min(ny - y, zfp_block_size); j++) {
                                for (i = 0; i < std::min(nx - x, zfp_block_size); i++) {
                                    pData[(x + i) * sx + (y + j) * sy + (z + k) * sz + (w + l) * sw] = oblock[i + zfp_block_size * (j + zfp_block_size * (k + zfp_block_size * l))];
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    zfp_stream_align(zfp);
    return zfp_stream_compressed_size(zfp);
}

} // namespace _private
} // namespace ISMRMRD

#endif //ISMRMRD_COMPRESSION_TEMPLATES_H
