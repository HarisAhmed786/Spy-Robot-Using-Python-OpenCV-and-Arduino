#include "esp_camera.h"
#include <WiFi.h>
#include "esp_http_server.h"
#include "soc/soc.h"        // Disable brownout problems
#include "soc/rtc_cntl_reg.h" // Disable brownout problems

// --- USER CONFIGURATION ---
const char* ssid = "Saim 2.4G";
const char* password = "alpha123";

// --- GLOBAL VARIABLES & DECLARATIONS ---
httpd_handle_t camera_httpd = NULL;

// Declare the essential handler functions (Implementation MUST be provided elsewhere)
extern esp_err_t stream_handler(httpd_req_t *req);
extern esp_err_t index_handler(httpd_req_t *req); 

// Function to start the web server
void startCameraServer(); 


void setup() {
    // 1. Disable Brownout Detector (Crucial for ESP32-CAM stability)
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); 
    
    Serial.begin(115200);
    Serial.setDebugOutput(true);

    // 2. CAMERA CONFIGURATION (AI-Thinker Pinout)
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    
    // Pin Definitions (Standard AI-Thinker Pinout)
    config.pin_d0 = 5; config.pin_d1 = 18; config.pin_d2 = 19; config.pin_d3 = 21; 
    config.pin_d4 = 36; config.pin_d5 = 39; config.pin_d6 = 34; config.pin_d7 = 35; 
    config.pin_xclk = 4;        // XCLK pin
    config.pin_pclk = 22; config.pin_vsync = 25; config.pin_href = 23; 
    config.pin_sccb_sda = 26; config.pin_sccb_scl = 27; 
    config.pin_pwdn = 32; config.pin_reset = -1;
    
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;
    
    // Frame size and Quality (Adjust FRAMESIZE and jpeg_quality for performance)
    config.frame_size = FRAMESIZE_SVGA; // 800x600 resolution
    config.jpeg_quality = 10;           // Lower value = higher quality/size (10 is good)
    config.fb_count = 2;

    // Use PSRAM if available
    #if defined(CAMERA_MODEL_AI_THINKER)
        config.fb_location = CAMERA_FB_IN_PSRAM;
    #endif

    // Initialize Camera
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        return; 
    }

    // 3. Connect to WiFi
    Serial.print("Connecting to Wi-Fi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("\nWi-Fi connected.");
    Serial.print("Camera Stream Ready at: http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");

    // 4. Start the Web Server
    startCameraServer();
}

void loop() {
    // Loop is empty as the server runs in the background.
}


// ************************************************************
// ****************** START CAMERA SERVER IMPLEMENTATION *******
// ************************************************************

// Define the URI (route) for the stream
httpd_uri_t stream_uri = {
    .uri       = "/stream", // Access the stream at http://[IP_ADDRESS]/stream
    .method    = HTTP_GET,
    .handler   = stream_handler,
    .user_ctx  = NULL
};

// Define the URI (route) for the main page (index)
httpd_uri_t index_uri = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = index_handler,
    .user_ctx  = NULL
};


void startCameraServer() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.max_uri_handlers = 4; // Only need 4 (index, stream, control, capture)
    config.stack_size = 18000; 

    // Start the Streaming Server
    if (httpd_start(&camera_httpd, &config) == ESP_OK) {
        httpd_register_uri_handler(camera_httpd, &stream_uri); // Register /stream route
        httpd_register_uri_handler(camera_httpd, &index_uri);  // Register / route
    } else {
        Serial.println("Error starting HTTP server!");
    }
}

// NOTE: You MUST copy the implementation of the handler functions (index_handler and stream_handler) 
// from the official "CameraWebServer" example and paste them into your sketch 
// (or link them) for this code to compile and run successfully.