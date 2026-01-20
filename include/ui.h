#pragma once

#include <iostream>
#include "../include/dicom.h"

#include <QScreen>
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

class MainWindow : public QMainWindow {
    Q_OBJECT
    public:
        MainWindow(QWidget *parent = nullptr);
        virtual ~MainWindow();
    private slots:
        void openFileDialog();
    private:
        void _setImage(QImage image);
        QLabel *_label;
        QPushButton *_openButton;
        int _maxHeight, _maxWidth;
};
