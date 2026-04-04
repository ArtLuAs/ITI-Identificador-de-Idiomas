@echo off
set idioma=pt

echo --- Rodando o main Python ---
python tools\main.py %idioma%

echo --- Compilando o C++ ---
g++ main.cpp src\*.cpp -o interpretador.exe

if %errorlevel% == 0 (
    echo Compilacao concluida!
    echo --- Executando o C++ ---
    interpretador.exe --treinar "corpora\%idioma%.txt" "modelos\%idioma%.model" 4
) else (
    echo Erro na compilacao. Abortado.
)