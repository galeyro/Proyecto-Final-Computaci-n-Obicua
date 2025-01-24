#include <ArduinoBLE.h> // Librería para comunicación Bluetooth BLE

#define LDR_PIN 16 // Pin analógico donde está conectado el sensor de luz

BLEService lightService("19B10000-E8F2-537E-4F6C-D104768A1214"); // Servicio BLE para luminosidad
BLEStringCharacteristic lightCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify, 20); // Característica para enviar datos de luminosidad

void setup() {
  Serial.begin(9600);
  while (!Serial); // Esperar conexión Serial para debug

  if (!BLE.begin()) {
    Serial.println("Error al iniciar BLE!");
    while (1);
  }

  pinMode(LDR_PIN, INPUT); // Configurar pin del sensor de luz

  // Configuración del servicio BLE
  BLE.setLocalName("Luminosity Sensor");
  BLE.setAdvertisedService(lightService);

  lightService.addCharacteristic(lightCharacteristic); // Añadir característica al servicio
  BLE.addService(lightService); // Añadir servicio al periférico BLE

  BLE.advertise(); // Iniciar publicidad BLE
  Serial.println("Sensor de luz BLE iniciado.");
}

void loop() {
  // Esperar conexión de un dispositivo central (teléfono)
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Conectado a: ");
    Serial.println(central.address());

    while (central.connected()) {
      int lightValue = analogRead(LDR_PIN); // Leer valor analógico del sensor de luz
      float voltage = lightValue * (5.0 / 1023.0); // Convertir a voltaje
      String lightData = "Lum: " + String(lightValue) + " (" + String(voltage, 2) + "V)";

      // Enviar datos a través de BLE
      lightCharacteristic.writeValue(lightData);

      // Mostrar datos en monitor serial
      Serial.println(lightData);

      delay(1000); // Actualizar cada segundo
    }

    Serial.println("Dispositivo desconectado.");
  }
}
