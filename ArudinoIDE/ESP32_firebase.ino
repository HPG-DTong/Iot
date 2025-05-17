#include <WiFi.h>
#include <FirebaseESP32.h>
#include <DHT.h>

// Cấu hình WiFi
#define WIFI_SSID "Tong"    
#define WIFI_PASS "12345678to"

// Cấu hình Firebase
#define FIREBASE_HOST "cambiennhiet-d28ff-default-rtdb.firebaseio.com"  
#define FIREBASE_AUTH "StWKvXvWrTNM4MwKots2jRoza58uQeMcMjKRrq4oE"

// Cấu hình cảm biến
#define DHTPIN 4       // GPIO4 kết nối DHT22
#define DHTTYPE DHT22
#define LED_PIN LED_BUILTIN // Sử dụng LED_BUILTIN (GPIO2)

DHT dht(DHTPIN, DHTTYPE);

// Khai báo đối tượng Firebase
FirebaseData fbData;
FirebaseConfig config;
FirebaseAuth auth;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  dht.begin();

  // Kết nối WiFi
  Serial.print("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");

  // Cấu hình Firebase
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Kiểm tra kết nối Firebase
  if (Firebase.setString(fbData, "/test/connection", "ok")) {
    Serial.println("Connected to Firebase successfully!");
  } else {
    Serial.println("Failed to connect to Firebase: " + fbData.errorReason());
  }
}

void loop() {
  // Đọc dữ liệu từ DHT22
  float temp = dht.readTemperature();
  float humi = dht.readHumidity();

  if (!isnan(temp) && !isnan(humi)) {
    // Gửi dữ liệu lên Firebase theo cấu trúc mới
    if (Firebase.setFloat(fbData, "/NhietDo", temp)) {  
      Serial.printf("Nhiệt độ: %.2f°C đã gửi lên Firebase\n", temp);
    } else {
      Serial.println("Lỗi gửi nhiệt độ: " + fbData.errorReason());
    }

    if (Firebase.setFloat(fbData, "/DoAm", humi)) {  
      Serial.printf("Độ ẩm: %.2f%% đã gửi lên Firebase\n", humi);
    } else {
      Serial.println("Lỗi gửi độ ẩm: " + fbData.errorReason());
    }
  } else {
    Serial.println("Lỗi đọc từ cảm biến DHT!");
  }

  // Nhận lệnh điều khiển LED từ Firebase (kiểu số 0/1)
  if (Firebase.getInt(fbData, "/led")) {  
    int ledState = fbData.intData();
    digitalWrite(LED_PIN, ledState ? HIGH : LOW);
    Serial.println("Trạng thái LED: " + String(ledState ? "BẬT" : "TẮT"));
  } else {
    Serial.println("Lỗi đọc trạng thái LED: " + fbData.errorReason());
  }

  delay(2000); // Chờ 2 giây
}