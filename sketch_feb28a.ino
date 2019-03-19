#include <ESP8266WiFi.h>
const char* ssid = "homework"; // Nome da Rede
const char* password = "DidiroMinhaSenhaSuperSecreta"; //Password da rede 
int ledPin = 13; // GPIO2 do ESP8266 e D7 do NodeMCU 
WiFiServer server(80); // Porto 80 
void setup() { 
Serial.begin(115200); 
delay(10); 
pinMode(ledPin, OUTPUT); // Define o D7 como saída 
digitalWrite(ledPin, LOW); // O LED começa desligado
// Comunicação com a rede WiFi 
Serial.println(); 
Serial.println(); 
Serial.print("Connecting to "); // Mensagem apresentada no monitor série
Serial.println(ssid); // Apresenta o nome da rede no monitor série
WiFi.begin(ssid, password); // Inicia a ligação a rede
 
while (WiFi.status() != WL_CONNECTED){ 
delay(500);
Serial.print("."); // Enquanto a ligação não for efectuada com sucesso é apresentado no monitor série uma sucessão de “.”
}
Serial.println(""); 
Serial.println("WiFi connected"); // Se a ligação é efectuada com sucesso apresenta esta mensagem no monitor série
// Servidor 
server.begin(); // Comunicação com o servidor 
Serial.println("Servidor iniciado"); //é apresentado no monitor série que o  servidor foi iniciado 
// Impressão do endereço IP 
Serial.print("Use o seguinte URL para a comunicação: "); 
Serial.print("http://"); 
Serial.print(WiFi.localIP()); //Abrindo o Brower com este IP acedemos á pagina HTML de controlo dos LED´s, sendo que este IP só está disponível na rede à qual o ESP8266 se encontra ligado
Serial.println("/"); 
}
void loop() {
// Verificação se o cliente está conectado 
WiFiClient client = server.available(); 
if (!client) { // Verifica se o cliente está conectado ao servidor, executa este ciclo até estar conectado
return;
} 
// Espera até o cliente enviar dados 
Serial.println("novo cliente"); //Apresenta esta mensagem quando o cliente se liga ao servidor
while(!client.available()){
delay(1);
} 
// Ler a primeira linha do pedido 
String request = client.readStringUntil('\r'); 
Serial.println(request); 
client.flush(); 
// Operação do pedido 
int value = LOW; 
if (request.indexOf("/LED=ON") != -1)  { 
digitalWrite(ledPin, HIGH); // Se o pedido no LedPin for LED=ON, acende o LED
value = HIGH;
}
if (request.indexOf("/LED=OFF") != -1)  { 
digitalWrite(ledPin, LOW); // Se o pedido no LedPin for LED=OFF, apaga o LED
value = LOW; 
}
// Inicialização da página HTML 
// Retorno do resposta
client.println("HTTP/1.1 200 OK"); 
client.println("Content-Type: text/html"); 
client.println(""); //  do not forget this one 
client.println("<!DOCTYPE HTML>");
client.println("<html>");
client.println("<head>");
client.println("<meta charset='utf-8'>");
client.println("<title>Asterix Soluções</title>");
client.println("<style>");
//Configuracao CSS do botao on
client.println(".ON {");
client.println("background-color: #4CAF50; /* Green */");
client.println("border: none;");
client.println("color: white;");
client.println("padding: 40px;");
client.println("text-align: center;");
client.println("text-decoration: none;");
client.println("display: inline-block;");
client.println("font-size: 45px;");
client.println("margin: 4px 2px;");
client.println("cursor: pointer;");
client.println("border-radius: 50%;");
client.println("width: 80%;}");

//Configuracao CSS do botao Off

client.println(".OFF {");
client.println("background-color: #f44336; /* Red */");
client.println("border: none;");
client.println("color: white;");
client.println("padding: 40px;");
client.println("text-align: center;");
client.println("text-decoration: none;");
client.println("display: inline-block;");
client.println("font-size: 45px;");
client.println("margin: 4px 2px;");
client.println("cursor: pointer;");
client.println("border-radius: 50%;");
client.println("width: 80%;}");
client.println("</style>");
client.println("</head>");
client.println("</body>");
client.print("Estado do LED: "); 
if(value == HIGH) {
// Se está ligado apresenta “on”
client.println("<a href=\"/LED=OFF\"\"><button class='OFF'>Turn Off </button></a><br />");  // Desligar o LED corresponde Turn Off 
client.print("</br>");
client.print("On"); 
} else {
  if(value == LOW ){
  client.println("<a href=\"/LED=ON\"\"><button class='ON'>Turn On </button></a>"); // Ligar o LED corresponde Turn On
  client.print("</br>");
  client.print("Off");// Se está desligado apresenta “Off”
        }
}
client.println("<br><br>");
client.println("</body>");
client.println("</html>"); 
delay(1); 
Serial.println("Cliente desconectado"); // Depois do cliente efectuar o pedido apresenta esta mensagem no monitor série
Serial.println(""); 
}
