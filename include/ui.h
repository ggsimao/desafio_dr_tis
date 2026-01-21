#pragma once

#include <iostream>
#include "../include/dicom.h"
#include "../include/helpers.h"

#include <algorithm>
#include <cstddef>
#include <qtablewidget.h>

#include <QScreen>
#include <qmainwindow.h>
#include <qwidget.h>
#include <QMainWindow>
#include <QPushButton>
#include <QFileDialog>
#include <QString>
#include <QDebug>
#include <QApplication>
#include <QLabel>
#include <QPixmap>
#include <QDialog>
#include <QVBoxLayout>
#include <QToolBar>
#include <qpixmap.h>
#include <qtoolbar.h>
#include <QImage>
#include <QGraphicsScene>
#include <QTimer>
#include <QTableWidget>

class MainWindow : public QMainWindow {
    Q_OBJECT
    public:
        MainWindow(QWidget *parent = nullptr);
        virtual ~MainWindow();
    private slots:
        void openFileDialog();
    private:
        void _updateImage();
        QLabel *_label;
        QPushButton *_openButton;
        std::unique_ptr<ImageData> _imageData;
        int _maxHeight, _maxWidth;
};
