cmake_minimum_required(VERSION 3.5)
project(testLab)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(ROOT .)
set(SRC_DIR ${HOME_DIR/Src})
set(CMAKE_C_STANDARD 11)


add_custom_target(testLab_doc ALL DEPENDS)
add_custom_command(TARGET testLab_doc PRE_BUILD
    COMMAND noweave -n -delay testLab.nw > testLab.tex
    COMMAND pdflatex testLab.tex)

