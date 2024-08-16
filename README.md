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

# Reality : using an ESP8266 


* Using an ESP8266
* on a NodeMCU-Board
* Using the deep-sleep-examples from  Rui Santos 
* This was running on batteries 
* but only for a month or so
* Reason was the using of a NodeMCU-Board which was using much more power than anticipated
* Therefore I forgot about the project...
* Recently I started to need it again.
* Now have power close to the doorbell
* So reused the old hw and the old code on ESP8266
* Still wanted to use deep-sleep 
* But not ring the phones when power goes up 
* Or a "ripple" on the power restarts the NodeMCU

==> 

* Current code in esp8266_sleep_ring.ino is THE real running code
* It was never here in my git-repo, just somehwere on my PC
* Now it is improved to check the restart-reason


Check for reset-reason is new


