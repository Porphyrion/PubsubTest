cmake_minimum_required(VERSION 3.14)

project(pubsub LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

find_package(cppzmq)

add_executable(pubsub
  main.cpp
  Publisher.cpp
  Subscriber.cpp
)

target_link_libraries(pubsub Qt${QT_VERSION_MAJOR}::Core cppzmq)

