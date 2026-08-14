#include <M5Unified.h>
#include "getSteps.h"
#include "getTime.h"
#include "getGps.h"
#include "jsonHandler.h"
#include "bleHandler.h"

int pressCount = 0;

void setup() {

  M5.begin();

  // すでにM5 Stickに保存されているJSONファイルを表示
  listFilesFromRoot();

  M5.Lcd.setRotation(3);
  M5.Lcd.setTextSize(2);
  M5.Lcd.println("Initializing...");

  //歩数カウントを初期化
  setupSteps();
  //GPSを初期化
  setupGPS();
  // BLE初期化
  initBLE(); 

  M5.Lcd.println("Press A button!");
}

void loop() {
  	M5.update();
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
			
			// 走行データをJSONに保存
			saveRunDataToFile(path, totalDistance, steps, elapsed);
			M5.Lcd.setTextColor(RED);
			M5.Lcd.println("Data was saved!");
		}
		// 3回目(画面とデータをリセット)
		else if (pressCount == 3) {

			pressCount = 0;

			M5.Lcd.fillScreen(BLACK);
			M5.Lcd.setTextColor(WHITE);
			M5.Lcd.setCursor(0,0);
			M5.Lcd.println("Initializing...");

			path.clear();
			totalDistance = 0.0;
			steps = 0;
			elapsed = 0;
			startDate, endDate = "";

			M5.Lcd.println("Press A button!");
		}	
	}

	// セントラルからSYNCコマンドが送られてきたら
	if (syncRequested) {
        syncRequested = false;
        txCharacteristic->setValue("READY");
        txCharacteristic->notify();
        delay(100);
        sendFileWithAck();
    }

	// セントラルからDELETEコマンドが送られてきたら
    if(deleteRequested){
		txCharacteristic->setValue("DELETE_processing...");
		txCharacteristic->notify();
		delay(100);
		deleteAllJsonFiles();
    }
}