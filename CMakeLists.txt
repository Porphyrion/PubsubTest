cmake_minimum_required(VERSION 3.14)

project(pubsub LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

find_package(cppzmq)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Publisher")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Subscriber")


