include_directories(${PROJECT_SOURCE_DIR}/cv_extension/src)
target_link_libraries(${PROJECT_SOURCE_DIR}/lib)

aux_source_directory(src SOURCE_CV_EX)

add_executable(main_cv main_cv.cpp ${SOURCE_CV_EX})
target_link_libraries(main_cv ${OpenCV_LIBS})