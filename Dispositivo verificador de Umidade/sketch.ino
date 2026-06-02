#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_topic_sub = "mackenzie/bomba";
const char* mqtt_topic_pub = "mackenzie/bomba/consumo"; 

#define LED_PIN 26

const float VAZAO_L_POR_SEGUNDO = 0.05; 

WiFiClient espClient;
PubSubClient client(espClient);

bool bombaAtiva = false;
unsigned long tempoInicioRega = 0;
unsigned long duracaoRegaMs = 0;
float litrosSolicitados = 0.0;

void callback(char* topic, byte* payload, unsigned int length) {
  String mensagem = "";
  for (int i = 0; i < length; i++) {
    mensagem += (char)payload[i];
  }
  Serial.print("Comando recebido: ");
  Serial.println(mensagem);

  if (mensagem == "OFF") {
    digitalWrite(LED_PIN, LOW);
    bombaAtiva = false;
    Serial.println("Bomba desligada via comando de emergência.");
    return;
  }

  float litros = mensagem.toFloat();

  if (litros > 0.0) {
    litrosSolicitados = litros;
    duracaoRegaMs = (litros / VAZAO_L_POR_SEGUNDO) * 1000;
    
    tempoInicioRega = millis();
    bombaAtiva = true;
    digitalWrite(LED_PIN, HIGH);
    
    Serial.print("Rega Iniciada! Alvo: ");
    Serial.print(litrosSolicitados);
    Serial.print(" L. Duração estimada: ");
    Serial.print(duracaoRegaMs / 1000);
    Serial.println(" segundos.");
  }
}

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
    
    // GERA UM ID ALEATÓRIO PARA NÃO CAIR A CONEXÃO
    String clientId = "Mackenzie_Bomba_";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println("Conectado com sucesso!");
      client.subscribe(mqtt_topic_sub);
    } else {
      Serial.print("Falhou, rc=");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0)); // Inicializa a semente aleatória
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (bombaAtiva) {
    unsigned long tempoAtual = millis();
    
    if (tempoAtual - tempoInicioRega >= duracaoRegaMs) {
      digitalWrite(LED_PIN, LOW);
      bombaAtiva = false;
      
      char msgFeedback[50];
      snprintf(msgFeedback, 50, "%.2f", litrosSolicitados);
      client.publish(mqtt_topic_pub, msgFeedback);
      
      Serial.print("Rega finalizada. Consumo publicado: ");
      Serial.print(msgFeedback);
      Serial.println(" Litros.");
    }
  }
}