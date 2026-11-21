/************************************************************** 
 *
 *  DHT_func
 * 
 *  此 task 專門負責讀取 DHT 感測器數值，
 *  絕對不與其他工作混雜，達到明確的分工。
 *  DHT 感測器的最新數值會被儲存在全域變數 xDHT_temp, xDHT_humid 中，
 *  供其他 task 在需要時存取。
 *  
 *  備註
 *  task 內部請全部使用大寫的 DELAY()
 *  使用 Adafruit 提供的 DHT Library，請安裝以下兩個:
 *     https://github.com/adafruit/Adafruit_Sensor
 *     https://github.com/adafruit/DHT-sensor-library
 *  
 **************************************************************/
 
#include <DHT.h>

#define DHT_PIN             23          // 設定感測器資料腳位
#define DHT_TYPE            DHT11       // 設定型號為 DHT11 或 DHT22
#define DHT_INTERVAL        1000        // (ms) 設定重複讀取感測器的時間間隔

// Task 共享變數說明
// xDHT_temp   : 最新溫度數據，供其他 task 使用 
// xDHT_humid  : 最新濕度數據，供其他 task 使用 

void DHT_func(void*) {
  //----此區塊用於宣告僅在此 task 內部使用的變數----
  DHT dht(DHT_PIN, DHT_TYPE);
  //-----------------------------------------------
  VOID SETUP() {                       // task 內的 VOID SETUP() 使用大寫
    Serial.println("[DHT] task begin");
    dht.begin();
  }

  VOID LOOP() {                       // task 內的 VOID LOOP() 使用大寫
    float t = NAN; float h = NAN;  
    while( isnan(t) || isnan(h) ) {
      t = dht.readTemperature();
      h = dht.readHumidity();
    }
    xDHT_temp = t;  xDHT_humid = h;  // 確認讀取到的數值正確後，再複製給共享全域變數
    Serial.printf("[DHT] temp : %.2f C\thumid : %.2f %%\n", xDHT_temp, xDHT_humid);
    
    DELAY(DHT_INTERVAL);              // 延遲至下一次讀取感測器
  }
}
