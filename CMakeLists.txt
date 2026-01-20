cmake_minimum_required(VERSION 3.10)

project(desafio_dr_tis VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_SHARED_MODULE_PREFIX "")

include(GNUInstallDirs)

set(TIS_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(TIS_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

find_package(DCMTK REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Gui Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_library(DCMTK::DCMTK INTERFACE IMPORTED)
target_link_libraries(DCMTK::DCMTK INTERFACE ${DCMTK_LIBRARIES})
target_include_directories(DCMTK::DCMTK INTERFACE ${DCMTK_INCLUDE_DIRS})

add_executable(desafio_dr_tis
    ${TIS_SOURCE_DIR}/main.cpp
    ${TIS_SOURCE_DIR}/dicom.cpp
    ${TIS_SOURCE_DIR}/ui.cpp
    ${TIS_INCLUDE_DIR}/ui.h
    ${TIS_SOURCE_DIR}/helpers.cpp
)
target_link_libraries(desafio_dr_tis PRIVATE DCMTK::DCMTK)
target_link_libraries(desafio_dr_tis PRIVATE Qt6::Gui Qt6::Widgets)
