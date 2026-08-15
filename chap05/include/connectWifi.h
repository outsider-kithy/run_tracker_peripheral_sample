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
		Serial.println("Wi-Fi is not available...");
    }
    if (WiFi.status() != WL_CONNECTED) {
        return;
    } else {
		Serial.println("Wi-Fi is available!");
	}
}

