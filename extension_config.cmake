# This file is included by DuckDB's build system. It specifies which extension to load

# Extension from this repo
duckdb_extension_load(toml
    SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}
)

# Any extra extensions that should be built
# json is included so tests can exercise ->, ->> and friends on parse_toml's output
duckdb_extension_load(json)