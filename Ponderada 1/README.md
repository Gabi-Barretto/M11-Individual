# Documentação da ALU de 8 Bits

## Introdução
Esta Unidade Lógica e Aritmética (ALU) foi desenvolvida para realizar uma série de operações lógicas e aritméticas em operandos de 8 bits. A ALU é composta de diversos componentes que trabalham em conjunto para fornecer a funcionalidade completa de um processador aritmético. A seguir, apresentaremos uma visão geral de cada um dos componentes utilizados neste projeto.

![Imagem](Mídia\ALU.png)

[Vídeo - Github]()
[Vídeo - Drive](https://drive.google.com/file/d/1OljJEi3arSLoqxeWVNnAkPtdPRVt8jhu/view?usp=sharing)

## Componentes

### 1. **Main**
O componente `main` é a unidade central do projeto da ALU de 8 bits. Ele integra todos os outros subcomponentes e é responsável por coordenar a operação global da ALU. No `main`, as entradas são recebidas, os sinais de controle são distribuídos para selecionar as operações apropriadas e os resultados são combinados e exibidos. Esse bloco principal serve como a "casa" onde toda a lógica se interconecta.

### 2. **Registrador**
O `Registrador` é um bloco que armazena temporariamente valores de 8 bits. Ele é essencial para armazenar operandos ou resultados intermediários e permite que os dados sejam utilizados em operações subsequentes. Registradores são usados frequentemente para armazenar o valor de `A` e `B`, além do valor de saída da ALU, de modo que possam ser utilizados em cálculos futuros.

### 3. **Left Shifter**
O componente `Left Shifter` realiza a operação de deslocamento lógico à esquerda (left shift) nos bits de um operando de 8 bits. Ao deslocar os bits para a esquerda, o valor numérico do operando é multiplicado por 2 para cada deslocamento realizado. O bit mais à direita é preenchido com `0`. Esse tipo de operação é útil em algoritmos de multiplicação e na manipulação de bits.

### 4. **Right Shifter**
O `Right Shifter` realiza a operação de deslocamento lógico à direita (right shift). O deslocamento à direita divide o valor do operando por 2 a cada deslocamento, descartando o bit menos significativo (à direita) e preenchendo o bit mais significativo com `0`. Essa operação é amplamente utilizada em algoritmos de divisão e manipulação de bits.

### 5. **Adder (Somador)**
O `Adder` é um somador de 8 bits que realiza a operação de adição entre dois operandos (`A` e `B`). Ele também pode considerar um bit de carry (ou `carry-in`) da posição anterior, permitindo somas maiores ou a combinação com outras operações aritméticas, como subtração via complemento de dois. O `Adder` gera uma saída de 8 bits e um bit de carry-out, que indica se houve transbordo.

### 6. **3-to-8 (Decodificador 3-para-8)**
O `3-to-8` é um decodificador que converte uma entrada de 3 bits em uma saída de 8 linhas. Ele é usado como uma unidade de controle para selecionar qual operação a ALU deve realizar, com base nos sinais de controle fornecidos. Cada combinação dos 3 bits de entrada ativa uma única linha de saída, que é usada para selecionar qual bloco da ALU estará ativo em um determinado momento.

### 7. **Comparator (Comparador)**
O `Comparator` é responsável por comparar os dois operandos de 8 bits (`A` e `B`). Ele gera sinais de status indicando se `A` é igual a `B`, maior que `B` ou menor que `B`. Esses sinais são importantes em instruções condicionais de um processador, onde decisões devem ser tomadas com base nos valores comparados.

### 8. **ALU**
O componente `ALU` é a unidade que reúne todas as operações aritméticas e lógicas. Ele conecta os componentes como o `Adder`, `Subtractor`, `Left Shifter`, `Right Shifter`, e `Comparator`, de forma que as operações sejam escolhidas de acordo com os sinais de controle vindos do decodificador `3-to-8`. Este é o coração da Unidade Lógica e Aritmética e realiza as funções centrais necessárias para o processamento de dados.

### 9. **Subtractor (Subtrator)**
O `Subtractor` é responsável por realizar a subtração de dois operandos de 8 bits. Ele utiliza a lógica do complemento de dois para transformar a subtração em uma operação de adição (invertendo os bits de `B` e adicionando um). Ele também utiliza o bit de `borrow` para operações de múltiplas etapas e pode gerar um sinal de `borrow out` para indicar que houve um empréstimo durante a operação.

## Fluxo de Operação
1. **Entrada**: Os operandos de 8 bits (`A` e `B`) são armazenados nos `Registradores`.
2. **Seleção de Operação**: Um conjunto de sinais de controle é fornecido ao `3-to-8`, que decodifica e ativa a operação desejada, direcionando a ALU a realizar, por exemplo, uma soma, subtração, ou deslocamento.
3. **Execução da Operação**: O `Adder`, `Subtractor`, `Left Shifter`, `Right Shifter`, ou `Comparator` realiza a operação apropriada.
4. **Saída**: O resultado da operação é disponibilizado como saída, que pode ser armazenado de volta nos `Registradores` ou utilizado em operações subsequentes.