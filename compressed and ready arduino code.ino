#include <ESP8266WiFi.h>
#include <Servo.h>


// create servo objects to control servos on servo_pin_1 and servo_pin_2
Servo myservo_1, myservo_2;


// create wifi password && wifi SSID
const char WiFiAPPSK[] = "mypassword";
const char WiFiSSID[] = "wifiname";


// create wifi object 
WiFiServer server(80);


// servo pin declaration
byte servo_pin_1 = 12;
byte servo_pin_2 = 14;
// servo angle declaration with default angle of 90
byte servo_angle_1 = 90;
byte servo_angle_2 = 90;


void setup() {
	initHardware();
	setupWiFi();
}


void loop() {

	// Check if a client has connected 
	WiFiClient client = server.available();               
	if (!client) { return; }

	// Read the first line of the request
	String info = client.readStringUntil('\r');
	Serial.println("Info from ajax: " + info);
	Serial.println("servo 1 angle: " + String(servo_angle_1) + " servo 2 angle: " + String(servo_angle_2));
	client.flush();


	if(info.indexOf("/d?") != -1) {
		// setting servo 1 and 2 angles recived from ajax
		String temp = info.substring(info.indexOf("a") + 1, info.indexOf("b"));
		servo_angle_1 = temp.toInt();
		temp = info.substring(info.indexOf("b") + 1, info.indexOf("c"));
		servo_angle_2 = temp.toInt();

		//0- passing argument "0" will send the response with info about the angle of each servo to ajax
		//  client.print(requestMaker(0));
	} else {
		//1- passing argument "1" will send the response with the interface for the user 
		client.print(requestMaker(1));
	}

	if((servo_angle_1 >= 0 && servo_angle_1 <=180) && (servo_angle_2 >= 0 && servo_angle_2 <= 180)) {
		//settign servo 1 and 2 to the desired angle 
		myservo_1.write(servo_angle_1);
		myservo_2.write(servo_angle_2);
	}
	client.flush();
}

void setupWiFi(){
	WiFi.mode(WIFI_AP);
	WiFi.softAP(WiFiSSID, WiFiAPPSK);
}

void initHardware(){
	// initialization of serial port, wifi connection and 2 servos 
	Serial.begin(115200);
	server.begin();
	myservo_1.attach(servo_pin_1);
	myservo_2.attach(servo_pin_2);
}

String requestMaker(byte actionType) {

	// Response header 
	String s = "HTTP/1.1 200 OK\r\n";
	s += "Content-Type: text/html\r\n\r\n";
	s += "<!DOCTYPE html>\r\n<html>\r\n";

	if (actionType == 0) {
		// feedback if we are connected 
		s += "<servo>a" + String(servo_angle_1) + "b" + String(servo_angle_2) + "c</servo>";
	} else {
		// HTML,JS,CSS code returned once at the beginning.  
		s += "<head><meta charset='utf-8'>";
		s += "<meta name='viewport' content='width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no' />";
		s += "<title>esp8266 control</title><script type='text/javascript'>";
		s += "window.onload=()=>{function b(){l[0]=100/i.offsetWidth,l[1]=100/i.offsetHeight}";
		s += "function c(q){if('touchend'===q.type)return void d(q.target,[50,50]);";
		s += "let r=q.targetTouches[0],s=[];s[0]=Math.floor(r.pageX*l[0]),s[1]=Math.floor(r.pageY*l[1]),d(q.target,s)}";
		s += "function d(q,r){h(q,r);let s=[],t=[];s[0]=r[0]*m/100-0.5*m,";
		s += "s[1]=r[1]*m/100-0.5*m,t[0]=Math.floor(s[1]+s[0])+90,t[1]=Math.floor(s[1]-s[0])+90;";
		s += "o[0]===t[0]||o[1]===t[1]||0==t[0]%n&&1==t[1]%n||(o[0]=t[0],o[1]=t[1],g('a'+t[0]+'b'+t[1]+'c'))}";
		s += "function f(q){let r=q.type,s=String.fromCharCode(q.which),t='AWDS'.indexOf(s);";
		s += "-1!==t&&(p[t%2]=(1<t?m/2:-m/2)*('keyup'===r?0:1),";
		s += "p[0]===p[2]&&p[1]===p[3]||(p[2]=p[0],p[3]=p[1],h(i,p.map(u=>{return Math.ceil(100*(u+m/2)/m)})),";
		s += "g('a'+(p[1]-p[0]+90)+'b'+(p[1]+p[0]+90)+'c')))}function g(q){console.log(q);";
		s += "let r=new XMLHttpRequest;r.open('GET','http://192.168.4.1/d?'+q,!0),r.send(null)}";
		s += "function h(q,[r,s]){j.innerHTML='X '+('00'+r+'').slice(-3)+' Y '+('00'+s+'').slice(-3),";
		s += "k.style.top=Math.floor(s/l[1]-k.offsetHeight/2)+'px',k.style.left=Math.floor(r/l[0]-k.offsetWidth/2)+'px'}";
		s += "let i=document.body,j=document.querySelector('span'),k=document.querySelector('div'),";
		s += "l=[0,0],m=50,n=5,o=[],p=[0,0,0,0];";
		s += "['keyup','keydown','touchmove','touchstart','touchend'].forEach(q=>{";
		s += "i.addEventListener(q,q.indexOf('touch')?f:c,!1)}),window.onresize=()=>{b()},b()};";
		s += "</script><style type='text/css'>*{margin:0;padding:0; height: 100%; width:100%;}";
		s += "body{overflow: hidden;position:fixed;background:black;font-size:2em;text-align:center;color:white} ";
		s += "div{   box-sizing: border-box;height: 70px; width: 70px; position:absolute;top:calc(50vh - 35px);";
		s += "left:calc(50vw - 35px); z-index: 1000;border-radius: 50%;border:3px solid red;}</style>";
		s += "</head><body><span>ready</span><div></div></body>";
	}
	s += "\r\n</html>\r\n";
	return s;
}