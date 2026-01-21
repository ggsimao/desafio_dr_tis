#include "../include/ui.h"

const double SCREEN_PADDING_SCALE = 0.75;

MainWindow::MainWindow(QWidget *parent) {
    QWidget *main_widget = new QWidget(this);
    setCentralWidget(main_widget);
    centralWidget()->setMinimumSize(0, 0);
    this->setMinimumSize(0, 0);

    QHBoxLayout *general_layout = new QHBoxLayout(main_widget);

    // ---------------------------------------------------------------

    QWidget* left_widget = new QWidget();
    general_layout->addWidget(left_widget);
    QVBoxLayout *left_layout = new QVBoxLayout(left_widget);
    left_layout->setSizeConstraint(QLayout::SetMinimumSize);

    QToolBar* toolbar = new QToolBar();
    QPushButton *file_button = new QPushButton("Read File");
    connect(file_button, &QPushButton::clicked, this, &MainWindow::openFileDialog);
    toolbar->addWidget(file_button);
    left_layout->addWidget(toolbar);

    this->_label = new QLabel();
    this->_label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    left_layout->addWidget(this->_label);

    QGraphicsScene * graphic = new QGraphicsScene(left_widget);

    QHBoxLayout* level_layout = new QHBoxLayout(left_widget);
    QLabel* level_label = new QLabel();
    level_label->setText("Window level: ");
    QSlider* level_widget = new QSlider(Qt::Orientation::Horizontal);
    level_layout->addWidget(level_label);
    level_layout->addWidget(level_widget);
    QHBoxLayout* width_layout = new QHBoxLayout(left_widget);
    QLabel* width_label = new QLabel();
    width_label->setText("Window width: ");
    QSlider* width_widget = new QSlider(Qt::Orientation::Horizontal);
    width_layout->addWidget(width_label);
    width_layout->addWidget(width_widget);
    left_layout->addLayout(level_layout);
    left_layout->addLayout(width_layout);

    // ---------------------------------------------------------------

    QWidget* right_widget = new QWidget();
    QVBoxLayout *right_layout = new QVBoxLayout(right_widget);
    right_layout->setSizeConstraint(QLayout::SetMinimumSize);
    general_layout->addWidget(right_widget);

    QLabel* right_text = new QLabel();
    right_text->setText("Metadados:");
    right_layout->addWidget(right_text);
    QTableWidget *table_widget = new QTableWidget(0, 3);
    right_layout->addWidget(table_widget);
    QStringList labels;
    labels << "Group" << "Tag description" << "Value";
    table_widget->setHorizontalHeaderLabels(labels);

    // ---------------------------------------------------------------

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
        tr("DICOM Files (*.dcm *.DCM);;")
    );

    if (!fileName.isEmpty()) {
        const char* fileNameArray = fileName.toLocal8Bit().data();
        this->_imageData = std::make_unique<ImageData>(fileNameArray);
        this->_updateImage();
    }
}

void MainWindow::_updateImage() {
    if (this->_imageData->getStatus() != READY || this->_imageData == nullptr) {
        return;
    }
    const QImage image = generate_qimage(*this->_imageData);

    int imageWidth = image.width();
    int imageHeight = image.height();
    double widthScale = std::min(this->_maxWidth, imageWidth) / double(imageWidth);
    double heightScale = std::min(this->_maxHeight, imageHeight) / double(imageHeight);
    double imageScale = std::min(widthScale, heightScale);

    this->_label->setPixmap(
        QPixmap::fromImage(image.scaled(int(imageWidth * imageScale), int(imageHeight * imageScale)))
    );
    this->_label->setMinimumSize(QSize(0, 0));
    this->_label->updateGeometry();
    resize(sizeHint());
    QTimer::singleShot(50, this, [this]() {
        this->resize(this->sizeHint());
    });
}
