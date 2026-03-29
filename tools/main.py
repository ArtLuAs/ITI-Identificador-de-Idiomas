def extrair_frase_apos_duas_colunas(arquivo_entrada, arquivo_saida):
    """
    Lê um arquivo .tsv (ID, idioma, Frase) e extrai a 3ª parte em diante.
    """
    # Usamos 'a' (append) para não apagar o que já estiver no arquivo de saída
    with open(arquivo_entrada, 'r', encoding='utf-8') as f_in, \
         open(arquivo_saida, 'a', encoding='utf-8') as f_out:
        
        for linha in f_in:
            linha = linha.rstrip('\n')
            if not linha:
                continue
                
            # Divide em 3 partes (separadas pelas 2 primeiras tabulações)
            partes = linha.split('\t', 2)
            if len(partes) >= 3:
                f_out.write(partes[2] + '\n')


def extrair_frase_apos_uma_coluna(arquivo_entrada, arquivo_saida):
    """
    Lê um arquivo .txt (ID, Frase) e extrai a 2ª parte em diante.
    """
    # Usamos 'a' (append) aqui também
    with open(arquivo_entrada, 'r', encoding='utf-8') as f_in, \
         open(arquivo_saida, 'a', encoding='utf-8') as f_out:
        
        for linha in f_in:
            linha = linha.rstrip('\n')
            if not linha:
                continue
                
            # Divide em 2 partes (separadas pela 1ª tabulação)
            partes = linha.split('\t', 1)
            if len(partes) >= 2:
                f_out.write(partes[1] + '\n')


# ==========================================
# ===== CONFIGURAÇÃO E EXECUÇÃO ============
# ==========================================

# 1. Defina os nomes dos seus arquivos
arquivo_tsv_entrada = 'por_sentences.tsv'
arquivo_txt_entrada = 'portugues_news.txt'
arquivo_unico_saida = 'portugues_data.txt'

# 2. Prepara o arquivo de saída
# Abrimos rapidinho com 'w' só para criar o arquivo do zero (ou limpar ele) 
# antes de começar a adicionar as coisas com o 'a' nas funções.
open(arquivo_unico_saida, 'w', encoding='utf-8').close()

# 3. Executa as funções na ordem que você preferir
print("Processando o arquivo .tsv...")
extrair_frase_apos_duas_colunas(arquivo_tsv_entrada, arquivo_unico_saida)

print("Processando o arquivo .txt...")
extrair_frase_apos_uma_coluna(arquivo_txt_entrada, arquivo_unico_saida)

print(f"Sucesso! Todas as frases foram salvas em: {arquivo_unico_saida}")