#include "../headers/ModeloPersistente.hpp"
#include "../headers/TrieNode.hpp"
#include <fstream>
#include <stdexcept>
#include <cstdint>
#include <vector>

using namespace std;

// ============================================================
// Formato binário do arquivo .model:
//
//  [4 bytes] int32  - maxOrder (ordem k do modelo)
//  [4 bytes] uint32 - tamanho do histórico
//  [histórico * 4 bytes] - símbolos do histórico (deque)
//  Nós da Trie em pré-ordem (DFS):
//    [4 bytes] uint32 - número de filhos deste nó
//    [258 * 4 bytes]  - frequências do freqTable (símbolos 0..257)
//    [4 bytes] uint32 - tamanho de activeSymbols
//    [activeSymbols * 4 bytes] - símbolos ativos
//    Para cada filho:
//      [4 bytes] uint32 - chave (símbolo)
//      <recursão do nó filho>
// ============================================================

static void salvarNo(ofstream& out, const TrieNode* node) {
    // Número de filhos
    uint32_t numFilhos = static_cast<uint32_t>(node->children.size());
    out.write(reinterpret_cast<const char*>(&numFilhos), sizeof(numFilhos));

    // Frequências 0..257
    for (uint32_t i = 0; i < 258; i++) {
        uint32_t freq = node->freqTable->get(i);
        out.write(reinterpret_cast<const char*>(&freq), sizeof(freq));
    }

    // activeSymbols
    uint32_t numAtivos = static_cast<uint32_t>(node->activeSymbols.size());
    out.write(reinterpret_cast<const char*>(&numAtivos), sizeof(numAtivos));
    for (uint32_t sym : node->activeSymbols) {
        out.write(reinterpret_cast<const char*>(&sym), sizeof(sym));
    }

    // Filhos (chave + recursão)
    for (const auto& [chave, filho] : node->children) {
        out.write(reinterpret_cast<const char*>(&chave), sizeof(chave));
        salvarNo(out, filho.get());
    }
}

static void carregarNo(ifstream& in, TrieNode* node) {
    uint32_t numFilhos;
    in.read(reinterpret_cast<char*>(&numFilhos), sizeof(numFilhos));
    if (!in) throw runtime_error("Erro ao ler numero de filhos.");

    // Frequências 0..257
    for (uint32_t i = 0; i < 258; i++) {
        uint32_t freq;
        in.read(reinterpret_cast<char*>(&freq), sizeof(freq));
        if (!in) throw runtime_error("Erro ao ler frequencias.");
        if (freq > 0)
            node->freqTable->set(i, freq);
    }

    // activeSymbols
    uint32_t numAtivos;
    in.read(reinterpret_cast<char*>(&numAtivos), sizeof(numAtivos));
    if (!in) throw runtime_error("Erro ao ler activeSymbols.");
    node->activeSymbols.clear();
    for (uint32_t j = 0; j < numAtivos; j++) {
        uint32_t sym;
        in.read(reinterpret_cast<char*>(&sym), sizeof(sym));
        if (!in) throw runtime_error("Erro ao ler simbolo ativo.");
        node->activeSymbols.push_back(sym);
    }

    // Filhos
    for (uint32_t f = 0; f < numFilhos; f++) {
        uint32_t chave;
        in.read(reinterpret_cast<char*>(&chave), sizeof(chave));
        if (!in) throw runtime_error("Erro ao ler chave do filho.");
        TrieNode* filho = node->createChild(chave);
        carregarNo(in, filho);
    }
}

void salvarModelo(const ContextModel& model, const string& arquivoSaida) {
    ofstream out(arquivoSaida, ios::binary);
    if (!out.is_open())
        throw runtime_error("Nao foi possivel criar o arquivo: " + arquivoSaida);

    int32_t ordem = model.getMaxOrder();
    out.write(reinterpret_cast<const char*>(&ordem), sizeof(ordem));

    const auto& hist = model.getHistory();
    uint32_t histSize = static_cast<uint32_t>(hist.size());
    out.write(reinterpret_cast<const char*>(&histSize), sizeof(histSize));
    for (uint32_t sym : hist)
        out.write(reinterpret_cast<const char*>(&sym), sizeof(sym));

    salvarNo(out, model.getRoot());
    out.close();
}

ContextModel carregarModelo(const string& arquivoEntrada) {
    ifstream in(arquivoEntrada, ios::binary);
    if (!in.is_open())
        throw runtime_error("Nao foi possivel abrir o arquivo: " + arquivoEntrada);

    int32_t ordem;
    in.read(reinterpret_cast<char*>(&ordem), sizeof(ordem));
    if (!in) throw runtime_error("Erro ao ler ordem do modelo.");

    ContextModel model(static_cast<int>(ordem));

    uint32_t histSize;
    in.read(reinterpret_cast<char*>(&histSize), sizeof(histSize));
    if (!in) throw runtime_error("Erro ao ler tamanho do historico.");
    for (uint32_t i = 0; i < histSize; i++) {
        uint32_t sym;
        in.read(reinterpret_cast<char*>(&sym), sizeof(sym));
        if (!in) throw runtime_error("Erro ao ler historico.");
        model.forceHistory(sym);
    }

    // Carrega por cima da raiz existente
    carregarNo(in, model.getRoot());
    in.close();
    return model;
}
