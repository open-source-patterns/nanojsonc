message(STATUS "[Tests] BUILD_TESTS is ON — configuring test targets")

add_executable(test_object test_object.c)
target_link_libraries(test_object PRIVATE nanojsonc)
add_test(NAME nanoJSONc.TestObject COMMAND test_object)

add_executable(test_array test_array.c)
target_link_libraries(test_array PRIVATE nanojsonc)
add_test(NAME nanoJSONc.TestArray COMMAND test_array)
