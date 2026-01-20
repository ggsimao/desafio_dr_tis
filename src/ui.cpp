#include "../include/ui.h"
#include <algorithm>
#include <qtablewidget.h>

const double SCREEN_PADDING_SCALE = 0.75;

MainWindow::MainWindow(QWidget *parent) {
    QWidget *main_widget = new QWidget(this);
    setCentralWidget(main_widget);
    centralWidget()->setMinimumSize(0, 0);
    this->setMinimumSize(0, 0);

    QHBoxLayout *general_layout = new QHBoxLayout(main_widget);
    QWidget* image_widget = new QWidget();
    general_layout->addWidget(image_widget);
    QVBoxLayout *image_layout = new QVBoxLayout(image_widget);
    image_layout->setSizeConstraint(QLayout::SetMinimumSize);

    QToolBar* toolbar = new QToolBar();
    QPushButton *file_button = new QPushButton("Read File");
    connect(file_button, &QPushButton::clicked, this, &MainWindow::openFileDialog);
    toolbar->addWidget(file_button);
    image_layout->addWidget(toolbar);

    this->_label = new QLabel();
    this->_label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    image_layout->addWidget(this->_label);

    QGraphicsScene * graphic = new QGraphicsScene(image_widget);

    QRect const rec = QGuiApplication::primaryScreen()->geometry();
    this->_maxHeight = rec.height() * SCREEN_PADDING_SCALE;
    this->_maxWidth = rec.width() * SCREEN_PADDING_SCALE;

    QTableWidget *table_widget = new QTableWidget(1, 3);
    general_layout->addWidget(table_widget);
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
        const QImage image = generate_qimage(ImageData(fileNameArray));
        this->_setImage(image);
    }
}

void MainWindow::_setImage(QImage image) {
    int imageWidth = image.width();
    int imageHeight = image.height();
    double widthScale = std::min(this->_maxWidth, imageWidth) / double(imageWidth);
    double heightScale = std::min(this->_maxHeight, imageHeight) / double(imageHeight);
    double imageScale = std::min(widthScale, heightScale);

    this->_label->setMinimumSize(QSize(0, 0));
    this->_label->setPixmap(
        QPixmap::fromImage(image.scaled(int(imageWidth * imageScale), int(imageHeight * imageScale)))
    );
    this->_label->updateGeometry();
    resize(sizeHint());
    QTimer::singleShot(50, this, [this]() {
        this->resize(this->sizeHint());
    });
}
