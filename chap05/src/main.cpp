#include <M5Unified.h>
#include "getSteps.h"
#include "getTime.h"
#include "getGps.h"

int pressCount = 0;

void setup() {
  //connectWifi();

  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.setTextSize(2);
  M5.Lcd.println("Initializing...");

  //歩数カウントを初期化
  setupSteps();
  //タイマーを初期化
  setupTimer();
  //GPSを初期化
  setupGPS();
  // BLE初期化
//   initBLE(); 


  M5.Lcd.println("Press A button!");
}

void loop() {
  	M5.update();

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
			//タイマースタート
			startTimer();
			//GPSスタート
			startGPS();
		}

		// 2回目
		else if (pressCount == 2) {
			// 歩数カウントストップ
			stopCountSteps();
			// タイマーストップ
			stopTimer();
			// GPSストップ
			stopGPS();
			// 次に押した時にまた1回目に戻す
			pressCount = 0;
			path.clear();
		}
	}

	if(pressCount == 1){
		updateGPS();
	} else if(pressCount == 2){
		stopGPS();
	}
}

