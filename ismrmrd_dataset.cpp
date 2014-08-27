#include "ismrmrd_dataset.h"

namespace ISMRMRD {
//
// Dataset class implementation
//
// Constructor
Dataset::Dataset(const char* filename, const char* groupname, bool create_file_if_needed)
{
    // TODO error checking and exception throwing
    // Initialize the dataset
    int status;
    status = ismrmrd_init_dataset(&dset_, filename, groupname);
    // Open the file
    status = ismrmrd_open_dataset(&dset_, create_file_if_needed);
}

// Destructor
Dataset::~Dataset()
{
    int status = ismrmrd_close_dataset(&dset_);
}

// XML Header
int Dataset::writeHeader(const std::string& xmlstring)
{
    int status = ismrmrd_write_header(&dset_, xmlstring.c_str());
    return status;
}

int Dataset::writeHeader(const char* xml)
{
    int status = ismrmrd_write_header(&dset_, xml);
    return status;
}

int Dataset::readHeader(std::string& xmlstring){
    char * temp = ismrmrd_read_header(&dset_);
    xmlstring = std::string(temp);
    free(temp);
    return ISMRMRD_NOERROR;
}

char * Dataset::readHeader()
{
    // The C-API uses malloc.  In Cpp we expect pointers to have been created with new,
    // so we make a copy.
    char * temp = ismrmrd_read_header(&dset_);
    size_t xmllength = strlen(temp);
    char * xml = new char[xmllength];
    memcpy(xml, temp, xmllength);
    free(temp);
    return xml;
}

// Acquisitions
int Dataset::appendAcquisition(const Acquisition &acq)
{
    int status = ismrmrd_append_acquisition(&dset_, &acq.acq_);
    return status;
}

Acquisition * Dataset::readAcquisition(unsigned long index)
{
    Acquisition * acq = new Acquisition();
    int status = ismrmrd_read_acquisition(&dset_, index, &acq->acq_);
    return acq;
}

unsigned long Dataset::getNumberOfAcquisitions()
{
    unsigned long num =  ismrmrd_get_number_of_acquisitions(&dset_);
    return num;
}

} // namespace ISMRMRD