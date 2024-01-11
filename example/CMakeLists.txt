add_executable(example1 example1.c)
target_link_libraries(example1 PRIVATE nanojsonc)
add_test(NAME TestExample1 COMMAND example1)

add_executable(example2 example2.c)
target_link_libraries(example2 PRIVATE nanojsonc)
add_test(NAME TestExample2 COMMAND example2)

add_executable(example3 example3.c)
target_link_libraries(example3 PRIVATE nanojsonc)
configure_file("example3.json" "${CMAKE_BINARY_DIR}/example/example3.json" COPYONLY)
add_test(NAME TestExample3 COMMAND example3)