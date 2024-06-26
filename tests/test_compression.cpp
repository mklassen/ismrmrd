#include "../libsrc/compression_templates.h"
#include "ismrmrd/serialize.h"
#include <boost/mpl/list.hpp>
#include <boost/test/unit_test.hpp>
#include <random>

BOOST_AUTO_TEST_SUITE(CompressionTest)

BOOST_AUTO_TEST_CASE(test_promote_uint16) {
    constexpr uint dims = 1;
    constexpr uint terms = 1u << (dims * 2u);
    uint16_t iblock1[] = { 1, 65535, 0, 50 };
    uint16_t iblock2[] = { 1, 1, 1, 1 };
    int32 oblock1[terms];
    int32 oblock2[terms];

    ISMRMRD::_private::promote(oblock1, iblock1, terms, 0);
    ISMRMRD::_private::demote(iblock2, oblock1, terms, 0);
    zfp_promote_uint16_to_int32(oblock2, iblock1, dims);
    BOOST_CHECK_EQUAL(memcmp(iblock1, iblock2, sizeof(*iblock1) * terms), 0);
    BOOST_CHECK_EQUAL(memcmp(oblock1, oblock2, sizeof(*oblock1) * terms), 0);

    iblock1[1] = (1u<<15) - 1;
    ISMRMRD::_private::promote(oblock1, iblock1, terms, 1);
    ISMRMRD::_private::demote(iblock2, oblock1, terms, 1);
    BOOST_CHECK_EQUAL(memcmp(iblock1, iblock2, sizeof(*iblock1) * terms), 0);
}

BOOST_AUTO_TEST_CASE(test_promote_int16) {
    constexpr uint dims = 1;
    constexpr uint terms = 1u << (dims * 2u);
    int16_t iblock1[] = { 1, 32767, -1, -32768 };
    int16_t iblock2[] = { 1, 1, 1, 1 };
    int32 oblock1[terms];
    int32 oblock2[terms];

    ISMRMRD::_private::promote(oblock1, iblock1, terms, 0);
    ISMRMRD::_private::demote(iblock2, oblock1, terms, 0);

    BOOST_CHECK_EQUAL(memcmp(iblock1, iblock2, sizeof(*iblock1) * terms), 0);

    zfp_promote_int16_to_int32(oblock2, iblock1, dims);
    BOOST_CHECK_EQUAL(memcmp(oblock1, oblock2, sizeof(*oblock1) * terms), 0);

    iblock1[0] = 0;

    ISMRMRD::_private::promote(oblock1, iblock1, terms, 0);
    ISMRMRD::_private::demote(iblock2, oblock1, terms, 0);

    BOOST_CHECK_EQUAL(memcmp(iblock1, iblock2, sizeof(*iblock1) * terms), 0);

    zfp_promote_int16_to_int32(oblock2, iblock1, dims);
    BOOST_CHECK_EQUAL(memcmp(oblock1, oblock2, sizeof(*oblock1) * terms), 0);

    iblock1[0] = 1;
    iblock1[1] = (1u<<14) - 1;
    iblock1[2] = -1;
    iblock1[3] = -static_cast<int16_t>(1u<<14);

    ISMRMRD::_private::promote(oblock1, iblock1, terms, 1);
    ISMRMRD::_private::demote(iblock2, oblock1, terms, 1);

    BOOST_CHECK_EQUAL(memcmp(iblock1, iblock2, sizeof(*iblock1) * terms), 0);
}

BOOST_AUTO_TEST_CASE(test_promote_uint8) {
    constexpr uint dims = 1;
    constexpr uint terms = 1u << (dims * 2u);
    uint8_t iblock1[] = { 1, 255, 0, 50 };
    uint8_t iblock2[] = { 1, 1, 1, 1 };
    int32 oblock1[terms];
    int32 oblock2[terms];

    ISMRMRD::_private::promote(oblock1, iblock1, terms, 0);
    ISMRMRD::_private::demote(iblock2, oblock1, terms, 0);

    BOOST_CHECK_EQUAL(memcmp(iblock1, iblock2, sizeof(*iblock1) * terms), 0);

    zfp_promote_uint8_to_int32(oblock2, iblock1, dims);
    BOOST_CHECK_EQUAL(memcmp(oblock1, oblock2, sizeof(*oblock1) * terms), 0);
}

