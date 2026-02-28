# sat-sim/simcore/tests/CMakeLists.txt
cmake_minimum_required(VERSION 3.20)

# Lightweight approach: each test is a tiny executable, registered with CTest.
# (No googletest required.)

function(add_sim_test TEST_NAME TEST_SRC)
  add_executable(${TEST_NAME} ${TEST_SRC})
  target_link_libraries(${TEST_NAME} PRIVATE simcore)
  target_compile_features(${TEST_NAME} PRIVATE cxx_std_20)
  add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endfunction()

add_sim_test(test_orbit   ${CMAKE_CURRENT_SOURCE_DIR}/vec3_test.cpp)
#add_sim_test(test_attitude ${CMAKE_CURRENT_SOURCE_DIR}/test_attitude.cpp)
#add_sim_test(test_frames  ${CMAKE_CURRENT_SOURCE_DIR}/test_frames.cpp)