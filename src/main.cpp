#include "../include/dicom.h"
#include "../include/ui.h"

#include <QApplication>

int main(int argc, char** argv) {
    // Habilita (de)codecs para leitura e escrita de arquivos DICOM, desligado ao final da função.
    DCMTKCodecs dcmtk_codecs;

    QApplication a(argc, argv);

    MainWindow mw = MainWindow();
    mw.show();

    return a.exec();
}
