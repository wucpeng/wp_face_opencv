cmake_minimum_required(VERSION 3.3)
cmake_policy(SET CMP0042 NEW)
set (CMAKE_CXX_STANDARD 11)
#set(CMAKE_VERBOSE_MAKEFILE ON) #显示具体的编译过程

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

project (wp_face_opencv)
find_package( OpenCV REQUIRED )

message(status " This is the CMAKE_CURRENT_SOURCE_DIR srcs:" ${CMAKE_CURRENT_SOURCE_DIR})

message(status " This is the OpenCV_INCLUDE_DIRS srcs:" ${OpenCV_INCLUDE_DIRS})
message(status " This is the OpenCV_LIBS srcs:" ${OpenCV_LIBS})
message(status " This is the OpenCV_VERSION srcs:" ${OpenCV_VERSION})
message(status " This is the CMAKE_JS_INC srcs:" ${CMAKE_JS_INC})
message(status " This is the CMAKE_JS_LIB srcs:" ${CMAKE_JS_LIB})
message(status " This is the CMAKE_JS_VERSION srcs:" ${CMAKE_JS_VERSION})

#指定头文件目录
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories(${CMAKE_JS_INC})
include_directories(./)	
include_directories(./cnn)

file(GLOB_RECURSE sources_CNN ./cnn/*.cpp ./cnn/*.h)

file(GLOB sources *.cc *.h)

set(server_STAT_SRCS
  ${sources}
  ${sources_CNN}
)

#指定静态和动态文件目录
#link_directories(./linux_so)

message(status " SRC srcs:" ${server_STAT_SRCS})
add_library(${PROJECT_NAME} SHARED ${server_STAT_SRCS} ${CMAKE_JS_SRC})

target_link_libraries(${PROJECT_NAME} curl)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
# Include Node-API wrappers
execute_process(COMMAND node -p "require('node-addon-api').include"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE NODE_ADDON_API_DIR
        )
string(REGEX REPLACE "[\r\n\"]" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})

target_include_directories(${PROJECT_NAME} PRIVATE ${NODE_ADDON_API_DIR})

# define NPI_VERSION
add_definitions(-DNAPI_VERSION=3)