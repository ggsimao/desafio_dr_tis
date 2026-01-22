#pragma once

#include <iostream>
#include <map>

// #include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include <dcmtk/dcmjpeg/djdecode.h>
#include <dcmtk/dcmjpeg/djencode.h>
#include <dcmtk/dcmdata/dcrledrg.h>
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmimgle/dcmimage.h"
#include <dcmtk/ofstd/ofcond.h>
#include <dcmtk/ofstd/ofstring.h>

#include <dcmtk/dcmimgle/didocu.h>
#include <dcmtk/dcmimage/dicoimg.h>
#include <dcmtk/dcmimgle/diutils.h>
#include <utility>

struct DCMTKCodecs {
    DCMTKCodecs();
    ~DCMTKCodecs();
};

struct ImageDataStatus {
    enum ImageDataStatusType {EMPTY, INVALID, READY} type;
    std::string message;
};

class MetadataNode {
    public:
        MetadataNode(std::string tag, std::string tag_name, std::string value);
        ~MetadataNode();
        void addChild(MetadataNode child);
        MetadataNode getChild(int index);
        int childCount();
        std::string getTag();
        std::string getTagName();
        std::string getValue();
    private:
        std::string _tag, _tagName, _value;
        std::vector<MetadataNode> _children;
};

MetadataNode generate_metadata_subtree(DcmElement* element);

class ImageData {
    public:
        ImageData(const std::string fileName);
        virtual ~ImageData();

        // getters
        ImageDataStatus getStatus() const;
        int getWidth() const;
        int getHeight() const;
        bool isMonochrome() const;

        // processam informação do arquivo
        uchar* getOutputData() const;
        std::vector<MetadataNode> getMetadata();
        void getMinMaxValues(double &min, double &max) const;
        void getWindowLevelWidth(double &level, double &width);

        // manipulam atributos
        void setWindowLevelWidth(double level, double width);
    private:
        ImageDataStatus _status;
        std::string _fileName;
        std::unique_ptr<DicomImage> _image;
        DcmFileFormat _fileFormat;
};
