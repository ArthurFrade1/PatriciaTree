#!/bin/bash

# Nome do arquivo executável
EXECUTAVEL="./exe"

# Nome do arquivo fonte C++
ARQUIVO_CPP="PatriciaTree.cpp"

# Compilar o código C++
if g++ "$ARQUIVO_CPP" -o "$EXECUTAVEL"; then
    # A compilação foi bem-sucedida
    echo "Compilação bem-sucedida. Executando o programa..."
    # Verificar se o executável existe antes de executá-lo
    if [ -f "$EXECUTAVEL" ]; then
        # Executar o programa
        "$EXECUTAVEL"
    else
        echo "Erro: O arquivo executável não foi encontrado."
    fi
else
    echo "Erro: Falha na compilação."
fi
