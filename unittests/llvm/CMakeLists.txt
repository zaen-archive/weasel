find_package(WEASEL)

add_executable(TestLLVM main.cpp)

target_link_libraries(TestLLVM ${WEASEL_LIBS})
