#pragma once

#include <iostream>
#include "../include/dicom.h"

#include <algorithm>
#include <cstddef>
#include <qtreewidget.h>
#include <string>
#include "dcmtk/dcmimgle/dcmimage.h"
#include "dcmtk/dcmimage/diregist.h"

#include <QScreen>
#include <qmainwindow.h>
#include <qtmetamacros.h>
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
#include <QImage>
#include <qpixmap.h>
#include <qtoolbar.h>
#include <QImage>
#include <QGraphicsScene>
#include <QTimer>
#include <QTreeWidget>
#include <QSlider>

QImage generate_qimage(const ImageData& image);
void feed_tree_widget(QTreeWidgetItem* parent, MetadataNode child);

class WindowLevelWidthWidget : public QWidget {
    Q_OBJECT
    public:
        WindowLevelWidthWidget(QWidget *parent = nullptr);
        virtual ~WindowLevelWidthWidget();
        void setImage(ImageData& _image);
        double getLevel();
        double getWidth();
    signals:
        void levelChanged(double level);
        void widthChanged(double width);
    private slots:
        void updateWindowLevel(double level);
        void updateWindowWidth(double width);
    private:
        double _level, _width, _min, _max;
        QLabel *_levelValueLabel, *_widthValueLabel;
        QSlider *_levelSlider, *_widthSlider;
};

class MainWindow : public QMainWindow {
    Q_OBJECT
    public:
        MainWindow(QWidget *parent = nullptr);
        virtual ~MainWindow();
    private slots:
        void openFileDialog();
        void updateWindowLevel(int level);
        void updateWindowWidth(int width);
    private:
        bool _updateImage();
        void _updateMetadata();
        QLabel *_label;
        QPushButton *_openButton;
        WindowLevelWidthWidget *_wlw_widget;
        QTreeWidget *_metadataTree;
        std::unique_ptr<ImageData> _imageData;
        int _maxHeight, _maxWidth;
};
