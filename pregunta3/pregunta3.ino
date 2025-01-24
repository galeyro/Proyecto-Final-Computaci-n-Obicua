#include <DHT.h>

// Definición del DHT22
#define DHTPIN 15  // Pin conectado al sensor DHT22
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

// Variables globales para los sensores
float temperatura, humedad;

void setup() {
  // Inicializar comunicación serial
  Serial.begin(9600);

  // Inicializar DHT22
  dht.begin();

  Serial.println("Sistema listo para enviar datos por puerto serie.");
}

void loop() {
  // Leer datos del DHT22
  temperatura = dht.readTemperature();
  humedad = dht.readHumidity();

  // Verificar si la lectura fue exitosa
  if (isnan(temperatura) || isnan(humedad)) {
    Serial.println("Error al leer el sensor DHT22.");
    return;
  }

  // Enviar datos por puerto serie en formato JSON
  Serial.print("{\"temperatura\":");
  Serial.print(temperatura);
  Serial.print(",\"humedad\":");
  Serial.print(humedad);
  Serial.println("}");

  // Esperar 1 segundo antes de la siguiente lectura
  delay(1000);
}
