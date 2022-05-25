add_executable(test-subscriber
  main.cpp
  Subscriber.cpp Subscriber.h
  SubsciberApp.h SubsciberApp.cpp
  CommandLineParserSub.h CommandLineParserSub.cpp
)

target_link_libraries(test-subscriber Qt${QT_VERSION_MAJOR}::Core cppzmq)
