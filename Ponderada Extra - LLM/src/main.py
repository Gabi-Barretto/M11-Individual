from transformers import pipeline

# Inicializa o pipeline de geração de texto com DistilGPT-2
generator = pipeline('text-generation', model='distilgpt2')

def gerar_texto(prompt, max_length=50):
    """
    Função para gerar texto baseado no prompt dado.
    """
    # Gera a continuação do texto com truncamento corrigido
    result = generator(prompt, max_length=max_length, truncation=True, num_return_sequences=1)

    # Retorna o texto gerado
    return result[0]['generated_text']

def main():
    print("Bem-vindo ao gerador de texto com DistilGPT-2!")
    
    # Loop para continuar solicitando prompts até o usuário desejar parar
    while True:
        # Solicita o prompt inicial do usuário
        prompt = input("Digite o texto inicial (ou 'sair' para encerrar): ")

        # Verifica se o usuário deseja encerrar
        if prompt.lower() == 'sair':
            print("Encerrando o gerador de texto.")
            break

        # Define o comprimento máximo do texto gerado
        try:
            max_length = int(input("Digite o comprimento máximo do texto gerado (número inteiro): "))
        except ValueError:
            print("Valor inválido. Usando o comprimento padrão de 50.")
            max_length = 50

        # Gera o texto com base no prompt e no comprimento especificado
        texto_gerado = gerar_texto(prompt, max_length=max_length)
        
        # Exibe o texto gerado
        print("\nTexto gerado:\n")
        print(texto_gerado)
        print("\n" + "="*50 + "\n")

if __name__ == "__main__":
    main()