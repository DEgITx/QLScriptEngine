project(QLScriptEngineExample)
cmake_minimum_required(VERSION 2.8)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}/qlscriptengine
	${CMAKE_CURRENT_SOURCE_DIR}/qlscriptengine/lua
)

add_subdirectory(qlscriptengine)

aux_source_directory(. SRC_LIST)

add_definitions(-DWITH_LUA)

add_executable(
    ${PROJECT_NAME}
    ${SRC_LIST}
)

target_link_libraries(
    ${PROJECT_NAME}
    QLScriptEngine
)

