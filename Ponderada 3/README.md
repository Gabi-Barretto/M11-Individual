# Sistema de Detecção de Faces com ESP32-CAM e Python (OpenCV)

## Sumário

1. [Introdução](#introducao)
2. [Objetivos do Projeto](#objetivos)
3. [Hardware Utilizado](#hardware)
4. [Software Utilizado](#software)
5. [Detalhes Técnicos](#tecnicos)
   - [Configuração do ESP32-CAM](#configuracao-esp32-cam)
   - [Detecção de Faces com Haarcascade](#haarcascade)
   - [Implementação do Script Python com OpenCV](#opencv)
6. [Passo a Passo de Implementação](#implementacao)
7. [Melhorias Futuras](#melhorias)
8. [Referências](#referencias)

<a name="introducao"></a>
## 1. Introdução
Neste projeto, desenvolvemos um sistema de monitoramento que captura imagens a partir da ESP32-CAM e realiza a detecção de faces utilizando Python e OpenCV. Esse sistema é útil para casos onde é necessário monitorar ambientes, identificar a presença de pessoas, e gerar alertas ou análises automáticas.

A ESP32-CAM é um microcontrolador de baixo custo com câmera integrada que envia frames de vídeo via WiFi. Utilizamos uma rede neural Haarcascade para realizar a detecção de faces nos frames recebidos.

#### **Evidência videográfica:**

[Github](https://github.com/Gabi-Barretto/M11-Individual/tree/main/Ponderada%202/M%C3%ADdia/teste.mp4)

[Drive](https://drive.google.com/file/d/1IcJNcKOOGsvSICuq5Fdwwuo3pNq62on0/view?usp=sharing)

<a name="objetivos"></a>
## 2. Objetivos do Projeto
- Capturar e transmitir vídeo ao vivo utilizando uma ESP32-CAM.
- Utilizar Python e OpenCV para processar o vídeo transmitido.
- Detectar faces em tempo real a partir dos frames capturados.
- Aprender sobre redes neurais Haarcascade e como aplicá-las em um cenário prático.

<a name="hardware"></a>
## 3. Hardware Utilizado
### 3.1. ESP32-CAM
A **ESP32-CAM** é um módulo microcontrolador com uma câmera embutida, capaz de realizar processamento de imagens e transmitir dados por WiFi. Os principais detalhes do hardware são:

- **Microcontrolador ESP32**: CPU dual-core com suporte a WiFi e Bluetooth.
- **Câmera OV2640**: Sensor de imagem que captura até 2MP de resolução.
- **PSRAM Integrado**: Memória RAM adicional para lidar com tarefas mais exigentes de imagem.
- **Flash LED**: Capaz de iluminar o ambiente quando necessário.

### 3.2. Requisitos para o Projeto
- **ESP32-CAM** (com PSRAM para melhor desempenho).
- **Adaptador FTDI** para programar a ESP32-CAM.
- **Fonte de alimentação USB**.
- **Cabo micro USB**.

<a name="software"></a>
## 4. Software Utilizado
- **Arduino IDE**: Utilizado para programar o ESP32-CAM.
- **Python 3.11**: Utilizado para desenvolver o script de processamento de vídeo.
- **OpenCV 4.8.0**: Biblioteca para processamento de imagem e visão computacional.
- **Matplotlib**: Utilizado para exibir os frames do vídeo em ambiente sem suporte GUI.
- **Biblioteca Haarcascade**: Utilizada para detectar faces.

<a name="tecnicos"></a>
## 5. Detalhes Técnicos

<a name="configuracao-esp32-cam"></a>
### 5.1. Configuração do ESP32-CAM
A configuração do ESP32-CAM foi realizada utilizando o código abaixo. O módulo foi configurado para iniciar um servidor HTTP e transmitir as imagens capturadas pela câmera.

#### Código ESP32-CAM
```cpp
#include "esp_camera.h"
#include <WiFi.h>
#define CAMERA_MODEL_AI_THINKER

#include "camera_pins.h"

const char *ssid = "NomeDoWiFi";
const char *password = "SenhaDoWiFi";

void startCameraServer();

void setup() {
  Serial.begin(115200);
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.frame_size = FRAMESIZE_QVGA;
  config.pixel_format = PIXFORMAT_JPEG;
  config.fb_count = 1;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Erro ao inicializar a câmera: 0x%x", err);
    return;
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  startCameraServer();
  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' para conectar.");
}

void loop() {
  delay(10000);
}
```
#### Configurações Importantes
- **Resolução (frame_size)**: Ajustada para `FRAMESIZE_QVGA` para manter um equilíbrio entre qualidade de imagem e eficiência de transmissão.
- **Pixel Format**: Usado `PIXFORMAT_JPEG` para transmitir vídeo comprimido.

<a name="haarcascade"></a>
### 5.2. Detecção de Faces com Haarcascade
**Haarcascade** é um método de detecção de objetos introduzido por Paul Viola e Michael Jones em 2001. Baseia-se em uma técnica que usa uma cascata de classificadores que, ao serem aplicados sucessivamente, conseguem identificar objetos em imagens de forma rápida e eficiente.

#### Como Funciona
- **Características Haar**: São usadas como filtros que examinam padrões claros e escuros na imagem (por exemplo, áreas claras e escuras de uma face).
- **Janela Deslizante**: O algoritmo aplica uma janela de tamanho variável em toda a imagem para encontrar áreas que correspondem a um padrão pré-definido.
- **Classificador em Cascata**: Consiste em uma série de etapas que rejeitam rapidamente regiões que não contêm o objeto, enquanto examinam com mais detalhes regiões que passam pelos primeiros filtros.

#### Implementação no Python
Para este projeto, utilizamos o classificador pré-treinado `haarcascade_frontalface_default.xml` para detectar faces.

<a name="opencv"></a>
### 5.3. Implementação do Script Python com OpenCV
O script Python recebe o stream da ESP32-CAM e aplica o classificador Haarcascade para identificar e marcar as faces. Abaixo está o código do script Python.

#### Código Python
```python
import cv2
import numpy as np
import matplotlib.pyplot as plt
import time

# Carregar o classificador Haarcascade
face_cascade = cv2.CascadeClassifier('C:/Users/Inteli/Documents/GitHub/M11-Individual/Ponderada 2/src/haarcascade_frontalface_default.xml')

# URL do stream de vídeo da ESP32-CAM
camera_url = 'http://192.168.254.176:81/stream'

# Inicializar a captura de vídeo
cap = cv2.VideoCapture(camera_url)

# Verificar se a captura foi aberta corretamente
if not cap.isOpened():
    print("Erro ao abrir o stream de vídeo.")
    exit()

while True:
    # Capturar frame
    ret, frame = cap.read()
    if not ret:
        print("Falha ao capturar frame. Tentando reconectar...")
        time.sleep(1)
        continue

    # Converter para escala de cinza e detectar faces
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))

    # Desenhar retângulos ao redor das faces detectadas
    for (x, y, w, h) in faces:
        cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 2)

    # Exibir a imagem usando matplotlib
    plt.imshow(cv2.cvtColor(frame, cv2.COLOR_BGR2RGB))
    plt.axis('off')
    plt.show()

    # Sair após visualizar um frame
    break

cap.release()
```

#### Detalhes Importantes
- **Processamento de Imagem**: Cada frame capturado é convertido para escala de cinza antes de aplicar o classificador Haarcascade.
- **Filtragem e Detecção**: A função `detectMultiScale()` é usada para detectar faces.
- **Visualização**: Como não tínhamos suporte a GUI, utilizamos `matplotlib` para visualizar as imagens.

<a name="implementacao"></a>
## 6. Passo a Passo de Implementação
1. **Configuração do ESP32-CAM**: Programar a ESP32-CAM com a IDE Arduino, configurando WiFi e o servidor da câmera.
2. **Configuração do Ambiente Python**:
   - Instalar Python e OpenCV (`pip install opencv-python matplotlib`).
   - Garantir que o arquivo Haarcascade (`haarcascade_frontalface_default.xml`) esteja no local certo.
3. **Executar o Script Python**:
   - Executar o script Python para se

 conectar ao servidor de vídeo da ESP32-CAM e processar os frames.

<a name="melhorias"></a>
## 7. Melhorias Futuras
- **Aprimorar a qualidade do stream**: Ajustar a resolução e qualidade da imagem na ESP32-CAM.
- **Detecção em Tempo Real**: Utilizar uma interface gráfica para permitir visualização em tempo real.
- **Adicionar Deep Learning**: Substituir Haarcascade por redes neurais convolucionais (CNNs), como MobileNet, para uma detecção mais precisa.

<a name="referencias"></a>
## 8. Referências
- **ESP32-CAM Datasheet**: [Espressif Documentation](https://www.espressif.com/en/products/socs/esp32)
- **OpenCV Documentation**: [OpenCV Haarcascade Classifiers](https://github.com/opencv/opencv/tree/master/data/haarcascades)
- **Viola, P., & Jones, M.**: [Rapid Object Detection using a Boosted Cascade of Simple Features, 2001.](https://ieeexplore.ieee.org/document/990517)