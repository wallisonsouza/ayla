target_sources(ayla PRIVATE

    # AST
    ${CMAKE_CURRENT_LIST_DIR}/ast/DeclarationsDumper.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ast/ExpressionsDumper.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ast/LiteralsDumper.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ast/NameDumper.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ast/PatternsDumper.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ast/StatementsDumper.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ast/TopLevelDumper.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ast/TreeLayout.cpp

    # Lexer
    ${CMAKE_CURRENT_LIST_DIR}/lexer/TokenDumper.cpp
)