cmake_minimum_required(VERSION 2.8)

# Project name
project(QLScriptEngine)

option (WITH_LUA "Enables lua script engine" ON)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
)

set(HEADERS_LOCAL_TOMOC
)

set(HEADERS_LOCAL
	luaengine.h
        qlscriptengine.h
)

set(SOURCES_LOCAL
	luaengine.cpp
        qlscriptengine.cpp
)

if(WITH_LUA)
      find_package(Lua51 REQUIRED)
      include_directories(${LUA_INCLUDE_DIR})
      add_definitions(-DWITH_LUA)
endif(WITH_LUA)

#QT4_WRAP_CPP(MOC_LOCAL ${HEADERS_LOCAL_TOMOC})

source_group("qlscriptengine\\Sources" FILES 
				${HEADERS_LOCAL} 
				${HEADERS_LOCAL_TOMOC}
				${SOURCES_LOCAL})

add_library(QLScriptEngine STATIC
	${SOURCES_LOCAL}
	${MOC_LOCAL}
	${HEADERS_LOCAL}
	${HEADERS_LOCAL_TOMOC}
)



#target_link_libraries(Video
#     	${FFMPEG_LIBRARIES_STANDART}
#	${MACOSX_FRAMEWORK_LIBS}
#)
