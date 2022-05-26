add_executable(test-publisher
  main.cpp
  Publisher.cpp Publisher.h
  PublisherApp.cpp PublisherApp.h
  DataProducer.cpp DataProducer.h
  CommandLineParser.h CommandLineParser.cpp
  ChannelManager.h ChannelManager.cpp
  PublisherManager.h PublisherManager.cpp
)

target_link_libraries(test-publisher Qt${QT_VERSION_MAJOR}::Core cppzmq)
