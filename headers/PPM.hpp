#ifndef PPM_HPP
#define PPM_HPP

#include <string>
#include <vector>
#include "Avaliador.hpp"

// ---- Compressor PPM-C ----
void compressPPM(const std::string& input, const std::string& outputFile, int order);
void decompressPPM(const std::string& inputFile, const std::string& outputFile, int order);

// ---- Interpretador (treino incremental) ----
// Treina (ou incrementa) o modelo de um idioma e salva o .model
void treinarModelo(
    const std::string& corpusFile,
    const std::string& arquivoModelo,
    int order = 5,
    bool verbose = true
);

// ---- Avaliador (identificação de idiomas) ----
std::vector<ResultadoIdioma> avaliarTexto(
    const std::string& texto,
    const std::string& pastaModelos
);
std::string identificarIdioma(
    const std::string& texto,
    const std::string& pastaModelos
);

#endif // PPM_HPP
