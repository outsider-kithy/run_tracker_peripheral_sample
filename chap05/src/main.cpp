#include <M5Unified.h>
#include "getSteps.h"
#include "getGps.h"
#include "getTime.h"
#include "connectWifi.h"

int pressCount = 0;

void setup() {
  connectWifi();

  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.setTextSize(2);
  M5.Lcd.println("Initializing...");

  //歩数カウントを初期化
  setupSteps();
  //GPSを初期化
  setupGPS();
  //タイマーを初期化
  setupTimer();

  M5.Lcd.println("Press A button!");
}

void loop() {
  	M5.update();
	updateSteps();
	updateGPS();

	// Aボタンが押された瞬間
	if (M5.BtnA.wasPressed()) {
		// 押された回数を増やす
		pressCount++;
		// 1回目
		if (pressCount == 1) {
			M5.Lcd.setTextColor(YELLOW);
			M5.Lcd.println("Tracking Start!");
			//歩数カウントスタート
			startCountSteps();
			//GPSスタート
			startGPS();
			//タイマースタート
			startTimer();
		}

		// 2回目
		else if (pressCount == 2) {
			// 歩数カウントストップ
			stopCountSteps();
			// GPSストップ
			stopGPS();
			// タイマーストップ
			stopTimer();
			
			M5.Lcd.setTextColor(RED);
			M5.Lcd.println("Data was saved!");
		}
		// 3回目(画面とデータをリセット)
		else if (pressCount == 3) {

			pressCount = 0;
			path.clear();
			totalDistance = 0.0;
			steps = 0;
			elapsed = 0;
			startDate, endDate = "";

			// ディープスリープして1秒後に再起動
			esp_sleep_enable_timer_wakeup(1000000);
			esp_deep_sleep_start();
		}	
	}
}