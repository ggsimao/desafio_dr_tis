#pragma once

#include "../include/dicom.h"

#include <QScreen>
#include <QMainWindow>
#include <QPushButton>
#include <QFileDialog>
#include <QString>
#include <QApplication>
#include <QLabel>
#include <QPixmap>
#include <QDialog>
#include <QVBoxLayout>
#include <QToolBar>
#include <QImage>
#include <QGraphicsScene>
#include <QTimer>
#include <QTreeWidget>
#include <QSlider>
#include <QMessageBox>
#include <qwidget.h>

/*! \brief Gera um objeto QImage contendo a mesma imagem
    \param image Objeto ImageData contendo um objeto DicomImage, cujos dados são usados para criar um objeto QImage
    \return Objeto QImage contendo a mesma imagem. Se o parâmetro image for inválido, a imagem retornante é vazia.
 */
QImage generate_qimage(const ImageData& image);

/*! \brief Cria um novo objeto QTreeWidgetItem filho de um objeto QTreeWidgetItem já existente contendo os mesmos dados
    de um objeto MetadataNode, e seus objetos filhos recursivamente
    \param parent Objeto QTreeWidgetItem pai do novo objeto QTreeWidgetItem gerado
    \param child Objeto contendo os dados que preencherão o novo objeto QTreeWidgetItem
 */
void feed_tree_widget(QTreeWidgetItem* parent, MetadataNode child);

/*! \brief Widget que aglomera as funções de QSliders para ajuste de valores de window level e window width
    \param _level, _width Valores atuais de window level e window width, respectivamente
    \param _min, _max Valores mínimo e máximo, respectivamente, da imagem atual
    \param _levelValueLabel, _widthValueLabel Exibem para o usuário quais os valores atuais de window level e window
    width, respectivamente
    \param _levelSlider, _widthSlider Sliders que ajustam os valores de window level e window width, respectivamente
 */
class WindowLevelWidthWidget : public QWidget {
    Q_OBJECT
    public:
        WindowLevelWidthWidget(QWidget *parent = nullptr);
        virtual ~WindowLevelWidthWidget();
        /*! \brief Ajusta os valores de mínimo e máximo atuais, assim como os valores iniciais de window level e window
            width com base nos dados da imagem carregada
            \param _image Imagem DICOM carregada à interface
         */
        void setImage(ImageData& _image);

        // getters

        double getLevel();
        double getWidth();
    signals:
        void levelChanged(double level);
        void widthChanged(double width);
    private slots:
        // atualizam o estado intero do objeto e emitem os sinais levelChanged e widthChanged respectivamente

        void updateWindowLevel(double level);
        void updateWindowWidth(double width);
    private:
        double _level, _width, _min, _max;
        QLabel *_levelValueLabel, *_widthValueLabel;
        QSlider *_levelSlider, *_widthSlider;
};

/*! \brief Janela contenedora da aplicação. Possui botão para selecionar arquivo DICOM a ser carregado, área para
    exibição de imagem, sliders para ajuste de valores window level e window width, e exibição de hierarquia de
    metadados
    \param _imageLabel Área de exibição de imagem
    \param _wlw_widget Sliders de ajuste de window level e window width
    \param _metadataTree Área de visualização de metadados
    \param _imageData Dados da imagem lida de arquivo DICOM
    \param _maxHeight, _maxWidth Altura e largura máximas da janela
 */
class MainWindow : public QMainWindow {
    Q_OBJECT
    public:
        MainWindow(QWidget *parent = nullptr);
        virtual ~MainWindow();
    private slots:
        /*! \brief Abre uma janela navegadora do sistema de arquivos filtrando por arquivos de extensão ".dcm" ou
        ".DCM", carregando o arquivo selecionado em um atributo do tipo ImageData e atualizando, se o arquivo for
        válido, a imagem exibida, os valores de window level e window width, a árvore de metadados e as dimensões da
        janela
         */
        void openFileDialog();
        /*! \brief Altera o valor de window level da imagem carregada e atualiza sua exibição
         */
        void updateWindowLevel(int level);
        /*! \brief Altera o valor de window width da imagem carregada e atualiza sua exibição
         */
        void updateWindowWidth(int width);
    private:
        /*! \brief Atualiza a imagem exibida com a imagem carregada em _imageData
         */
        bool _updateImage();
        /*! \brief Atualiza a árvore de metadados com os dados do arquivo carregado em _imageData
         */
        void _updateMetadata();
        QLabel *_imageLabel;
        WindowLevelWidthWidget *_wlw_widget;
        QTreeWidget *_metadataTree;
        std::unique_ptr<ImageData> _imageData;
        int _maxHeight, _maxWidth;
};
