# Doorbell-Ring on Handsets with Fritzbox through ESP32

Aim of this project is to ring specific or all handsets connected to a FritzBox when the doorbell rings.

I am not the first and I will not be the last to try to achieve this goal.  

Basically I found 2 different methods to achieve that goal:

* using the functionality of a call to all handsets via "**9"
* using the functionality to change the Ringtone of a specific handset


Some of the stuff I found was rather old, some of it using strange interfaces. 
At the current state it is a collection of source-code found at different places in the Web.  

## Lua-Scripts with http: Change Ringtone

On one hand it uses a functionality found in:  
https://www.open4me.de/index.php/2016/10/fritzbox-esp8266-laesst-telefon-klingeln/

This is using the functionality of changing the ringtone in a specific handset and then running the ringing in that handset.  
To achieve that it is using http-calls to the Lua-Scripts in the Fritzbox.
 
## TR064 - Functionality : Call all with call-helper

Also is uses the ability to ring on all handsets with "**9".

This is mainly cloned from:

* http://www.roehrenkramladen.de/Tuerklingel/TK-FB-V1a-1.html
* https://github.com/kbr/fritzconnection

which in the end points to an article in the german magazin: c't
here

* https://www.heise.de/select/ct/2018/17/1534215254552977

All these examples are using TR-064 as interface to the Fritzbox.


