/*****************************************************************
 *  範例說明
 *     - WiFi task  : 負責管理 WiFi 連線
 *     - DHT task   : 專門持續讀取 DHT 感測器數值
 *     - Blink task : 負責控制 LED 閃爍，可根據需求啟動
 *     - AsyncWebServer task : 負責建立非同步 WebServer
 *                             在每次 WiFi 連線成功後執行
 *  
 *  備註
 *  請確認已安裝下列 4 個 Library
 *  非同步 WebServer 所需 Library:
 *     https://github.com/me-no-dev/AsyncTCP
 *     https://github.com/me-no-dev/ESPAsyncWebServer
 * 
 *  DHT 感測器所需 Library:
 *     https://github.com/adafruit/Adafruit_Sensor
 *     https://github.com/adafruit/DHT-sensor-library
 ******************************************************************/

#include <TridentTD_EasyFreeRTOS32.h>

// 宣告 Task 變數與 Task 執行的函式原型
TridentOS   DHT_task, Blink_task, WiFi_task, AsyncWebServer_task, IMU_task;
void DHT_func(void*), Blink_func(void*), WiFi_func(void*), AsyncWebServer_func(void*), MPU_func(void*);

//----此區塊用於宣告在不同 Task 之間共享的變數------
int     xBlink_Delay;    // 控制 LED 閃爍速度 (標準命名以 x 開頭)
float   xDHT_temp;       // DHT 讀取到的最新溫度值 (標準命名以 x 開頭)
float   xDHT_humid;      // DHT 讀取到的最新濕度值 (標準命名以 x 開頭)
float   xax,xay,xaz;     // MPU6050 讀取到的 Yaw, Pitch, Roll
//---------------------------------------------------------

void setup(){  // 主程式的 setup() 使用小寫
  Serial.begin(115200); Serial.println();
    
  DHT_task.start( DHT_func );
  WiFi_task.start( WiFi_func );
  IMU_task.start( MPU_func );  
}

void loop(){}  // 主程式的 loop() 使用小寫，雖不需要執行內容但必須保留
