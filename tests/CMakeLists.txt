find_package(GTest REQUIRED)

add_executable(GraphEngineTests
    TestMain.cpp
    Tensor/TestStorage.cpp
    # Add more test source files here
)

target_link_libraries(GraphEngineTests
  PRIVATE
  GraphEngine
  GTest::gtest
)

include(GoogleTest)
gtest_discover_tests(GraphEngineTests)