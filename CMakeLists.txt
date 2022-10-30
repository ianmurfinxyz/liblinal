cmake_minimum_required(VERSION 3.20)
project(liblinal CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(LINAL_TEST_SOURCE test/test.cc)

include(conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

find_package(GTest)
enable_testing()

add_executable(linal_test ${LINAL_TEST_SOURCE})
target_include_directories(linal_test PUBLIC include ${CONAN_INCLUDE_DIRS})
target_link_directories(linal_test PUBLIC ${CONAN_LIB_DIRS})
target_link_libraries(linal_test ${CONAN_LIBS})

include(GoogleTest)
gtest_discover_tests(linal_test)

#target_link_libraries(${PROJECT_NAME} gtest::gtest)
