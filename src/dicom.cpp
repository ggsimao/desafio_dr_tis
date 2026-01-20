#include "../include/dicom.h"
#include <dcmtk/dcmimgle/dcmimage.h>
#include <utility>


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

    this->_image = new DicomImage(fileNameData);
    std::cout << this->_image->getWidth() << ", " << this->_image->getHeight() << std::endl;

    OFCondition status = this->_fileFormat.loadFile(fileNameData);
    if (status.good())
    {
        std::cout << "Successfully read file " << fileNameData << std::endl;
        _valid = true;
    }
    else {
        std::cerr << "Error: cannot read DICOM file " << fileNameData <<  " (" << status.text() << ")" << std::endl;
        _valid = false;
    }
}

ImageData::~ImageData() {
    delete this->_image;
}

bool ImageData::isValid() {
    return this->_valid;
}

int ImageData::getHeight() {
    return this->_image->getHeight();
}

int ImageData::getWidth() {
    return this->_image->getWidth();
}

uchar* ImageData::getOutputData() {
    return (uchar*)(this->_image->getOutputData(8));
}

std::map<std::string, std::string> ImageData::getMetadata() {
    std::map<std::string, std::string> metadata;

    if (this->_valid) {
        OFString patientName;
        DcmDataset *dataset = this->_fileFormat.getDataset();
        if (dataset->findAndGetOFString(DCM_PatientName, patientName).good())
        {
            metadata.emplace(std::make_pair("Patient's Name", patientName));
            std::cout << "Patient's Name: " << patientName << std::endl;
        }
        else
            std::cerr << "Error: cannot access Patient's Name!" << std::endl;
    }

    return metadata;
}

// TODO: TIRAR DAQUI
QImage ImageData::generate_qimage() {
    uchar* image_data = (uchar*)(this->_image->getOutputData(8));

    QImage show_image(
        image_data,
        this->_image->getWidth(),
        this->_image->getHeight(),
        this->_image->getWidth(),
        QImage::Format_Indexed8
    );


    QVector<QRgb> table(256);
    for (int i = 0; i < 256; ++i)
        table[i] = qRgb(i, i, i);

    show_image.setColorTable(table);

    return show_image.copy();
}
