import os

def extrair_frase_apos_duas_colunas(arquivo_entrada, arquivo_saida):
    """Lê um arquivo .tsv (ID, idioma, Frase) e extrai a 3ª parte em diante."""
    with open(arquivo_entrada, 'r', encoding='utf-8') as f_in, \
         open(arquivo_saida, 'a', encoding='utf-8') as f_out:
        
        for linha in f_in:
            linha = linha.rstrip('\n')
            if not linha: continue
                
            partes = linha.split('\t', 2)
            if len(partes) >= 3:
                f_out.write(partes[2] + '\n')


def extrair_frase_apos_uma_coluna(arquivo_entrada, arquivo_saida):
    """Lê um arquivo .txt (ID, Frase) e extrai a 2ª parte em diante."""
    with open(arquivo_entrada, 'r', encoding='utf-8') as f_in, \
         open(arquivo_saida, 'a', encoding='utf-8') as f_out:
        
        for linha in f_in:
            linha = linha.rstrip('\n')
            if not linha: continue
                
            partes = linha.split('\t', 1)
            if len(partes) >= 2:
                f_out.write(partes[1] + '\n')


# ==========================================
# ===== CONFIGURAÇÃO DINÂMICA ==============
# ==========================================

# 1. Defina o idioma e a pasta de destino
idioma = 'pt'              # Ex: 'pt', 'en', 'es'
pasta_destino = 'corpora'  # Pode ser 'corpora', 'modelos', etc.
extensao = '.model'        # Pode ser '.txt', '.model', etc.

# 2. Cria a pasta automaticamente se ela ainda não existir no seu computador
os.makedirs(pasta_destino, exist_ok=True)

# 3. Monta o caminho completo do arquivo de saída (Ex: corpora/pt.model)
arquivo_unico_saida = os.path.join(pasta_destino, f"{idioma}{extensao}")

# 4. Defina os nomes dos seus arquivos de entrada
arquivo_tsv_entrada = 'idioma.tsv'
arquivo_txt_entrada = 'idioma_news.txt'

# ==========================================
# ===== EXECUÇÃO ===========================
# ==========================================

# Limpa o arquivo de saída (caso você rode o script várias vezes seguidas)
# Usando o bloco 'with', não precisamos do .close() no final
with open(arquivo_unico_saida, 'w', encoding='utf-8') as f:
    pass 

print(f"O arquivo final será salvo em: {arquivo_unico_saida}")

print("Processando o arquivo .tsv...")
extrair_frase_apos_duas_colunas(arquivo_tsv_entrada, arquivo_unico_saida)

print("Processando o arquivo .txt...")
extrair_frase_apos_uma_coluna(arquivo_txt_entrada, arquivo_unico_saida)

print("Sucesso! Extração concluída.")