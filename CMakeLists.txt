cmake_minimum_required(VERSION 2.8)
project(Tools_CXX_Linux)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# 选择要编译的功能模块
option(ALGORITHM_EXTENSION "if build algorithm_extension module" ON)
option(CV_EXTENSION "if build cv_extension module" ON)
option(DATA_EXTENSION "if build data_extension module" ON)
option(IO_EXTENSION "if build io_extension module" ON)
option(DESIGN_PATTERN "if build design patterns example" ON)

if(ALGORITHM_EXTENSION)
    add_subdirectory(algorithm_extension)
endif()

if (CV_EXTENSION)
    set(OpenCV_DIR /home/wurui/opencv/opencv-3.3.0/distribute)
    find_package(OpenCV 3.3.0 REQUIRED)
    add_subdirectory(cv_extension)
endif ()

if(DATA_EXTENSION)
    add_subdirectory(data_extension)
endif()

if (IO_EXTENSION)
    add_subdirectory(io_extension)
endif()

if (DESIGN_PATTERN)
    add_subdirectory(design_pattern)
endif()