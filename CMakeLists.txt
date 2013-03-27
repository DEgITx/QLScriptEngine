cmake_minimum_required(VERSION 2.8)

# Project name
project(QLScriptEngine)

option (WITH_LUA "Enables lua script engine" ON)
if(WIN32)
	option (BUILD_LUA "Enables lua self build" ON)
else()
	option (BUILD_LUA "Enables lua self build" OFF)
endif()


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

set(LINK_LIBRARIES_LOCAL
        ""
)

if(WITH_LUA)
	  if (BUILD_LUA)
			add_subdirectory(lua)
			include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/lua")
			set (LUA_LIBRARIES "lua")
		else (BUILD_LUA)
			find_package(Lua51 REQUIRED)
			include_directories(${LUA_INCLUDE_DIR})
		endif (BUILD_LUA)
		set(LINK_LIBRARIES_LOCAL ${LINK_LIBRARIES_LOCAL} ${LUA_LIBRARIES})
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



target_link_libraries(QLScriptEngine
        ${LINK_LIBRARIES_LOCAL}
)
