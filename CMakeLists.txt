# Set project information
cmake_minimum_required(VERSION 3.7.0)
project(svel LANGUAGES C CXX VERSION 0.0.1)

# Set CXX Requirements
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/;${CMAKE_MODULE_PATH}")
set(CMAKE_VERBOSE_MAKEFILE ON)

# Enable CTest
include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# Get CPM
set(CPM_DOWNLOAD_VERSION 0.27.2)
set(CPM_DOWNLOAD_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
    message(STATUS "Downloading CPM.cmake")
    file(DOWNLOAD https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
endif()
include(${CPM_DOWNLOAD_LOCATION})

#Dependencies
#------------------

# Vulkan, GLFW, GLM
find_package(Vulkan REQUIRED)
if (WIN32)
    set(GLFW_INCLUDE_DIRS "./deps/glfw/include")
    set(GLFW_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/deps/glfw/lib-vc2022/glfw3.lib")
else()
    find_package(PkgConfig REQUIRED)
    pkg_search_module(GLFW REQUIRED glfw3)
endif()

CPMAddPackage(
    NAME glm
    GITHUB_REPOSITORY g-truc/glm
    GIT_TAG 0.9.9.8
)

message(INFO "${glm_SOURCE_DIR}")
CPMAddPackage(
    NAME stb
    GITHUB_REPOSITORY nothings/stb
    GIT_TAG af1a5bc352164740c1cc1354942b1c6b72eacb8a
)
include_directories(SYSTEM ${stb_SOURCE_DIR})

CPMAddPackage(
    NAME RapidXML
    GITHUB_REPOSITORY discord/rapidxml
    GIT_TAG 2ae4b2888165a393dfb6382168825fddf00c27b9
)
#------------------

add_subdirectory(SVEL)