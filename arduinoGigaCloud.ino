#include <WiFi.h>
#include <CoAPSimple.h>

#define BOTAO 53
#define RELE_1 52

char ssid[] = "";
char pass[] = "";

WiFiServer server(80);
Coap coap(server);

bool estado_botao = false;

void setup() {
  Serial.begin(9600);
  while (!Serial) { }

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Falha na comunicação com o módulo Wi-Fi!");
    while (true);
  }

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Conectando à rede WPA SSID: ");
    Serial.println(ssid);
    WiFi.begin(ssid, pass);
    delay(10000);
  }

  Serial.println("Conectado à rede Wi-Fi!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  pinMode(BOTAO, INPUT_PULLUP);
  pinMode(RELE_1, OUTPUT);
  digitalWrite(RELE_1, HIGH);

  server.begin();
}

void loop() {
  WiFiClient client = server.available(); // Espera por uma conexão

  if (client) { // Se houver uma conexão
    if (coap.available()) {
      packet = coap.readPacket();
      Serial.print("Recebido de: ");
      Serial.println(packet.senderIP.toString());

      if (packet.code == CoapCode::GET) {
        if (packet.uriPath == "/ligar") {
          digitalWrite(RELE_1, LOW); // Liga o relé
          coap.sendResponse(packet.senderIP, packet.senderPort, CoapCode::CONTENT, "Cafeteira Ligada");
        } else if (packet.uriPath == "/desligar") {
          digitalWrite(RELE_1, HIGH); // Desliga o relé
          coap.sendResponse(packet.senderIP, packet.senderPort, CoapCode::CONTENT, "Cafeteira Desligada");
        } else {
          coap.sendResponse(packet.senderIP, packet.senderPort, CoapCode::NOT_FOUND, "Rota não encontrada");
        }
      }
    }
  }
}
