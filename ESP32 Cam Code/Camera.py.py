import cv2

cap = cv2.VideoCapture("http:// 192.168.18.126:81/stream")

while True:
    ret, frame = cap.read()
    if ret:
        cv2.imshow("ESP32-CAM Stream", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
