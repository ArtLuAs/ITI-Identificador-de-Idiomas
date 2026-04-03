import os
import sys

def extrair_frase_apos_duas_colunas(arquivo_entrada, arquivo_saida):
    with open(arquivo_entrada, 'r', encoding='utf-8') as f_in, \
         open(arquivo_saida, 'a', encoding='utf-8') as f_out:
        
        for linha in f_in:
            linha = linha.rstrip('\n')
            if not linha: continue
                
            partes = linha.split('\t', 2)
            if len(partes) >= 3:
                f_out.write(partes[2] + '\n')

def extrair_frase_apos_uma_coluna(arquivo_entrada, arquivo_saida):
    with open(arquivo_entrada, 'r', encoding='utf-8') as f_in, \
         open(arquivo_saida, 'a', encoding='utf-8') as f_out:
        
        for linha in f_in:
            linha = linha.rstrip('\n')
            if not linha: continue
                
            partes = linha.split('\t', 1)
            if len(partes) >= 2:
                f_out.write(partes[1] + '\n')


# Lê o idioma do argumento passado pelo run.sh (ex: python3 main.py "pt")
# Se não receber nenhum argumento, usa 'pt' como padrão
idioma = sys.argv[1] if len(sys.argv) > 1 else 'pt'

pasta_destino = 'corpora'
pasta_entrada = 'data input'
extensao = '.txt'

# Cria a pasta 'corpora' a partir do terminal aberto (na raiz)
os.makedirs(pasta_destino, exist_ok=True)

# Define o caminho de saída relativo: "corpora/pt.txt" (ou o idioma recebido)
arquivo_unico_saida = os.path.join(pasta_destino, f"{idioma}{extensao}")

# Define os caminhos de entrada relativos apontando para a pasta "data input"
arquivo_tsv_entrada = os.path.join(pasta_entrada, 'idioma.tsv')
arquivo_txt_entrada = os.path.join(pasta_entrada, 'idioma_news.txt')

# Cria/Limpa o arquivo de saída txt
with open(arquivo_unico_saida, 'w', encoding='utf-8') as f:
    pass

print(f"Idioma: {idioma}")
print(f"O arquivo final será salvo em: {arquivo_unico_saida}")

print("Processando o arquivo .tsv...")
try:
    extrair_frase_apos_duas_colunas(arquivo_tsv_entrada, arquivo_unico_saida)
except FileNotFoundError:
    print(f"  -> ERRO: Não achei o arquivo '{arquivo_tsv_entrada}'.")

print("Processando o arquivo .txt...")
try:
    extrair_frase_apos_uma_coluna(arquivo_txt_entrada, arquivo_unico_saida)
except FileNotFoundError:
    print(f"  -> ERRO: Não achei o arquivo '{arquivo_txt_entrada}'.")

print("Fim do processo.")
