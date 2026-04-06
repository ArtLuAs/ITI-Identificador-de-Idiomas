# :earth_americas::mag_right: ITI Identificador de Idiomas (PPM-C)

- ### [:dart: Objetivo](#dart-objetivo-1)
- ### [:world_map: Sobre o Projeto](#world_map-sobre-o-projeto-1)
- ### [:abc: Dependências](#abc-dependências-1)
- ### [:file_folder: Documentação](#file_folder-documentação-1)
- ### [:gear: Como rodar](#gear-como-rodar-1)
- ### [:arrow_down: Baixar o projeto](https://github.com/ArtLuAs/ITI-Identificador-de-Idiomas/archive/refs/heads/main.zip)

## Disciplina de Introdução à Teoria da Informação

Esse foi um projeto desenvolvido por discentes do curso de *Engenharia da Computação da Universidade Federal da Paraíba*, curso este que pertence ao *[Centro de Informática](http://ci.ufpb.br/)*, localizado na *[Rua dos Escoteiros S/N - Mangabeira - João Pessoa - Paraíba - Brasil](https://g.co/kgs/xobLzCE)*. O projeto implementa um sistema identificador de idiomas baseado no algoritmo PPM-C, que utiliza entropia cruzada para determinar, a partir de modelos estatísticos previamente treinados, qual idioma melhor se ajusta a um texto de entrada. A avaliação do projeto, realizada pelo docente, foi realizada por meio da verificação do funcionamento do projeto e da validação dos resultados.

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

O objetivo geral deste trabalho foi desenvolver e avaliar um identificador de idiomas que se adequam especificamente à codificação ASCII Estendido (256 caracteres), utilizando o algoritmo PPM-C. O estudo foca em mapear quais línguas se adequam perfeitamente a esse limite sem sofrer perda de informação ou requerer codificações mais complexas (como o UTF-8). Os experimentos foram conduzidos utilizando corpora textuais abrangendo 27 idiomas de diversas famílias linguísticas.

## :world_map: Sobre o Projeto

### Modelagem e Entropia Cruzada:
A identificação de idiomas foi modelada como um problema de compressão de dados, onde busca-se representar uma sequência de símbolos usando o menor número possível de bits. O modelo calcula a **entropia cruzada** de um texto de entrada frente a cada um dos modelos de idioma previamente treinados. A taxa de custo é expressa em **bits por caractere (BPC)**. O idioma que resultar no menor BPC é assumido como o idioma real do texto, indicando que seu modelo previu a sequência de caracteres com maior exatidão.

### Arquitetura do Sistema:
O sistema é composto por três módulos principais, estruturados em torno de uma árvore de contextos (Trie):

- **Treinamento (`--treinar`)**: Processa o corpus de um idioma, alimentando a árvore de contextos (Trie + Árvore de Fenwick) e salvando o modelo resultante em disco no diretório `modelos/`.
- **Identificação (`--identificar`)**: Carrega todos os modelos do diretório `modelos/` e calcula o BPC do texto de entrada frente a cada um. O idioma identificado é aquele com o menor custo em bits por caractere.
- **Ordem de Contexto**: A profundidade máxima de contexto adotada foi $K_{max}=4$, definida em função das limitações de hardware disponível. Valores maiores de $K$ aumentam a acurácia em textos longos, porém elevam significativamente o tamanho da Trie e o tempo de processamento.

### Idiomas Analisados:
Foram analisados **27 idiomas** baseados no alfabeto latino, categorizados por família linguística:

| Família | Idiomas |
|---|---|
| **Românicas** | Português, Espanhol, Francês, Italiano, Catalão, Galego, Corso, Valão |
| **Germânicas** | Inglês, Alemão, Holandês, Sueco, Dinamarquês, Norueguês, Islandés, Faroense, Afrikaans, Luxemburguês |
| **Fino-Úgricas** | Finlandês, Estoniano |
| **Célticas** | Irlandês, Bretão |
| **Austronésias** | Indonésio, Tagalo |
| **Níger-Congo** | Suaíli |
| **Isoladas / Outros** | Basco, Albanês |

### Bases de Dados:
Os corpora de treinamento foram extraídos de duas fontes abertas:
- **[Projeto Tatoeba](https://tatoeba.org/)**: Extração de sentenças curtas e variadas do cotidiano.
- **[Leipzig Corpora Collection](https://wortschatz.uni-leipzig.de/)** (Universidade de Leipzig): Volumes massivos de notícias e artigos da Wikipédia, com preferência por recortes de aproximadamente **100 mil linhas** por idioma.

## :abc: Dependências

Este projeto foi desenvolvido utilizando apenas a **biblioteca padrão do C++ (STL)**, sem dependências externas. O pré-processamento dos corpora utiliza **Python 3**.

```sh
pip install -r tools/requirements.txt
```

### Biblioteca Padrão C++

- **`<iostream>`**: Saída de mensagens de progresso e resultados no terminal.
- **`<fstream>`**: Leitura dos corpora de treinamento e escrita/leitura dos arquivos `.model` em disco.
- **`<string>`** e **`<string_view>`**: Gerenciamento de cadeias de caracteres e leitura eficiente de contextos sem alocações desnecessárias.
- **`<vector>`**, **`<deque>`**, **`<map>`**, **`<unordered_map>`**: Estruturas fundamentais para a Trie e para o histórico de contextos.
  - `vector`: Armazena as frequências dos símbolos em cada nó da Trie.
  - `deque`: Mantém a janela deslizante dos últimos `k` caracteres processados no `ContextModel`.
  - `unordered_map`: Mapeia cada símbolo ao nó filho correspondente na Trie, permitindo busca em O(1).
- **`<cmath>`**: Funções logarítmicas (`log2`) para o cálculo de BPC e entropia cruzada.
- **`<algorithm>`**: `fill` para reinicializar o vetor de exclusão (`isExcluded`) a cada símbolo processado.
- **`<iomanip>`**: Formatação de saída numérica com `std::fixed` e `std::setprecision` nos logs de BPC.
- **`<stdexcept>`**: Tratamento de erros com `std::invalid_argument` e `std::runtime_error`.
- **`<limits>`**: Cálculo do `maximumTotal` para evitar overflow nas somas de frequência.
- **`<cstdint>`**: Tipos de largura fixa (`uint32_t`, `uint64_t`) para portabilidade e controle preciso nas tabelas de frequência.


## :file_folder: Documentação

### :open_file_folder: Estrutura do Projeto

```text
.
├── data input/                     # Textos de entrada para identificação
├── modelos/                        # Modelos treinados por idioma (gerados pelo --treinar)
├── headers/                        # Arquivos de cabeçalho C++
├── src/                            # Implementações C++
├── tools/                          # Scripts Python para pré-processamento dos corpora
├── main.cpp                        # Ponto de entrada: --treinar e --identificar
├── run.sh                          # Script para treinar um idioma (Linux/macOS)
├── run.bat                         # Script para treinar um idioma (Windows)
└── README.md                       # Este arquivo
```

### :page_facing_up: Classes e Atributos

O sistema é estruturado em torno de classes que implementam a codificação aritmética, o gerenciamento de frequências e o modelo de contextos PPM.

**1. Classes de Entrada/Saída de Bits**
Responsáveis por ler e escrever bits individuais em fluxos de bytes.

- **`BitInputStream`**
  - `input` (`std::istream&`): Fluxo de entrada de bytes.
  - `currentByte` (`int`): Byte atual sendo processado.
  - `numBitsRemaining` (`int`): Bits restantes a ler do `currentByte`.

- **`BitOutputStream`**
  - `output` (`std::ostream&`): Fluxo de saída de bytes.
  - `currentByte` (`int`): Byte sendo construído bit a bit.
  - `numBitsFilled` (`int`): Bits já armazenados no `currentByte`.

**2. Classes Base da Codificação Aritmética**

- **`ArithmeticCoderBase`** (classe abstrata)
  - `numStateBits` (`int`): Precisão do intervalo aritmético.
  - `fullRange`, `halfRange`, `quarterRange`, `minimumRange` (`uint64_t`): Constantes para controle do intervalo.
  - `maximumTotal` (`uint64_t`): Soma máxima de frequências suportada.
  - `low`, `high` (`uint64_t`): Limites atual do intervalo.

- **`ArithmeticDecoder`** — `input` (`BitInputStream&`), `code` (`uint64_t`).
- **`ArithmeticEncoder`** — `output` (`BitOutputStream&`), `numUnderflow` (`int`).

**3. Classes de Tabela de Frequência**

- **`SimpleFrequencyTable`** (implementa `FrequencyTable`)
  - `frequencies` (`std::vector<uint32_t>`): Frequências absolutas de cada símbolo (0–255 + escape).
  - `tree` (`std::vector<uint32_t>`): Árvore de Fenwick para somas acumuladas em O(log n).
  - `total` (`uint32_t`): Soma total das frequências.
  - `savedFrequencies`: Pilha de frequências salvas para o mecanismo de exclusão (PPM-C).

**4. Classes do Modelo PPM**

- **`TrieNode`**
  - `freqTable` (`std::unique_ptr<SimpleFrequencyTable>`): Tabela de frequências para este contexto.
  - `children` (`std::map<uint32_t, std::unique_ptr<TrieNode>>`): Filhos na árvore de prefixos.
  - `activeSymbols` (`std::vector<uint32_t>`): Símbolos já vistos neste contexto (usado na exclusão).

- **`ContextModel`**
  - `maxOrder` (`int`): Ordem máxima do modelo ($K_{max}$).
  - `root` (`std::unique_ptr<TrieNode>`): Raiz da Trie.
  - `history` (`std::deque<uint32_t>`): Janela deslizante com os últimos `maxOrder` símbolos.

## :gear: Como rodar

> **Atenção:** Lembre de baixar o projeto e extraí-lo devidamente do `.zip`.

#### Requisitos
- Compilador C++ (`g++` ou `clang++`)
- Python 3

---

### Compilação

```sh
g++ main.cpp ./src/*.cpp -o interpretador
```

---

### Etapa 1 — Treinamento (gerar os modelos)

Antes de treinar, o corpus precisa ser pré-processado pelo script Python. Edite a variável `idioma` no topo do `run.sh` (ou `run.bat`) para o código do idioma desejado e execute:

```sh
# Linux / macOS / Git Bash
bash run.sh
```

```bat
rem Windows
run.bat
```

O script realiza automaticamente:
1. O pré-processamento do corpus via `tools/main.py`
2. A compilação do C++
3. O treinamento do modelo com o comando:

```sh
./interpretador --treinar "corpora/<idioma>.txt" "modelos/<idioma>.model" 4
```

Repita o processo para cada idioma desejado. Os modelos ficam salvos em `modelos/`.

---

### Etapa 2 — Identificação

Com os modelos já treinados, execute:

```sh
# Identificar a partir de uma string direta
./interpretador --identificar "Seu texto aqui" modelos/

# Identificar a partir de um arquivo
./interpretador --identificar "data input/teste.txt" modelos/
```

O programa calculará o BPC do texto frente a todos os modelos em `modelos/` e exibirá o idioma identificado, além do ranking completo por custo.

**OBS.:** Utilizamos barra normal (`/`) considerando ambientes como Git Bash, WSL e PowerShell. Em caso de problemas no `cmd.exe`, substitua por barra invertida (`\`).
