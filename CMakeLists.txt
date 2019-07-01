cmake_minimum_required(VERSION 3.5)
project(testLab)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(ROOT .)
set(SRC_DIR ${ROOT}/src)
set(CMAKE_C_STANDARD 11)

set(SRCS ${SRC_DIR}/instType.c ${SRC_DIR}/queue.c ${SRC_DIR}/list.c
    ${SRC_DIR}/wrapper.c ${SRC_DIR}/hashMap.c ${SRC_DIR}/pair.c)
set(HEADERS ${ROOT}/include)
add_executable(testLab ${SRCS})

project(unitTest)
link_libraries(cmocka)
add_executable(unitTest ${ROOT}/src/unitTest.c ${SRCS})
target_compile_definitions(unitTest PUBLIC _TEST_LAB_UNIT_TESTING_)

include_directories(${HEADERS})
