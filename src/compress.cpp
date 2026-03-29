#include "../headers/ArithmeticCoder.hpp"
#include "../headers/ContextModel.hpp"
#include "../headers/PPM.hpp"
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string_view>
#include <unordered_map>
#include <vector>

using namespace std;

double calcularEntropiaOrdemK(const string& input, int k) {
        if ((int)input.length() <= k)
                return 0.0;

        unordered_map<string, int> contextFreq;
        unordered_map<string, int> contextCharFreq;

        size_t nContexts = input.length() - k;

        for (size_t i = 0; i < nContexts; i++) {
                string ctx(input.data() + i, k);
                string ctxChar(input.data() + i, k + 1);
                contextFreq[ctx]++;
                contextCharFreq[ctxChar]++;
        }

        double entropiaK = 0.0;
        double N = static_cast<double>(nContexts);

        for (const auto& [ctxChar, count_cx] : contextCharFreq) {
                string ctx = ctxChar.substr(0, k);
                int count_c = contextFreq.at(ctx);

                double p_cx = count_cx / N;
                double p_x_given_c = static_cast<double>(count_cx) / count_c;

                entropiaK -= p_cx * log2(p_x_given_c);
        }

        return entropiaK;
}

void compressPPM(const string& input, const string& outputFile, int order) {

        // CÁLCULO DE TODAS AS ENTROPIAS (H_0 ATÉ H_K)
        map<unsigned char, int> freqMap;
        for (unsigned char c : input) {
                freqMap[c]++;
        }

        double entropia = 0.0;
        double tamanhoTotalStr = static_cast<double>(input.length());

        if (tamanhoTotalStr > 0.0) {
                for (auto const& [c, count] : freqMap) {
                        if (count > 0) {
                                double probabilidade = static_cast<double>(count) / tamanhoTotalStr;
                                entropia -= probabilidade * log2(probabilidade);
                        }
                }
        }

        // Vetor para guardar todas as entropias de 0 até K
        vector<double> entropias(order + 1, 0.0);
        entropias[0] = entropia;
        cout << "Entropia H_0: " << entropias[0] << " bits/char" << endl;

        // Salva automaticamente a entropia H_1, H_2 ... até H_K
        for (int k = 1; k <= order; k++) {
                entropias[k] = calcularEntropiaOrdemK(input, k);
                cout << "Entropia H_" << k << ": " << entropias[k] << " bits/char" << endl;
        }

        ofstream outStream(outputFile, ios::binary);
        BitOutputStream bitOut(outStream);
        ArithmeticEncoder encoder(32, bitOut);

        ContextModel model(order);
        vector<bool> isExcluded(258, false); // 258 símbolos (sem o 258/reset)

        // Abrindo csv
        string csvName = outputFile + "_grafico.csv";
        ofstream csvOut(csvName);

        // CABEÇALHO DINÂMICO NO CSV
        csvOut << "BytesProcessados,L_Barra_Acumulado,L_Barra_Teorico";
        for (int k = 0; k <= order; ++k) {
                csvOut << ",H_" << k;
        }
        csvOut << "\n";

        // L_BARRA TEÓRICO ADAPTATIVO
        double informacaoAdaptativaAcumulada = 0.0;
        long totalSymbolsProcessed = 0;

        auto encodeSymbol = [&](uint32_t symbol) {
                vector<TrieNode*> activeNodes = model.getActiveContextNodes();

                for (TrieNode* node : activeNodes) {
                        for (uint32_t i = 0; i < 258; i++) {
                                if (isExcluded[i])
                                        node->freqTable->excludeSymbol(i);
                        }

                        // O peso do Escape é igual à quantidade de símbolos únicos neste contexto
                        uint32_t uniqueSymbols = node->activeSymbols.size();
                        // Garante peso mínimo de 1 para evitar probabilidade zero caso o nó seja novo
                        uint32_t escapeWeight = (uniqueSymbols > 0) ? uniqueSymbols : 1;
                        node->freqTable->set(256, escapeWeight);

                        if (node->freqTable->get(symbol) > 0) {
                                // Cálculo da Informação do Símbolo no instante de predição
                                double p = static_cast<double>(node->freqTable->get(symbol)) / node->freqTable->getTotal();
                                informacaoAdaptativaAcumulada += -log2(p);

                                encoder.write(*(node->freqTable), symbol);
                                node->freqTable->restoreExcludedSymbols();
                                break;
                        } else {
                                // Cálculo da Informação do Escape no instante de predição
                                double p_escape = static_cast<double>(node->freqTable->get(256)) / node->freqTable->getTotal();
                                informacaoAdaptativaAcumulada += -log2(p_escape);

                                encoder.write(*(node->freqTable), 256); // Escape
                                for (uint32_t activeSym : node->activeSymbols) {
                                        isExcluded[activeSym] = true;
                                }
                                node->freqTable->restoreExcludedSymbols();
                        }
                }

                fill(isExcluded.begin(), isExcluded.end(), false);

                // Não atualiza o modelo se for o EOF (257)
                if (symbol != 257)
                        model.updateAndShift(symbol);
        };

        cout << "Comprimindo..." << endl;

        for (unsigned char c : input) {
                encodeSymbol(c);
                totalSymbolsProcessed++;
        }

        encodeSymbol(257); // EOF verdadeiro

        encoder.finish();
        bitOut.finish();
        outStream.flush();

        // Grava linha única de métricas no CSV
        double accumulatedBPS = static_cast<double>(outStream.tellp()) * 8.0 / totalSymbolsProcessed;
        double l_barra_teorico = informacaoAdaptativaAcumulada / totalSymbolsProcessed;

        csvOut << totalSymbolsProcessed << ","
               << accumulatedBPS << ","
               << l_barra_teorico;
        for (int k = 0; k <= order; ++k) {
                csvOut << "," << entropias[k];
        }
        csvOut << "\n";

        outStream.close();
        csvOut.close();

        cout << "Compressao concluida! Dados do grafico salvos em: " << csvName << endl;
}
