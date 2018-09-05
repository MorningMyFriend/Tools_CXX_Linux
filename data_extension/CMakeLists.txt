include_directories(${PROJECT_SOURCE_DIR}/data_extension/src)
target_link_libraries(${PROJECT_SOURCE_DIR}/lib)

aux_source_directory(src SOURCE_DATA_EX)

#add_library(dataEx SHARED ${SOURCE_DATA_EX})

add_executable(main_data main_data.cpp ${SOURCE_DATA_EX})
#target_link_libraries(main_data dataEx)