BOOST_AUTO_TEST_CASE(test_promote_int8) {
    constexpr uint dims = 1;
    constexpr uint terms = 1u << (dims * 2u);
    int8_t iblock1[] = { 1, 127, -1, -128 };
    int8_t iblock2[] = { 1, 1, 1, 1 };
    int32 oblock1[terms];
    int32 oblock2[terms];

    ISMRMRD::_private::promote(oblock1, iblock1, terms, 0);
    ISMRMRD::_private::demote(iblock2, oblock1, terms, 0);

    BOOST_CHECK_EQUAL(memcmp(iblock1, iblock2, sizeof(*iblock1) * terms), 0);

    zfp_promote_int8_to_int32(oblock2, iblock1, dims);
    BOOST_CHECK_EQUAL(memcmp(oblock1, oblock2, sizeof(*oblock1) * terms), 0);

    iblock1[0] = 0;

    ISMRMRD::_private::promote(oblock1, iblock1, terms, 0);
    ISMRMRD::_private::demote(iblock2, oblock1, terms, 0);

    BOOST_CHECK_EQUAL(memcmp(iblock1, iblock2, sizeof(*iblock1) * terms), 0);

    zfp_promote_int8_to_int32(oblock2, iblock1, dims);
    BOOST_CHECK_EQUAL(memcmp(oblock1, oblock2, sizeof(*oblock1) * terms), 0);
}

BOOST_AUTO_TEST_CASE(test_promote_uint32_int32) {
    constexpr uint dims = 1;
    constexpr uint terms = 1u << (dims * 2u);
    uint32_t iblock1[] = { 1, (1u << 31) - 1, 0, 50 };
    uint32_t iblock2[] = { 1, 1, 1, 1 };
    int32 oblock1[terms];

    ISMRMRD::_private::promote<int32, uint32>(oblock1, iblock1, terms, 0l);
    ISMRMRD::_private::demote<uint32, int32>(iblock2, oblock1, terms, 0l);

    BOOST_CHECK_EQUAL(memcmp(iblock1, iblock2, sizeof(*iblock1) * terms), 0);

    iblock1[1] = (1u<<30) - 1;
    ISMRMRD::_private::promote<int32, uint32>(oblock1, iblock1, terms, 1l);
    ISMRMRD::_private::demote<uint32, int32>(iblock2, oblock1, terms, 1l);

    BOOST_CHECK_EQUAL(memcmp(iblock1, iblock2, sizeof(*iblock1) * terms), 0);
}

BOOST_AUTO_TEST_CASE(test_promote_uint32_int64) {
    constexpr uint dims = 1;
    constexpr uint terms = 1u << (dims * 2u);
    uint32_t iblock1[] = { 1, 4294967295, 0, 50 };
    uint32_t iblock2[] = { 1, 1, 1, 1 };
    int64 oblock1[terms];

    ISMRMRD::_private::promote(oblock1, iblock1, terms, int64(0));
    ISMRMRD::_private::demote(iblock2, oblock1, terms, int64(0));

    BOOST_CHECK_EQUAL(memcmp(iblock1, iblock2, sizeof(*iblock1) * terms), 0);
}

BOOST_AUTO_TEST_CASE(test_promote_int32) {
    constexpr uint dims = 1;
    constexpr uint terms = 1u << (dims * 2u);
    int32_t iblock1[] = { 1, 2147483647, -1, -2147483648 };
    int32_t iblock2[] = { 1, 1, 1, 1 };
    int64 oblock1[terms];

    ISMRMRD::_private::promote(oblock1, iblock1, terms, int64(0));
    ISMRMRD::_private::demote(iblock2, oblock1, terms, int64(0));

    BOOST_CHECK_EQUAL(memcmp(iblock1, iblock2, sizeof(*iblock1) * terms), 0);

    iblock1[0] = 0;

    ISMRMRD::_private::promote(oblock1, iblock1, terms, int64(0));
    ISMRMRD::_private::demote(iblock2, oblock1, terms, int64(0));

    BOOST_CHECK_EQUAL(memcmp(iblock1, iblock2, sizeof(*iblock1) * terms), 0);
}

