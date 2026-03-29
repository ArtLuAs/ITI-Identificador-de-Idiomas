#include "../headers/Interpretador.hpp"
#include "../headers/ContextModel.hpp"
#include "../headers/ModeloPersistente.hpp"
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;
namespace fs = std::filesystem;

// Tamanho do bloco de progresso (em caracteres)
static constexpr size_t BLOCO_PROGRESSO = 100'000;

void treinarModelo(
    const string& corpusFile,
    const string& arquivoModelo,
    int order,
    bool verbose
) {
    // === 1. Abre o corpus ===
    ifstream corpus(corpusFile, ios::binary);
    if (!corpus.is_open())
        throw runtime_error("[Interpretador] Nao foi possivel abrir corpus: " + corpusFile);

    // === 2. Carrega modelo existente ou cria novo ===
    ContextModel model =
        fs::exists(arquivoModelo)
        ? (verbose && (cout << "[Interpretador] Carregando modelo existente: "
                           << arquivoModelo << endl), carregarModelo(arquivoModelo))
        : (verbose && (cout << "[Interpretador] Nenhum modelo encontrado. Criando novo (k="
                           << order << ")" << endl), ContextModel(order));

    // === 3. Lê e alimenta o modelo caractere a caractere ===
    size_t totalProcessado = 0;
    size_t proximoBloco    = BLOCO_PROGRESSO;

    if (verbose)
        cout << "[Interpretador] Treinando com: " << corpusFile << endl;

    int c;
    while ((c = corpus.get()) != EOF) {
        model.updateAndShift(static_cast<uint32_t>(c));
        totalProcessado++;

        if (verbose && totalProcessado >= proximoBloco) {
            cout << "[Interpretador] " << totalProcessado
                 << " chars processados..." << endl;
            proximoBloco += BLOCO_PROGRESSO;
        }
    }
    corpus.close();

    if (verbose)
        cout << "[Interpretador] Treinamento concluido. Total: "
             << totalProcessado << " chars." << endl;

    // === 4. Salva o modelo atualizado ===
    // Garante que a pasta de destino exista
    fs::path pastaDestino = fs::path(arquivoModelo).parent_path();
    if (!pastaDestino.empty() && !fs::exists(pastaDestino))
        fs::create_directories(pastaDestino);

    salvarModelo(model, arquivoModelo);

    if (verbose)
        cout << "[Interpretador] Modelo salvo em: " << arquivoModelo << endl;
}
