#include "headers/PPM.hpp"
#include "headers/Interpretador.hpp"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
namespace fs = std::filesystem;

// ================================================================
// Uso:
//
//  Comprimir:    ./main -c <entrada> <saida> [ordem=5]
//  Descomprimir: ./main -d <entrada> <saida> [ordem=5]
//
//  Treinar:      ./main --treinar <corpus.txt> <modelo.model> [ordem=5]
//                  Treina (ou incrementa) o modelo com o corpus.
//                  Ex: ./main --treinar corpora/pt.txt modelos/pt.model 5
//
//  Identificar:  ./main --identificar <texto.txt> <pasta_modelos>
//                  Identifica o idioma do texto usando os modelos salvos.
//                  Ex: ./main --identificar entrada.txt modelos/
//
//  Benchmark:    ./main [ordem=4] [saida.csv]
// ================================================================

void rodarFerramenta(const string& modo, const string& input,
                     const string& output, int order) {
    if (modo == "-c") {
        cout << "Iniciando compressao de " << input
             << " (K=" << order << ")" << endl;
        ifstream inOriginal(input, ios::binary);
        if (!inOriginal.is_open()) {
            cerr << "ERRO: Arquivo nao encontrado: " << input << endl;
            return;
        }
        string inputStr((istreambuf_iterator<char>(inOriginal)),
                        istreambuf_iterator<char>());
        inOriginal.close();

        auto start = chrono::high_resolution_clock::now();
        compressPPM(inputStr, output, order);
        auto end   = chrono::high_resolution_clock::now();
        chrono::duration<double> d = end - start;
        cout << "Compressao concluida em " << d.count() << " segundos." << endl;

    } else if (modo == "-d") {
        cout << "Iniciando descompressao de " << input
             << " (K=" << order << ")" << endl;
        auto start = chrono::high_resolution_clock::now();
        decompressPPM(input, output, order);
        auto end   = chrono::high_resolution_clock::now();
        chrono::duration<double> d = end - start;
        cout << "Descompressao concluida em " << d.count() << " segundos." << endl;
    }
}

int rodarBenchmark(int argc, char* argv[]) {
    // vector<string> listaArquivos = {
    //     "silesia/mr", "silesia/ooffice", "silesia/mozilla", "silesia/dickens",
    //     "silesia/xml", "silesia/osdb", "silesia/x-ray", "silesia/reymont",
    //     "silesia/nci", "silesia/samba", "silesia/webster", "silesia/sao",
    //     "silesia/silesia.tar"
    // };

    vector<string> listaArquivos = {"entrada.txt"};

    int ordemK         = (argc > 1) ? stoi(argv[1]) : 4;
    string benchmarkCsv = (argc > 2) ? argv[2] : "";

    ofstream benchOut;
    if (!benchmarkCsv.empty()) {
        benchOut.open(benchmarkCsv, ios::app);
        if (!benchOut.is_open()) {
            cerr << "ERRO: nao foi possivel abrir o CSV: " << benchmarkCsv << endl;
            return 1;
        }
    }

    uintmax_t totalOriginal   = 0;
    uintmax_t totalComprimido = 0;
    double totalTempoComp     = 0.0;
    double totalTempoDecomp   = 0.0;
    bool   totalOk            = true;

    for (const auto& nomeArquivo : listaArquivos) {
        cout << "\n=======================================================\n";
        cout << "Processando: " << nomeArquivo << endl;

        fs::path caminho(nomeArquivo);
        string stem    = caminho.stem().string();
        string extensao = caminho.extension().string();
        string arqComp  = "saidas/" + stem + "_comprimido.bin";
        string arqDecomp = "saidas/" + stem + "_descomprimido" + extensao;

        ifstream inOrig(nomeArquivo, ios::binary);
        if (!inOrig.is_open()) {
            cerr << "ERRO: " << nomeArquivo << endl;
            totalOk = false;
            continue;
        }
        string inputStr((istreambuf_iterator<char>(inOrig)),
                        istreambuf_iterator<char>());
        inOrig.close();

        auto t0 = chrono::high_resolution_clock::now();
        compressPPM(inputStr, arqComp, ordemK);
        auto t1 = chrono::high_resolution_clock::now();
        decompressPPM(arqComp, arqDecomp, ordemK);
        auto t2 = chrono::high_resolution_clock::now();

        chrono::duration<double> dComp   = t1 - t0;
        chrono::duration<double> dDecomp = t2 - t1;

        ifstream inDecomp(arqDecomp, ios::binary);
        string resultStr((istreambuf_iterator<char>(inDecomp)),
                         istreambuf_iterator<char>());
        inDecomp.close();

        bool ok = (inputStr == resultStr);
        cout << (ok ? "SUCESSO" : "ERRO") << endl;

        try {
            auto szOrig  = fs::file_size(nomeArquivo);
            auto szComp  = fs::file_size(arqComp);
            double bps   = (static_cast<double>(szComp) * 8.0) / szOrig;
            double ratio = (static_cast<double>(szComp) / szOrig) * 100.0;

            cout << "BPC: " << fixed << setprecision(3) << bps
                 << " | Razao: " << ratio << "%" << endl;

            totalOriginal   += szOrig;
            totalComprimido += szComp;
            totalTempoComp  += dComp.count();
            totalTempoDecomp += dDecomp.count();

            if (benchOut.is_open())
                benchOut << "PPM-C," << ordemK << "," << stem << ","
                         << szOrig << "," << szComp << ","
                         << fixed << setprecision(6) << bps << ","
                         << dComp.count() << "," << dDecomp.count() << ","
                         << (ok ? "OK" : "FAIL") << "\n";
        } catch (const fs::filesystem_error& e) {
            cerr << e.what() << endl;
            totalOk = false;
        }
        cout << "=======================================================\n";
    }

    if (benchOut.is_open() && totalOriginal > 0) {
        double totalBps = (static_cast<double>(totalComprimido) * 8.0) / totalOriginal;
        benchOut << "PPM-C," << ordemK << ",TOTAL,"
                 << totalOriginal << "," << totalComprimido << ","
                 << fixed << setprecision(6) << totalBps << ","
                 << totalTempoComp << "," << totalTempoDecomp << ","
                 << (totalOk ? "OK" : "FAIL") << "\n";
    }
    return 0;
}

