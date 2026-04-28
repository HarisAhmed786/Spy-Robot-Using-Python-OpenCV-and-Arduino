#include <WiFi.h>
#include <WiFiUdp.h>
#include <driver/i2s.h>
#include <ESP32Servo.h>

const char* ssid = "Saim 2.4G";
const char* pass = "alpha123";

WiFiUDP udp;
const char* laptopIP = "192.168.18.56"; 
const int udpPort = 5000;

WiFiServer server(80);

#define I2S_WS 15
#define I2S_SD 32
#define I2S_SCK 14

#define ENA 2
#define IN1 4
#define IN2 5
#define IN3 18
#define IN4 19
#define ENB 21

#define SERVO_PIN 23

int Speed = 255;
Servo myServo;

int servoPos = 90;
int stepSize = 30;

bool sendAudio = false; 

void Forward() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void Backward() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void StopCar() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void Left() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void Right() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) delay(200);
  server.begin();

  udp.begin(udpPort);

  myServo.attach(SERVO_PIN);
  myServo.write(servoPos);

  const i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 16000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 512
  };

  const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = -1,
    .data_in_num = I2S_SD
  };

  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
}

void loop() {

  if (sendAudio) {
      int16_t samples[128];
      size_t bytesRead;
      
      
      i2s_read(I2S_NUM_0, samples, sizeof(samples), &bytesRead, 0); 

      if (bytesRead > 0) {
          float gain = 8.0;
          for (int i = 0; i < 128; i++) {
            int32_t a = samples[i] * gain;
            if (a > 32767) a = 32767;
            if (a < -32768) a = -32768;
            samples[i] = a;
          }
          
          udp.beginPacket(laptopIP, udpPort);
          udp.write((uint8_t*)samples, sizeof(samples));
          udp.endPacket();
      }
  }


  WiFiClient client = server.available();
  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();


    if (request.indexOf("/forward") != -1) Forward();
    else if (request.indexOf("/backward") != -1) Backward();
    else if (request.indexOf("/left") != -1) Left();
    else if (request.indexOf("/right") != -1) Right();
    else if (request.indexOf("/stop") != -1) StopCar();


    else if (request.indexOf("/servo_left") != -1) {
       servoPos -= stepSize;
       if (servoPos < 0) servoPos = 0;
       myServo.write(servoPos);
    }
    else if (request.indexOf("/servo_right") != -1) {
       servoPos += stepSize;
       if (servoPos > 180) servoPos = 180;
       myServo.write(servoPos);
    }
    else if (request.indexOf("/servo_center") != -1) {
       servoPos = 90;
       myServo.write(servoPos);
    }
    
    
    else if (request.indexOf("/audio_on") != -1) {
        sendAudio = true;
    }
    else if (request.indexOf("/audio_off") != -1) {
        sendAudio = false;
    }

    client.println("HTTP/1.1 200 OK");
    client.println("Connection: close");
    client.println();
    client.stop();
  }
}