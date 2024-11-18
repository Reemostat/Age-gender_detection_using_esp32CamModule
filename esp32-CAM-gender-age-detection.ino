#include <WiFi.h>
#include <HTTPClient.h>
#include <esp_camera.h>

const char* ssid = "moto edge 30 pro";
const char* password = "sjei3+#(($74wj234";
const char* serverUrl = "http://192.168.223.249:5000/classify";  // Flask server URL

void setup() {
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Camera initialization (same as above)
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = 5;
  config.pin_d1 = 18;
  config.pin_d2 = 19;
  config.pin_d3 = 21;
  config.pin_d4 = 36;
  config.pin_d5 = 39;
  config.pin_d6 = 34;
  config.pin_d7 = 35;
  config.pin_xclk = 0;
  config.pin_pclk = 22;
  config.pin_vsync = 25;
  config.pin_href = 23;
  config.pin_sscb_sda = 26;
  config.pin_sscb_scl = 27;
  config.pin_pwdn = 32;
  config.pin_reset = -1;
  config.xclk_freq_hz = 20000000;
  config.ledc_timer = LEDC_TIMER_0;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_UXGA;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("Camera initialization failed");
    return;
  }
}

void loop() {
  // Capture image
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Failed to capture image");
    return;
  }

  // Send image to Flask server
  if (WiFi.status() == WL_CONNECTED) {
  HTTPClient http;
    http.begin(serverUrl);

    // Set headers for the request
    http.addHeader("Content-Type", "image/jpeg");

    // Send the image data as a POST request
    int httpResponseCode = http.POST(fb->buf, fb->len);

    // http.addHeader("Content-Type", "application/json");

    // String json = "{\"message\":\"Hello, server!\"}";
    // int httpResponseCode = http.POST(json);


    if (httpResponseCode > 0) {
      Serial.printf("POST request sent. Response Code: %d\n", httpResponseCode);
      String response = http.getString(); // Get the server's response
      Serial.println("Response: " + response);
      
    } else {
       String errorMessage = http.errorToString(httpResponseCode);
  Serial.printf("Error sending POST request: %d - %s\n", httpResponseCode, errorMessage.c_str());
    }

    // Clean up
    http.end();

  }

  esp_camera_fb_return(fb); // Return the frame buffer
  delay(5000);  // Capture and send image every 5 seconds (adjust as needed)
}
