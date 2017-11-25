
#include <ESP8266WiFi.h>
#include <ESP8266Ping.h>
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

const char* ssid2     = "SSID 1";
const char* password2 = "SSID 1 Password";
const char* ssid     = "SSID 2";
const char* password = "SSID 2 Password";
const char* remote_host = "8.8.8.8"; // Host to ping

bool lcdled = true; // LCD led blinking when there is an error in connection

char connectedssid[20];

unsigned long previousMillisForPing = 0;
const long Pinginterval = 45000;
unsigned long previousMillisForLCD = 0;
const long LCDUpdateinterval = 3000;

bool pingtest = false;
int wificount;
LiquidCrystal_PCF8574 lcd(0x3F);



void setup() {
  Serial.begin(115200);
   lcd.begin(16, 2);
   lcd.clear();
   lcd.home();
   lcd.setBacklight(125);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println("Connecting to WiFi");
  //=====================
   WiFi.mode(WIFI_STA);
   WiFi.disconnect();
   delay(100);
   
   int n = WiFi.scanNetworks();
	Serial.println("scan done");
	Serial.println(n);
	if (n == 0)
	Serial.println("no networks found");
	else
	for (int i = 0; i < n; ++i)
	{
		// Print SSID for each network found

			Serial.println(WiFi.SSID(i));
		if (WiFi.SSID(i) == ssid){
			Serial.println("SSID Found");
			WiFi.disconnect(true);
			WiFi.begin(ssid, password);
			break;
		}
		delay(10);
	}
	if(WiFi.status() != WL_CONNECTED) {
	
	for (int i = 0; i < n; ++i)
	{
		Serial.println(WiFi.SSID(i));
		if (WiFi.SSID(i) == ssid2){
			Serial.println("SSID2 Found");
			WiFi.disconnect(true);
			WiFi.begin(ssid2, password2);
			break;
		}
		delay(10);
	}	
	}
	

  

  Wire.begin();
  Wire.beginTransmission(0x3F);
  wifimonitor(0);

  Serial.println();
  Serial.print("WiFi connected with ip ");  
  Serial.println(WiFi.localIP());
 
	lcd.setCursor(0, 0);
	lcd.print("IP:");
	lcd.print(WiFi.localIP());
	
	lcd.setCursor(0,1);
	lcd.print(remote_host);
	delay(2000);
	scroller(20,150);
	lcd.clear();	
	 lcd.setCursor(0, 0);
	 lcd.print("WiFi Network");
	 lcd.setCursor(0, 1);
	 lcd.print(WiFi.SSID());
	 delay(2000);
	 
  Serial.print("Pinging host ");
  Serial.println(remote_host);
	pinger();
}

