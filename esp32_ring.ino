#include <WiFi.h>
#include <HTTPClient.h>
#include <MD5Builder.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <tr064.h>

#include "credential.h"

WebServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.printf("Verbunden mit %s.\n", SSID);
  Serial.printf("IP-Adresse: %s.\n", WiFi.localIP().toString().c_str());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS-Responder gestartet.");
  }

  server.on("/", handleRoot);

  server.on("/ring1", []() {
    server.send(200, "text/plain", "Ring auf Phone 1");
    NotifyHandSetViaRingTest( 1 );
  });

  server.on("/ring2", []() {
    server.send(200, "text/plain", "Ring auf Phone 2");
    NotifyHandSetViaRingTest( 2 );
  });

  server.on("/ringall", []() {
    server.send(200, "text/plain", "Ring on all Phones");
    NotifyAllHandSetsViaCallHelp(  );
  });


  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP-Server gestartet.");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  server.send(200, "text/plain", "<h1>Hier ist Jogis neuer ESP32!</h1>\n");
}

void handleNotFound() {
  String message = "Pfad ";
  message += server.uri();
  message += " wurde nicht gefunden.\n";
  server.send(404, "text/plain", message);
}



void NotifyAllHandSetsViaCallHelp( void )
{
  TR064 connection(TR064_PORT, FRITZ_IP, FRITZUSER, FRITZPASS );

  connection.init();
  String params[][2] = {{"NewX_AVM-DE_PhoneNumber", "**9"}};
 
  String req[][2] = {{}};
  String params1[][2] = {{}};
  connection.action("urn:dslforum-org:service:X_VoIP:1","X_AVM-DE_DialNumber", params, 1, req, 0);

 
  //Hier können Sie die Zeit, die das Telefon klingelt, in Millisekunden einstellen
  delay(15000);
 
  connection.action("urn:dslforum-org:service:X_VoIP:1","X_AVM-DE_DialHangup", params1, 1, req, 0);
}



bool NotifyHandSetViaRingTest( int HandSet ) 
{
  HTTPClient http;
  Serial.println("Notify Single Handset " + String( HandSet ) ) ;
 
  // Get Challenge
  http.begin("http://fritz.box/login_sid.lua");
  int retCode = http.GET();
  if (retCode != 200) 
  {
    Serial.println("Get Challengd failed! " + String(retCode));
    return false;
  }
  String result = http.getString();
  String challenge = result.substring(result.indexOf("<Challenge>") + 11, result.indexOf("<Challenge>") + 19);
 
  // Calculate Response
  String reponseASCII = challenge + "-" + FRITZPASS;
  String responseHEX = "";
  for (unsigned int i = 0; i  < reponseASCII.length(); i++) 
  {
    responseHEX = responseHEX + String(reponseASCII.charAt(i), HEX) + "00";
  }
 
  MD5Builder md5;
  md5.begin();
  md5.addHexString(responseHEX);
  md5.calculate();
 
  String response = challenge + "-" + md5.toString();
 
  // Login and get SID
  http.begin("http://fritz.box/login_sid.lua?user=" + FRITZUSER + "&response=" + response);
  retCode = http.GET();
  if (retCode != 200) 
  {
    Serial.println("Get Sid failed! " + String(retCode));
    return false;
  }
  result = http.getString();
  String sid = result.substring(result.indexOf("<SID>") + 5,  result.indexOf("<SID>") + 21);
 
  // Execute Ring Tone Test
  http.begin("http://fritz.box/fon_devices/edit_dect_ring_tone.lua?idx=" + String(HandSet) + "&sid=" + sid + "&ring_tone_radio_test=0&start_ringtest=2");
  retCode = http.GET();
  if (retCode != 200) 
  {
    Serial.println("Enable Ring Tone Test failed! " + String(retCode));
    return false;
  }
  result = http.getString();
 
  
  // Wait 10 sec
  delay(10 * 1000);
 
  // Stop Ring Tone Test
  http.begin("http://fritz.box/fon_devices/edit_dect_ring_tone.lua?idx=" + String(HandSet) + "&sid=" + sid + "&stop_ringtest=1&ringtone&start_ringtest=0");
  retCode = http.GET();
  if (retCode != 200) 
  {
    Serial.println("Stopping Ring Tone Test failed! " + String(retCode));
    return false;
  }
  result = http.getString();
 
  // Logout
  http.begin("http://fritz.box/login_sid.lua?logout=1&sid=" + sid);
  http.GET();
  result = http.getString();
  http.end();
 
  return true;
}

