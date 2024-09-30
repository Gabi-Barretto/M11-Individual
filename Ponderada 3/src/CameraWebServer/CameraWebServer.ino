#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_camera.h"
#include <WiFi.h>
#include <esp_timer.h>

// Configurações WiFi
const char *ssid = "Claro";
const char *password = "Prince123";

// Semáforo para controle de fluxo de imagem
SemaphoreHandle_t imgSemaphore;

// Função para configurar a câmera
void setupCamera() {
    camera_config_t config;
    // (Configurações da câmera aqui, iguais às do seu código)
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Erro na inicialização da câmera: 0x%x", err);
    }
}

// Thread de Aquisição de Imagens
void imageCaptureTask(void *pvParameters) {
    while (true) {
        camera_fb_t *fb = esp_camera_fb_get();
        if (fb) {
            // Nova imagem capturada, libera o semáforo
            xSemaphoreGive(imgSemaphore);
            esp_camera_fb_return(fb);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);  // Captura de imagem a cada segundo
    }
}

// Thread de Envio de Imagens
void imageSendTask(void *pvParameters) {
    while (true) {
        // Aguardar pelo semáforo da thread de aquisição de imagens
        if (xSemaphoreTake(imgSemaphore, portMAX_DELAY)) {
            // Enviar imagem via WiFi para o computador
            Serial.println("Enviando imagem...");
            // Aqui você poderia adicionar código para enviar a imagem por HTTP, MQTT, etc.
        }
    }
}

// Thread de Recebimento de Detecção
void detectionReceiveTask(void *pvParameters) {
    while (true) {
        if (Serial.available() > 0) {
            String msg = Serial.readString();
            Serial.printf("Recebido do PC: %s\n", msg.c_str());
            // Aqui você pode processar a informação recebida
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);  // Checagem periódica da entrada serial
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println();

    // Conectar ao WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi conectado");

    // Configurar a câmera
    setupCamera();

    // Criar o semáforo binário
    imgSemaphore = xSemaphoreCreateBinary();

    // Criar as threads
    xTaskCreate(imageCaptureTask, "CapturaImagem", 4096, NULL, 1, NULL);
    xTaskCreate(imageSendTask, "EnvioImagem", 4096, NULL, 1, NULL);
    xTaskCreate(detectionReceiveTask, "RecebimentoDeteccao", 2048, NULL, 1, NULL);
}

void loop() {
    // Não fazer nada no loop principal
}