#include <M5Unified.h>
#include "getSteps.h"

int pressCount = 0;

void setup() {

  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.setTextSize(2);
  M5.Lcd.println("Initializing...");

  //歩数カウントを初期化
  setupSteps();

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
		}

		// 2回目
		else if (pressCount == 2) {
			// 歩数カウントストップ
			stopCountSteps();
			// 次に押した時にまた1回目に戻す
			pressCount = 0;
		}
	}
}

