#include <SPI.h> // Librería comunicación SPI
#include <Ethernet.h> // Librería Ethernet
#include <DHT11.h> // Librería sensor temperatura y humedad

byte mac[] = {0x54, 0x55, 0x58, 0x10, 0x00, 0x24};
EthernetServer server(80); // Puerto en el 80
IPAddress dnServer(156, 35, 14 , 2); // Servidor DNS de la universidad de Oviedo
IPAddress gateway(192, 168, 61, 13); // Puerta de enlace predeterminada
IPAddress subnet(255, 255, 255, 0); // Máscara de subred

IPAddress ip(192, 168, 61, 37); // IP del Arduino

// Variables para el sensor de temperatura y humedad
int dht11Pin = 6;
DHT11 dht11(dht11Pin);

// Variables para la lógica
int redLedPin = 8; // pin del led rojo
#define TEMPERATURE_AND_HUMIDITY_ENDPOINT "/temperaturayhumedad"
#define SWITCH_ON_LED_ENDPOINT "/led/encender"
#define SWITCH_OFF_LED_ENDPOINT "/led/apagar"



void setup() {
  Serial.begin(9600);
  Serial.println(F("\n---------\n  Setup\n---------"));

  Ethernet.begin(mac, ip, dnServer, gateway, subnet);
  server.begin();

  // Imprimir la IP
  Serial.println(Ethernet.localIP());

  // Inicializar el led
  pinMode(redLedPin, OUTPUT);
}

void loop() {
  EthernetClient client = server.available();

  if (client) {
    Serial.println(F("Nueva peticion"));
    String petition = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read(); //Leer petición carácter a carácter
        petition.concat(c); // concatenar en un string

        // Si la petición ha acabado (el caracter es '\n')
        if (c == '\n') {
          petitionHasFinished(client, petition);
          break; // salimos del bucle
        }
      }
    }

    // Pequeña pausa para asegurar el envio de datos
    delay(1000);
    client.stop(); // cierra la conexión
  }
}


/* La petición ha acabado, por lo que comprobamos su contenido y actuamos en consecuencia. */
void petitionHasFinished(EthernetClient client, String petition) {
  if (contains(petition, TEMPERATURE_AND_HUMIDITY_ENDPOINT)) {
    sendClientTemperatureAndHumidityAnd_HTTP_Response(client);
  }
  else if (contains(petition, SWITCH_ON_LED_ENDPOINT)) {
    switchOnLed();
    sendClient_HTTP_OK_Response(client);
  }
  else if (contains(petition, SWITCH_OFF_LED_ENDPOINT)) {
    switchOffLed();
    sendClient_HTTP_OK_Response(client);
  }
}

/*
  Obtiene los datos de temperatura y humedad.
  Si los obtiene correctamente, los envia al cliente junto a una respuesta OK.
  Si no, devuelve una respuesta 500 Internal Server Error.
*/
void sendClientTemperatureAndHumidityAnd_HTTP_Response(EthernetClient client) {
  // Obtenemos la temperatura y la humedad del sensor
  float temperature, humidity;
  int exitCode = dht11.read(humidity, temperature);

  // Si el código de salida es 0, es que obtuvo los datos correctamente
  if (exitCode == 0)
    temperatureAndHumidityCorrectlyObtained(client, temperature, humidity);
  // Si no es 0, es que hubo un error
  else
    cantObtainTemperatureAndHumidity(client, exitCode);
}

/* Envia al cliente una respuesta 200 OK, y la temperatura y la humedad en formato JSON. */
void temperatureAndHumidityCorrectlyObtained(EthernetClient client, float temperature, float humidity) {
  Serial.println("Temperatura: " + String(temperature));
  Serial.println("Humedad: " + String(humidity));
  Serial.println(F("--------------------------------"));

  sendClient_HTTP_OK_Response(client);  
  client.println("{\"temperatura\":" + String(temperature) + ", \"humedad\":" + String(humidity) + "}");
  client.println();
}

/* Imprime el error por consola y envia al cliente una respuesta 500 Internal Server Error. */
void cantObtainTemperatureAndHumidity(EthernetClient client, int exitCode) {
  Serial.println("Error: " + String(exitCode));
  Serial.println(F("--------------------------------"));

  sendClient_HTTP_500_Error_Response(client);
}

/* Envia al cliente una respuesta HTTP 200 OK. */
void sendClient_HTTP_OK_Response(EthernetClient client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json;charset=utf-8");
  client.println("Server: Arduino");
  client.println("Access-Control-Allow-Origin: *");
  client.println();
}

/* Envia al cliente una respuesta HTTP 500 Internal Server Error. */
void sendClient_HTTP_500_Error_Response(EthernetClient client) {
  client.println("HTTP/1.1 500");
  client.println("Content-Type: application/json;charset=utf-8");
  client.println("Server: Arduino");
  client.println("Access-Control-Allow-Origin: *");
  client.println();
}

/* Enciende el led. */
void switchOnLed() {
  Serial.println(F("Se enciende el led"));
  digitalWrite(redLedPin, HIGH);
}

/* Apaga el led. */
void switchOffLed() {
  Serial.println(F("Se apaga el led"));
  digitalWrite(redLedPin, LOW);
}

/* Devuelve cierto si el primer String contiene al segundo. */
boolean contains(String string1, String string2) {
  if (string1.indexOf(string2) != -1)
    return true;
  else return false;
}