void loop() {
	unsigned long currentMillisForPing = millis();
	unsigned long currentMillisForLCD = millis();
	
	wifimonitor(1);
	
	if (currentMillisForPing - previousMillisForPing >= Pinginterval) {
		  // save the last time you blinked the LED
		  Serial.println(currentMillisForPing - previousMillisForPing > Pinginterval);
		  previousMillisForPing = currentMillisForPing;
		  Serial.println(currentMillisForPing - previousMillisForPing);
		  Serial.println(millis());
		 pinger();
		  Serial.println(pingtest);
	  }
	  if (currentMillisForLCD - previousMillisForLCD >= LCDUpdateinterval) {
		  // save the last time you blinked the LED
		  previousMillisForLCD = currentMillisForLCD;

	  }
	if(pingtest) {
		lcd.setBacklight(0);
		
		lcd.clear();
	    lcd.setCursor(0, 1);
		lcd.print("Ping:");
	    lcd.print(Ping.averageTime());
		lcd.print("ms");
		lcd.setCursor(0, 0);
		lcd.print("Host:");
		lcd.print("GOOGLE DNS");
		delay(5000);
		scroller(16,100);
		lcd.setCursor(17,0);
		lcd.print("WIFI CONNECTED-");
		lcd.print(WiFi.status());
		lcd.setCursor(17,1);
		lcd.print(WiFi.SSID());
		delay(3600);
	}
	
	else {
		
		if (lcdled = true)
		{
			lcd.setBacklight(255);
			Serial.println("Error :(");
			lcd.clear();
			lcd.home();
			lcd.setCursor(0, 0);
			lcd.print("  CONNECTIVITY  ");
			lcd.setCursor(0,1);
			lcd.print("    ISSUE :( -");
			lcd.print(WiFi.status());
			delay(750);
			lcd.setBacklight(0);
			delay(500);
			lcd.setBacklight(255);
			delay(500);
			lcd.setBacklight(0);
			delay(500);
			lcd.setBacklight(255);
			lcd.clear();
			delay(150);
			lcd.setCursor(0, 0);
			lcd.print("REQUEST TIME OUT");
			lcd.setCursor(0,1);
			lcd.print("HOST UNREACHABLE");
			delay(750);
			lcd.setBacklight(0);
			delay(500);
			lcd.setBacklight(255);
			delay(500);
			lcd.setBacklight(0);
			delay(500);
			lcd.setBacklight(255);
			delay(3600);
		} 
		else
		{
			lcd.setBacklight(0);
			Serial.println("Error :(");
			lcd.clear();
			lcd.home();
			lcd.setCursor(0, 0);
			lcd.print("  CONNECTIVITY  ");
			lcd.setCursor(0,1);
			lcd.print("    ISSUE :( -");
			lcd.print(WiFi.status());
			delay(2250);		
			lcd.clear();
			delay(150);
			lcd.setCursor(0, 0);
			lcd.print("REQUEST TIME OUT");
			lcd.setCursor(0,1);
			lcd.print("HOST UNREACHABLE");
			delay(5700);
		}
		
		
	}
	
	
}
void wifimonitor(int wmstatus){
	
  while (WiFi.status() != WL_CONNECTED) {
	  lcd.setBacklight(125);
	  delay(500);
	  Serial.print(".");
	  lcd.clear();
	  lcd.home();
	
	  
	  lcd.setCursor(0, 1);
	  lcd.print(WiFi.SSID());
	  lcd.setCursor(0, 0);
	  int curpos = 0;
	    if (wmstatus == 1)
	    {
		    lcd.print("Disconnected");
			delay(1000);
			lcd.setCursor(12,0);
			curpos = 12;
		    }
			else{
		    lcd.print("Starting");
			delay(1000);
			lcd.setCursor(8,0);
			curpos = 8;
	    }
	  
	 
	  while(WiFi.status() != WL_CONNECTED){
		  //----------------------
		if (wmstatus == 0){
			lcd.print(".");
			delay(300);
			curpos++;
			lcd.setCursor(curpos,0);
			if (curpos > 15)
			{
				lcd.clear();
				lcd.home();
				if (wmstatus = 0)
				{
					lcd.print("Starting");
				}
				
				lcd.setCursor(0,1);
				lcd.print(WiFi.SSID());
				lcd.setCursor(8,0);
				curpos = 8;
			}
			
		}else{
			lcd.print(".");
			delay(300);
			curpos++;
			lcd.setCursor(curpos,0);
			if (curpos > 15)
			{
				lcd.clear();
				lcd.home();
				if (wmstatus == 0)
				{
					lcd.print("Disconnected");
				}
				
				lcd.setCursor(0,1);
				lcd.print(WiFi.SSID());
				lcd.setCursor(12,0);
				curpos = 12;
			}
		}
				
		  
	  }
	   
  }

}
void scroller(int scrolldistant, int scrolldelay){
	for (int positionCounter = 0; positionCounter < scrolldistant; positionCounter++) {
		// scroll one position left:
		lcd.scrollDisplayLeft();
		// wait a bit:
		delay(scrolldelay);
	}
}
bool pinger(){
	Serial.println("Pinger Starting Pinging");
	pingtest = Ping.ping(remote_host);
	Serial.println("Success!!");
	Serial.print("Ping Average: ");
	Serial.println(Ping.averageTime());
		Serial.print("Ping Success :");
		Serial.println(pingtest);
	return pingtest;
	
	}
