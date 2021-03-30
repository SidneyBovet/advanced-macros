set(TARGET schema)

set(SCHEMAS
    src/actions.fbs)

set(SCHEMA_HEADERS
    "${CMAKE_CURRENT_BINARY_DIR}/schema")

build_flatbuffers(
    "${SCHEMAS}"        # flatbuffers_schemas
    "src"               # schema_include_dirs
    ${TARGET}           # custom_target_name
    ""                  # additional_dependencies
    "${SCHEMA_HEADERS}" # generated_includes_dir
    ""                  # binary_schemas_dir
    "${CMAKE_BINARY_DIR}/bin/Debug") # copy_text_schemas_dir