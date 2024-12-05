  #include <BLEDevice.h>
  #include <BLEServer.h>
  #include <BLEUtils.h>
  #include <BLE2902.h>
  #include "DHT.h"
  
  #define temperatureCelsius

  #define DHTTYPE DHT11 

  const int DHTPin = 21;
  DHT dht(DHTPin, DHTTYPE);
  
  #define SERVICE_UUID "91bad492-b950-4226-aa2b-4ede9fa42f59"
  
  
  BLECharacteristic dhtTemperatureCelsiusCharacteristics("cba1d466-344c-4be3-ab3f-189f80dd7518", BLECharacteristic::PROPERTY_NOTIFY);
  BLEDescriptor dhtTemperatureCelsiusDescriptor(BLEUUID((uint16_t)0x2902));
  
  BLECharacteristic dhtHumidityCharacteristics("ca73b3ba-39f6-4ab3-91ae-186dc9577d99", BLECharacteristic::PROPERTY_NOTIFY);
  BLEDescriptor dhtHumidityDescriptor(BLEUUID((uint16_t)0x2903));
  
  
  bool device_connected = false;
  
  class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
  device_connected = true;
  };
  
  void onDisconnect(BLEServer* pServer) {
  device_connected = false;
  }
  };
  
  void setup() {
  
  dht.begin();
  Serial.begin(115200);
  
  BLEDevice::init("ESP32-BLE");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *dhtService = pServer->createService(SERVICE_UUID);
  
  dhtService->addCharacteristic(&dhtTemperatureCelsiusCharacteristics);
  dhtTemperatureCelsiusDescriptor.setValue("DHT Temperature (Celsius)");
  dhtTemperatureCelsiusCharacteristics.addDescriptor(new BLE2902());
  
  dhtService->addCharacteristic(&dhtHumidityCharacteristics);
  dhtHumidityDescriptor.setValue("DHT humidity");
  dhtHumidityCharacteristics.addDescriptor(new BLE2902());
  
  dhtService->start();
  pServer->getAdvertising()->start();
  Serial.println("Waiting for ESP32 client connection...");
  }
  
  void loop() {
  if (device_connected) {
  float temp = dht.readTemperature();
  float f = dht.readTemperature(true);
  float hum = dht.readHumidity();
  
  if (isnan(hum) || isnan(temp)) {
  Serial.println("Failed to read from DHT sensor! Check connections");
  return;
  }
  
  static char temperature_celsius[7];
  dtostrf(temp, 6, 2, temperature_celsius);
  dhtTemperatureCelsiusCharacteristics.setValue(temperature_celsius);
  dhtTemperatureCelsiusCharacteristics.notify();
  Serial.print("Temperature Celsius: ");
  Serial.print(temp);
  Serial.print(" *C");
  
  static char humidity[7];
  dtostrf(hum, 6, 2, humidity);
  dhtHumidityCharacteristics.setValue(humidity);
  dhtHumidityCharacteristics.notify();
  Serial.print("  Humidity: ");
  Serial.print(hum);
  Serial.println(" %");
  delay(2000);
  }
  }
