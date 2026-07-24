cmake_minimum_required(VERSION 3.5)

# Set extension name here
set(TARGET_NAME toml)

set(EXTENSION_NAME ${TARGET_NAME}_extension)
set(LOADABLE_EXTENSION_NAME ${TARGET_NAME}_loadable_extension)

project(${TARGET_NAME})

set(CMAKE_CXX_STANDARD "17" CACHE STRING "C++ standard to enforce")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src/include)
# Vendored single-header toml++ (https://github.com/marzer/tomlplusplus)
include_directories(third_party/tomlplusplus)

set(EXTENSION_SOURCES src/toml_extension.cpp)

build_static_extension(${TARGET_NAME} ${EXTENSION_SOURCES})
build_loadable_extension(${TARGET_NAME} " " ${EXTENSION_SOURCES})

# toml++ requires C++17; DuckDB's build system defaults extensions to its own (older)
# standard, so force it at the target level
set_target_properties(${EXTENSION_NAME} ${LOADABLE_EXTENSION_NAME}
                      PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

# JSON output is serialized with DuckDB's embedded yyjson; the static extension gets it
# from DuckDB core, the loadable extension links it explicitly (same as extension/json)
target_link_libraries(${LOADABLE_EXTENSION_NAME} duckdb_yyjson)

install(
  TARGETS ${EXTENSION_NAME}
  EXPORT "${DUCKDB_EXPORT_SET}"
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}"
  ARCHIVE DESTINATION "${INSTALL_LIB_DIR}")
