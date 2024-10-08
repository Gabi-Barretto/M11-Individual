### Utilizando Modelos de Linguagem Grande (LLM) no Raspberry Pi 5 com DistilGPT-2

#### 1. **Introdução**

Este guia atualizado mostra como rodar o modelo **DistilGPT-2** no **Raspberry Pi 5** para gerar textos dinamicamente. O modelo é uma versão compacta do GPT-2, ajustada para rodar em dispositivos com recursos limitados, como o Raspberry Pi. Agora, além de rodar o modelo com sucesso, o código permite interatividade via terminal, corrigiu o problema de truncamento e oferece a opção de ajustar o comprimento do texto gerado a cada interação.

**Importante:** O DistilGPT-2 foi treinado para trabalhar com textos em **inglês**, portanto, os prompts fornecidos pelo usuário devem estar nesse idioma para gerar textos corretamente.

[Vídeo Demonstrativo](https://drive.google.com/file/d/1IxEfgYz4nK3jJHoNU66NPCAIg5XyA4d2/view?usp=sharing)

#### 2. **Pré-requisitos**
Antes de começar, você precisará garantir que o seguinte está configurado:
- Um **Raspberry Pi 5** com sistema operacional Linux.
- **Python 3.12** instalado.
- Acesso à internet para instalação das dependências.

#### 3. **Instalação das Dependências**
Instale as bibliotecas necessárias usando o `pip`. Abra o terminal do Raspberry Pi e execute:

```bash
pip install transformers torch
```

Isso irá instalar:
- **Transformers**: Biblioteca que facilita o uso de modelos de linguagem como DistilGPT-2.
- **Torch**: Framework de aprendizado profundo usado pelo modelo.

#### 4. **Script Python: Geração Dinâmica de Texto**
Este script permite gerar texto dinamicamente. Ele corrige os erros de truncamento e oferece a opção de inserir prompts repetidamente via terminal:

```python
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
```

#### 5. **Rodando o Script**
1. Salve o código como, por exemplo, `dynamic_text_generator.py`.
2. Execute o script no terminal:

   ```bash
   python3 dynamic_text_generator.py
   ```

3. Insira um prompt em **inglês** quando solicitado e ajuste o comprimento máximo de texto para cada geração. O programa continuará a solicitar novos prompts até que você digite `"exit"`.

### Exemplo de Execução

```bash
Welcome to the DistilGPT-2 text generator!
Enter your initial text in English (or 'exit' to stop): Once upon a time
Enter the maximum text length (integer value): 60

Generated text:

Once upon a time in a forest, there was a mysterious creature hiding in the shadows. The wind howled through the trees as it...

==================================================

Enter your initial text in English (or 'exit' to stop): A robot was standing
Enter the maximum text length (integer value): 40

Generated text:

A robot was standing in the middle of the room, waiting for the command. Suddenly, it began to move.

==================================================

Enter your initial text in English (or 'exit' to stop): exit
Shutting down the text generator.
```

#### 6. **Advertências e Soluções**
Ao rodar o código, você pode encontrar algumas advertências:

1. **Advertência sobre `clean_up_tokenization_spaces`:**
   - **Mensagem**: 
     ```bash
     FutureWarning: `clean_up_tokenization_spaces` was not set. It will be set to `True` by default. 
     ```
   - **Solução**: Pode ser ignorada, pois não afeta o funcionamento imediato. No entanto, você pode passar explicitamente `clean_up_tokenization_spaces=True` ao configurar o pipeline se desejar.

2. **Advertência sobre truncamento:**
   - **Mensagem**:
     ```bash
     Truncation was not explicitly activated but `max_length` is provided.
     ```
   - **Solução**: Foi corrigida ao adicionar `truncation=True` na chamada do pipeline.

#### 7. **Considerações Técnicas**
- **Interatividade:** O script agora solicita um prompt a cada iteração, permitindo gerar textos continuamente sem precisar reiniciar o script.
- **Truncamento Controlado:** O problema de truncamento automático foi resolvido. Agora, o comprimento máximo do texto gerado pode ser ajustado a cada execução.
- **Tratamento de Erros:** Se o usuário inserir um valor inválido para o comprimento do texto, o script usará o valor padrão de 50.

#### 8. **Otimizações Futuras**
- **Quantização:** Você pode otimizar ainda mais o uso de memória e desempenho quantizando o modelo para 8 bits, o que reduziria o uso de memória e aumentaria a eficiência.
- **TensorFlow Lite**: Considerar a conversão do modelo para **TensorFlow Lite** para melhorar a performance em dispositivos embarcados como o Raspberry Pi.

#### 9. **Conclusão**
Agora, com este guia, você tem um gerador de textos dinâmico rodando no seu Raspberry Pi 5, que permite inserir novos prompts em **inglês** e ajustar o comprimento dos textos de forma interativa. Com as correções e melhorias feitas, o processo está mais robusto e eficiente para seu uso.