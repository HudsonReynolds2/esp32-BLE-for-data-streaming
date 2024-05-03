#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLECharacteristic* pTxCharacteristic = NULL; // Declare pTxCharacteristic globally

// Function to generate random heart rate values
float heartRateValues() {
  return random(50, 201) + random(0, 100) / 100.0;
}

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0)); // For heartRateValues()

  Serial.println("Starting BLE work!");

  BLEDevice::init("TestWithRandomValues");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pTxCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
  );

  // Set an initial value for the characteristic
  pTxCharacteristic->setValue("Hello World says Hudson");

  // Start the service
  pService->start();

  // Set up advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06); // Functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  Serial.println("Characteristic defined! Now you can read it on your phone!");
}

void loop() {
  // Generate a random heart rate value
  float randHeartRate = heartRateValues();
  Serial.print("Random Heart Rate: ");
  Serial.println(randHeartRate);

  String stringValue = String(randHeartRate);

  // Update the characteristic value
  pTxCharacteristic->setValue(stringValue.c_str());
  pTxCharacteristic->notify();

  delay(1000); // Wait for 1 second
}
