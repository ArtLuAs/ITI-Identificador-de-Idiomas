# :earth_americas::mag_right: ITI Identificador de Idiomas (PPM-C)

- ### [:dart: Objetivo](#dart-objetivo-1)
- ### [:world_map: Sobre o Projeto](#world_map-sobre-o-projeto-1)
- ### [:abc: Dependências](#abc-dependências-1)
- ### [:file_folder: Documentação](#file_folder-documentação-1)
- ### [:gear: Como rodar](#gear-como-rodar-1)
- ### [:arrow_down: Baixar o projeto](https://github.com/ArtLuAs/ITI-Identificador-de-Idiomas/archive/refs/heads/main.zip)

## Disciplina de Introdução à Teoria da Informação

Esse foi um projeto desenvolvido por discentes do curso de *Engenharia da Computação da Universidade Federal da Paraíba*, curso este que pertence ao *[Centro de Informática](http://ci.ufpb.br/)*, localizado na *[Rua dos Escoteiros S/N - Mangabeira - João Pessoa - Paraíba - Brasil](https://g.co/kgs/xobLzCE)*. O projeto implementa um sistema que consiste em um compressor e descompressor baseado no algoritmo PPM-C, otimizado para fontes não estacionárias através de adaptação dinâmica com monitoramento de performance e reset de tabelas de frequências. A avaliação do projeto, realizada pelo docente, foi realizada por meio da verificação do funcionamento do projeto e a da validação dos resultados.

### :earth_americas: Autores:

-  :luggage:  *[Arthur Maximus Cavalcante Beuttenmuller](https://github.com/Maximusthr)*
-  :luggage:  *[Eduardo Asfuri Carvalho](https://github.com/Asfuri)*
-  :luggage:  *[Lucas Henrique Vieira da Silva](https://github.com/hvslucas)*

###  :earth_americas: Docente:

:luggage: Leonardo Vidal Batista

<p align="center">
  <a href="#page_facing_up-package-iti-ppm-c-1">
    <img alt="C-3PO_droid" src="https://github.com/user-attachments/assets/bd3e166f-8032-4981-952b-7257628e9355" width="228" height="437" />
  </a>
</p>


## :dart: Objetivo:

O objetivo geral deste trabalho foi desenvolver e avaliar um identificador de idiomas que se adequam especificamente à codificação ASCII 256, utilizando o algoritmo PPM-C. O estudo foca em mapear quais línguas se adequam perfeitamente a esse limite sem sofrer perda de informação ou requerer codificações mais complexas (como o UTF-8). Os experimentos foram conduzidos utilizando corpora textuais abrangendo 27 idiomas.

[^1]: ***[Corpus Silesia](https://sun.aei.polsl.pl/~sdeor/index.php?page=silesia)***

## :world_map: Sobre o Projeto

### Modelagem e Entropia Cruzada:
A identificação de idiomas foi modelada como um problema de compressão de dados. O modelo calcula a entropia cruzada de um texto de entrada frente aos modelos previamente treinados. A taxa real de custo é calculada através de bits por caractere (BPC). O idioma que resultar no menor comprimento médio é assumido como o idioma real do texto, indicando que seu modelo previu a sequência com maior exatidão.

### Arquitetura do Sistema:
A arquitetura de compressão foi dividida em duas etapas principais, estruturadas através de uma árvore de contextos (Trie):

- **Treinamento (`interpretador.cpp`)**: Módulo responsável por processar os extensos corpora de cada um dos 27 idiomas, alimentando a árvore de contextos e ajustando as probabilidades.
- **Persistência (`modelopersistente`)**: Para evitar reprocessamento, um código auxiliar serializa as informações da Trie em disco, salvando os modelos em um diretório `models/`.
- **Identificação (`avaliador.cpp`)**: Quando uma nova frase é submetida, este módulo carrega os modelos treinados e calcula o custo de codificação, prevendo o idioma de origem com base no menor custo em bits/char.
- **Ordem de Contexto**: Para viabilizar a execução dentro das limitações computacionais de hardware, a profundidade da memória do contexto adotada foi restrita a $K_{max}=4$.

### Bases de Dados:
Foram analisados idiomas de diversas famílias linguísticas (Românicas, Germânicas, Célticas, etc.) utilizando dados de duas fontes abertas:
- **Projeto Tatoeba**: Extração de sentenças curtas e estruturadas.
- **Leipzig Corpora Collection**: Volumes massivos de dados, priorizando recortes de aproximadamente 100 mil linhas de notícias e Wikipédia por idioma.

## :abc: Dependências

Este projeto foi desenvolvido utilizando apenas a biblioteca padrão do C++ (STL), sem dependências externas. Abaixo, destacamos as principais bibliotecas utilizadas e sua aplicação no sistema de compressão PPM com codificação aritmética.

### Biblioteca Padrão

- **`<iostream>`**: Utilizada para operações básicas de entrada e saída no terminal, como exibir mensagens de progresso, resultados de compressão/descompressão e logs de depuração.
  - **Implementação:** Durante a execução dos programas `compress` e `decompress`, mensagens são impressas para informar o usuário sobre o andamento do processo, taxas de compressão e possíveis eventos como reset do modelo.

- **`<fstream>`**: Fornece funcionalidades para leitura e escrita de arquivos binários, essenciais para manipular os dados de entrada (texto original) e gerar o arquivo comprimido.
  - **Implementação:** No compressor, o texto de entrada é lido via `ifstream` e o fluxo de bits comprimido é escrito em um arquivo binário através de `ofstream`. O descompressor realiza a operação inversa.

- **`<string>`** e **`<string_view>`**: Gerenciamento eficiente de cadeias de caracteres, utilizado para armazenar o texto de entrada e para manipular visualizações de contexto sem cópias desnecessárias.
  - **Implementação:** A função `calcularEntropiaOrdemK` utiliza `string_view` para percorrer contextos de tamanho fixo sem alocar memória extra, otimizando o cálculo da entropia empírica.

- **`<vector>`**, **`<deque>`**, **`<map>`**, **`<unordered_map>`**: Estruturas de dados fundamentais para o modelo PPM.
  - **Implementação:**
    - `vector`: Armazena as frequências dos símbolos na tabela de frequência (`SimpleFrequencyTable`) e a lista de símbolos ativos em cada nó da Trie.
    - `deque`: Gerencia a janela deslizante de histórico no `ContextModel`, mantendo os últimos `k` símbolos processados.
    - `map`/`unordered_map`: Na Trie (`TrieNode`), os filhos são armazenados em um `unordered_map` (ou `map`) que mapeia o símbolo para o nó correspondente, permitindo busca rápida por contexto.

- **`<cmath>`**: Utilizado para cálculos matemáticos, especialmente funções logarítmicas para entropia e informação.
  - **Implementação:** As funções `log2` são empregadas no cálculo da entropia de ordem-0 e ordem-k, bem como na acumulação da informação teórica (`l_barra_teorico`) durante a compressão.

- **`<algorithm>`**: Fornece algoritmos genéricos como `fill` para preencher vetores, utilizado na reinicialização da máscara de exclusão.
  - **Implementação:** Após processar cada símbolo, o vetor `isExcluded` é limpo com `fill(isExcluded.begin(), isExcluded.end(), false)` para preparar a próxima iteração.

- **`<iomanip>`**: Permite a formatação de saída, utilizada para exibir valores numéricos com precisão controlada.
  - **Implementação:** Nas mensagens de console, valores como taxas de compressão (bits por símbolo) são exibidos com `std::fixed` e `std::setprecision` para facilitar a leitura.

- **`<stdexcept>`**: Fornece classes de exceção padrão para tratamento de erros.
  - **Implementação:** Em diversos pontos do código (como na verificação de argumentos inválidos ou overflow aritmético), exceções como `std::invalid_argument`, `std::overflow_error` e `std::logic_error` são lançadas para garantir robustez.

- **`<limits>`**: Utilizado para obter valores máximos de tipos inteiros, auxiliando em verificações de estouro.
  - **Implementação:** No codificador aritmético (`ArithmeticCoderBase`), o valor `maximumTotal` é calculado com base em `std::numeric_limits<decltype(fullRange)>::max()` para evitar que o somatório das frequências ultrapasse os limites do intervalo.

- **`<cstdint>`**: Define tipos de inteiros com largura fixa (`uint32_t`, `uint64_t`), garantindo portabilidade e controle preciso sobre os tamanhos das variáveis utilizadas nos cálculos aritméticos.
  - **Implementação:** Todo o núcleo da codificação aritmética e das tabelas de frequência utiliza `uint32_t` e `uint64_t` para manipular intervalos e somas sem ambiguidades entre plataformas.


## :file_folder: Documentação

### :open_file_folder: Estrutura do Projeto

```text
.
├── documentos/                     # Documentação adicional do projeto
├── headers/
│   ├── ArithmeticCoder.hpp         # Codificação aritmética (bits e codificador/decodificador)
│   ├── ContextModel.hpp            # Modelo de contextos PPM (Trie e gerenciamento)
│   ├── PPM.hpp                     # Funções públicas de compressão/descompressão
│   ├── tabelaFrequencia.hpp        # Tabelas de frequência com Fenwick Tree e exclusão
│   └── TrieNode.hpp                # Nó da Trie com frequências e filhos
├── src/
│   ├── ArithmeticCoder.cpp         # Implementação da E/S de bits e codificação
│   ├── compress.cpp                # Compressão PPM com monitoramento e reset
│   ├── ContextModel.cpp            # Gerenciamento da árvore de contextos
│   ├── decompress.cpp              # Descompressão PPM com escapes e reset
│   ├── tabelaFrequencia.cpp        # Operações em tabelas de frequência
│   └── TrieNode.cpp                # Manipulação dos nós da Trie
├── main.cpp                        # Ponto de entrada: compressão/descompressão de arquivo
├── benchmark.ps1                   # Script PowerShell para execução de benchmarks (comparação com 7z)
├── plotar_benchmark.py             # Geração de gráficos de benchmark (comparação com 7z)
├── plotar_grafico.py               # Visualização da performance (BPS vs entropias)
├── plotar_resets.py                # Diagnóstico de resets do modelo adaptativo
├── plotar_benchmark_consolidado.py # Geração de gráficos de benchmark (para todos os Ks)
└── README.md                       # Este arquivo, documenta o projeto
```

### :page_facing_up: Classes e Atributos

O sistema de compressão PPM é estruturado em torno de classes que implementam a codificação aritmética, o gerenciamento de frequências e o modelo de contextos. Abaixo estão as principais classes e seus atributos.

**1. Classes de Entrada/Saída de Bits**  
Responsáveis por ler e escrever bits individuais em fluxos de bytes.

- **`BitInputStream`**  
  **Atributos:**
  - `input` (`std::istream&`): Referência para o fluxo de entrada de bytes.  
  - `currentByte` (`int`): Byte atual sendo processado (pode ser `EOF`).  
  - `numBitsRemaining` (`int`): Quantidade de bits restantes a serem lidos do `currentByte`.

- **`BitOutputStream`**  
  **Atributos:**
  - `output` (`std::ostream&`): Referência para o fluxo de saída de bytes.  
  - `currentByte` (`int`): Byte sendo construído bit a bit.  
  - `numBitsFilled` (`int`): Número de bits já armazenados no `currentByte`.

**2. Classes Base da Codificação Aritmética**  
Fornecem a lógica compartilhada entre codificador e decodificador.

- **`ArithmeticCoderBase`** (classe abstrata)  
  **Atributos:** 
  - `numStateBits` (`int`): Número de bits usados para o estado interno (precisão).  
  - `fullRange` (`uint64_t`): Valor máximo do intervalo (`1 << numStateBits`).  
  - `halfRange`, `quarterRange`, `minimumRange` (`uint64_t`): Constantes derivadas para detecção de underflow/overflow.  
  - `maximumTotal` (`uint64_t`): Maior soma de frequências suportada.  
  - `stateMask` (`uint64_t`): Máscara para limitar os valores ao intervalo.  
  - `low`, `high` (`uint64_t`): Limites inferior e superior do intervalo atual.

- **`ArithmeticDecoder`** (herda de `ArithmeticCoderBase`)  
  **Atributos:**
  - `input` (`BitInputStream&`): Fluxo de bits de entrada.  
  - `code` (`uint64_t`): Valor codificado atual (os bits já lidos).

- **`ArithmeticEncoder`** (herda de `ArithmeticCoderBase`)  
  **Atributos:** 
  - `output` (`BitOutputStream&`): Fluxo de bits de saída.  
  - `numUnderflow` (`int`): Contador de underflows pendentes para serem escritos.

**3. Classes de Tabela de Frequência**  
Gerenciam as contagens de símbolos para o codificador aritmético, incluindo suporte a exclusão (princípio PPMC) e redimensionamento.

- **`FrequencyTable`** (classe abstrata)  
  Define a interface para consulta e atualização de frequências.

- **`FlatFrequencyTable`** (implementa `FrequencyTable`)  
  **Atributos:**  
  - `numSymbols` (`uint32_t`): Número de símbolos; todos têm frequência 1.

- **`SimpleFrequencyTable`** (implementa `FrequencyTable`)  
  **Atributos:**
  - `frequencies` (`std::vector<uint32_t>`): Vetor com as frequências absolutas de cada símbolo.  
  - `tree` (`std::vector<uint32_t>`): Árvore de Fenwick para consultas rápidas de soma acumulada.  
  - `total` (`uint32_t`): Soma total de todas as frequências.  
  - `savedFrequencies` (`std::vector<std::pair<uint32_t, uint32_t>>`): Pilha de frequências originais de símbolos excluídos temporariamente.

**4. Classes do Modelo PPM**  
Representam a estrutura de contextos (Trie) e o gerenciamento do histórico.

- **`TrieNode`**  
  **Atributos:**
  - `freqTable` (`std::unique_ptr<SimpleFrequencyTable>`): Tabela de frequências para este contexto (símbolos 0–255, 256 (escape), 257 (EOF), 258 (reset)).  
  - `children` (`std::map<uint32_t, std::unique_ptr<TrieNode>>`): Mapeamento de símbolo para o próximo nó na árvore (contexto de ordem superior).  
  - `activeSymbols` (`std::vector<uint32_t>`): Lista de símbolos já vistos neste contexto (usada para exclusão e cálculo do peso do escape).

- **`ContextModel`**  
  **Atributos:**  
  - `maxOrder` (`int`): Ordem máxima do modelo PPM (k).  
  - `root` (`std::unique_ptr<TrieNode>`): Raiz da árvore (contexto de ordem 0).  
  - `history` (`std::deque<uint32_t>`): Janela deslizante com os últimos símbolos processados (tamanho ≤ `maxOrder`).

## :gear: Como rodar

[**Atenção:** Lembre de baixar o projeto e extraí-lo devidamente do `.zip`.](#zap-simulação-de-circuitos-rlc-paralelo)

#### Requisitos
- Um compilador C++ (recomendamos `g++` ou `clang++`)
- Terminal de linha de comando
- Python 3 com as bibliotecas `matplotlib` e `pandas` para gerar os gráficos de desempenho

```sh
pip install matplotlib pandas
```

### No Windows

Utilizando `g++` para compilar:

```sh
g++ src/*.cpp main.cpp -o ppm_compressao -O2
```

Utilizando `clang++` para compilar:

```sh
clang++ src/*.cpp main.cpp -o ppm_compressao -O2
```

Para rodar:

```sh
# Comprimir um arquivo (Ex: ordem K=4)
./ppm_compressao.exe -c silesia/dickens dickens.bin 4

# Descomprimir um arquivo
./ppm_compressao.exe -d dickens.bin dickens_restaurado.txt 4

# Para rodar o benchmark em todo o Corpus Silesia (comportamento padrão):
./ppm_compressao.exe 4 benchmark_resultados.csv
```

Pós-execução do PPM-C:

```sh:
python plotar_grafico.py
```

### No Linux

Utilizando `g++` para compilar e executar em seguida:

```sh
g++ src/*.cpp main.cpp -o ppm_compressao -O2 && ./ppm_compressao
```

Utilizando `clang++` para compilar e executar em seguida:

```sh
clang++ src/*.cpp main.cpp -o ppm_compressao -O2 && ./ppm_compressao
```

Pós-execução do PPM-C:

```sh:
python3 plotar_grafico.py
```

**OBS.:** Utilizamos de barra normal ('/') considerando um ambiente como Git Bash, WSL e PowerShell, considere utilizar de barra invertida ('\\') em caso de não compilar
