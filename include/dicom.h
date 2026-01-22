#pragma once

#include "dcmtk/dcmdata/dcfilefo.h"
#include <dcmtk/dcmjpeg/djdecode.h>
#include <dcmtk/dcmjpeg/djencode.h>
#include <dcmtk/dcmdata/dcrledrg.h>
#include "dcmtk/dcmimgle/dcmimage.h"
#include <dcmtk/ofstd/ofcond.h>
#include <dcmtk/ofstd/ofstring.h>

#include <dcmtk/dcmimgle/didocu.h>
#include <dcmtk/dcmimage/dicoimg.h>
#include <dcmtk/dcmimgle/diutils.h>

/*! \brief Estrutura que utiliza princípio RAII para manter ativos os codecs DICOM da biblioteca DCMTK durante a
    execução do programa
 */
struct DCMTKCodecs {
    DCMTKCodecs();
    ~DCMTKCodecs();
};

/*! \brief Estrutura que representa estados de validade de um objeto do tipo ImageData, carregando consigo uma mensagem
    de erro caso o estado seja inválido por algum motivo
    \param type Estado de validade do objeto
    \param message Mensagem associada ao estado de validade
 */
struct ImageDataStatus {
    enum ImageDataStatusType {EMPTY, INVALID, READY} type;
    std::string message;
};

/*! \brief Representa um nodo na árvore de metadados
    \param _tag Grupo e elemento do metadado
    \param _tagName Nome do metadado
    \param _value Valor do metadado
    \param _children Lista de nodos filhos
 */
class MetadataNode {
    public:
        MetadataNode(std::string tag, std::string tag_name, std::string value);
        ~MetadataNode();
        void addChild(MetadataNode child);
        MetadataNode getChild(int index);
        int childCount();
        std::string getTag();
        std::string getTagName();
        std::string getValue();
    private:
        std::string _tag, _tagName, _value;
        std::vector<MetadataNode> _children;
};

/*! \brief Gera um objeto MetadataNode equivalente ao objeto element, assim como um para todos os elementos filhos, de
    forma recursiva, automaticamente conectando-os
    \param element Um objeto contendo tag, nome da tag e valor do metadado, usado para criar um objeto do tipo
    MetadataNode
    \return Um nodo em uma árvore de metadados, já com seus filhos em seu atributo _children
 */
MetadataNode generate_metadata_subtree(DcmElement* element);

/*! \brief Armazena diversos objetos relacionados a um arquivo DICOM, necessários para extrair a imagem e seus metadados
    \param _status Estado de validade do objeto
    \param _fileName Nome do arquivo lido
    \param _image A imagem extraída do arquivo
    \param _fileFormat Acessa os dados do arquivo, como metadados, window level e window width; também usado para
    extrair a imagem
 */
class ImageData {
    public:
        ImageData(const std::string fileName);
        virtual ~ImageData();

        // getters

        ImageDataStatus getStatus() const;

        // wrappers de getters de DicomImage

        int getWidth() const;
        int getHeight() const;
        bool isMonochrome() const;
        uchar* getOutputData() const;
        void getMinMaxValues(double &min, double &max) const;

        // processam informação do arquivo

        /*! \brief Gera uma lista de metadados da imagem, cada um com sua própria hierarquia de dados no formato de uma
            árvore
            \return Lista de metadados
         */
        std::vector<MetadataNode> getMetadata();
        /*! \brief Lê os valores de window level e window width do arquivo. Se ausentes, calcula valores baseados nos
            valores de mínimo e máximo
            \param level, width Retornam os valores de window level e window width, respectivamente
         */
        void getWindowLevelWidth(double &level, double &width);

        // manipulam atributos

        /*! \brief Altera os valores de window level e window width da imagem
            \param level Novo valor de window level
            \param width Novo valor de window width
         */
        void setWindowLevelWidth(double level, double width);
    private:
        ImageDataStatus _status;
        std::string _fileName;
        std::unique_ptr<DicomImage> _image;
        DcmFileFormat _fileFormat;
};
