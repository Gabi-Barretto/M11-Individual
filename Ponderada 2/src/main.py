import cv2
import numpy as np
import time

# Caminho para o arquivo haarcascade
face_cascade = cv2.CascadeClassifier('C:/Users/Inteli/Documents/GitHub/M11-Individual/Ponderada 2/src/haarcascade_frontalface_default.xml')

# URL do stream de vídeo da ESP32-CAM
camera_url = 'http://192.168.254.176:81/stream'

# Inicializar a captura de vídeo a partir da ESP32-CAM
cap = cv2.VideoCapture(camera_url)

# Tentar reconectar várias vezes se não conseguir abrir o stream
attempts = 5
while not cap.isOpened() and attempts > 0:
    print("Tentando abrir o stream de vídeo...")
    cap = cv2.VideoCapture(camera_url)
    attempts -= 1
    time.sleep(2)

# Verificar se a captura foi aberta corretamente
if not cap.isOpened():
    print("Erro ao abrir o stream de vídeo. Verifique a conexão ou URL.")
    exit()

while True:
    # Capturar frame por frame
    ret, frame = cap.read()

    # Verificar se o frame foi capturado corretamente
    if not ret:
        print("Falha ao capturar frame. Tentando reconectar...")
        cap = cv2.VideoCapture(camera_url)
        time.sleep(1)
        continue

    # Aumentar a nitidez da imagem com um kernel de nitidez
    kernel_sharpening = np.array([[-1, -1, -1], [-1, 9, -1], [-1, -1, -1]])
    sharpened = cv2.filter2D(frame, -1, kernel_sharpening)

    # Ajustar o contraste e o brilho (alfa > 1 aumenta contraste, beta ajusta brilho)
    alpha = 1.5  # Contraste
    beta = 20    # Brilho
    adjusted = cv2.convertScaleAbs(sharpened, alpha=alpha, beta=beta)

    # Converter o frame para escala de cinza
    gray = cv2.cvtColor(adjusted, cv2.COLOR_BGR2GRAY)

    # Detectar faces no frame
    faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))

    # Desenhar um retângulo ao redor de cada face detectada
    for (x, y, w, h) in faces:
        cv2.rectangle(adjusted, (x, y), (x+w, y+h), (255, 0, 0), 2)

    # Exibir o frame com as faces detectadas
    cv2.imshow('Deteccao de Faces', adjusted)

    # Sair com a tecla 'q'
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Liberar os recursos
cap.release()
cv2.destroyAllWindows()