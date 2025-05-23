
cmake_minimum_required( VERSION 3.0 )

project( YOLOV7 )
enable_language( CUDA )
find_package(CUDA)
set( CMAKE_C_STANDARD 99 )
set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g -fpic -fpie -fpermissive  -std=c++11 -pthread" )

find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS})
find_package(jsoncpp CONFIG REQUIRED)



# global include_directories
include_directories( /usr/local/cuda/include )
#add judgement about system:

MESSAGE(STATUS "CMAKE_HOST_SYSTEM_PROCESSOR is ${CMAKE_HOST_SYSTEM_PROCESSOR}")

if (${CMAKE_HOST_SYSTEM_PROCESSOR} EQUAL aarch64)
    include_directories( /usr/include/aarch64-linux-gnu/ ) # for jetson
elseif(${CMAKE_HOST_SYSTEM_PROCESSOR} EQUAL x86_64)
    include_directories( /usr/lib/x86_64-linux-gnu/ )
endif()

include_directories( "${CMAKE_SOURCE_DIR}/src/" )
include_directories( "/usr/include/jsoncpp/")
# global definitions
add_definitions( -w)

# global library path
if (${CMAKE_HOST_SYSTEM_PROCESSOR} EQUAL aarch64)
    link_directories( "/usr/lib/aarch64-linux-gnu/" )
elseif(${CMAKE_HOST_SYSTEM_PROCESSOR} EQUAL x86_64)
    link_directories( "/usr/lib/x86_64-linux-gnu/" )
endif()

link_directories( "/usr/lib/" )
link_directories( "/usr/local/lib/")
link_directories( "/usr/local/cuda/lib64/" )

FILE(GLOB_RECURSE YOLO_SRC src/*.cpp )
add_library( yolo SHARED ${YOLO_SRC} )
target_link_libraries(yolo PRIVATE nvinfer)
target_link_libraries(yolo PRIVATE nvinfer_plugin)
target_link_libraries(yolo PRIVATE nvparsers)
target_link_libraries(yolo PRIVATE nvonnxparser cudart ${OpenCV_LIBS})

add_executable(detect samples/detect.cpp )
target_link_libraries(detect yolo cudart ${OpenCV_LIBS} )

add_executable(video_detect samples/video_detect.cpp )
target_link_libraries(video_detect yolo cudart ${OpenCV_LIBS} )

add_executable(validate_coco samples/validate_coco.cpp )
target_link_libraries(validate_coco yolo cudart ${OpenCV_LIBS} )
target_link_libraries(validate_coco jsoncpp)
