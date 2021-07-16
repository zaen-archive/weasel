find_package(WeaselCmake)

add_executable(TestLex main.cpp)

target_link_libraries(TestLex ${WEASEL_LIBS})
