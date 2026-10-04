target_sources(ayla PRIVATE

    # Lexer
    ${CMAKE_CURRENT_LIST_DIR}/lexer/LexerComment.cpp
    ${CMAKE_CURRENT_LIST_DIR}/lexer/LexerNewLine.cpp
    ${CMAKE_CURRENT_LIST_DIR}/lexer/MatchIdentifier.cpp
    ${CMAKE_CURRENT_LIST_DIR}/lexer/MatchNumber.cpp
    ${CMAKE_CURRENT_LIST_DIR}/lexer/MatchOperator.cpp
    ${CMAKE_CURRENT_LIST_DIR}/lexer/MatchString.cpp

    # Parser
    ${CMAKE_CURRENT_LIST_DIR}/parser/Parser.cpp
    ${CMAKE_CURRENT_LIST_DIR}/parser/ParserDeclaration.cpp
    ${CMAKE_CURRENT_LIST_DIR}/parser/ParserExpression.cpp
    ${CMAKE_CURRENT_LIST_DIR}/parser/ParserLiteral.cpp
    ${CMAKE_CURRENT_LIST_DIR}/parser/ParserName.cpp
    ${CMAKE_CURRENT_LIST_DIR}/parser/ParserPattern.cpp
    ${CMAKE_CURRENT_LIST_DIR}/parser/ParserStatement.cpp
    ${CMAKE_CURRENT_LIST_DIR}/parser/ParserType.cpp
)