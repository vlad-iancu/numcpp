cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
add_library(numcpp numcpp/narray.cpp numcpp/refcount.cpp)
target_include_directories(numcpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_library(diagnostics benchmark/diagnostics.cpp)
target_include_directories(diagnostics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/benchmark)
target_link_libraries(diagnostics PUBLIC numcpp)

link_libraries(numcpp diagnostics)

add_executable(array_iteration_1 benchmark/array_iteration/array_iteration_1.cpp)
add_executable(register_storage_test benchmark/memory/memory_access.cpp)
add_executable(matrix_test benchmark/matrix_test.cpp)
add_executable(main benchmark/main.cpp)

target_compile_options(register_storage_test PUBLIC "-Wno-register")

set_target_properties(array_iteration_1 PROPERTIES COMPILE_FLAGS "-save-temps")
set_target_properties(register_storage_test PROPERTIES COMPILE_FLAGS "-save-temps")

#target_link_libraries(driver PUBLIC numcpp)
#target_link_libraries(matrix_v_ndarray_v1 PUBLIC numcpp)
#target_link_libraries(matrix_v_ndarray_v2 PUBLIC numcpp)


