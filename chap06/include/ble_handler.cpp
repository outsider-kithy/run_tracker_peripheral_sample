#include <M5Unified.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// サービスUUID・キャラクタリスティックUUIDの設定
#define SERVICE_UUID        "12345678-1234-5678-1234-56789abcdef0"
#define CHARACTERISTIC_UUID "abcd1234-5678-90ab-cdef-1234567890ab"

// サーバー・キャラクタリスティックの宣言
static BLEServer* pServer = nullptr;
static BLECharacteristic* pCharacteristic = nullptr;
static bool deviceConnected = false;

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) override {
    deviceConnected = true;
  }
  void onDisconnect(BLEServer* pServer) override {
    deviceConnected = false;
    pServer->startAdvertising();
  }
};

void initBLE() {
  Serial.begin(9600);

  //"M5Stick Run Tracker"という名前のデバイスとして初期化・サービスを開始
  BLEDevice::init("M5Stick Run Tracker");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // キャラクタリステックスの設定
  BLEService* pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
  );

  //サーバーを起動
  pCharacteristic->addDescriptor(new BLE2902());
  pService->start();

  //アドバタイジングを開始
  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  BLEDevice::startAdvertising();
  Serial.println("BLE is Initialized!");
}

void sendDataViaBLE(const std::vector<std::pair<double, double>>& path,
                    double totalDistance,
                    int steps,
                    int elapsedSeconds) {
                      
  // --- JSON生成 ---
  String json = "{";
  json += "\"points\":[";

  for (size_t i = 0; i < path.size(); i++) {
    json += String("{\"lat\":") + String(path[i].first, 6) +
            ",\"lng\":" + String(path[i].second, 6) + "}";
    if (i < path.size() - 1) json += ",";
  }

  json += "],";
  json += "\"distance\":" + String(totalDistance, 2) + ",";
  json += "\"steps\":" + String(steps) + ",";
  json += "\"elapsedSeconds\":" + String(elapsedSeconds) + ",";
  json += "\"startDate\":\"" + startDate + "\",";
  json += "\"endDate\":\"" + endDate + "\"";
  json += "}";

  // --- BLE特性に書き込み ---
  pCharacteristic->setValue(json.c_str());
  pCharacteristic->notify();  // 通知送信

  M5.Lcd.println("BLE data sent!");
  Serial.println("Sent via BLE:");
  Serial.println(json);
}