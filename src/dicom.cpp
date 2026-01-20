#include "../include/dicom.h"


DCMTKCodecs::DCMTKCodecs() {
    DJDecoderRegistration::registerCodecs();
    DJEncoderRegistration::registerCodecs();
    DcmRLEDecoderRegistration::registerCodecs();
}

DCMTKCodecs::~DCMTKCodecs() {
    DJDecoderRegistration::cleanup();
    DJEncoderRegistration::cleanup();
    DcmRLEDecoderRegistration::cleanup();
}

ImageData::ImageData(const char* fileName) {
    this->_fileName = std::string(fileName);
    const char* fileNameData = this->_fileName.data();
    std::cout << this->_fileName << std::endl;

    OFCondition status = this->_fileFormat.loadFile(this->_fileName.data());

    DcmDataset *dataset = this->_fileFormat.getDataset();
    this->_image = new DicomImage(dataset, EXS_Unknown, CIF_MayDetachPixelData);
    if (this->_image != NULL && this->_image->getStatus() == EIS_Normal) {
        std::cout << this->_image->getWidth() << ", " << this->_image->getHeight() << std::endl;
    } else {
        // TODO
        std::cerr << "Error: cannot read DICOM file " <<
        fileNameData <<  " (" << DicomImage::getString(this->_image->getStatus()) << ")" << std::endl;
    }
}

ImageData::~ImageData() {
    delete this->_image;
}

int ImageData::getHeight() const {
    return this->_image->getHeight();
}

int ImageData::getWidth() const {
    return this->_image->getWidth();
}

bool ImageData::isMonochrome() const {
    return this->_image->isMonochrome();
}

uchar* ImageData::getOutputData() const {
    return (uchar*)(this->_image->getOutputData(8));
}

std::map<std::string, std::string> ImageData::getMetadata() {
    std::map<std::string, std::string> metadata;

    OFCondition status = this->_fileFormat.loadFile(this->_fileName.data());
    if (status.good()) {
        OFString patientName;
        DcmDataset *dataset = this->_fileFormat.getDataset();
        dataset->print(std::cout);
        // if (dataset->findAndGetOFString(DCM_PatientName, patientName).good())
        // {
        //     metadata.emplace(std::make_pair("Patient's Name", patientName));
        //     std::cout << "Patient's Name: " << patientName << std::endl;
        // }
        // else
        //     std::cerr << "Error: cannot access Patient's Name!" << std::endl;
    } else {
        std::cerr << "Error: cannot read DICOM file " << this->_fileName.data() <<  " (" << status.text() << ")" << std::endl;
    }

    return metadata;
}
