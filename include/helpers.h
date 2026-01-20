#pragma once

#include "../include/dicom.h"

#include "dcmtk/dcmimgle/dcmimage.h"
#include "dcmtk/dcmimage/diregist.h"

#include <QImage>

QImage generate_qimage(const ImageData& image);
