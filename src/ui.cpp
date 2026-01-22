#include "../include/ui.h"
#include <qtreewidget.h>

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
        return QImage();
    }
}

void feed_tree_widget(QTreeWidgetItem* parent, MetadataNode child) {
    QStringList labels;
    labels << child.getTag().data() << child.getTagName().data() << child.getValue().data();
    QTreeWidgetItem *child_item = new QTreeWidgetItem(labels);
    parent->addChild(child_item);
    for (int i = 0; i < child.childCount(); i++) {
        feed_tree_widget(child_item, child.getChild(i));
    }
}

// ----------------------------------------------------------------------------------------------------

WindowLevelWidthWidget::WindowLevelWidthWidget(QWidget* parent) {
    QVBoxLayout* root_layout = new QVBoxLayout(this);

    QHBoxLayout* level_layout = new QHBoxLayout();
    QLabel* level_label = new QLabel();
    level_label->setText("Window level: ");
    this->_levelSlider = new QSlider(Qt::Orientation::Horizontal);
    connect(this->_levelSlider, &QSlider::valueChanged, this, &WindowLevelWidthWidget::updateWindowLevel);
    this->_levelValueLabel = new QLabel();
    level_layout->addWidget(level_label);
    level_layout->addWidget(this->_levelSlider);
    level_layout->addWidget(this->_levelValueLabel);

    QHBoxLayout* width_layout = new QHBoxLayout();
    QLabel* width_label = new QLabel();
    width_label->setText("Window width: ");
    this->_widthSlider = new QSlider(Qt::Orientation::Horizontal);
    connect(this->_widthSlider, &QSlider::valueChanged, this, &WindowLevelWidthWidget::updateWindowWidth);
    this->_widthValueLabel = new QLabel();
    width_layout->addWidget(width_label);
    width_layout->addWidget(this->_widthSlider);
    width_layout->addWidget(this->_widthValueLabel);

    root_layout->addLayout(level_layout);
    root_layout->addLayout(width_layout);
}

WindowLevelWidthWidget::~WindowLevelWidthWidget() {}

double WindowLevelWidthWidget::getLevel() {
    return this->_level;
}

double WindowLevelWidthWidget::getWidth() {
    return this->_width;
}

void WindowLevelWidthWidget::updateWindowLevel(double level) {
    this->_level = level;
    this->_levelValueLabel->setText(QString::number((int)this->_level));
    emit levelChanged(level);
}

void WindowLevelWidthWidget::updateWindowWidth(double width) {
    this->_width = width;
    this->_widthValueLabel->setText(QString::number((int)this->_width));
    emit widthChanged(width);
}

void WindowLevelWidthWidget::setImage(ImageData& image) {
    image.getMinMaxValues(this->_min, this->_max);
    this->_levelSlider->setRange(this->_min, this->_max);
    this->_widthSlider->setRange(1, this->_max - this->_min);

    image.getWindowLevelWidth(this->_level, this->_width);
    this->_levelSlider->setValue(this->_level);
    this->_widthSlider->setValue(this->_width);

    this->_levelValueLabel->setText(QString::number((int)this->_level));
    this->_widthValueLabel->setText(QString::number((int)this->_width));
}

// ----------------------------------------------------------------------------------------------------

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

    this->_wlw_widget = new WindowLevelWidthWidget(left_widget);
    connect(this->_wlw_widget, &WindowLevelWidthWidget::levelChanged,
        this, &MainWindow::updateWindowLevel);
    connect(this->_wlw_widget, &WindowLevelWidthWidget::widthChanged,
        this, &MainWindow::updateWindowWidth);
    left_layout->addWidget(this->_wlw_widget);

    // ---------------------------------------------------------------

    QWidget* right_widget = new QWidget();
    QVBoxLayout *right_layout = new QVBoxLayout(right_widget);
    right_layout->setSizeConstraint(QLayout::SetMinimumSize);
    general_layout->addWidget(right_widget);

    QLabel* right_text = new QLabel();
    right_text->setText("Metadata:");
    right_layout->addWidget(right_text);
    _metadataTree= new QTreeWidget();
    _metadataTree->setColumnCount(3);
    right_layout->addWidget(_metadataTree);
    QStringList labels;
    labels << "Group, Element" << "Tag description" << "Value";
    _metadataTree->setHeaderLabels(labels);

    // ---------------------------------------------------------------

    QRect const rec = QGuiApplication::primaryScreen()->geometry();
    // escalonar por valores arbitrários, estimando o tamanho final da janela
    this->_maxHeight = rec.height() * 0.8;
    this->_maxWidth = rec.width() * 0.6;
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
        this->_wlw_widget->setImage(*this->_imageData);

        // se imagem for válida, muda nome da janela
        if (this->_updateImage()) {
            QString new_title = QString("desafio_dr_tis - ") + fileName;
            this->setWindowTitle(new_title);
        }

        this->_updateMetadata();

        // atualiza tamanho da janela
        this->_label->setMinimumSize(QSize(0, 0));
        this->_label->updateGeometry();
        resize(sizeHint());
        QTimer::singleShot(50, this, [this]() {
            this->resize(this->sizeHint());
        });
    }
}

void MainWindow::updateWindowLevel(int level) {
    if (this->_imageData->getStatus().type != ImageDataStatus::READY || this->_imageData == nullptr) {
        return;
    }
    this->_imageData->setWindowLevelWidth(level, this->_wlw_widget->getWidth());
    this->_updateImage();
}

void MainWindow::updateWindowWidth(int width) {
    if (this->_imageData->getStatus().type != ImageDataStatus::READY || this->_imageData == nullptr) {
        return;
    }
    this->_imageData->setWindowLevelWidth(this->_wlw_widget->getLevel(), width);
    this->_updateImage();
}

bool MainWindow::_updateImage() {
    if (this->_imageData->getStatus().type != ImageDataStatus::READY || this->_imageData == nullptr) {
        this->_label->clear();
        return false;
    }
    const QImage image = generate_qimage(*this->_imageData);

    int imageWidth = image.width();
    int imageHeight = image.height();
    double widthScale = std::min(this->_maxWidth, imageWidth) / double(imageWidth);
    double heightScale = std::min(this->_maxHeight, imageHeight) / double(imageHeight);
    double imageScale = std::min(widthScale, heightScale); // para caber na tela

    this->_label->setPixmap(
        QPixmap::fromImage(image.scaled(int(imageWidth * imageScale), int(imageHeight * imageScale)))
    );

    return true;
}

void MainWindow::_updateMetadata() {
    this->_metadataTree->clear();
    if (this->_imageData->getStatus().type != ImageDataStatus::READY || this->_imageData == nullptr) {
        return;
    }
    std::vector<MetadataNode> metadata = this->_imageData->getMetadata();
    for (int i = 0; i < metadata.size(); i++) {
        MetadataNode node = metadata[i];
        QStringList labels;
        labels << node.getTag().data() << node.getTagName().data() << node.getValue().data();
        QTreeWidgetItem *top_level_item = new QTreeWidgetItem(labels);
        this->_metadataTree->addTopLevelItem(top_level_item);
        for (int j = 0; j < node.childCount(); j++) {
            feed_tree_widget(top_level_item, node.getChild(j));
        }
    }
}
