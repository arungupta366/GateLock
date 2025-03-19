#include <ESP8266WiFi.h>

const int MAX_RUNNING_TIME = 3600000;
const char* ssid = "Arun"; // fill in here your router or wifi SSID
const char* password = "63666366"; // fill in here your router or wifi password
 #define RELAY 3 // relay connected to  GPIO0
WiFiServer server(80);
 
void setup() 
{
  pinMode(RELAY, INPUT_PULLUP);  // Disable internal pull-down, enable pull-up
  delay(100);  // Short delay to stabilize GPIO state

  Serial.begin(115200,SERIAL_8N1,SERIAL_TX_ONLY); // must be same baudrate with the Serial Monitor
  Serial.setDebugOutput(true);
  pinMode(RELAY,OUTPUT);
  digitalWrite(RELAY,HIGH);
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  server.setNoDelay(true);
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("https://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}

// prepare a web page to be send to a client (web browser)
String prepareHtmlPage()
{
  String htmlPage;
  htmlPage.reserve(1024);               // prevent ram fragmentation
  htmlPage = F("HTTP/1.1 200 OK\r\n"
               "Content-Type: text/json\r\n"
               "Connection: close\r\n"  // the connection will be closed after completion of the response
               "\r\n"
               "{\"gateStatus\":\"open\"}"
               "\r\n");
               
  return htmlPage;
}
 
void loop() 
{
  // Serial.println("loop");
  if(millis()> MAX_RUNNING_TIME){
    // Serial.println("ESP RESTARTED");
    ESP.restart();
  }
  // Check if a client has connected
  WiFiClient client = server.accept();
  delay(500);
  if (!client) 
  {
    // Serial.println("no client");
    client.stop();
    delay(100);
    return;
  }
 
  // Wait until the client sends some data
  // Serial.println("new client");
  delay(50);
  if(!client.available())
  {
    // Serial.println("new client not available");
    return;
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  // Serial.println(request);
  client.flush();
  client.println(prepareHtmlPage());
  // Match the request
  if (request.indexOf("/RELAY=ON") != -1)  
  {
    // Serial.println("RELAY=ON");
    digitalWrite(RELAY,LOW);
    delay(400);
    digitalWrite(RELAY,HIGH);
  }
  delay(500);
  // client.stop();
  // Serial.println("Client Stop");
  // Serial.println("Loop Completed");
  // Serial.println("");
}