int main(int argc, char* argv[]) {
    cout << unitbuf;

    if (argc >= 2 && string(argv[1]) == "--treinar") {
        // ./main --treinar <corpus> <modelo> [ordem=5]
        if (argc < 4) {
            cerr << "Uso: ./main --treinar <corpus.txt> <modelo.model> [ordem=5]" << endl;
            return 1;
        }
        string corpus = argv[2];
        string modelo = argv[3];
        int ordem     = (argc > 4) ? stoi(argv[4]) : 5;
        treinarModelo(corpus, modelo, ordem, true);
        return 0;
    }

    if (argc >= 2 && string(argv[1]) == "--identificar") {
        // ./main --identificar <texto.txt> <pasta_modelos>
        if (argc < 4) {
            cerr << "Uso: ./main --identificar <texto.txt> <pasta_modelos>" << endl;
            return 1;
        }
        ifstream arqTexto(argv[2]);
        if (!arqTexto.is_open()) {
            cerr << "ERRO: Nao foi possivel abrir: " << argv[2] << endl;
            return 1;
        }
        string texto((istreambuf_iterator<char>(arqTexto)),
                     istreambuf_iterator<char>());
        arqTexto.close();

        string pastaModelos = argv[3];
        cout << "\nAvaliando texto contra modelos em: " << pastaModelos << endl;
        cout << "---------------------------------------" << endl;

        auto start = chrono::high_resolution_clock::now();
        auto resultados = avaliarTexto(texto, pastaModelos);
        auto end   = chrono::high_resolution_clock::now();

        if (resultados.empty()) {
            cerr << "Nenhum modelo encontrado ou erro na avaliacao." << endl;
            return 1;
        }

        
        cout << "\n=== RESULTADO ==="  << endl;
        for (size_t i = 0; i < resultados.size(); i++) {
            cout << (i == 0 ? ">> " : "   ")
            << resultados[i].idioma
            << "  BPC: " << fixed << setprecision(4) << resultados[i].bpc
            << "  bits: " << fixed << setprecision(1) << resultados[i].bitsTotal
            << endl;
        }
        cout << "\nIdioma identificado: " << resultados.front().idioma << endl;
        
        chrono::duration<double> d = end - start;
        cout << "Teste concluido: " << d.count() << " segundos." << endl;
        return 0;
    }

    if (argc >= 4 &&
        (string(argv[1]) == "-c" || string(argv[1]) == "-d")) {
        string modo   = argv[1];
        string input  = argv[2];
        string output = argv[3];
        int ordemK    = (argc > 4) ? stoi(argv[4]) : 4;
        rodarFerramenta(modo, input, output, ordemK);
        return 0;
    }

    if (argc >= 2 && string(argv[1]) == "--help") {
        cout << "Uso:\n"
             << "  Treinar:      ./main --treinar <corpus.txt> <modelo.model> [ordem=5]\n"
             << "  Identificar:  ./main --identificar <texto.txt> <pasta_modelos>\n"
             << "  Comprimir:    ./main -c <entrada> <saida> [ordem=5]\n"
             << "  Descomprimir: ./main -d <entrada> <saida> [ordem=5]\n"
             << "  Benchmark:    ./main [ordem=4] [saida.csv]\n";
        return 0;
    }

    return rodarBenchmark(argc, argv);
}
