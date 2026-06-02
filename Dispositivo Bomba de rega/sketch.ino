#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_topic = "mackenzie/umidade";

#define LED_PIN 26
#define SENSOR_PIN 34

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (client.connect("ESP32_Mackenzie_1")) {
      Serial.println("Conectado!");
    } else {
      Serial.print("Falhou, rc=");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  int valorBruto = analogRead(SENSOR_PIN);
  float umidade = map(valorBruto, 0, 4095, 100, 0);

  // LED acende se umidade abaixo de 30%
  if (umidade < 30.0) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("ALERTA: Umidade baixa! LED aceso.");
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  char msg[50];
  snprintf(msg, 50, "%.1f", umidade);
  client.publish(mqtt_topic, msg);

  Serial.print("Umidade publicada: ");
  Serial.println(msg);

  delay(5000);
}