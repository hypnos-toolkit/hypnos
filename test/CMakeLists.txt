cmake_minimum_required(VERSION 3.6)

set(REPO ~/ClionProjects/.repo)

project(HypnosTest)

include(CTest)
enable_testing()

include(${REPO}/DownloadProject/DownloadProject.cmake)
download_project(
        PROJ                googletest
        GIT_REPOSITORY      https://github.com/google/googletest.git
        GIT_TAG             master
        UPDATE_DISCONNECTED 1
)

add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)

set(TEST_SRCS hypnos_test.cpp)
add_executable(runUnitTests ${TEST_SRCS})
target_link_libraries(runUnitTests gtest gmock hypnos)