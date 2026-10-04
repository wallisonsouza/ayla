#!/bin/bash

set -e

# Garante a execução a partir do diretório raiz do projeto
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

export CC=clang
export CXX=clang++

# Opções de compilação e execução
BUILD_TYPE="Debug"
BUILD_ONLY=false
ENABLE_ASAN=false
RUN_ARGS=()

for arg in "$@"; do
    case "$arg" in
        --asan)
            ENABLE_ASAN=true
            ;;
        --release)
            BUILD_TYPE="Release"
            ;;
        --build-only|-b)
            BUILD_ONLY=true
            ;;
        *)
            RUN_ARGS+=("$arg")
            ;;
    esac
done

mkdir -p build
cd build

# Flags de compilação e avisos
if [ "$BUILD_TYPE" = "Release" ]; then
    C_FLAGS="-O3 -DNDEBUG"
    CXX_FLAGS="-O3 -DNDEBUG -Wall -Wextra -Wno-unused-parameter -Wno-mismatched-tags"
else
    C_FLAGS="-O0 -g"
    CXX_FLAGS="-O0 -g -Wall -Wextra -Wno-unused-parameter -Wno-mismatched-tags"
fi

if [ "$ENABLE_ASAN" = true ] || [ "${ASAN:-0}" = "1" ]; then
    echo "==> AddressSanitizer & UndefinedBehaviorSanitizer ativados"
    C_FLAGS="$C_FLAGS -fsanitize=address,undefined -fno-omit-frame-pointer"
    CXX_FLAGS="$CXX_FLAGS -fsanitize=address,undefined -fno-omit-frame-pointer"
fi

# Detecta ccache se disponível no sistema
CMAKE_LAUNCHERS=()
if command -v ccache &>/dev/null; then
    CMAKE_LAUNCHERS=(
        -DCMAKE_C_COMPILER_LAUNCHER=ccache
        -DCMAKE_CXX_COMPILER_LAUNCHER=ccache
    )
fi

cmake -G Ninja \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DCMAKE_C_FLAGS="$C_FLAGS" \
    -DCMAKE_CXX_FLAGS="$CXX_FLAGS" \
    "${CMAKE_LAUNCHERS[@]}" \
    ..

MAX_THREADS="${MAX_THREADS:-2}"
THREADS=$(nproc)

if [ "$THREADS" -gt "$MAX_THREADS" ]; then
    THREADS=$MAX_THREADS
fi

ninja -j "$THREADS"

if [ "$BUILD_ONLY" = true ]; then
    echo "==> Build concluído com sucesso!"
    exit 0
fi

if [ -t 1 ] && [ -n "$TERM" ]; then
    clear 2>/dev/null || true
fi

# Se nenhum argumento de execução foi passado, executa o padrão
if [ ${#RUN_ARGS[@]} -eq 0 ]; then
    ./ayla run ../src/Tests.ayla --dump ast
else
    ./ayla "${RUN_ARGS[@]}"
fi