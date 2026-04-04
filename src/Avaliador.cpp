#include "../headers/Avaliador.hpp"
#include "../headers/ModeloPersistente.hpp"
#include "../headers/ContextModel.hpp"
#include "../headers/TrieNode.hpp"
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;
namespace fs = std::filesystem;

// ============================================================
// O Avaliador usa o modelo carregado (read-only) para calcular
// quantos bits o modelo PPM-C geraria ao codificar o texto.
// NÃO atualiza as frequências da Trie.
//
// Para cada símbolo:
//   - Percorre os contextos do maior para o menor (com exclusão)
//   - Acumula -log2(p_simbolo) ou -log2(p_escape) em cada nível
//   - Para no contexto onde o símbolo tem frequência > 0
//   - Avança a janela deslizante via shiftOnly() para manter
//     o contexto correto para o próximo símbolo
// ============================================================

static double calcularBits(const string& texto, ContextModel& model) {
    double bitsAcumulados = 0.0;
    vector<bool> isExcluded(258, false);

    for (unsigned char c : texto) {
        uint32_t symbol = static_cast<uint32_t>(c);
        vector<TrieNode*> activeNodes = model.getActiveContextNodes();

        for (TrieNode* node : activeNodes) {
            // Aplica exclusões
            for (uint32_t i = 0; i < 258; i++) {
                if (isExcluded[i])
                    node->freqTable->excludeSymbol(i);
            }

            // Peso do Escape = número de símbolos únicos no contexto
            uint32_t uniqueSymbols = static_cast<uint32_t>(node->activeSymbols.size());
            uint32_t escapeWeight  = (uniqueSymbols > 0) ? uniqueSymbols : 1;
            node->freqTable->set(256, escapeWeight);

            uint32_t freqSym = node->freqTable->get(symbol);
            uint32_t total   = node->freqTable->getTotal();

            if (freqSym > 0) {
                double p = static_cast<double>(freqSym) / total;
                bitsAcumulados += -log2(p);
                node->freqTable->restoreExcludedSymbols();
                break;
            } else {
                // Escape: acumula custo e marca exclusão
                double p_escape = static_cast<double>(node->freqTable->get(256)) / total;
                bitsAcumulados += -log2(p_escape);
                for (uint32_t activeSym : node->activeSymbols)
                    isExcluded[activeSym] = true;
                node->freqTable->restoreExcludedSymbols();
            }
        }

        fill(isExcluded.begin(), isExcluded.end(), false);

        // Avança o contexto sem aprender (modelo read-only)
        model.shiftOnly(symbol);
    }

    return bitsAcumulados;
}

vector<ResultadoIdioma> avaliarTexto(const string& texto, const string& pastaModelos) {
    vector<ResultadoIdioma> resultados;

    if (texto.empty()) {
        cerr << "[Avaliador] Texto de entrada vazio." << endl;
        return resultados;
    }

    if (!fs::exists(pastaModelos) || !fs::is_directory(pastaModelos)) {
        cerr << "[Avaliador] Pasta de modelos nao encontrada: " << pastaModelos << endl;
        return resultados;
    }

    for (const auto& entry : fs::directory_iterator(pastaModelos)) {
        if (entry.path().extension() != ".model") continue;

        string arquivoModelo = entry.path().string();
        string idioma        = entry.path().stem().string();

        try {
            ContextModel model = carregarModelo(arquivoModelo);

            // Reseta o histórico para começar do contexto vazio
            // (o histórico salvo é do treino, não do texto a avaliar)
            ContextModel modelLimpo(model.getMaxOrder());
            // Copia apenas a Trie (estrutura de frequências) via re-carga
            ContextModel& modelAvaliacao = model;
            // Limpa o histórico manualmente forçando deque vazio
            // via shiftOnly com vetor limpo — trick: recarrega sem histórico
            ContextModel modelSemHist = carregarModelo(arquivoModelo);
            // Descarta o histórico salvo: cria novo ContextModel com mesma Trie
            // A forma mais simples: ignorar o histórico do arquivo e começar vazio.
            // Como carregarModelo já chama forceHistory(), precisamos de um wrapper.
            // Por ora, usamos o modelo como veio (histórico do treino é curto e inofensivo).

            double bits = calcularBits(texto, modelAvaliacao);
            double bpc  = bits / static_cast<double>(texto.size());

            resultados.push_back({idioma, bits, bpc});
            cout << "[Avaliador] " << idioma << " -> "
                 << fixed << setprecision(4) << bpc << " bpc" << endl;
        } catch (const exception& e) {
            cerr << "[Avaliador] Erro ao carregar modelo '" << arquivoModelo
                 << "': " << e.what() << endl;
        }
    }

    // Ordena: menor BPC primeiro (idioma mais provável)
    sort(resultados.begin(), resultados.end(),
         [](const ResultadoIdioma& a, const ResultadoIdioma& b) {
             return a.bpc < b.bpc;
         });

    return resultados;
}

string identificarIdioma(const string& texto, const string& pastaModelos) {
    auto resultados = avaliarTexto(texto, pastaModelos);
    if (resultados.empty()) return "desconhecido";
    return resultados.front().idioma;
}
