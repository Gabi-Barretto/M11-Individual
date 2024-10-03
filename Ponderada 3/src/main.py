import requests
import cv2
import threading
import time
import signal
import sys

# Definir o IP do ESP32-CAM
esp32_ip = "192.168.254.176"

# Semáforo para controlar a aquisição da imagem
image_ready_semaphore = threading.Semaphore(0)
image_path = "captured_image.jpg"
running = True  # Flag para controlar a execução das threads

# Função para capturar imagem do ESP32-CAM
def capture_image():
    global image_path, running
    url = f"http://{esp32_ip}/capture"
    while running:
        try:
            response = requests.get(url, timeout=10)
            if response.status_code == 200:
                with open(image_path, "wb") as f:
                    f.write(response.content)
                print("Imagem capturada e salva como 'captured_image.jpg'")
                image_ready_semaphore.release()  # Liberar o semáforo indicando que a imagem está pronta
            else:
                print(f"Erro ao capturar imagem. Código de status: {response.status_code}")
        except requests.exceptions.RequestException as e:
            print(f"Erro na solicitação: {e}")
        
        # Aguarda 5 segundos antes de capturar a próxima imagem
        time.sleep(5)

# Função para processar a imagem
def process_image():
    global image_path, running
    while running:
        # Aguardar a captura da imagem
        if image_ready_semaphore.acquire(timeout=5):
            # Processar a imagem capturada
            img = cv2.imread(image_path)
            gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
            face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')
            faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5)

            # Desenhar retângulos ao redor das faces detectadas
            for (x, y, w, h) in faces:
                cv2.rectangle(img, (x, y), (x + w, y + h), (255, 0, 0), 2)

            # Salvar a imagem processada
            processed_image_path = "processed_image.jpg"
            cv2.imwrite(processed_image_path, img)
            print("Imagem processada e salva como 'processed_image.jpg'")

            # Enviar a imagem processada de volta ao ESP32-CAM
            send_processed_image(processed_image_path)

# Função para enviar a imagem processada de volta ao ESP32-CAM
def send_processed_image(image_path):
    url = f"http://{esp32_ip}/upload"
    with open(image_path, 'rb') as f:
        files = {'plain': f}
        try:
            response = requests.post(url, files=files)
            if response.status_code == 200:
                print("Imagem processada enviada com sucesso")
            else:
                print(f"Erro ao enviar imagem. Código de status: {response.status_code}")
        except requests.exceptions.RequestException as e:
            print(f"Erro ao enviar imagem: {e}")

# Função para lidar com a interrupção do programa
def signal_handler(sig, frame):
    global running
    print("\nInterrupção recebida. Encerrando o programa...")
    running = False
    sys.exit(0)

# Configurar o sinal de interrupção
signal.signal(signal.SIGINT, signal_handler)

# Criar e iniciar a thread de aquisição de imagem
acquisition_thread = threading.Thread(target=capture_image)
acquisition_thread.daemon = True
acquisition_thread.start()

# Executar a thread principal para processar a imagem
if __name__ == "__main__":
    process_image()