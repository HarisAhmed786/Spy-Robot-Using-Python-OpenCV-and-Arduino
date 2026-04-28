#include <WiFi.h>
#include <driver/i2s.h>

#define I2S_WS      15
#define I2S_SD      32
#define I2S_SCK     14

// ===== Replace with your Wi-Fi and Laptop info =====
const char* ssid = "Saim 2.4G";        // Your Wi-Fi name
const char* pass = "alpha123";    // Your Wi-Fi password
const char* laptopIP = "192.168.18.56";    // Laptop IP on same network
const int udpPort = 5000;                    // UDP port (same as Python)

// ====================================================
WiFiUDP udp;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Connect to Wi-Fi
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to WiFi!");
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());

  udp.begin(udpPort);

  // I2S config
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

  Serial.println("Audio streaming started...");
}

void loop() {
  int16_t samples[128];  // 128 samples per packet (smaller = more reliable)
  size_t bytesRead;

  i2s_read(I2S_NUM_0, samples, sizeof(samples), &bytesRead, portMAX_DELAY);

  // Send to laptop
  udp.beginPacket(laptopIP, udpPort);
  udp.write((uint8_t*)samples, sizeof(samples));
  udp.endPacket();

  Serial.println("Packet sent");
}     
ESP32 IP: 192.168.18.126
ena 2 in1 4 in2 5 in3 18 in4 19 enb 21
http://192.168.18.137/