template <typename T, typename D>
void run_compression(uint nx, uint ny, uint nz, uint nw, zfp_type type, int32 shift) {
    auto field = std::unique_ptr<zfp_field, decltype(&zfp_field_free)>(zfp_field_alloc(), &zfp_field_free);
    auto zfp = std::unique_ptr<zfp_stream, decltype(&zfp_stream_close)>(zfp_stream_open(nullptr), &zfp_stream_close);

    field->nx = nx;
    field->ny = ny;
    field->nz = nz;
    field->nw = nw;
    uint terms = nx * (ny ? ny : 1) * (nz ? nz : 1) * (nw ? nw : 1);

    std::vector<T>data1(terms);
    std::vector<T>data2(terms);

    std::vector<uint8_t> buffer;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<T> distribution(std::numeric_limits<T>::min() >> shift, std::numeric_limits<T>::max() >> shift);
    for (size_t i = 0; i < terms; i++)
        data1[i] = distribution(gen);

    zfp_field_set_type(field.get(), type);
    zfp_field_set_pointer(field.get(), data1.data());

    buffer.resize(zfp_stream_maximum_size(zfp.get(), field.get()));

    auto stream = std::unique_ptr<bitstream, decltype(&stream_close)>(stream_open(buffer.data(), buffer.size()), &stream_close);

    if (nullptr == stream)
        BOOST_FAIL("Unable to allocate bitstream");

    zfp_stream_set_reversible(zfp.get());

    zfp_stream_set_bit_stream(zfp.get(), stream.get());
    zfp_stream_rewind(zfp.get());

    size_t zfpsize1 = ISMRMRD::_private::compress<T, D>(zfp.get(), field.get(), shift);

    BOOST_REQUIRE_NE(zfpsize1, 0);

    zfp_stream_rewind(zfp.get());
    zfp_field_set_pointer(field.get(), data2.data());

    size_t zfpsize2 = ISMRMRD::_private::decompress<T, D>(zfp.get(), field.get(), shift);

    BOOST_REQUIRE_EQUAL(zfpsize1, zfpsize2);

    BOOST_CHECK_EQUAL(memcmp(data1.data(), data2.data(), sizeof(*(data1.data())) * data1.size()), 0);
}

typedef boost::mpl::list<uint16_t, int16_t> test_types;

BOOST_AUTO_TEST_CASE_TEMPLATE(test_compress, T, test_types) {
    run_compression<T, int32>(4, 6, 4, 0, zfp_type_int32, 0);
    run_compression<T, int32>(4, 4, 4, 4, zfp_type_int32, 0);
    run_compression<T, int32>(4, 6, 4, 0, zfp_type_int32, 4);
    run_compression<T, int32>(4, 4, 4, 4, zfp_type_int32, 4);
}

typedef boost::mpl::list<uint32_t, int32_t> test32_types;

BOOST_AUTO_TEST_CASE_TEMPLATE(test_compress32, T, test32_types) {
    run_compression<T, int64>(4, 6, 4, 0, zfp_type_int64, 0);
    run_compression<T, int64>(4, 4, 4, 4, zfp_type_int64, 0);
    run_compression<T, int64>(4, 6, 4, 0, zfp_type_int64, 4);
    run_compression<T, int64>(4, 4, 4, 4, zfp_type_int64, 4);
}

BOOST_AUTO_TEST_CASE(test_compress_int32) {
    auto field = std::unique_ptr<zfp_field, decltype(&zfp_field_free)>(zfp_field_alloc(), &zfp_field_free);
    auto zfp = std::unique_ptr<zfp_stream, decltype(&zfp_stream_close)>(zfp_stream_open(nullptr), &zfp_stream_close);

    field->nx = 4;
    field->ny = 4;
    field->nz = 4;
    field->nw = 4;
    uint terms = field->nx * (field->ny ? field->ny : 1) * (field->nz ? field->nz : 1) * (field->nw ? field->nw : 1);

    std::vector<int32>data1(terms);
    std::vector<int32>data2(terms);
    std::vector<uint8_t> buffer;

    for (size_t i = 0; i < terms; i++)
        data1[i] = 1;

    zfp_field_set_type(field.get(), zfp_type_int32);
    zfp_field_set_pointer(field.get(), data1.data());

    buffer.resize(zfp_stream_maximum_size(zfp.get(), field.get()));

    auto stream = std::unique_ptr<bitstream, decltype(&stream_close)>(stream_open(buffer.data(), buffer.size()), &stream_close);

    if (nullptr == stream)
        BOOST_FAIL("Failed to allocate bitstream");

    zfp_stream_set_reversible(zfp.get());

    zfp_stream_set_bit_stream(zfp.get(), stream.get());
    zfp_stream_rewind(zfp.get());
    zfp_write_header(zfp.get(), field.get(), ZFP_HEADER_FULL);

    size_t zfpsize1 = zfp_compress(zfp.get(), field.get());

    BOOST_REQUIRE_NE(zfpsize1, 0);

    buffer.resize(zfp_stream_compressed_size(zfp.get()));

    zfp_stream_rewind(zfp.get());
    zfp_field_set_pointer(field.get(), data2.data());

    zfp_read_header(zfp.get(), field.get(), ZFP_HEADER_FULL);

    size_t zfpsize2 = zfp_decompress(zfp.get(), field.get());

    BOOST_REQUIRE_EQUAL(zfpsize1, zfpsize2);
    BOOST_CHECK_EQUAL(memcmp(data1.data(), data2.data(), sizeof(*(data1.data())) * data1.size()), 0);
}
BOOST_AUTO_TEST_SUITE_END()
