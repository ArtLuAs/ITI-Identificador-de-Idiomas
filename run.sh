#!/bin/bash

# =============================================
# Idioma a ser processado.
# Altere APENAS esta linha para trocar o idioma.
# =============================================
idioma="pt"

# 1. Executa o main Python passando o idioma como argumento
echo "--- Rodando o main Python (idioma: $idioma) ---"
python3 tools/main.py "$idioma"

echo -e "\n--- Compilando o código C++ ---"
# 2. Compila o main.cpp
g++ main.cpp ./src/*.cpp -o interpretador

# 3. Verifica se a compilação foi bem-sucedida
if [ $? -eq 0 ]; then
    echo "Compilação concluída com sucesso!"
    echo -e "\n--- Executando o programa C++ ---"

    # Usa a mesma variável $idioma definida acima
    ./interpretador --treinar "corpora/${idioma}.txt" "modelos/${idioma}.model" 4
else
    echo "Erro na compilação do C++. A execução foi abortada."
fi
