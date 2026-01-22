# Desafio Dr. Tis

## Plataforma

Este projeto foi desenvolvido em sistema Linux Mint 21.2 Cinnamon. É esperado que as instruções deste arquivo também se apliquem a sistemas Ubuntu.

## Dependências

Este projeto usa as bibliotecas DCMTK para leitura de arquivos DICOM e Qt para interface, além de CMake para compilação. Para instalá-las, execute o comando:

```
$ sudo apt install libdcmtk-dev qt6-base-dev cmake
```

Mais informações sobre a instalação dessas ferramentas podem ser encontradas em:

- DCMTK: https://dicom.offis.de/en/dcmtk/dcmtk-tools/
- Qt: https://doc.qt.io/qt-6/get-and-install-qt.html
- CMake: https://cmake.org/cmake/help/latest/guide/tutorial/Before%20You%20Begin.html#getting-cmake

Foram usadas as versões 6 do Qt e 3.10 do CMake para a implementação deste projeto.

## Compilação e execução

Para compilar o projeto, primeiro se geram os arquivos CMake:

```
$ cmake -S <diretorio-raiz> -B <diretorio-de-build> -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

Por exemplo, o seguinte comando executado a partir do diretório `desafio_dr_tis`:

```
$ cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

Criará um subdiretório `build`, dentro do qual os seguintes comandos podem ser usados para compilar e executar o projeto, respectivamente:

```
$ make
$ ./desafio_dr_tis
```

## Uso

Ao executar o programa, será aberta uma janela de interface com quatro áreas:

- Botão de seleção de arquivo (superior esquerda)
- Visualização de imagem (central esquerda, vazia se não houver uma imagem válida carregada)
- Ajuste de valores window level e window width (inferior esquerda)
- Metadados (direita)

Ao clicar no botão "Open File", será aberta uma janela de navegação do sistema de arquivos para a seleção de um arquivo com extensão `.dcm` ou `.DCM`. Se for um arquivo válido, a área de imagem se preencherá, os valores de window level e window width serão inicializados e os metadados serão exibidos.

## Descrição da solução

Ao ser selecionado um arquivo DICOM, a biblioteca DCTMK é usada para ler o arquivo como um objeto DicomImage, que é convertido em um objeto QImage para ser exibido na interface. O objeto também é usado em conjunto com elementos da interface que alteram os valores window level e window width da imagem, alterando sua exibição dinamicamente. Também é usado um objeto DcmFileFormat para ler os metadados e exibi-los em um componente QTreeWidget de maneira hierárquica.
