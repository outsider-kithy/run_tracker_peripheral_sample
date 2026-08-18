#pragma once
#include <M5Unified.h>

// 初期値
float STEP_THRESHOLD = 0.35f;

int steps = 0;
bool stepActive = false;

// 初期化
void setupSteps() {

    if (!M5.Imu.isEnabled()) {
        M5.Imu.begin();
    }
    delay(50);
}

// 歩数カウント開始
void updateSteps() {

    float accX, accY, accZ;

    if (!M5.Imu.getAccel(&accX, &accY, &accZ)) {
        return;
    }

    // 加速度ベクトル
    float magnitude = sqrt(accX * accX + accY * accY + accZ * accZ);

    // 重力成分を除外
    float dynamicAccel = fabs(magnitude - 1.0f);

    // 歩行ピーク検出
    if (dynamicAccel > STEP_THRESHOLD && !stepActive) {
        stepActive = true;
        steps++;
        // Serial.printf(
        //     "STEP %d accel=%.3f\n",
        //     steps,
        //     dynamicAccel
        // );
    }

    // しきい値より十分小さくなったら次の歩行を検出可能にする
    if (dynamicAccel < STEP_THRESHOLD * 0.5f) {
        stepActive = false;
    }

    delay(20);
}

// 歩数カウント開始
void startCountSteps(){
	steps = 0;
}


// 歩数カウント停止
void stopCountSteps() {
    M5.Lcd.setCursor(0, 60);
	M5.Lcd.setTextColor(WHITE);
    M5.Lcd.print(steps);
    M5.Lcd.print(" steps, ");
}

