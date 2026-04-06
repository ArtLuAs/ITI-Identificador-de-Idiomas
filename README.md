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

- **Treinamento (`interpretador.cpp`)**: Processa os corpora de cada um dos 27 idiomas, alimentando a árvore de contextos (Trie + Árvore de Fenwick) e ajustando as probabilidades dos caracteres. Para evitar overflow com grandes volumes de dados, as frequências passam por um mecanismo de *aging* (halving) quando atingem um limite predefinido.
- **Persistência (`modelopersistente`)**: Para evitar o reprocessamento dos corpora a cada execução, este módulo serializa a Trie em disco, salvando os modelos consolidados de cada idioma no diretório `models/`. O total de modelos gerados ocupa aproximadamente **6 GB**.
- **Identificação (`avaliador.cpp`)**: Carrega os 27 modelos do diretório `models/` e calcula o custo de codificação (BPC) do texto de entrada frente a cada um. O idioma identificado é aquele que apresentar o menor valor de BPC.
- **Ordem de Contexto**: A profundidade máxima de contexto adotada foi $K_{max}=4$, definida em função das limitações de hardware disponível. Valores maiores de $K$ aumentam a acurácia em textos longos, porém elevam significativamente o tamanho da Trie e o tempo de processamento.

### Idiomas Analisados:
Foram analisados **27 idiomas** baseados no alfabeto latino, categorizados por família linguística:

| Família | Idiomas |
|---|---|
| **Românicas** | Português, Espanhol, Francês, Italiano, Catalão, Galego, Corso, Valão |
| **Germânicas** | Inglês, Alemão, Holandês, Sueco, Dinamarquês, Norueguês, Islandês, Faroense, Afrikaans, Luxemburguês |
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

Este projeto foi desenvolvido utilizando apenas a **biblioteca padrão do C++ (STL)**, sem dependências externas. O módulo de pós-processamento e geração de gráficos utiliza **Python 3** com as bibliotecas `matplotlib` e `pandas`.

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
├── documentos/                     # Relatório e apresentação do projeto
├── models/                         # Modelos treinados por idioma (gerados pelo interpretador)
├── headers/
│   ├── ArithmeticCoder.hpp         # Codificação aritmética (bits e codificador/decodificador)
│   ├── ContextModel.hpp            # Modelo de contextos PPM (Trie e gerenciamento)
│   ├── tabelaFrequencia.hpp        # Tabelas de frequência com Fenwick Tree e exclusão
│   └── TrieNode.hpp                # Nó da Trie com frequências e filhos
├── src/
│   ├── ArithmeticCoder.cpp         # Implementação da E/S de bits e codificação
│   ├── ContextModel.cpp            # Gerenciamento da árvore de contextos
│   ├── tabelaFrequencia.cpp        # Operações nas tabelas de frequência
│   └── TrieNode.cpp                # Manipulação dos nós da Trie
├── interpretador.cpp               # Treinamento: lê corpora e gera os arquivos .model
├── modelopersistente.cpp           # Serialização/desserialização da Trie em disco
├── avaliador.cpp                   # Identificação: calcula BPC e aponta o idioma
├── treinar_todos.sh                # Script para treinar todos os 27 idiomas em sequência
├── plotar_resultados.py            # Geração dos gráficos de BPC por idioma
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
- Terminal de linha de comando
- Python 3 com `matplotlib` e `pandas` para geração dos gráficos

```sh
pip install matplotlib pandas
```

---

### Etapa 1 — Treinamento (gerar os modelos)

Compile o interpretador:

```sh
# Linux / Git Bash / WSL
g++ src/*.cpp interpretador.cpp modelopersistente.cpp -o interpretador -O2

# Windows (PowerShell)
g++ src/*.cpp interpretador.cpp modelopersistente.cpp -o interpretador.exe -O2
```

Treine um idioma específico (ex: Português):

```sh
./interpretador corpus/por.txt models/por.model 4
```

Ou treine todos os idiomas de uma vez usando o script:

```sh
bash treinar_todos.sh
```

> O parâmetro final (`4`) é o valor de $K_{max}$. Os modelos gerados ficam no diretório `models/` e ocupam cerca de **6 GB** no total.

---

### Etapa 2 — Identificação (avaliar um texto)

Compile o avaliador:

```sh
# Linux / Git Bash / WSL
g++ src/*.cpp avaliador.cpp modelopersistente.cpp -o avaliador -O2

# Windows (PowerShell)
g++ src/*.cpp avaliador.cpp modelopersistente.cpp -o avaliador.exe -O2
```

Execute a identificação:

```sh
# Linux
./avaliador "Eu amo Teoria da Informação!" models/ 4

# Windows
./avaliador.exe "Eu amo Teoria da Informação!" models/ 4
```

O sistema calculará o BPC do texto frente a cada um dos 27 modelos e exibirá o idioma identificado, além da lista completa de idiomas ordenada por custo.

---

### Etapa 3 — Geração dos gráficos

Após executar o avaliador e exportar os resultados em `.txt`:

```sh
python3 plotar_resultados.py   # Linux / macOS
python  plotar_resultados.py   # Windows
```

**OBS.:** Utilizamos barra normal (`/`) considerando ambientes como Git Bash, WSL e PowerShell. Em caso de problemas no Windows com `cmd.exe`, substitua por barra invertida (`\`).
