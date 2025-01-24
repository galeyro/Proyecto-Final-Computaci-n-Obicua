#include <Wire.h>
#include <ACROBOTIC_SSD1306.h> // Librería para controlar el OLED
#include <DHT.h> // Librería para el sensor DHT

#define DHTPIN 15 // Pin donde está conectado el sensor DHT
#define DHTTYPE DHT22 // Especificamos el tipo de sensor (DHT22 en este caso)

DHT dht(DHTPIN, DHTTYPE); // Inicialización del sensor DHT

void setup() {
  Serial.begin(9600);
  dht.begin(); // Inicializar el sensor DHT

  Wire.begin();
  oled.init(); // Inicializar pantalla OLED
  oled.clearDisplay(); // Limpiar pantalla
  oled.setFont(font5x7); // Establecer la fuente por defecto

  // Mostrar mensaje de bienvenida
  oled.setTextXY(0, 0);
  oled.putString("DHT22 Sensor");
  oled.setTextXY(1, 0);
  oled.putString("Initializing...");
  delay(2000);
  oled.clearDisplay();
}

void loop() {
  // Leer los datos del sensor DHT
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Verificar si hubo algún error en la lectura
  if (isnan(h) || isnan(t)) {
    oled.clearDisplay();
    oled.setTextXY(0, 0);
    oled.putString("Sensor Error");
    delay(2000);
    return;
  }

  // Mostrar los valores en el OLED
  oled.clearDisplay();
  oled.setTextXY(0, 0);
  oled.putString("Temp: ");
  oled.putString(String(t, 1).c_str()); // Convertir float a String con 1 decimal y luego a char*
  oled.putString(" C");

  oled.setTextXY(1, 0);
  oled.putString("Hum: ");
  oled.putString(String(h, 1).c_str());
  oled.putString(" %");

  // Opcional: Mostrar datos en el monitor serial
  Serial.print("Temp: ");
  Serial.print(t, 1);
  Serial.print(" C, Hum: ");
  Serial.print(h, 1);
  Serial.println(" %");

  delay(2000); // Actualizar cada 2 segundos
}