#include <M5Unified.h>
#include <WiFi.h>

const char* ssid = "YOUR_SSID"; 
const char* password = "YOUR_PASSWORD"; 

void connectWifi(){
	Serial.begin(9600);
	WiFi.begin(ssid, password);
	unsigned long startAttempt = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 5000) {
        delay(500);
		Serial.print("Wi-Fi is not available...");
    }
    if (WiFi.status() != WL_CONNECTED) {
        return;
    } else {
		Serial.print("Wi-Fi is available!");
	}
}

