include_directories(${PROJECT_SOURCE_DIR}/3rdParty/jsoncpp/include)

link_directories(${PROJECT_SOURCE_DIR}/3rdParty/jsoncpp/lib)

add_executable(main_json main_json.cpp)
target_link_libraries(main_json jsoncpp)

#add_executable(main_xmlvoc main_gen_xmlvoc.cpp Markup.cpp detectionWrapper.cpp)