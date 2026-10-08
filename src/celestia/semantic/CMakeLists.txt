target_sources(ayla PRIVATE

    # Collector
    ${CMAKE_CURRENT_LIST_DIR}/collector/SymbolCollector.cpp
    ${CMAKE_CURRENT_LIST_DIR}/collector/SymbolCollectorDeclaration.cpp
    ${CMAKE_CURRENT_LIST_DIR}/collector/SymbolCollectorPattern.cpp


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

    # Module
    ${CMAKE_CURRENT_LIST_DIR}/module/ModuleLookup.cpp

    # Checker
    ${CMAKE_CURRENT_LIST_DIR}/checker/TypeCheckerDeclarations.cpp
    ${CMAKE_CURRENT_LIST_DIR}/checker/TypeCheckerExpressions.cpp
    ${CMAKE_CURRENT_LIST_DIR}/checker/TypeCheckerStatements.cpp
    ${CMAKE_CURRENT_LIST_DIR}/checker/TypeCheckerPatterns.cpp
    ${CMAKE_CURRENT_LIST_DIR}/checker/TypeCheckerLiterals.cpp

    ${CMAKE_CURRENT_LIST_DIR}/checker/TypeInference.cpp
        ${CMAKE_CURRENT_LIST_DIR}/checker/TypeBuilder.cpp
    ${CMAKE_CURRENT_LIST_DIR}/checker/TypeResolution.cpp
    ${CMAKE_CURRENT_LIST_DIR}/checker/TypeChecker.cpp
    ${CMAKE_CURRENT_LIST_DIR}/checker/TypeSystem.cpp
)