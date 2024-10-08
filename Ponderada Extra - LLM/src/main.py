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
    print("Welcome to the DistilGPT-2 text generator!")
    
    # Loop para continuar solicitando prompts até o usuário desejar parar
    while True:
        # Solicita o prompt inicial do usuário
        prompt = input("Enter your initial text in English (or 'exit' to stop): ")

        # Verifica se o usuário deseja encerrar
        if prompt.lower() == 'exit':
            print("Shutting down the text generator.")
            break

        # Define o comprimento máximo do texto gerado
        try:
            max_length = int(input("Enter the maximum text length (integer value): "))
        except ValueError:
            print("Invalid value. Using default length of 50.")
            max_length = 50

        # Gera o texto com base no prompt e no comprimento especificado
        generated_text = gerar_texto(prompt, max_length=max_length)
        
        # Exibe o texto gerado
        print("\nGenerated text:\n")
        print(generated_text)
        print("\n" + "="*50 + "\n")

if __name__ == "__main__":
    main()