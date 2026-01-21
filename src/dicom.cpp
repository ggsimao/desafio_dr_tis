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

ImageData::ImageData(const std::string fileName) : _fileName(fileName) {
    const char* fileNameData = this->_fileName.data();

    std::cout << fileNameData << std::endl;

    OFCondition status = this->_fileFormat.loadFile(this->_fileName.data());

    DcmDataset *dataset = this->_fileFormat.getDataset();
    this->_image = std::make_unique<DicomImage>(dataset, EXS_Unknown, CIF_MayDetachPixelData);
    if (this->_image == NULL) {
        this->_status = EMPTY;
    } else if (this->_image->getStatus() == EIS_Normal) {
        this->_status = READY;
        std::cout << this->_image->getWidth() << ", " << this->_image->getHeight() << std::endl;
    } else {
        // TODO
        this->_status = INVALID;
        std::cerr << "Error: cannot read DICOM file " <<
        fileNameData <<  " (" << DicomImage::getString(this->_image->getStatus()) << ")" << std::endl;
    }
}

ImageData::~ImageData() {}

ImageDataStatus ImageData::getStatus() const {
    return this->_status;
}

int ImageData::getHeight() const {
    if (this->_status != READY) {
        return 0;
    } else
        return this->_image->getHeight();
}

int ImageData::getWidth() const {
    if (this->_status != READY) {
        return 0;
    } else
        return this->_image->getWidth();
}

bool ImageData::isMonochrome() const {
    if (this->_status != READY) {
        return false;
    } else
        return this->_image->isMonochrome();
}

uchar* ImageData::getOutputData() const {
    if (this->_status != READY) {
        return nullptr;
    } else
        return (uchar*)(this->_image->getOutputData(8));
}

std::map<std::string, std::string> ImageData::getMetadata() {
    std::map<std::string, std::string> metadata;

    if (this->_status != READY) {
        return metadata;
    }

    OFCondition status = this->_fileFormat.loadFile(this->_fileName.data());
    if (status.good()) {
        OFString patientName;
        DcmDataset *dataset = this->_fileFormat.getDataset();
    } else {
        std::cerr << "Error: cannot read DICOM file " << this->_fileName.data() <<  " (" << status.text() << ")" << std::endl;
    }

    return metadata;
}
