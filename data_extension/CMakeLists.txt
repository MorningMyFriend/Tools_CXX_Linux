include_directories(${PROJECT_SOURCE_DIR}/data_extension/src)

aux_source_directory(src SOURCE_DATA_EX)

add_executable(main_data main_data.cpp ${SOURCE_DATA_EX})