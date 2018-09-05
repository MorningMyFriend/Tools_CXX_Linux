include_directories(${PROJECT_SOURCE_DIR}/io_extension/src)
#target_link_libraries(${PROJECT_SOURCE_DIR}/lib)

aux_source_directory(src SOURCE_IO_EX)

#add_library(ioEx SHARED ${SOURCE_IO_EX})

add_executable(main_io main_io.cpp ${SOURCE_IO_EX})
#target_link_libraries(main_io ioEx)