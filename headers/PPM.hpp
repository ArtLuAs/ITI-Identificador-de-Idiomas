#ifndef PPM_HPP
#define PPM_HPP

#include <string>
#include <vector>
#include "Avaliador.hpp"

// Compressor PPM-C
void compressPPM(const std::string& input, const std::string& outputFile, int order);
void decompressPPM(const std::string& inputFile, const std::string& outputFile, int order);

// Avaliador de idiomas
std::vector<ResultadoIdioma> avaliarTexto(
    const std::string& texto,
    const std::string& pastaModelos
);
std::string identificarIdioma(
    const std::string& texto,
    const std::string& pastaModelos
);

#endif // PPM_HPP
