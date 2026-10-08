cmake_minimum_required(VERSION 3.28)

project(ayla LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(AYLA_APPLICATION_SOURCES
    src/ayla/AylaMain.cpp

    src/ayla/comand/CommandLine.cpp

    src/ayla/language/AylaLanguage.cpp
    src/ayla/language/Operators.cpp
    src/ayla/language/Tokens.cpp
)


set(AYLA_COMPILER_SOURCES
    src/celestia/compiler/CompilationRules.cpp
    src/celestia/compiler/Compiler.cpp
)

set(AYLA_LOWERING_SOURCES

    # Declarations
    src/celestia/lowering/declaration/LowerCapabilityDeclaration.cpp
    src/celestia/lowering/declaration/LowerDeclarations.cpp
    src/celestia/lowering/declaration/LowerStructDeclaration.cpp
    src/celestia/lowering/declaration/LowerVariableDeclaration.cpp

    # Expressions
    src/celestia/lowering/expression/LowerExpression.cpp
    src/celestia/lowering/expression/LowerLiteral.cpp

    # Patterns
    src/celestia/lowering/pattern/LowerPattern.cpp

    # Types
    src/celestia/lowering/type/LowerType.cpp

    # Core
    src/celestia/lowering/LoweringDeclaration.cpp
)


set(AYLA_IR_SOURCES
    src/celestia/ir/IRDumper.cpp
)


set(AYLA_TRANSPILER_SOURCES
src/celestia/diagnostic/formatter/TypeFormatter.cpp
    src/celestia/transpiler/CGenerator.cpp
    src/celestia/transpiler/declaration/CStructDeclarationGen.cpp
)


set(AYLA_INFRASTRUCTURE_SOURCES

    # Files
    src/celestia/file/FileDiscovery.cpp

    # Diagnostics
    src/celestia/diagnostic/text/LineCutter.cpp
    src/celestia/diagnostic/text/MarkerBuilder.cpp
)


add_executable(ayla
    main.cpp

    ${AYLA_APPLICATION_SOURCES}
    ${AYLA_COMPILER_SOURCES}
    ${AYLA_LOWERING_SOURCES}
    ${AYLA_IR_SOURCES}
    ${AYLA_TRANSPILER_SOURCES}
    ${AYLA_INFRASTRUCTURE_SOURCES}
)

add_subdirectory(src/celestia/semantic)
add_subdirectory(src/celestia/syntax)
add_subdirectory(src/debug)
# -------------------------------------------------------------
# Includes
# -------------------------------------------------------------

target_include_directories(ayla PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)

find_package(ICU REQUIRED COMPONENTS uc i18n)

target_link_libraries(ayla PRIVATE
    ICU::uc
    ICU::i18n
)


# -------------------------------------------------------------
# Run
# -------------------------------------------------------------

add_custom_target(run
    COMMAND ayla
    DEPENDS ayla
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)