# duckdb-toml

A DuckDB extension for reading TOML. It provides a single scalar function, `parse_toml(VARCHAR) → JSON`:

## Usage

```sql
D SELECT parse_toml('
    [server]
    host = "localhost"
    port = 5432
  ') AS cfg;
┌───────────────────────────────────────────────┐
│                      cfg                      │
│                     json                      │
├───────────────────────────────────────────────┤
│ {"server":{"host":"localhost","port":5432}}   │
└───────────────────────────────────────────────┘
```

The result is DuckDB's `JSON` logical type, so `->`, `->>`, `json_transform`, etc. work directly:

```sql
SELECT cfg->'server'->>'host',
       (cfg->'server'->'port')::INT,
       (cfg->>'deployed_at')::TIMESTAMPTZ
FROM ...;
```

### Notes

- TOML datetimes/dates/times become RFC 3339 strings, which cast cleanly via `::TIMESTAMPTZ`, `::DATE`, etc.
- Non-finite floats (`inf`, `-inf`, `nan` — legal in TOML, not in JSON) become strings that still cast to `DOUBLE`.
- Output keys are sorted alphabetically, not kept in source order. This behavior is comes from the toml++ library, which stores tables in a sorted map. This behavior complies with the TOML spec which states that tables are unordered.

Parsing is handled by a vendored copy of [toml++](https://github.com/marzer/tomlplusplus).

### Reading files, URLs, S3

By composition with `read_text` — local files, https, s3, and globs all work:

```sql
SELECT parse_toml(content) AS cfg FROM read_text('s3://bucket/config.toml');
```

## Building and testing

This repository is based on the [DuckDB extension template](https://github.com/duckdb/extension-template). There are no external dependencies:

```sh
git submodule update --init
GEN=ninja make release
make test
```

The build produces a duckdb shell with the extension loaded (`./build/release/duckdb`) and the loadable extension binary (`./build/release/extension/toml/toml.duckdb_extension`).
