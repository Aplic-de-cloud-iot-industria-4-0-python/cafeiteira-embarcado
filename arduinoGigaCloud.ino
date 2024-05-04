#include <WiFi.h>

#define BOTAO 53
#define RELE_1 52

char ssid[] = "";
char pass[] = "";

WiFiServer server(80);

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
    Serial.println("Novo cliente conectado.");
    String request = client.readStringUntil('\r'); // Lê a requisição HTTP até encontrar o caractere de retorno de carro (\r)
    Serial.println(request); // Imprime a requisição na porta serial

    if (request.indexOf("/ligar") != -1) { // Se a requisição for para ligar
      digitalWrite(RELE_1, LOW); // Liga o relé
      client.println("HTTP/1.1 200 OK"); // Responde com código 200 OK
      client.println("Content-Type: text/plain");
      client.println();
      client.println("Cafeteira Ligada");
    } else if (request.indexOf("/desligar") != -1) { // Se a requisição for para desligar
      digitalWrite(RELE_1, HIGH); // Desliga o relé
      client.println("HTTP/1.1 200 OK"); // Responde com código 200 OK
      client.println("Content-Type: text/plain");
      client.println();
      client.println("Cafeteira Desligada");
    } else { // Se a requisição for para uma rota desconhecida
      client.println("HTTP/1.1 404 Not Found"); // Responde com código 404 Not Found
      client.println("Content-Type: text/plain");
      client.println();
      client.println("Rota não encontrada");
    }

    delay(1); // Espera para o cliente enviar todos os dados
    client.stop(); // Fecha a conexão
    Serial.println("Cliente desconectado.");
  }
}
