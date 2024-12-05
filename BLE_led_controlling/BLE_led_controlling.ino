#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID        "3b964345-3304-4506-9080-e55cf77dae6b"
#define CHARACTERISTIC_UUID "56d6ba54-a577-43e3-9bfd-df3db3b33b88"

#define LED 4

class MyCallbacks: public BLECharacteristicCallbacks 
{
    void onWrite(BLECharacteristic *pCharacteristic) 
    {
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) 
      {
        Serial.println("*********");
        Serial.print("New value: ");
        for (int i = 0; i < value.length(); i++)
          Serial.print(value[i]);

        Serial.println();
        Serial.println("*********");
      }
      if(value == "ON")
      {
        Serial.println("Turning ON the led");
        digitalWrite(LED, HIGH);
      }
      if(value == "OFF")
      {
        Serial.println("Turning OFF the led");
        digitalWrite(LED, LOW);
      }


    }
};

void setup() 
{
  Serial.begin(115200);
  pinMode(LED,OUTPUT);

  BLEDevice::init("ESP32-BLE");
  BLEServer *pServer = BLEDevice::createServer();

  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setCallbacks(new MyCallbacks());

  pCharacteristic->setValue("Hello!");
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
}
