#!/bin/bash

# 1. Executa o script Python na pasta tools
echo "--- Rodando o script Python ---"
python3 tools/script.py

# Se você também tiver o main.py na raiz para rodar, basta adicionar a linha abaixo:
# python3 main.py

echo -e "\n--- Compilando o código C++ ---"
# 2. Compila o main.cpp e gera um executável chamado 'programa_cpp' dentro da pasta interpretador
g++ interpretador/main.cpp -o interpretador/programa_cpp

# 3. Verifica se a compilação foi bem-sucedida (código de saída 0 significa sucesso)
if [ $? -eq 0 ]; then
    echo "Compilação concluída com sucesso!"
    echo -e "\n--- Executando o programa C++ ---"
    
    # Executa o programa gerado
    ./interpretador/programa_cpp
else
    # Se houver erro de sintaxe no C++, o script avisa e não tenta rodar
    echo "Erro na compilação do C++. A execução foi abortada."
fi