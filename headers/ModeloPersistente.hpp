#ifndef MODELOPERSISTENTE_HPP
#define MODELOPERSISTENTE_HPP

#include <string>
#include "ContextModel.hpp"

// Salva a Trie inteira de um ContextModel em arquivo binário
void salvarModelo(const ContextModel& model, const std::string& arquivoSaida);

// Carrega a Trie de um arquivo binário e retorna o ContextModel reconstruído
// Lança std::runtime_error se o arquivo não puder ser aberto ou estiver corrompido
ContextModel carregarModelo(const std::string& arquivoEntrada);

#endif // MODELOPERSISTENTE_HPP
