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

#include <QImage>

struct DCMTKCodecs {
    DCMTKCodecs();
    ~DCMTKCodecs();
};

class ImageData {
    public:
        ImageData(const char* file_name);
        virtual ~ImageData();
        bool isValid();
        int getWidth();
        int getHeight();
        uchar* getOutputData();
        std::map<std::string, std::string> getMetadata();
        QImage generate_qimage(); // TODO: tirar da classe
    private:
        std::string _fileName;
        DicomImage* _image;
        DcmFileFormat _fileFormat;
        bool _valid;
};
