include_directories(${PROJECT_SOURCE_DIR}/design_pattern/src)

aux_source_directory(src SOURCE_DESIGN_PATTERN)

add_executable(main_consumer main_consumer.cpp ${SOURCE_DESIGN_PATTERN})