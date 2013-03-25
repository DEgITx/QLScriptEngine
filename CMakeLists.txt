cmake_minimum_required(VERSION 2.8)

# Project name
project(QLScriptEngine)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
)

set(HEADERS_LOCAL_TOMOC
)

set(HEADERS_LOCAL
)

set(SOURCES_LOCAL
)

QT4_WRAP_CPP(MOC_LOCAL ${HEADERS_LOCAL_TOMOC})

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
