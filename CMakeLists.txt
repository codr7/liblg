cmake_minimum_required(VERSION 3.5)

project(liblg)
add_compile_options(-std=c11 -Wall -Werror)

file(GLOB_RECURSE src src/lg/*.c)

add_library(lib_static STATIC ${src})
set_target_properties(lib_static PROPERTIES OUTPUT_NAME lg)
target_include_directories(lib_static PUBLIC src/)

add_executable(test ${src} src/test.c)
target_compile_options(test PUBLIC -O2 -g)
target_include_directories(test PUBLIC src/)
target_link_libraries(test)

add_executable(liblg ${src} src/main.c)
target_compile_options(liblg PUBLIC -O2 -g)
target_include_directories(liblg PUBLIC src/)
target_link_libraries(liblg)

file(GLOB inc src/lg/*.h)
install(FILES ${inc} DESTINATION include/lg)
install(FILES build/liblg.a DESTINATION lib)
install(FILES build/liblg DESTINATION bin PERMISSIONS WORLD_EXECUTE)
