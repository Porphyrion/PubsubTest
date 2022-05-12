add_executable(subscriber
  main.cpp
  Subscriber.cpp
)

target_link_libraries(subscriber Qt${QT_VERSION_MAJOR}::Core cppzmq)