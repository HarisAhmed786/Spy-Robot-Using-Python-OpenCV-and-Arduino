import socket
import pyaudio

UDP_PORT = 5000
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("0.0.0.0", UDP_PORT))

p = pyaudio.PyAudio()
stream = p.open(format=pyaudio.paInt16,
                channels=1,
                rate=16000,
                output=True)

print(f"Listening for ESP32 Audio on Port {UDP_PORT}")
print("   (Audio will start when you enable it on the Web Interface)")

try:
    while True:
        data, addr = sock.recvfrom(4096)
        stream.write(data)
except KeyboardInterrupt:
    print("Stopping...")
    stream.stop_stream()
    stream.close()
    p.terminate()