#include "../include/helpers.h"

QImage generate_qimage(const ImageData& image) {
    const uchar* image_data = (uchar*)(image.getOutputData());

    if (image_data != NULL) {
        QImage::Format format;
        int channels = 1;
        if (image.isMonochrome()) {
            format = QImage::Format_Grayscale8;
        } else {
            format = QImage::Format_RGB888;
            channels = 3;
        }
        QImage show_image(
            image_data,
            image.getWidth(),
            image.getHeight(),
            image.getWidth() * channels,
            format
        );
        return show_image.copy();
    } else {
        // TODO
        std::cerr << "Image is null" << std::endl;
    }
}
