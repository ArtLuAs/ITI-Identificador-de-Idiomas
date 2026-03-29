#ifndef INTERPRETADOR_HPP
#define INTERPRETADOR_HPP

#include <string>

// Treina o modelo PPM-C com o corpus informado e salva em arquivoModelo.
//
// Se arquivoModelo já existir, o modelo é carregado antes do treino
// (aprendizado incremental: aprende, salva, aprende, salva...).
//
// Parâmetros:
//   corpusFile   - caminho para o arquivo de texto de treinamento
//   arquivoModelo- caminho de saída/entrada do arquivo .model
//   order        - ordem k do PPM (padrão recomendado: 5)
//   verbose      - imprime progresso a cada bloco processado
void treinarModelo(
    const std::string& corpusFile,
    const std::string& arquivoModelo,
    int order = 5,
    bool verbose = true
);

#endif // INTERPRETADOR_HPP
