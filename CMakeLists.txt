cmake_minimum_required(VERSION 3.3)
project(Chess)


# O projeto foi desenvolvido no windows mas deve, apesar de não ter sido testado, funcionar no linux
# existem alguns ifdefs da vida que servem pra ""garantir"" o multiplataforma. (ATENÇÃO PARA A VARIAVEL SFML_ROOT ABAIXO)
# O projeto deve ser compilado com as bibliotecas estáticas do cmake (assim o binário pode ser redistribuido)
# Como o projeto foi desenvolvido com o GCC 5 através do mingw os downloads "pré prontos"
# do SFML não serviam, por isso compilei o SFML na mão
# a partir do tutorial deles (que é bem fácil): http://www.sfml-dev.org/tutorials/2.3/compile-with-cmake.php
# Ou seja, para compilar o projeto na sua máquina basta compilar o SFML EM MODO ESTÁTICO (ver no tutorial como)
# e alterar a variavel SFML_ROOT abaixo para apontar para a sua instalação do SFML

#flags
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Werror")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -s -O2 -DNDEBUG  ")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -O0")
set(SFML_STATIC_LIBRARIES TRUE)


set(SFML_ROOT "C:\\Program Files (x86)\\SFML")

if (WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
endif ()

#sfml
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})


#diretórios de saida
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build/}")


# ordem dos componentes importa
find_package(SFML 2 REQUIRED graphics window system)

#executável principal
set(SOURCE_FILES
        src/main.cpp
        src/ResourceManager.cpp
        src/ResourceManager.hpp
        src/Pieces.cpp
        src/Pieces.hpp
        src/Board.cpp
        src/Board.hpp
        src/utils.hpp
        src/Square.cpp
        src/Square.hpp
        src/BoardState.cpp)

add_executable(Chess ${SOURCE_FILES})
set_target_properties(Chess PROPERTIES OUTPUT_NAME "Chess-${CMAKE_BUILD_TYPE}")

if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(Chess ${SFML_LIBRARIES})
    target_link_libraries(Chess ${SFML_DEPENDENCIES})
else ()
    message(SEND_ERROR "SFML library not found. Please set SFML_ROOT.")
endif ()