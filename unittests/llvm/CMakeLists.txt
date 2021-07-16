find_package(WeaselCmake)

add_executable(TestLLVM main.cpp)

target_link_libraries(TestLLVM ${WEASEL_LIBS})
