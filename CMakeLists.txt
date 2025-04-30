cmake_minimum_required(VERSION 3.28)
project(Omni-Snake-NEAT LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(cereal
    GIT_REPOSITORY https://github.com/USCiLab/cereal.git
    GIT_TAG v1.3.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL TRUE
)
FetchContent_Populate(cereal)
add_library(cereal INTERFACE)

target_include_directories(cereal INTERFACE ${cereal_SOURCE_DIR}/include)

set(OMNISNAKE_SOURCES
    src/OmniSnake/main.cpp
    src/OmniSnake/Controller.cpp
    src/OmniSnake/Renderer.cpp
    src/OmniSnake/snake.cpp
    src/OmniSnake/Source.cpp
)
set(OMNISNAKE_HEADERS
    src/OmniSnake/configuration.h
    src/OmniSnake/Controller.h
    src/OmniSnake/events.h
    src/OmniSnake/Renderer.h
    src/OmniSnake/rng.h
    src/OmniSnake/snake.h
    src/OmniSnake/utility.h
)

set(NEAT_SOURCES
    src/NEAT/Mutation.cpp
    src/NEAT/NeuralNetwork.cpp
    src/NEAT/Population.cpp
)
set(NEAT_HEADERS
    src/NEAT/Genome.h
    src/NEAT/Mutation.h
    src/NEAT/neat_config.h
    src/NEAT/NeuralNetwork.h
    src/NEAT/Population.h
)

add_executable(main
    ${OMNISNAKE_SOURCES}
    ${OMNISNAKE_HEADERS}
    ${NEAT_SOURCES}
    ${NEAT_HEADERS}
)

target_link_libraries(main PRIVATE sfml-graphics cereal)
target_compile_features(main PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET main
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:main>
        VERBATIM)
endif()
