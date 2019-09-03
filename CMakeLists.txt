cmake_minimum_required(VERSION 3.5)
project(testLab)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(ROOT .)
set(SRC_DIR ${ROOT}/src)
set(CMAKE_C_STANDARD 11)

set(SRCS
  ${SRC_DIR}/instType.c
  ${SRC_DIR}/queue.c
  ${SRC_DIR}/list.c
  ${SRC_DIR}/wrapper.c
  ${SRC_DIR}/hashMap.c
  ${SRC_DIR}/pair.c)

set(HEADERS ${ROOT}/include)
add_executable(testLab ${SRCS})

project(unitTest)
link_libraries(cmocka)
add_executable(unitTest ${ROOT}/src/unitTest.c ${SRCS})
target_compile_definitions(unitTest PUBLIC _TEST_LAB_UNIT_TESTING_)

project(interpreter)
set(I_SRC_DIR ${ROOT}/interpreter)
set(I_AST_SRC_DIR ${I_SRC_DIR}/ast-tree)
set(I_MOD_SRC_DIR ${I_SRC_DIR}/internalModule)
set(I_SRCS
  ${I_SRC_DIR}/hashTypes.c
  ${I_AST_SRC_DIR}/parameter.c
  ${I_AST_SRC_DIR}/expression.c
  ${I_AST_SRC_DIR}/primitive.c
  ${I_AST_SRC_DIR}/variable.c
  ${I_AST_SRC_DIR}/scope.c
  ${I_AST_SRC_DIR}/object.c
  ${I_AST_SRC_DIR}/func.c
  ${I_AST_SRC_DIR}/statement.c
  ${I_AST_SRC_DIR}/program.c
  ${I_AST_SRC_DIR}/array.c
  ${I_SRC_DIR}/parser.c
  ${I_SRC_DIR}/lexer.c
  ${I_MOD_SRC_DIR}/module.c
  ${I_SRC_DIR}/misc.c)

set(I_MOD_SRCS
  ${I_MOD_SRC_DIR}/basicModule.c)

add_executable(interpreter_integration ${SRCS} ${I_SRCS} ${I_MOD_SRCS} ${I_SRC_DIR}/iTest.c)

project(interpreterTest)
link_libraries(cmocka)
add_executable(interpreterTest ${I_AST_SRC_DIR}/unitTest.c ${I_SRCS} ${SRCS} ${I_MOD_SRCS})
target_compile_definitions(interpreterTest PUBLIC _AST_TREE_TESTING_)

add_custom_target(interpreter ALL DEPENDS)
add_custom_target(interpreter_H ALL DEPENDS)
add_custom_command(TARGET interpreter PRE_BUILD
  COMMAND flex -o ./interpreter/lexer.c ./interpreter/tScript.l)
add_custom_command(TARGET interpreter PRE_BUILD
  COMMAND bison -o ./interpreter/parser.c ./interpreter/tScript.y)
add_custom_command(TARGET interpreter_H PRE_BUILD
  COMMAND bison -d -o ./interpreter/parser.c ./interpreter/tScript.y)

include_directories(${HEADERS} ${I_SRC_DIR} ${I_AST_SRC_DIR} ${I_MOD_SRC_DIR})
