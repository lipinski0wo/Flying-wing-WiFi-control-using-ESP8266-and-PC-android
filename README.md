# Flying Wing WiFi control with ESP8266 and your PC or Phone browser 

In this project we are trying to use ESP8266 WiFi microcontroller as Flying Wing / Delta Wing aircraft communication board. 

Files: 

- HTML and CSS.html		=> html and css code used to make the structure of website nice and clean.

- JS control panel.js	=> js code giving dynamic behaviour and data exchange between esp8266 and browser

- compressed and ready arduino code.ino		=> ready to use arduino code with added html, css and js code available in the files above.



The code was tested on ESP8266-12S with 2 voltage converters: first one from 3,7 v to 3,3 v for esp8266, second one from 3,7 v to 5,0 v powering two Servos TowerPro SG-90 - micro. 3,7 v was provided by 1 cell LiPo  battery. 

Note: It is vital to provide steady power for ESP8266. If not the module will likely throw an error and disconnect.




How to use it:

- use "compressed and ready arduino code.ino" and add this code to your ESP8266 via Arduino IDE. 
- fire up your ESP8266 and connnect with it using your PC or phone.
	- default password is: 'mypassword', and WiFi name is: 'wifiname'.
- open your internet browser (chrome or firefox will do) and type this address: 'http://192.168.4.1/'.
- at this point it is ready to use: for PC controls are WSAD keys and for phone the whole browser screen.
- you can open console on your PC to monitor basic data being exchanged.



