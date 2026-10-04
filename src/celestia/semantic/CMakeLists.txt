target_sources(ayla PRIVATE

    # Collector
    ${CMAKE_CURRENT_LIST_DIR}/collector/SymbolCollector.cpp
    ${CMAKE_CURRENT_LIST_DIR}/collector/SymbolCollectorDeclaration.cpp
    ${CMAKE_CURRENT_LIST_DIR}/collector/SymbolCollectorPattern.cpp
    ${CMAKE_CURRENT_LIST_DIR}/collector/SymbolCollectorStatement.cpp

    # Resolver
    ${CMAKE_CURRENT_LIST_DIR}/resolver/declaration/ResolveModuleDeclaration.cpp
    ${CMAKE_CURRENT_LIST_DIR}/resolver/Resolver.cpp
    ${CMAKE_CURRENT_LIST_DIR}/resolver/ResolverDeclaration.cpp
    ${CMAKE_CURRENT_LIST_DIR}/resolver/ResolverExpression.cpp
    ${CMAKE_CURRENT_LIST_DIR}/resolver/ResolverGeneric.cpp
    ${CMAKE_CURRENT_LIST_DIR}/resolver/ResolverLiteral.cpp
    ${CMAKE_CURRENT_LIST_DIR}/resolver/ResolverName.cpp
    ${CMAKE_CURRENT_LIST_DIR}/resolver/ResolverPattern.cpp
    ${CMAKE_CURRENT_LIST_DIR}/resolver/ResolverType.cpp
    ${CMAKE_CURRENT_LIST_DIR}/resolver/ResolveStatement.cpp

    # Scope / Symbol
    ${CMAKE_CURRENT_LIST_DIR}/scope/ScopeLookup.cpp
    ${CMAKE_CURRENT_LIST_DIR}/symbol/SymbolTable.cpp

    # Enum
    ${CMAKE_CURRENT_LIST_DIR}/enum/EnumChecker.cpp
    ${CMAKE_CURRENT_LIST_DIR}/enum/EnumCollector.cpp
    ${CMAKE_CURRENT_LIST_DIR}/enum/EnumResolver.cpp

    # Function
    ${CMAKE_CURRENT_LIST_DIR}/function/FunctionChecker.cpp

    # Module
    ${CMAKE_CURRENT_LIST_DIR}/module/ModuleLookup.cpp

    # Checker
    ${CMAKE_CURRENT_LIST_DIR}/checker/declarations/CheckModuleDeclaration.cpp
    ${CMAKE_CURRENT_LIST_DIR}/checker/declarations/CheckStructDeclaration.cpp
    ${CMAKE_CURRENT_LIST_DIR}/checker/declarations/CheckVariableDeclaration.cpp

    ${CMAKE_CURRENT_LIST_DIR}/checker/expressions/CheckBinary.cpp
    ${CMAKE_CURRENT_LIST_DIR}/checker/expressions/CheckerExpression.cpp
    ${CMAKE_CURRENT_LIST_DIR}/checker/expressions/CheckPattern.cpp
    ${CMAKE_CURRENT_LIST_DIR}/checker/expressions/Literals.cpp

    ${CMAKE_CURRENT_LIST_DIR}/checker/statements/CheckReturnStatement.cpp

    ${CMAKE_CURRENT_LIST_DIR}/checker/Inference.cpp
    ${CMAKE_CURRENT_LIST_DIR}/checker/InferExpression.cpp
    ${CMAKE_CURRENT_LIST_DIR}/checker/InferLiteral.cpp
    ${CMAKE_CURRENT_LIST_DIR}/checker/TypeChecker.cpp
    ${CMAKE_CURRENT_LIST_DIR}/checker/TypeSystem.cpp
)