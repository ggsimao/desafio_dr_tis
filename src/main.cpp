#include "../include/dicom.h"
#include "../include/ui.h"

#include <iostream>

// #include "dcmtk/dcmdata/libi2d/i2d.h"
// #include "dcmtk/dcmdata/libi2d/i2djpgs.h"
// #include "dcmtk/dcmdata/libi2d/i2dplsc.h"

// #include "dcmtk/dcmdata/dcfilefo.h"
// #include "dcmtk/dcmdata/dcistrmb.h"


#include <QApplication>
#include <QLabel>
#include <QPixmap>
#include <QDialog>
#include <QVBoxLayout>
#include <QToolBar>
#include <qpixmap.h>
#include <qpushbutton.h>
#include <qtoolbar.h>
#include <QPushButton>
#include <QImage>
#include <QGraphicsScene>

int main(int argc, char** argv) {
    DCMTKCodecs dcmtk_codecs;

    // ImageData image_read = ImageData("/home/acerola/Desktop/Projects/desafio_dr_tis/data/anonymized_mamo.dcm");

    // QImage show_image = image_read.generate_qimage();

    // show_image = show_image.copy();
    // show_image.save("../output/file.jpg");


    QApplication a(argc, argv);

    // QDialog dialog;
    // QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // QToolBar *toolbar = new QToolBar();
    // QPushButton *file_button = new QPushButton("Read File");
    // toolbar->addWidget(file_button);
    // layout->addWidget(toolbar);

    // QLabel *label = new QLabel();
    // layout->addWidget(label);

    // QGraphicsScene * graphic = new QGraphicsScene(&dialog);

    // label->setPixmap(QPixmap::fromImage(show_image.scaled(image_read.getWidth() / 4, image_read.getHeight() / 4)));
    // label->resize(show_image.size());

    // label->setScaledContents(true);

    // dialog.show();

    MainWindow mw = MainWindow();

    mw.show();

    return a.exec();
}
