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

MetadataNode::MetadataNode(std::string tag, std::string tag_name, std::string value)
    : _tag(tag), _tagName(tag_name), _value(value) {}

MetadataNode::~MetadataNode() {}

void MetadataNode::addChild(MetadataNode child) {
    this->_children.push_back(child);
}

MetadataNode MetadataNode::getChild(int index) {
    return this->_children[index];
}

int MetadataNode::childCount() {
    return this->_children.size();
}


std::string MetadataNode::getTag() {
    return this->_tag;
}

std::string MetadataNode::getTagName() {
    return this->_tagName;
}

std::string MetadataNode::getValue() {
    return this->_value;
}

MetadataNode generate_metadata_subtree(DcmElement* element) {
    // gera o nodo raiz da sub-árvore
    OFString value;
    DcmTag tag = element->getTag();
    element->getOFStringArray(value);
    MetadataNode node = MetadataNode(tag.toString(), tag.getTagName(), value);

    // se nodo tem filhos, gera uma sub-árvore pra cada um
    if (!element->isLeaf()) {
        DcmSequenceOfItems* seq = dynamic_cast<DcmSequenceOfItems*>(element);
        for (int i = 0; i < seq->card(); i++) {
            DcmItem *item = seq->getItem(i);
            item->getTag();
            for (int j = 0; j < item->card(); j++) {
                DcmElement* e = item->getElement(i);
                node.addChild(generate_metadata_subtree(e));
            }
        }
    }
    return node;
}

ImageData::ImageData(const std::string fileName) : _fileName(fileName) {
    const char* fileNameData = this->_fileName.data();

    OFCondition status = this->_fileFormat.loadFile(this->_fileName.data());

    DcmDataset *dataset = this->_fileFormat.getDataset();
    if (!status.good()) {
        this->_status = ImageDataStatus{ImageDataStatus::INVALID, status.text()};
    } else {
        this->_image = std::make_unique<DicomImage>(dataset, EXS_Unknown, CIF_MayDetachPixelData);
        if (this->_image == NULL) {
            this->_status = ImageDataStatus{ImageDataStatus::EMPTY, "Image is empty!"};
        } else if (this->_image->getStatus() == EIS_Normal) {
            this->_status = ImageDataStatus{ImageDataStatus::READY, "Image is ready."};
        } else {
            this->_status = ImageDataStatus{
                ImageDataStatus::INVALID,
                DicomImage::getString(this->_image->getStatus())
            };
        }
    }
}

ImageData::~ImageData() {}

ImageDataStatus ImageData::getStatus() const {
    return this->_status;
}

int ImageData::getHeight() const {
    if (this->_status.type != ImageDataStatus::READY) {
        return 0;
    } else
        return this->_image->getHeight();
}

int ImageData::getWidth() const {
    if (this->_status.type != ImageDataStatus::READY) {
        return 0;
    } else
        return this->_image->getWidth();
}

bool ImageData::isMonochrome() const {
    if (this->_status.type != ImageDataStatus::READY) {
        return false;
    } else
        return this->_image->isMonochrome();
}

uchar* ImageData::getOutputData() const {
    if (this->_status.type != ImageDataStatus::READY) {
        return nullptr;
    } else
        return (uchar*)(this->_image->getOutputData(8));
}

std::vector<MetadataNode> ImageData::getMetadata() {
    std::vector<MetadataNode> metadata;

    if (this->_status.type != ImageDataStatus::READY) {
        return metadata;
    } else {
        DcmDataset *dataset = this->_fileFormat.getDataset();
        for (int i = 0; i < dataset->card(); i++) {
            DcmElement *e = dataset->getElement(i);
            OFString value;
            DcmTag tag = e->getTag();
            e->getOFStringArray(value);
            metadata.push_back(generate_metadata_subtree(e));
        }
    }

    return metadata;
}

void ImageData::getMinMaxValues(double &min, double &max) const {
    this->_image->getMinMaxValues(min, max);
}

void ImageData::getWindowLevelWidth(double &level, double &width) {
    DcmDataset* dataset = this->_fileFormat.getDataset();
    OFCondition level_status = dataset->findAndGetFloat64(DcmTagKey(0x0010, 0x1050), level);
    OFCondition width_status = dataset->findAndGetFloat64(DcmTagKey(0x0010, 0x1051), width);
    if (level_status.good() && width_status.good()) return;

    // Se o arquivo não tem valores iniciais para window level e window width, calcula-os
    double min, max;
    this->getMinMaxValues(min, max);
    if (!level_status.good()) {
        level = (max + min) / 2;
    }
    if (!width_status.good()) {
        width = max - min;
    }
}

void ImageData::setWindowLevelWidth(double level, double width) {
    this->_image->setWindow(level, width);
}
