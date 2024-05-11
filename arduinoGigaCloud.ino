#include <WiFi.h>
#include <WiFiUdp.h>
#include <coap-simple.h>

#define BOTAO 53
#define RELE_1 52

const char* ssid     = "ESTACIO-VISITANTES";
const char* password = "estacio@2014";

WiFiUDP udp;
Coap coap(udp);

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // LED State
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);
  
  coap.server(callback_cafeteira_ligar, "ligar");
  coap.server(callback_cafeteira_desligar, "desligar");

  Serial.println("Setup Response Callback");
  coap.response(callback_response);

  // start coap server/client
  coap.start();
}

void loop() {
  delay(1000);
  coap.loop();
}

void callback_cafeteira_ligar(CoapPacket &packet, IPAddress ip, int port) {
  digitalWrite(RELE_1, LOW); // Liga o relé
  coap.sendResponse(ip, port, packet.messageid, "Cafeteira Ligada");
}

void callback_cafeteira_desligar(CoapPacket &packet, IPAddress ip, int port) {
  digitalWrite(RELE_1, HIGH); // Desliga o relé
  coap.sendResponse(ip, port, packet.messageid, "Cafeteira Desligada");
}

// CoAP client response callback
void callback_response(CoapPacket &packet, IPAddress ip, int port) {
  Serial.println("[Coap Response got]");
  
  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = NULL;
  
  Serial.println(p);
}
