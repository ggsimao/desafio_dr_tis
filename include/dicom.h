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

#include <dcmtk/dcmimgle/didocu.h>
#include <dcmtk/dcmimage/dicoimg.h>
#include <dcmtk/dcmimgle/diutils.h>
#include <utility>

struct DCMTKCodecs {
    DCMTKCodecs();
    ~DCMTKCodecs();
};

enum ImageDataStatus {
    EMPTY,
    INVALID,
    READY,
};

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
        std::map<std::string, std::string> getMetadata();
    private:
        ImageDataStatus _status;
        std::string _fileName;
        std::unique_ptr<DicomImage> _image;
        DcmFileFormat _fileFormat;
};
