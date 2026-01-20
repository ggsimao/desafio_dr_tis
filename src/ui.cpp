#include "../include/ui.h"

MainWindow::MainWindow(QWidget *parent) {
    // QDialog dialog;
    QWidget *main_widget = new QWidget(this);
    setCentralWidget(main_widget);
    QVBoxLayout *layout = new QVBoxLayout(main_widget);

    QToolBar *toolbar = new QToolBar();
    QPushButton *file_button = new QPushButton("Read File");
    connect(file_button, &QPushButton::clicked, this, &MainWindow::openFileDialog);
    toolbar->addWidget(file_button);
    layout->addWidget(toolbar);

    this->_label = new QLabel();
    // QLabel *label = new QLabel();
    layout->addWidget(this->_label);

    QGraphicsScene * graphic = new QGraphicsScene(main_widget);

    // label->setPixmap(QPixmap::fromImage(show_image.scaled(image_read.getWidth() / 4, image_read.getHeight() / 4)));
    // label->resize(show_image.size());

    // label->setScaledContents(true);

    // dialog.show();
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
    this->_label->setPixmap(QPixmap::fromImage(image.scaled(image.width() / 4, image.height() / 4)));
    // label->resize(image.size());

    // this->_label->setScaledContents(true);
}
