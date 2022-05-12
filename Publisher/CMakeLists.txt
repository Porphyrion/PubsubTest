add_executable(publisher
  main.cpp
  Publisher.cpp
  publisherapp.cpp
  DataProducer.cpp
)

target_link_libraries(publisher Qt${QT_VERSION_MAJOR}::Core cppzmq Boost::atomic)
