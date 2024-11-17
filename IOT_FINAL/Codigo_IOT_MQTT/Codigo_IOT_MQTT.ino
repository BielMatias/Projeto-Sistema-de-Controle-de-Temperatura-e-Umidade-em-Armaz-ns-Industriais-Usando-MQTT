#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define DHTPIN D3         // Pino onde o DHT22 está conectado
#define DHTTYPE DHT22     // Tipo de sensor
#define RELAY_PIN D1      // Pino onde o relé está conectado

// Configurações do Wi-Fi e MQTT
const char* ssid = "Sua Rede";            // Nome da rede Wi-Fi
const char* password = "Sua senha";     // Senha do Wi-Fi
const char* mqtt_server = "seu IP"; // Substitua pelo IP correto do broker MQTT na rede
const int mqtt_port = 1883;            // Porta do broker MQTT (default: 1883)

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Relé desligado inicialmente

  // Inicializa o sensor DHT
  dht.begin();

  // Conecta ao Wi-Fi
  setup_wifi();

  // Configura o broker MQTT
  client.setServer(mqtt_server, mqtt_port);
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando-se ao Wi-Fi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  // Tenta conectar ao Wi-Fi com uma verificação de timeout
  int attempt = 0;
  while (WiFi.status() != WL_CONNECTED && attempt < 20) { // Tenta por 10 segundos
    delay(500);
    Serial.print(".");
    attempt++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConectado ao Wi-Fi!");
    Serial.print("Endereço IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFalha ao conectar ao Wi-Fi. Reiniciando...");
    ESP.restart(); // Reinicia o dispositivo se não conseguir conectar
  }
}

void reconnect() {
  // Reconeção ao MQTT com um nome de cliente único
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    
    // Gera um nome de cliente único usando o último byte do MAC
    String clientId = "NodeMCUClient-";
    clientId += String(WiFi.macAddress().substring(9)); // Adiciona o último byte do MAC

    // Tenta conectar
    if (client.connect(clientId.c_str())) {
      Serial.println("Conectado ao broker MQTT!");
      
      // Aqui você pode adicionar a inscrição em tópicos, caso necessário
      // client.subscribe("topico/exemplo");
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos...");
      
      // Mensagens de erro comuns
      if (client.state() == -2) {
        Serial.println("Erro -2: Servidor MQTT inatingível. Verifique o IP e a porta.");
      } else if (client.state() == -4) {
        Serial.println("Erro -4: Falha na autenticação MQTT.");
      } else if (client.state() == -1) {
        Serial.println("Erro -1: Erro de TCP ou conexão com o broker.");
      }
      
      delay(5000); // Aguarda 5 segundos antes de tentar novamente
    }
  }
}

void loop() {
  // Certifique-se de que o cliente MQTT está conectado
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Leitura do sensor
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Falha na leitura do DHT!");
    return;
  }

  // Publica os dados no MQTT
  char tempStr[8];
  dtostrf(t, 1, 2, tempStr);
  client.publish("armazem/temperatura", tempStr);

  char humStr[8];
  dtostrf(h, 1, 2, humStr);
  client.publish("armazem/umidade", humStr);

  // Controle do relé baseado na temperatura
  if (t > 25.0) { // Define o valor de temperatura ideal para ligar o ventilador
    digitalWrite(RELAY_PIN, HIGH); // Liga o relé
    client.publish("armazem/status", "Ventilador Ligado");
  } else {
    digitalWrite(RELAY_PIN, LOW); // Desliga o relé
    client.publish("armazem/status", "Ventilador Desligado");
  }

  delay(2000); // Aguarda 2 segundos para nova leitura
}
