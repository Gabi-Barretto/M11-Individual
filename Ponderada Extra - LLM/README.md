### Utilizando Modelos de Linguagem Grande (LLM) no Raspberry Pi 5 com DistilGPT-2

#### 1. **Introdução**

Este guia atualizado mostra como rodar o modelo **DistilGPT-2** no **Raspberry Pi 5** para gerar textos dinamicamente. O modelo é uma versão compacta do GPT-2, ajustada para rodar em dispositivos com recursos limitados, como o Raspberry Pi. Agora, além de rodar o modelo com sucesso, o código permite interatividade via terminal, corrigiu o problema de truncamento e oferece a opção de ajustar o comprimento do texto gerado a cada interação.

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
```

#### 5. **Rodando o Script**
1. Salve o código como, por exemplo, `gerador_dinamico.py`.
2. Execute o script no terminal:

   ```bash
   python3 gerador_dinamico.py
   ```

3. Insira um prompt quando solicitado e ajuste o comprimento máximo de texto para cada geração. O programa continuará a solicitar novos prompts até que você digite `"sair"`.

### Exemplo de Execução

```bash
Bem-vindo ao gerador de texto com DistilGPT-2!
Digite o texto inicial (ou 'sair' para encerrar): Era uma vez
Digite o comprimento máximo do texto gerado (número inteiro): 60

Texto gerado:

Era uma vez uma floresta sombria, onde as criaturas da noite se escondiam nas sombras. Uma coruja
observava calmamente enquanto os ventos sopravam.

==================================================

Digite o texto inicial (ou 'sair' para encerrar): Um robô estava no
Digite o comprimento máximo do texto gerado (número inteiro): 40

Texto gerado:

Um robô estava no comando da missão, mas algo deu errado. O sistema travou.

==================================================

Digite o texto inicial (ou 'sair' para encerrar): sair
Encerrando o gerador de texto.
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
Agora, com este guia, você tem um gerador de textos dinâmico rodando no seu Raspberry Pi 5, que permite inserir novos prompts e ajustar o comprimento dos textos de forma interativa. Com as correções e melhorias feitas, o processo está mais robusto e eficiente para seu uso.