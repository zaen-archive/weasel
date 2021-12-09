find_package(WEASEL)

add_executable(TestLex main.cpp)

target_link_libraries(TestLex ${WEASEL_LIBS})
