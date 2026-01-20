#include "../include/ui.h"
#include <algorithm>

const double SCREEN_PADDING_SCALE = 0.75;

MainWindow::MainWindow(QWidget *parent) {
    QWidget *main_widget = new QWidget(this);
    setCentralWidget(main_widget);
    QVBoxLayout *layout = new QVBoxLayout(main_widget);

    QToolBar *toolbar = new QToolBar();
    QPushButton *file_button = new QPushButton("Read File");
    connect(file_button, &QPushButton::clicked, this, &MainWindow::openFileDialog);
    toolbar->addWidget(file_button);
    layout->addWidget(toolbar);

    this->_label = new QLabel();
    layout->addWidget(this->_label);

    QGraphicsScene * graphic = new QGraphicsScene(main_widget);

    QRect const rec = QGuiApplication::primaryScreen()->geometry();
    this->_maxHeight = rec.height() * SCREEN_PADDING_SCALE;
    this->_maxWidth = rec.width() * SCREEN_PADDING_SCALE;
}

MainWindow::~MainWindow() {}

void MainWindow::openFileDialog() {
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open DICOM File"),
        "..",
        tr("DICOM Files (*.dcm);;")
    );

    if (!fileName.isEmpty()) {
        const char* fileNameArray = fileName.toLocal8Bit().data();
        QImage image = ImageData(fileNameArray).generate_qimage();
        this->_setImage(image);
    }
}

void MainWindow::_setImage(QImage image) {
    int imageWidth = image.width();
    int imageHeight = image.height();
    double widthScale = std::min(this->_maxWidth, imageWidth) / double(imageWidth);
    double heightScale = std::min(this->_maxHeight, imageHeight) / double(imageHeight);
    double imageScale = std::min(widthScale, heightScale);

    this->_label->setPixmap(
        QPixmap::fromImage(image.scaled(int(imageWidth * imageScale), int(imageHeight * imageScale)))
    );
    this->resize(this->sizeHint());
}
