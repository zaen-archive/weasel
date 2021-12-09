find_package(WEASEL)

add_executable(weasel driver.cpp)

target_link_libraries(weasel ${WEASEL_LIBS})
