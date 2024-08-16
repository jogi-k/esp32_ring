#include <ESP8266WiFi.h>
#include <tr064.h>


#include "credential.h"

extern "C" {
#include "user_interface.h"

struct rst_info *rtc_info = system_get_rst_info();

}

/*
 * derived from, thanks for the inspiration  :
 * ESP8266 Deep sleep mode example
 * Rui Santos 
 * Complete Project Details https://randomnerdtutorials.com
 */
 
void setup() {

  Serial.begin(115200);
  Serial.setTimeout(2000);

  // Wait for serial to initialize.
  while(!Serial) { }
  Serial.println("====");
  Serial.println("reset reason: ");
  Serial.println(rtc_info->reason);
  if (rtc_info->reason == 5 )  // only when coming from Deep-Sleep, not when e.g. switched on 
  {
    Serial.println("Coming from Deep-Sleep !");
    WiFi.begin(SSID, PASSWORD );
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    Serial.println("Going to call all handsets and then go to deep sleep again");
    NotifyAllHandSetsViaCallHelpOn( );  
    delay(10000);
    NotifyAllHandSetsViaCallHelpOff( );  
  }
  ESP.deepSleep(0); 
  
}

void loop() {
}


TR064 connection(TR064_PORT, FRITZ_IP, FRITZUSER, FRITZPASS );

void NotifyAllHandSetsViaCallHelpOn( void )
{

  connection.init();
  String params[][2] = {{"NewX_AVM-DE_PhoneNumber", "**9"}};
  String req[][2] = {{}};
  connection.action("urn:dslforum-org:service:X_VoIP:1","X_AVM-DE_DialNumber", params, 1, req, 0);
}


void NotifyAllHandSetsViaCallHelpOff( void )
{
  String req[][2] = {{}};
  String params1[][2] = {{}};
  connection.action("urn:dslforum-org:service:X_VoIP:1","X_AVM-DE_DialHangup", params1, 1, req, 0);

}
