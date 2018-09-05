include_directories(${PROJECT_SOURCE_DIR}/algorithm_extension/src)
#target_link_libraries(${PROJECT_SOURCE_DIR}/lib)

aux_source_directory(src SOURCE_ALGORITHM_EX)

#add_library(algorithmEx ${SOURCE_ALGORITHM_EX})

add_executable(main_algorithm main_algorithm.cpp ${SOURCE_ALGORITHM_EX})
#target_link_libraries(main_algorithm algorithmEx)