project(texSynth)

cmake_minimum_required(VERSION 2.6)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# Specify the list of directories to search for cmake modules.
set(CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

find_package(Qt4 REQUIRED)
SET(QT_USE_QTOPENGL TRUE)
SET(QT_USE_QTXML TRUE)
SET(QT_USE_QTNETWORK TRUE)
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

find_package(OpenGL REQUIRED)
find_package(OpenEXR REQUIRED)

# CUDA 
set(CUDA_BUILD_CUBIN OFF)
set(CUDA_BUILD_TYPE  Device)
set(CUDA_NVCC_FLAGS "")

if(NOT CUDA_INSTALL_PREFIX)
  set(CUDA_INSTALL_PREFIX /usr/local/cuda)
endif()
find_package(Cuda REQUIRED)

include_directories(${CUDA_INSTALL_PREFIX}/include)
cuda_include_directories(
    ${CMAKE_BINARY_DIR}
    ${QT_INCLUDES}
    ${CUDA_INSTALL_PREFIX}/include
)
set(CUDA_NVCC_FLAGS -use_fast_math -m64 -arch sm_20)

# Macro for adding a cuda executable if cuda is found and a regular
# executable otherwise.
macro(_add_possibly_cuda_executable target)
    if(CUDA_FOUND)
        cuda_add_executable(${target} ${ARGN})
    else()
        add_executable(${target} ${ARGN})
    endif()
endmacro()


# Macro for adding a cuda library if cuda is found and a regular
# library otherwise.
macro(_add_possibly_cuda_library target)
    if(CUDA_FOUND)
        cuda_add_library(${target} ${ARGN})
    else()
        add_library(${target} ${ARGN})
    endif()
endmacro()

add_subdirectory(src)
