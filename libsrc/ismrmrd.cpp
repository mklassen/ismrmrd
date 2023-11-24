#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <stdexcept>

#include <iostream>
#include "ismrmrd/ismrmrd.h"

namespace ISMRMRD {


bool operator==(const EncodingCounters& ec1, const EncodingCounters& ec2){
    return ec1.kspace_encode_step_1 == ec2.kspace_encode_step_1 &&
        ec1.kspace_encode_step_2 == ec2.kspace_encode_step_2 &&
        ec1.average == ec2.average &&
        ec1.slice == ec2.slice &&
        ec1.contrast == ec2.contrast &&
        ec1.phase == ec2.phase &&
        ec1.repetition == ec2.repetition &&
        ec1.set == ec2.set &&
        ec1.segment == ec2.segment &&
        #if ISMRMRD_ICE_SUPPORT
        memcmp(ec1.user, ec2.user, sizeof(ec1.user)) == 0;
        #else
        std::equal(std::begin(ec1.user), std::end(ec1.user), std::begin(ec2.user));
        #endif


}

bool operator==(ISMRMRD_AcquisitionHeader const &left, ISMRMRD_AcquisitionHeader const &right)
{
    return (left.version == right.version) &&
           (left.flags == right.flags) &&
           (left.measurement_uid == right.measurement_uid) &&
           (left.scan_counter == right.scan_counter) &&
           (left.acquisition_time_stamp == right.acquisition_time_stamp) &&
           #if ISMRMRD_ICE_SUPPORT
           memcmp(left.physiology_time_stamp, right.physiology_time_stamp, sizeof(left.physiology_time_stamp)) == 0 &&
           #else
           std::equal(std::begin(left.physiology_time_stamp), std::end(left.physiology_time_stamp), std::begin(right.physiology_time_stamp)) &&
           #endif
           (left.number_of_samples == right.number_of_samples) &&
           (left.available_channels == right.available_channels) &&
           (left.active_channels == right.active_channels) &&
           #if ISMRMRD_ICE_SUPPORT
           memcmp(left.channel_mask, right.channel_mask, sizeof(left.channel_mask)) == 0 &&
           #else
           std::equal(std::begin(left.channel_mask), std::end(left.channel_mask), std::begin(right.channel_mask)) &&
           #endif
           (left.discard_pre == right.discard_pre) &&
           (left.discard_post == right.discard_post) &&
           (left.center_sample == right.center_sample) &&
           (left.encoding_space_ref == right.encoding_space_ref) &&
           (left.trajectory_dimensions == right.trajectory_dimensions) &&
           (left.sample_time_us == right.sample_time_us) &&
           #if ISMRMRD_ICE_SUPPORT
           memcmp(left.position, right.position, sizeof(left.position)) == 0 &&
           memcmp(left.read_dir, right.read_dir, sizeof(left.read_dir)) == 0 &&
           memcmp(left.phase_dir, right.phase_dir, sizeof(left.phase_dir)) == 0 &&
           memcmp(left.slice_dir, right.slice_dir, sizeof(left.slice_dir)) == 0 &&
           memcmp(left.patient_table_position, right.patient_table_position, sizeof(left.patient_table_position)) == 0 &&
           #else
           std::equal(std::begin(left.position), std::end(left.position), std::begin(right.position)) &&
           std::equal(std::begin(left.read_dir), std::end(left.read_dir), std::begin(right.read_dir)) &&
           std::equal(std::begin(left.phase_dir), std::end(left.phase_dir), std::begin(right.phase_dir)) &&
           std::equal(std::begin(left.slice_dir), std::end(left.slice_dir), std::begin(right.slice_dir)) &&
           std::equal(std::begin(left.patient_table_position), std::end(left.patient_table_position), std::begin(right.patient_table_position)) &&
           #endif
           (left.idx == right.idx) &&
           #if ISMRMRD_ICE_SUPPORT
           memcmp(left.user_int, right.user_int, sizeof(left.user_int)) == 0 &&
           memcmp(left.user_int, right.user_int, sizeof(left.user_int)) == 0;
           #else
           std::equal(std::begin(left.user_int), std::end(left.user_int), std::begin(right.user_int)) &&
           std::equal(std::begin(left.user_float), std::end(left.user_float), std::begin(right.user_float));
           #endif
}

bool operator==(ISMRMRD_Acquisition const &left, ISMRMRD_Acquisition const &right)
{
    return (left.head == right.head) &&
           ((left.traj == right.traj) || ((left.traj != NULL) && (right.traj != NULL) && (memcmp(left.traj, right.traj, ismrmrd_size_of_acquisition_traj(&left)) == 0))) &&
           ((left.data == right.data) || ((left.data != NULL) && (right.data != NULL) && (memcmp(left.data, right.data, ismrmrd_size_of_acquisition_data(&left)) == 0)));
}

bool operator==(ISMRMRD_ImageHeader const &left, ISMRMRD_ImageHeader const &right)
{
    return (left.version == right.version) &&
           (left.data_type == right.data_type) &&
           (left.flags == right.flags) &&
           (left.measurement_uid == right.measurement_uid) &&
#if ISMRMRD_ICE_SUPPORT
           memcmp(left.matrix_size, right.matrix_size, sizeof(left.matrix_size)) == 0 &&
           memcmp(left.field_of_view, right.field_of_view, sizeof(left.field_of_view)) == 0 &&
#else
           std::equal(std::begin(left.matrix_size), std::end(left.matrix_size), std::begin(right.matrix_size)) &&
           std::equal(std::begin(left.field_of_view), std::end(left.field_of_view), std::begin(right.field_of_view)) &&
           #endif
           (left.channels == right.channels) &&
#if ISMRMRD_ICE_SUPPORT
           memcmp(left.position, right.position, sizeof(left.position)) == 0 &&
           memcmp(left.read_dir, right.read_dir, sizeof(left.read_dir)) == 0 &&
           memcmp(left.phase_dir, right.phase_dir, sizeof(left.phase_dir)) == 0 &&
           memcmp(left.slice_dir, right.slice_dir, sizeof(left.slice_dir)) == 0 &&
           memcmp(left.patient_table_position, right.patient_table_position, sizeof(left.patient_table_position)) == 0 &&
#else
           std::equal(std::begin(left.position), std::end(left.position), std::begin(right.position)) &&
           std::equal(std::begin(left.read_dir), std::end(left.read_dir), std::begin(right.read_dir)) &&
           std::equal(std::begin(left.phase_dir), std::end(left.phase_dir), std::begin(right.phase_dir)) &&
           std::equal(std::begin(left.slice_dir), std::end(left.slice_dir), std::begin(right.slice_dir)) &&
           std::equal(std::begin(left.patient_table_position), std::end(left.patient_table_position), std::begin(right.patient_table_position)) &&
#endif
           (left.average == right.average) &&
           (left.slice == right.slice) &&
           (left.contrast == right.contrast) &&
           (left.phase == right.phase) &&
           (left.repetition == right.repetition) &&
           (left.set == right.set) &&
           (left.acquisition_time_stamp == right.acquisition_time_stamp) &&
#if ISMRMRD_ICE_SUPPORT
           memcmp(left.physiology_time_stamp, right.physiology_time_stamp, sizeof(left.physiology_time_stamp)) == 0 &&
#else
           std::equal(std::begin(left.physiology_time_stamp), std::end(left.physiology_time_stamp), std::begin(right.physiology_time_stamp)) &&
#endif
           (left.image_type == right.image_type) &&
           (left.image_index == right.image_index) &&
           (left.image_series_index == right.image_series_index) &&
#if ISMRMRD_ICE_SUPPORT
           memcmp(left.user_int, right.user_int, sizeof(left.user_int)) == 0 &&
           memcmp(left.user_int, right.user_int, sizeof(left.user_int)) == 0;
#else
           std::equal(std::begin(left.user_int), std::end(left.user_int), std::begin(right.user_int)) &&
           std::equal(std::begin(left.user_float), std::end(left.user_float), std::begin(right.user_float));
#endif
}

bool operator==(ISMRMRD_Image const &left, ISMRMRD_Image const &right)
{
    return (left.head == right.head) &&
           (memcmp(left.attribute_string, right.attribute_string, ismrmrd_size_of_image_attribute_string(&left)) == 0) &&
           ((left.data == right.data) || ((left.data != NULL) && (right.data != NULL) && (memcmp(left.data, right.data, ismrmrd_size_of_image_data(&left)) == 0)));
}

bool operator==(ISMRMRD_NDArray const &left, ISMRMRD_NDArray const &right)
{
    return (left.version == right.version) &&
           (left.data_type == right.data_type) &&
           (left.ndim == right.ndim) &&
#if ISMRMRD_ICE_SUPPORT
           memcmp(left.dims, right.dims, sizeof(*(left.dims) * left.ndim)) == 0 &&
        #else
           std::equal(std::begin(left.dims), std::begin(left.dims) + left.ndim, std::begin(right.dims)) &&
#endif
           ((left.data == right.data) || ((left.data != NULL) && (right.data != NULL) && (memcmp(left.data, right.data, ismrmrd_size_of_ndarray_data(&left)) == 0)));
}

//
// AcquisitionHeader class implementation
//
// Constructors
AcquisitionHeader::AcquisitionHeader() {
    if (ismrmrd_init_acquisition_header(this) != ISMRMRD_NOERROR) {
        throw std::runtime_error(build_exception_string());
    }
}

// Flag methods
bool AcquisitionHeader::isFlagSet(const ISMRMRD_AcquisitionFlags val) const {
    return ismrmrd_is_flag_set(flags, val);
}

void AcquisitionHeader::setFlag(const ISMRMRD_AcquisitionFlags val) {
    ismrmrd_set_flag(&flags, val);
}

void AcquisitionHeader::clearFlag(const ISMRMRD_AcquisitionFlags val) {
    ismrmrd_clear_flag(&flags, val);
}

void AcquisitionHeader::clearAllFlags() {
    ismrmrd_clear_all_flags(&flags);
}

// Channel mask methods
bool AcquisitionHeader::isChannelActive(uint16_t channel_id) const {
    return ismrmrd_is_channel_on(channel_mask, channel_id);
}
void AcquisitionHeader::setChannelActive(uint16_t channel_id) {
    ismrmrd_set_channel_on(channel_mask, channel_id);
}
void AcquisitionHeader::setChannelNotActive(uint16_t channel_id) {
    ismrmrd_set_channel_off(channel_mask, channel_id);
}
void AcquisitionHeader::setAllChannelsNotActive() {
    ismrmrd_set_all_channels_off(channel_mask);
}

bool AcquisitionHeader::operator==(const AcquisitionHeader &hdr) const {

   ISMRMRD_AcquisitionHeader const &left = *this;
   return left == static_cast<ISMRMRD_AcquisitionHeader const &> (hdr);
}

//
// Acquisition class Implementation
//
// Constructors, assignment operator, destructor
Acquisition::Acquisition() {
    if (ismrmrd_init_acquisition(&acq) != ISMRMRD_NOERROR) {
        throw std::runtime_error(build_exception_string());
    }
}

Acquisition::Acquisition(uint16_t num_samples, uint16_t active_channels, uint16_t trajectory_dimensions){
    if (ismrmrd_init_acquisition(&acq) != ISMRMRD_NOERROR) {
        throw std::runtime_error(build_exception_string());
    }
    this->resize(num_samples,active_channels,trajectory_dimensions);
}

#if !ISMRMRD_ICE_SUPPORT
Acquisition::Acquisition(std::unique_ptr<ISMRMRD_Acquisition> pacq) : acq(*pacq) {}
#endif

Acquisition::Acquisition(const Acquisition &other) {
    int err = 0;
    // This is a deep copy
    err = ismrmrd_init_acquisition(&acq);
    if (err) {
        throw std::runtime_error(build_exception_string());
    }
    err = ismrmrd_copy_acquisition(&acq, &other.acq);
    if (err) {
        throw std::runtime_error(build_exception_string());
    }
}

#if !ISMRMRD_ICE_SUPPORT
Acquisition::Acquisition(Acquisition &&other) : acq(other.acq){
   other.acq.data = NULL;
   other.acq.traj = NULL;
}
#endif

Acquisition & Acquisition::operator= (const Acquisition &other) {
    // Assignment makes a copy
    int err = 0;
    if (this != &other )
    {
        ismrmrd_cleanup_acquisition(&acq);
        err = ismrmrd_init_acquisition(&acq);
        if (err) {
            throw std::runtime_error(build_exception_string());
        }
        err = ismrmrd_copy_acquisition(&acq, &other.acq);
        if (err) {
            throw std::runtime_error(build_exception_string());
        }
    }
    return *this;
}

#if !ISMRMRD_ICE_SUPPORT
Acquisition & Acquisition::operator= (Acquisition &&other){
   ismrmrd_cleanup_acquisition(&acq);
   acq = other.acq;
   other.acq.data = NULL;
   other.acq.traj = NULL;
   return *this;
}
#endif

bool Acquisition::operator==(Acquisition const &other) const
{
   return this->acq == other.acq;
}

Acquisition::~Acquisition() {
    ismrmrd_cleanup_acquisition(&acq);
}

// Accessors and mutators
uint16_t Acquisition::version() const {
    return acq.head.version;
}

uint64_t Acquisition::flags() const {
    return acq.head.flags;
}

const uint32_t (&Acquisition::physiology_time_stamp() const) [ISMRMRD_PHYS_STAMPS] {
    return acq.head.physiology_time_stamp;
}

uint16_t Acquisition::number_of_samples() const  {
    return acq.head.number_of_samples;
}

uint16_t Acquisition::available_channels() const {
    return acq.head.available_channels;
}

uint16_t Acquisition::active_channels() const {
    return acq.head.active_channels;
}

const uint64_t (&Acquisition::channel_mask() const) [ISMRMRD_CHANNEL_MASKS] {
    return acq.head.channel_mask;
}

uint16_t Acquisition::discard_pre() const {
    return acq.head.discard_pre;
}

uint16_t Acquisition::discard_post() const {
    return acq.head.discard_post;
}

uint16_t Acquisition::center_sample() const {
    return acq.head.center_sample;
}

uint16_t Acquisition::encoding_space_ref() const {
    return acq.head.encoding_space_ref;
}

uint16_t Acquisition::trajectory_dimensions() const  {
    return acq.head.trajectory_dimensions;
}

float Acquisition::sample_time_us() const {
    return acq.head.sample_time_us;
}

const float (&Acquisition::position() const )[3] {
    return acq.head.position;
}

const float (&Acquisition::read_dir() const )[3] {
    return acq.head.read_dir;
}

const float (&Acquisition::phase_dir() const)[3] {
    return acq.head.phase_dir;
}

const float (&Acquisition::slice_dir() const )[3] {
    return acq.head.slice_dir;
}

const float (&Acquisition::patient_table_position() const)[3] {
    return acq.head.patient_table_position;
}

const ISMRMRD_EncodingCounters &Acquisition::idx() const {
    return acq.head.idx;
}

const int32_t (&Acquisition::user_int() const ) [ISMRMRD_USER_INTS] {
    return acq.head.user_int;
}

const float (&Acquisition::user_float() const) [ISMRMRD_USER_FLOATS] {
    return acq.head.user_float;
}


uint32_t &Acquisition::measurement_uid() {
    return acq.head.measurement_uid;
}

uint32_t &Acquisition::scan_counter() {
    return acq.head.scan_counter;
}

uint32_t &Acquisition::acquisition_time_stamp() {
    return acq.head.acquisition_time_stamp;
}

uint32_t (&Acquisition::physiology_time_stamp()) [ISMRMRD_PHYS_STAMPS] {
    return acq.head.physiology_time_stamp;
}

const uint16_t &Acquisition::number_of_samples() {
    return acq.head.number_of_samples;
}

uint16_t &Acquisition::available_channels() {
    return acq.head.available_channels;
}

const uint16_t &Acquisition::active_channels() {
    return acq.head.active_channels;
}

const uint64_t (&Acquisition::channel_mask()) [ISMRMRD_CHANNEL_MASKS] {
    return acq.head.channel_mask;
}

uint16_t &Acquisition::discard_pre() {
    return acq.head.discard_pre;
}

uint16_t &Acquisition::discard_post() {
    return acq.head.discard_post;
}

uint16_t &Acquisition::center_sample() {
    return acq.head.center_sample;
}

uint16_t &Acquisition::encoding_space_ref() {
    return acq.head.encoding_space_ref;
}

const uint16_t &Acquisition::trajectory_dimensions() {
    return acq.head.trajectory_dimensions;
}

float &Acquisition::sample_time_us() {
    return acq.head.sample_time_us;
}

float (&Acquisition::position())[3] {
    return acq.head.position;
}

float (&Acquisition::read_dir())[3] {
    return acq.head.read_dir;
}

float (&Acquisition::phase_dir())[3] {
    return acq.head.phase_dir;
}

float (&Acquisition::slice_dir())[3] {
    return acq.head.slice_dir;
}

float (&Acquisition::patient_table_position())[3] {
    return acq.head.patient_table_position;
}

ISMRMRD_EncodingCounters &Acquisition::idx() {
    return acq.head.idx;
}

int32_t (&Acquisition::user_int()) [ISMRMRD_USER_INTS] {
    return acq.head.user_int;
}

float (&Acquisition::user_float()) [ISMRMRD_USER_FLOATS] {
    return acq.head.user_float;
}

// Sizes
size_t Acquisition::getNumberOfDataElements() const {
    size_t num = acq.head.number_of_samples * acq.head.active_channels;
    return num;
}

size_t Acquisition::getDataSize() const {
    size_t num = acq.head.number_of_samples * acq.head.active_channels;
    return num*sizeof(complex_float_t);
}

size_t Acquisition::getNumberOfTrajElements() const {
    size_t num = acq.head.number_of_samples * acq.head.trajectory_dimensions;
    return num;
}

size_t Acquisition::getTrajSize() const {
    size_t num = acq.head.number_of_samples * acq.head.trajectory_dimensions;
    return num*sizeof(float);
}

// Data and Trajectory accessors
const AcquisitionHeader & Acquisition::getHead() const {
    // This returns a reference
    return *static_cast<const AcquisitionHeader *>(&acq.head);
}

void Acquisition::setHead(const AcquisitionHeader &other) {
    memcpy(&acq.head, &other, sizeof(AcquisitionHeader));
    if (ismrmrd_make_consistent_acquisition(&acq) != ISMRMRD_NOERROR) {
        throw std::runtime_error(build_exception_string());
    }
}

void Acquisition::resize(uint16_t num_samples, uint16_t active_channels, uint16_t trajectory_dimensions){
       acq.head.number_of_samples = num_samples;
       acq.head.active_channels = active_channels;
       acq.head.trajectory_dimensions = trajectory_dimensions;
       if (ismrmrd_make_consistent_acquisition(&acq) != ISMRMRD_NOERROR) {
           throw std::runtime_error(build_exception_string());
       }
}

const complex_float_t * Acquisition::getDataPtr() const {
    return acq.data;
}

complex_float_t * Acquisition::getDataPtr() {
    return acq.data;
}

void Acquisition::setData(complex_float_t * data) {
    memcpy(acq.data,data,this->getNumberOfDataElements()*sizeof(complex_float_t));
}

complex_float_t & Acquisition::data(uint16_t sample, uint16_t channel){
       size_t index = size_t(sample)+size_t(channel)*size_t(acq.head.number_of_samples);
       return acq.data[index];
}

const float * Acquisition::getTrajPtr() const {
    return acq.traj;
}

float * Acquisition::getTrajPtr() {
    return acq.traj;
}

void Acquisition::setTraj(float* traj) {
       memcpy(acq.traj,traj,this->getNumberOfTrajElements()*sizeof(float));
}

float & Acquisition::traj(uint16_t dimension, uint16_t sample){
    size_t index = size_t(sample)*size_t(acq.head.trajectory_dimensions)+size_t(dimension);
    return acq.traj[index];
}

const complex_float_t * Acquisition::data_begin() const{
       return acq.data;
}

const complex_float_t * Acquisition::data_end() const {
       return acq.data+size_t(acq.head.number_of_samples)*size_t(acq.head.active_channels);
}

complex_float_t * Acquisition::data_begin(){
       return acq.data;
}

complex_float_t * Acquisition::data_end() {
       return acq.data+size_t(acq.head.number_of_samples)*size_t(acq.head.active_channels);
}

const float * Acquisition::traj_begin() const {
       return acq.traj;
}

const float * Acquisition::traj_end() const {
       return acq.traj+size_t(acq.head.number_of_samples)*size_t(acq.head.trajectory_dimensions);
}
 float * Acquisition::traj_begin() {
       return acq.traj;
}

float * Acquisition::traj_end() {
       return acq.traj+size_t(acq.head.number_of_samples)*size_t(acq.head.trajectory_dimensions);
}

// Flag methods
bool Acquisition::isFlagSet(const uint64_t val) const {
    return ismrmrd_is_flag_set(acq.head.flags, val);
}
void Acquisition::setFlag(const uint64_t val) {
    ismrmrd_set_flag(&acq.head.flags, val);
}
void Acquisition::clearFlag(const uint64_t val) {
    ismrmrd_clear_flag(&acq.head.flags, val);
}
void Acquisition::clearAllFlags() {
    ismrmrd_clear_all_flags(&acq.head.flags);
}

// Channel mask methods
bool Acquisition::isChannelActive(uint16_t channel_id) const {
    return ismrmrd_is_channel_on(acq.head.channel_mask, channel_id);
}
void Acquisition::setChannelActive(uint16_t channel_id) {
    ismrmrd_set_channel_on(acq.head.channel_mask, channel_id);
}
void Acquisition::setChannelNotActive(uint16_t channel_id) {
    ismrmrd_set_channel_off(acq.head.channel_mask, channel_id);
}
void Acquisition::setAllChannelsNotActive() {
    ismrmrd_set_all_channels_off(acq.head.channel_mask);
}
uint32_t Acquisition::measurement_uid() const {
   return acq.head.measurement_uid;
}
uint32_t Acquisition::scan_counter() const {
   return acq.head.scan_counter;
}
uint32_t Acquisition::acquisition_time_stamp() const {
   return acq.head.acquisition_time_stamp;
}

//
// ImageHeader class Implementation
//

// Constructor
ImageHeader::ImageHeader() {
    if (ismrmrd_init_image_header(this) != ISMRMRD_NOERROR) {
        throw std::runtime_error(build_exception_string());
    }
};

// Flag methods
bool ImageHeader::isFlagSet(const uint64_t val) const {
    return ismrmrd_is_flag_set(flags, val);
};

void ImageHeader::setFlag(const uint64_t val) {
    ismrmrd_set_flag(&flags, val);
};

void ImageHeader::clearFlag(const uint64_t val) {
    ismrmrd_clear_flag(&flags, val);
};

void ImageHeader::clearAllFlags() {
    ismrmrd_clear_all_flags(&flags);
};

//
// Image class Implementation
//

// Constructors
template <typename T> Image<T>::Image(uint16_t matrix_size_x,
                                      uint16_t matrix_size_y,
                                      uint16_t matrix_size_z,
                                      uint16_t channels)
{
    if (ismrmrd_init_image(&im) != ISMRMRD_NOERROR) {
        throw std::runtime_error(build_exception_string());
    }
    im.head.data_type = static_cast<uint16_t>(get_data_type<T>());
    resize(matrix_size_x, matrix_size_y, matrix_size_z, channels);
}

#if !ISMRMRD_ICE_SUPPORT
template <typename T> Image<T>::Image(std::unique_ptr<ISMRMRD_Image> pim) : im(*pim) {}
#endif

template <typename T> Image<T>::Image(const Image<T> &other) {
    int err = 0;
    // This is a deep copy
    err = ismrmrd_init_image(&im);
    if (err) {
        throw std::runtime_error(build_exception_string());
    }
    err = ismrmrd_copy_image(&im, &other.im);
    if (err) {
        throw std::runtime_error(build_exception_string());
    }
}

#if !ISMRMRD_ICE_SUPPORT
template <typename T> Image<T>::Image(Image &&other) : im(other.im){
   other.im.data = NULL;
   other.im.attribute_string = NULL;
   other.im.head.attribute_string_len = 0;
}
#endif

template <typename T> Image<T> & Image<T>::operator= (const Image<T> &other)
{
    int err = 0;
    // Assignment makes a copy
    if (this != &other )
    {
        ismrmrd_cleanup_image(&im);
        err = ismrmrd_init_image(&im);
        if (err) {
            throw std::runtime_error(build_exception_string());
        }
        err = ismrmrd_copy_image(&im, &other.im);
        if (err) {
            throw std::runtime_error(build_exception_string());
        }
    }
    return *this;
}

#if !ISMRMRD_ICE_SUPPORT
template <typename T> Image<T> & Image<T>::operator= (Image &&other){
   ismrmrd_cleanup_image(&im);
   im = other.im;
   other.im.data = NULL;
   other.im.attribute_string = NULL;
   other.im.head.attribute_string_len = 0;
   return *this;
}
#endif

template <typename T> bool Image<T>::operator== (const Image<T> &other) const
{
   return this->im == other.im;
}

template <typename T> Image<T>::~Image() {
    ismrmrd_cleanup_image(&im);
}

// Image dimensions
template <typename T> void Image<T>::resize(uint16_t matrix_size_x,
                                            uint16_t matrix_size_y,
                                            uint16_t matrix_size_z,
                                            uint16_t channels)
{
    // TODO what if matrix_size_x = 0?
    if (matrix_size_y == 0) {
        matrix_size_y = 1;
    }
    if (matrix_size_z == 0) {
        matrix_size_z = 1;
    }
    if (channels == 0) {
        channels = 1;
    }

    im.head.matrix_size[0] = matrix_size_x;
    im.head.matrix_size[1] = matrix_size_y;
    im.head.matrix_size[2] = matrix_size_z;
    im.head.channels = channels;
    if (ismrmrd_make_consistent_image(&im) != ISMRMRD_NOERROR) {
        throw std::runtime_error(build_exception_string());
    }
}

template <typename T> uint16_t Image<T>::getMatrixSizeX() const
{
    return im.head.matrix_size[0];
}

template <typename T> void Image<T>::setMatrixSizeX(uint16_t matrix_size_x)
{
    // TODO what if matrix_size_x = 0?
    im.head.matrix_size[0] = matrix_size_x;
    if (ismrmrd_make_consistent_image(&im) != ISMRMRD_NOERROR) {
        throw std::runtime_error(build_exception_string());
    }
}

template <typename T> uint16_t Image<T>::getMatrixSizeY() const
{
    return im.head.matrix_size[1];
}

template <typename T> void Image<T>::setMatrixSizeY(uint16_t matrix_size_y)
{
    if (matrix_size_y == 0) {
        matrix_size_y = 1;
    }
    im.head.matrix_size[1] = matrix_size_y;
    if (ismrmrd_make_consistent_image(&im) != ISMRMRD_NOERROR) {
        throw std::runtime_error(build_exception_string());
    }
}

template <typename T> uint16_t Image<T>::getMatrixSizeZ() const
{
    return im.head.matrix_size[2];
}

template <typename T> void Image<T>::setMatrixSizeZ(uint16_t matrix_size_z)
{
    if (matrix_size_z == 0) {
        matrix_size_z = 1;
    }
    im.head.matrix_size[2] = matrix_size_z;
    if (ismrmrd_make_consistent_image(&im) != ISMRMRD_NOERROR) {
        throw std::runtime_error(build_exception_string());
    }
}

template <typename T> uint16_t Image<T>::getNumberOfChannels() const
{
    return im.head.channels;
}

template <typename T> void Image<T>::setNumberOfChannels(uint16_t channels)
{
    if (channels == 0) {
        channels = 1;
    }

    im.head.channels = channels;
    if (ismrmrd_make_consistent_image(&im) != ISMRMRD_NOERROR) {
        throw std::runtime_error(build_exception_string());
    }
}


template <typename T> void Image<T>::setFieldOfView(float fov_x, float fov_y, float fov_z)
{
    im.head.field_of_view[0] = fov_x;
    im.head.field_of_view[1] = fov_y;
    im.head.field_of_view[2] = fov_z;
}

template <typename T> void Image<T>::setFieldOfViewX(float fov_x)
{
    im.head.field_of_view[0] = fov_x;
}

template <typename T> float Image<T>::getFieldOfViewX() const
{
    return im.head.field_of_view[0];
}

template <typename T> void Image<T>::setFieldOfViewY(float fov_y)
{
    im.head.field_of_view[1] = fov_y;
}

template <typename T> float Image<T>::getFieldOfViewY() const
{
    return im.head.field_of_view[1];
}

template <typename T> void Image<T>::setFieldOfViewZ(float fov_z)
{
    im.head.field_of_view[2] = fov_z;
}

template <typename T> float Image<T>::getFieldOfViewZ() const
{
    return im.head.field_of_view[2];
}


// Positions and orientations
template <typename T> void Image<T>::setPosition(float x, float y, float z)
{
    im.head.position[0] = x;
    im.head.position[1] = y;
    im.head.position[2] = z;
}

template <typename T> float Image<T>::getPositionX() const
{
    return im.head.position[0];
}

template <typename T> void Image<T>::setPositionX(float x)
{
    im.head.position[0] = x;
}

template <typename T> float Image<T>::getPositionY() const
{
    return im.head.position[1];
}

template <typename T> void Image<T>::setPositionY(float y)
{
    im.head.position[1] = y;
}

template <typename T> float Image<T>::getPositionZ() const
{
    return im.head.position[2];
}

template <typename T> void Image<T>::setPositionZ(float z)
{
    im.head.position[2] = z;
}


template <typename T> void Image<T>::setReadDirection(float x, float y, float z)
{
    im.head.read_dir[0] = x;
    im.head.read_dir[1] = y;
    im.head.read_dir[2] = z;
}

template <typename T> float Image<T>::getReadDirectionX() const
{
    return im.head.read_dir[0];
}

template <typename T> void Image<T>::setReadDirectionX(float x)
{
    im.head.read_dir[0] = x;
}

template <typename T> float Image<T>::getReadDirectionY() const
{
    return im.head.read_dir[1];
}

template <typename T> void Image<T>::setReadDirectionY(float y)
{
    im.head.read_dir[1] = y;
}

template <typename T> float Image<T>::getReadDirectionZ() const
{
    return im.head.read_dir[2];
}

template <typename T> void Image<T>::setReadDirectionZ(float z)
{
    im.head.read_dir[2] = z;
}


template <typename T> void Image<T>::setPhaseDirection(float x, float y, float z)
{
    im.head.phase_dir[0] = x;
    im.head.phase_dir[1] = y;
    im.head.phase_dir[2] = z;
}

template <typename T> float Image<T>::getPhaseDirectionX() const
{
    return im.head.phase_dir[0];
}

template <typename T> void Image<T>::setPhaseDirectionX(float x)
{
    im.head.phase_dir[0] = x;
}

template <typename T> float Image<T>::getPhaseDirectionY() const
{
    return im.head.phase_dir[1];
}

template <typename T> void Image<T>::setPhaseDirectionY(float y)
{
    im.head.phase_dir[1] = y;
}

template <typename T> float Image<T>::getPhaseDirectionZ() const
{
    return im.head.phase_dir[2];
}

template <typename T> void Image<T>::setPhaseDirectionZ(float z)
{
    im.head.phase_dir[2] = z;
}

template <typename T> void Image<T>::setSliceDirection(float x, float y, float z)
{
    im.head.slice_dir[0] = x;
    im.head.slice_dir[1] = y;
    im.head.slice_dir[2] = z;
}

template <typename T> float Image<T>::getSliceDirectionX() const
{
    return im.head.slice_dir[0];
}

template <typename T> void Image<T>::setSliceDirectionX(float x)
{
    im.head.slice_dir[0] = x;
}

template <typename T> float Image<T>::getSliceDirectionY() const
{
    return im.head.slice_dir[1];
}

template <typename T> void Image<T>::setSliceDirectionY(float y)
{
    im.head.slice_dir[1] = y;
}

template <typename T> float Image<T>::getSliceDirectionZ() const
{
    return im.head.slice_dir[2];
}

template <typename T> void Image<T>::setSliceDirectionZ(float z)
{
    im.head.slice_dir[2] = z;
}

template <typename T> void Image<T>::setPatientTablePosition(float x, float y, float z)
{
    im.head.patient_table_position[0] = x;
    im.head.patient_table_position[1] = y;
    im.head.patient_table_position[2] = z;
}

template <typename T> float Image<T>::getPatientTablePositionX() const
{
    return im.head.patient_table_position[0];
}

template <typename T> void Image<T>::setPatientTablePositionX(float x)
{
    im.head.patient_table_position[0] = x;
}

template <typename T> float Image<T>::getPatientTablePositionY() const
{
    return im.head.patient_table_position[1];
}

template <typename T> void Image<T>::setPatientTablePositionY(float y)
{
    im.head.patient_table_position[1] = y;
}

template <typename T> float Image<T>::getPatientTablePositionZ() const
{
    return im.head.patient_table_position[2];
}

template <typename T> void Image<T>::setPatientTablePositionZ(float z)
{
    im.head.patient_table_position[2] = z;
}

template <typename T> uint16_t Image<T>::getVersion() const
{
    return im.head.version;
}

template <typename T> ISMRMRD_DataTypes Image<T>::getDataType() const
{
    return static_cast<ISMRMRD_DataTypes>(im.head.data_type);
}

template <typename T> uint32_t Image<T>::getMeasurementUid() const
{
    return im.head.measurement_uid;
}

template <typename T> void Image<T>::setMeasurementUid(uint32_t measurement_uid)
{
    im.head.measurement_uid = measurement_uid;
}


template <typename T> uint16_t Image<T>::getAverage() const
{
    return im.head.average;
}

template <typename T> void  Image<T>::setAverage(uint16_t average)
{
    im.head.average = average;
}

template <typename T> uint16_t Image<T>::getSlice() const
{
    return im.head.slice;
}

template <typename T> void  Image<T>::setSlice(uint16_t slice)
{
    im.head.slice = slice;
}

template <typename T> uint16_t Image<T>::getContrast() const
{
    return im.head.contrast;
}

template <typename T> void  Image<T>::setContrast(uint16_t contrast)
{
    im.head.contrast = contrast;
}

template <typename T> uint16_t Image<T>::getPhase() const
{
    return im.head.phase;
}

template <typename T> void  Image<T>::setPhase(uint16_t phase)
{
    im.head.phase = phase;
}

template <typename T> uint16_t Image<T>::getRepetition() const
{
    return im.head.repetition;
}

template <typename T> void  Image<T>::setRepetition(uint16_t repetition)
{
    im.head.repetition = repetition;
}

template <typename T> uint16_t Image<T>::getSet() const
{
    return im.head.set;
}

template <typename T> void  Image<T>::setSet(uint16_t set)
{
    im.head.set = set;
}

template <typename T> uint32_t Image<T>::getAcquisitionTimeStamp() const
{
    return im.head.acquisition_time_stamp;
}

template <typename T> void  Image<T>::setAcquisitionTimeStamp(uint32_t acquisition_time_stamp)
{
    im.head.acquisition_time_stamp = acquisition_time_stamp;
}

template <typename T> uint32_t Image<T>::getPhysiologyTimeStamp(unsigned int stamp_id) const
{
    return im.head.physiology_time_stamp[stamp_id];
}

template <typename T> void  Image<T>::setPhysiologyTimeStamp(unsigned int stamp_id, uint32_t value)
{
    im.head.physiology_time_stamp[stamp_id] = value;
}

template <typename T> uint16_t Image<T>::getImageType() const
{
    return im.head.image_type;
}

template <typename T> void Image<T>::setImageType(uint16_t image_type)
{
    im.head.image_type = image_type;
}

template <typename T> uint16_t Image<T>::getImageIndex() const
{
    return im.head.image_index;
}

template <typename T> void Image<T>::setImageIndex(uint16_t image_index)
{
    im.head.image_index = image_index;
}

template <typename T> uint16_t Image<T>::getImageSeriesIndex() const
{
    return im.head.image_series_index;
}

template <typename T> void Image<T>::setImageSeriesIndex(uint16_t image_series_index)
{
    im.head.image_series_index = image_series_index;
}


// User parameters
template <typename T> float Image<T>::getUserFloat(unsigned int index) const
{
    return im.head.user_float[index];
}

template <typename T> void Image<T>::setUserFloat(unsigned int index, float value)
{
    im.head.user_float[index] = value;
}

template <typename T> int32_t Image<T>::getUserInt(unsigned int index) const
{
    return im.head.user_int[index];
}

template <typename T> void Image<T>::setUserInt(unsigned int index, int32_t value)
{
    im.head.user_int[index] = value;
}


// Flag methods
template <typename T> uint64_t Image<T>::getFlags() const {
    return im.head.flags;
}

template <typename T> bool Image<T>::isFlagSet(const uint64_t val) const {
    return ismrmrd_is_flag_set(im.head.flags, val);
}

template <typename T> void Image<T>::setFlag(const uint64_t val) {
    ismrmrd_set_flag(&(im.head.flags), val);
}

template <typename T> void Image<T>::setFlags(const uint64_t val) {
    ismrmrd_set_flags(&(im.head.flags), val);
}

template <typename T> void Image<T>::clearFlag(const uint64_t val) {
    ismrmrd_clear_flag(&(im.head.flags), val);
}

template <typename T> void Image<T>::clearAllFlags() {
    ismrmrd_clear_all_flags(&(im.head.flags));
}

// Header
template <typename T> ImageHeader &Image<T>::getHead() {
    // This returns a reference
    return *static_cast<ImageHeader *>(&im.head);
}

template <typename T> const ImageHeader &Image<T>::getHead() const {
    // This returns a reference
    return *static_cast<const ImageHeader *>(&im.head);
}

template <typename T> void Image<T>::setHead(const ImageHeader &other) {
    if (other.data_type != im.head.data_type) {
        throw std::runtime_error("Cannot assign a header of a different data type.");
    }

    memcpy(&im.head, &other, sizeof(ImageHeader));
    if (ismrmrd_make_consistent_image(&im) != ISMRMRD_NOERROR) {
        throw std::runtime_error(build_exception_string());
    }
}

// Attribute string
template <typename T> void Image<T>::getAttributeString(std::string &attr) const
{
   if (im.attribute_string)
      attr.assign(im.attribute_string);
   else
      attr.assign("");
}

template <typename T> const char *Image<T>::getAttributeString() const
{
   return im.attribute_string;
}

template <typename T> void Image<T>::setAttributeString(const std::string &attr)
{
    this->setAttributeString(attr.c_str());
}

template <typename T> void Image<T>::setAttributeString(const char *attr)
{
    // Get the string length
    size_t length = strlen(attr);

    // Allocate space plus a null terminator and check for success
    char *newPointer = (char *)realloc(im.attribute_string, (length+1) * sizeof(*im.attribute_string));
    if (NULL==newPointer) {
        throw std::runtime_error(build_exception_string());
    }

    // Make changes only if reallocation was successful
    im.attribute_string = newPointer;
    im.head.attribute_string_len = static_cast<uint32_t>(length);

    // Set the null terminator and copy the string
    im.attribute_string[length] = '\0';
    strncpy(im.attribute_string, attr, length+1);
}

template <typename T> size_t Image<T>::getAttributeStringLength() const
{
    return im.head.attribute_string_len;
}

// Data
template <typename T> T * Image<T>::getDataPtr() {
     return static_cast<T*>(im.data);
}

template <typename T> const T * Image<T>::getDataPtr() const {
     return static_cast<const T*>(im.data);
}

template <typename T> size_t Image<T>::getNumberOfDataElements() const {
    size_t num = 1;
    num *= im.head.matrix_size[0];
    num *= im.head.matrix_size[1];
    num *= im.head.matrix_size[2];
    num *= im.head.channels;
    return num;
}

template <typename T> size_t Image<T>::getDataSize() const {
    return ismrmrd_size_of_image_data(&im);
}

template <typename T> T * Image<T>::begin() {
     return static_cast<T*>(im.data);
}

template <typename T> T * Image<T>::end() {
     return static_cast<T*>(im.data)+this->getNumberOfDataElements();
}

template <typename T> T & Image<T>::operator () (uint16_t ix, uint16_t iy, uint16_t iz, uint16_t channel) {
     size_t index = ix \
             + size_t(im.head.matrix_size[0])*iy \
             + size_t(im.head.matrix_size[1])*size_t(im.head.matrix_size[0])*iz \
             + size_t(im.head.matrix_size[1])*size_t(im.head.matrix_size[0])*size_t(im.head.matrix_size[2])*channel;
     return static_cast<T*>(im.data)[index];
}

//
// Array class Implementation
//
template <typename T> NDArray<T>::NDArray()
{
    if (ismrmrd_init_ndarray(&arr) != ISMRMRD_NOERROR) {
        throw std::runtime_error(build_exception_string());
    }
    arr.data_type = static_cast<uint16_t>(get_data_type<T>());
}

template <typename T> NDArray<T>::NDArray(const std::vector<size_t> dimvec)
{
    if (ismrmrd_init_ndarray(&arr) != ISMRMRD_NOERROR) {
        throw std::runtime_error(build_exception_string());
    }
    arr.data_type = static_cast<uint16_t>(get_data_type<T>());
    resize(dimvec);
}

template <typename T> NDArray<T>::NDArray(const NDArray<T> &other)
{
    int err = 0;
    err = ismrmrd_init_ndarray(&arr);
    if (err) {
        throw std::runtime_error(build_exception_string());
    }
    err = ismrmrd_copy_ndarray(&arr, &other.arr);
    if (err) {
        throw std::runtime_error(build_exception_string());
    }
}

template <typename T> NDArray<T>::~NDArray()
{
    ismrmrd_cleanup_ndarray(&arr);
}

template <typename T> NDArray<T> & NDArray<T>::operator= (const NDArray<T> &other)
{
    int err = 0;
    // Assignment makes a copy
    if (this != &other )
    {
        err = ismrmrd_init_ndarray(&arr);
        if (err) {
            throw std::runtime_error(build_exception_string());
        }
        err = ismrmrd_copy_ndarray(&arr, &other.arr);
        if (err) {
            throw std::runtime_error(build_exception_string());
        }
    }
    return *this;
}

template <typename T> bool NDArray<T>::operator== (const NDArray<T> &other) const
{
    return this->arr == other.arr;
}

template <typename T> uint16_t NDArray<T>::getVersion() const {
    return arr.version;
};

template <typename T> ISMRMRD_DataTypes NDArray<T>::getDataType() const {
    return static_cast<ISMRMRD_DataTypes>( arr.data_type );
}

template <typename T> uint16_t NDArray<T>::getNDim() const {
    return  arr.ndim;
};

template <typename T> const size_t (&NDArray<T>::getDims())[ISMRMRD_NDARRAY_MAXDIM] {
    return arr.dims;
};

template <typename T> void NDArray<T>::resize(const std::vector<size_t> dimvec) {
    if (dimvec.size() > ISMRMRD_NDARRAY_MAXDIM) {
        throw std::runtime_error("Input vector dimvec is too long.");
    }
    arr.ndim = static_cast<uint16_t>(dimvec.size());
    for (int n=0; n<arr.ndim; n++) {
        arr.dims[n] = dimvec[n];
    }
    if (ismrmrd_make_consistent_ndarray(&arr) != ISMRMRD_NOERROR) {
        throw std::runtime_error(build_exception_string());
    }
}

template <typename T> T * NDArray<T>::getDataPtr() {
    return static_cast<T*>(arr.data);
}

template <typename T> const T * NDArray<T>::getDataPtr() const {
    return static_cast<T*>(arr.data);
}

template <typename T> size_t NDArray<T>::getDataSize() const {
    return ismrmrd_size_of_ndarray_data(&arr);
}

template <typename T> size_t NDArray<T>::getNumberOfElements() const {
    size_t num = 1;
    for (int n = 0; n < arr.ndim; n++) {
        size_t v = arr.dims[n];
        // This is necessary to prevent bad GCC loop optimization!
        if (v > 0) {
            num *= v;
        }
    }
    return num;
}

template <typename T> T * NDArray<T>::begin() {
    return static_cast<T*>(arr.data);
}

template <typename T> T * NDArray<T>::end() {
    return static_cast<T*>(arr.data)+this->getNumberOfElements();
}

template <typename T> T & NDArray<T>::operator () (uint16_t x, uint16_t y, uint16_t z, uint16_t w, uint16_t n, uint16_t m, uint16_t l){
       size_t index = 0;
       uint16_t indices[ISMRMRD_NDARRAY_MAXDIM] = {x,y,z,w,n,m,l};
       size_t stride = 1;
       for (uint16_t i = 0; i < arr.ndim; i++){
               index += indices[i]*stride;
               stride *= arr.dims[i];
       }

       return static_cast<T*>(arr.data)[index];
}

// Specializations
// Allowed data types for Images and NDArrays
template <> EXPORTISMRMRD ISMRMRD_DataTypes get_data_type<uint16_t>()
{
    return ISMRMRD_USHORT;
}

template <> EXPORTISMRMRD inline ISMRMRD_DataTypes get_data_type<int16_t>()
{
    return ISMRMRD_SHORT;
}

template <> EXPORTISMRMRD inline ISMRMRD_DataTypes get_data_type<uint32_t>()
{
    return ISMRMRD_UINT;
}

template <> EXPORTISMRMRD inline ISMRMRD_DataTypes get_data_type<int32_t>()
{
    return ISMRMRD_INT;
}

template <> EXPORTISMRMRD inline ISMRMRD_DataTypes get_data_type<float>()
{
    return ISMRMRD_FLOAT;
}

template <> EXPORTISMRMRD inline ISMRMRD_DataTypes get_data_type<double>()
{
    return ISMRMRD_DOUBLE;
}

template <> EXPORTISMRMRD inline ISMRMRD_DataTypes get_data_type<complex_float_t>()
{
    return ISMRMRD_CXFLOAT;
}

template <> EXPORTISMRMRD inline ISMRMRD_DataTypes get_data_type<complex_double_t>()
{
    return ISMRMRD_CXDOUBLE;
}

// Images
template class EXPORTISMRMRD Image<uint16_t>;
template class EXPORTISMRMRD Image<int16_t>;
template class EXPORTISMRMRD Image<uint32_t>;
template class EXPORTISMRMRD Image<int32_t>;
template class EXPORTISMRMRD Image<float>;
template class EXPORTISMRMRD Image<double>;
template class EXPORTISMRMRD Image<complex_float_t>;
template class EXPORTISMRMRD Image<complex_double_t>;

// NDArrays
template class EXPORTISMRMRD NDArray<uint16_t>;
template class EXPORTISMRMRD NDArray<int16_t>;
template class EXPORTISMRMRD NDArray<uint32_t>;
template class EXPORTISMRMRD NDArray<int32_t>;
template class EXPORTISMRMRD NDArray<float>;
template class EXPORTISMRMRD NDArray<double>;
template class EXPORTISMRMRD NDArray<complex_float_t>;
template class EXPORTISMRMRD NDArray<complex_double_t>;


// Helper function for generating exception message from ISMRMRD error stack
std::string build_exception_string(void)
{
    char *file = NULL, *func = NULL, *msg = NULL;
    int line = 0, code = 0;
    std::stringstream stream;
    for (int i = 0; ismrmrd_pop_error(&file, &line, &func, &code, &msg); ++i) {
        if (i > 0) {
            stream << std::endl;
        }
        stream << "ISMRMRD " << ismrmrd_strerror(code) << " in " << func <<
                " (" << file << ":" << line << ": " << msg;
    }
    return stream.str();
}


} // namespace ISMRMRD
