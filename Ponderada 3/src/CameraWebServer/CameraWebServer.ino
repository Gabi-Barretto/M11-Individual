#include "esp_camera.h"
#include <WiFi.h>
#include "esp_http_server.h"
#include "FS.h"
#include "SD_MMC.h"

const char *ssid = "Wifi";       // Substitua pelo nome da sua rede WiFi
const char *password = "Senha";  // Substitua pela senha da sua rede WiFi

httpd_handle_t camera_httpd = NULL;
SemaphoreHandle_t captureSemaphore;
SemaphoreHandle_t receiveSemaphore;

// Função da task de captura de imagem
void captureImageTask(void *pvParameters) {
    httpd_req_t *req = (httpd_req_t *)pvParameters;

    // Espera o semáforo ser liberado para começar a captura
    if (xSemaphoreTake(captureSemaphore, portMAX_DELAY)) {
        camera_fb_t *fb = esp_camera_fb_get();
        if (!fb) {
            Serial.println("Falha ao capturar imagem");
            httpd_resp_send_500(req);
            vTaskDelete(NULL);
        }

        // Envia a imagem capturada como resposta
        esp_err_t res = httpd_resp_set_type(req, "image/jpeg");
        if (res == ESP_OK) {
            res = httpd_resp_set_hdr(req, "Content-Disposition", "inline; filename=capture.jpg");
        }
        if (res == ESP_OK) {
            res = httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
        }
        if (res == ESP_OK) {
            res = httpd_resp_send(req, (const char *)fb->buf, fb->len);
        }

        if (res != ESP_OK) {
            Serial.println("Falha ao enviar a imagem");
        } else {
            Serial.println("Imagem enviada com sucesso");
        }

        esp_camera_fb_return(fb);
        vTaskDelete(NULL);  // Deleta a task após completar
    }
}

// Função da task para receber imagem processada e salvar no SD
void receiveImageTask(void *pvParameters) {
    httpd_req_t *req = (httpd_req_t *)pvParameters;

    // Espera o semáforo ser liberado para começar a recepção
    if (xSemaphoreTake(receiveSemaphore, portMAX_DELAY)) {
        // Lógica para receber e salvar a imagem processada
        char buf[1024];
        int total_len = req->content_len;
        int cur_len = 0;
        File file = SD_MMC.open("/processed_image.jpg", FILE_WRITE);

        if (!file) {
            Serial.println("Falha ao abrir arquivo no SD");
            httpd_resp_send_500(req);
            vTaskDelete(NULL);
        }

        while (cur_len < total_len) {
            int received = httpd_req_recv(req, buf, sizeof(buf));
            if (received <= 0) {
                Serial.println("Erro ao receber a imagem");
                file.close();
                httpd_resp_send_500(req);
                vTaskDelete(NULL);
            }
            file.write((const uint8_t *)buf, received);
            cur_len += received;
        }

        file.close();
        Serial.println("Imagem processada salva no SD");
        httpd_resp_send(req, "Imagem recebida e salva no SD", HTTPD_RESP_USE_STRLEN);
        vTaskDelete(NULL);  // Deleta a task após completar
    }
}

// Função para lidar com o endpoint /capture
static esp_err_t capture_handler(httpd_req_t *req) {
    // Cria a task de captura de imagem
    BaseType_t taskCreated = xTaskCreate(
        captureImageTask,       // Função da task
        "Capture Image Task",   // Nome da task
        4096,                   // Tamanho da stack
        req,                    // Parâmetro passado (o request)
        1,                      // Prioridade da task
        NULL                    // Handle da task
    );

    if (taskCreated != pdPASS) {
        Serial.println("Erro ao criar a task de captura");
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    // Libera o semáforo para a task de captura iniciar
    xSemaphoreGive(captureSemaphore);

    return ESP_OK;
}

// Função para lidar com o endpoint /receive
static esp_err_t receive_handler(httpd_req_t *req) {
    // Cria a task de recepção de imagem processada
    BaseType_t taskCreated = xTaskCreate(
        receiveImageTask,       // Função da task
        "Receive Image Task",   // Nome da task
        4096,                   // Tamanho da stack
        req,                    // Parâmetro passado (o request)
        1,                      // Prioridade da task
        NULL                    // Handle da task
    );

    if (taskCreated != pdPASS) {
        Serial.println("Erro ao criar a task de recepção");
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    // Libera o semáforo para a task de recepção iniciar
    xSemaphoreGive(receiveSemaphore);

    return ESP_OK;
}

// Configuração do servidor web
void startCameraServer() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = 80;

    httpd_uri_t capture_uri = {
        .uri = "/capture",
        .method = HTTP_GET,
        .handler = capture_handler,
        .user_ctx = NULL
    };

    httpd_uri_t receive_uri = {
        .uri = "/receive",
        .method = HTTP_POST,
        .handler = receive_handler,
        .user_ctx = NULL
    };

    if (httpd_start(&camera_httpd, &config) == ESP_OK) {
        httpd_register_uri_handler(camera_httpd, &capture_uri);
        httpd_register_uri_handler(camera_httpd, &receive_uri);
        Serial.println("Servidor HTTP iniciado");
    } else {
        Serial.println("Erro ao iniciar o servidor HTTP");
    }
}

void setup() {
    Serial.begin(115200);

    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 10;
    config.fb_count = 1;

    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Falha ao inicializar a câmera, erro 0x%x", err);
        return;
    }

    if (!SD_MMC.begin()) {
        Serial.println("Falha ao montar o cartão SD");
        return;
    }
    Serial.println("Cartão SD montado com sucesso");

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("WiFi conectado");

    // Inicializa o servidor da câmera
    startCameraServer();

    Serial.print("Use 'http://");
    Serial.print(WiFi.localIP());
    Serial.println("/capture' para capturar uma imagem");

    // Cria semáforos para controle
    captureSemaphore = xSemaphoreCreateBinary();
    receiveSemaphore = xSemaphoreCreateBinary();

    if (captureSemaphore == NULL || receiveSemaphore == NULL) {
        Serial.println("Erro ao criar os semáforos");
    }
}

void loop() {
    // O loop está vazio porque as tasks estão sendo executadas em paralelo
}
