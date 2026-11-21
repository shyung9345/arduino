/*****************************************************************
 *  範例 
 *     - WiFi task  : 用於管理 WiFi
 *     - DHT task   : 用於持續讀取 DHT 感測器的數值
 *     - Blink task : 用於控制 LED 閃爍，可依需求呼叫執行
 *     - AsyncWebServer task : 用於建立非同步網頁伺服器，每次成功連線 WiFi 後便會執行
 *  
 *  注意事項
 *  請確保已安裝這 4 個函式庫
 *  用於非同步網頁伺服器的函式庫
 *     https://github.com/me-no-dev/AsyncTCP
 *     https://github.com/me-no-dev/ESPAsyncWebServer
 * 
 *  用於 DHT 感測器的函式庫
 *     https://github.com/adafruit/Adafruit_Sensor
 *     https://github.com/adafruit/DHT-sensor-library
 *
 ******************************************************************/

#include <TridentTD_EasyFreeRTOS32.h>

// 宣告任務變數與任務函式
TridentOS   DHT_task, Blink_task, WiFi_task, AsyncWebServer_task, IMU_task;
void DHT_func(void*), Blink_func(void*), WiFi_func(void*), AsyncWebServer_func(void*), MPU_func(void*);

//----此區塊用於宣告任務間共用資料的變數 ------
int     xBlink_Delay;    // 設定 LED 閃爍速度（標準命名規範以 x 開頭）
float   xDHT_temp;       // DHT 最新讀取到的溫度值（標準命名規範以 x 開頭）
float   xDHT_humid;      // DHT 最新讀取到的濕度值（標準命名規範以 x 開頭）
float   xax,xay,xaz;
//---------------------------------------------------------

void setup(){  // 小寫的主函式 setup()
  Serial.begin(115200); Serial.println();
    
  DHT_task.start( DHT_func );
  WiFi_task.start( WiFi_func );
  IMU_task.start( MPU_func );  
}

void loop(){}  // 小寫的主函式 loop()，雖然不一定需要用到，但請務必保留
