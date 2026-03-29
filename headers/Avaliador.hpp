#ifndef AVALIADOR_HPP
#define AVALIADOR_HPP

#include <string>
#include <vector>
#include <utility> // pair

// Resultado da avaliação de um texto contra um modelo
struct ResultadoIdioma {
    std::string idioma;   // ex: "pt", "en", "de"
    double bitsTotal;     // total de bits gerados pelo modelo para o texto
    double bpc;           // bits por caractere (bitsTotal / len(texto))
};

// Avalia o texto contra todos os modelos na pasta 'pastaModelos' (arquivos *.model)
// Retorna o vetor ordenado do menor BPC (mais provável) para o maior
std::vector<ResultadoIdioma> avaliarTexto(
    const std::string& texto,
    const std::string& pastaModelos
);

// Retorna apenas o idioma identificado (o primeiro do vetor acima)
std::string identificarIdioma(
    const std::string& texto,
    const std::string& pastaModelos
);

#endif // AVALIADOR_HPP
