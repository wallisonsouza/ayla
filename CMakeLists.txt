cmake_minimum_required(VERSION 3.16)
project(ayla LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE  SOURCES
    src/*.cpp
)

add_executable(ayla
    main.cpp
    ${SOURCES}
)

target_include_directories(ayla PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)

# ===================== ICU =====================
find_package(ICU REQUIRED COMPONENTS uc i18n)
target_link_libraries(ayla PRIVATE ICU::uc ICU::i18n)

add_custom_target(run
    COMMAND ayla
    DEPENDS ayla
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)


