/***************************************************** 
 *
 *  WiFi_func 
 *
 *  此 task 專門負責 WiFi 連線。
 *  若 WiFi 斷線，會啟動 Blink_task 讓 LED 快速閃爍作為警示。
 *  
 *  備註
 *  task 內部請全部使用大寫的 DELAY()
 *  
 ******************************************************/

#include <WiFi.h>

#define WIFI_SSID           "Cutie"
#define WIFI_PASSWORD       "tttyyy123"
#define WIFI_INTERVAL       2           // (分鐘) 檢查 WiFi 狀態的時間間隔

void WiFi_func(void*){  
  //----此區塊用於宣告僅在此 task 內部使用的變數----

  //-----------------------------------------------

  VOID SETUP(){                       // task 內的 VOID SETUP() 使用大寫
    WiFi.begin( WIFI_SSID, WIFI_PASSWORD );
  }
  
  VOID LOOP(){                        // task 內的 VOID LOOP() 使用大寫
    if( !WiFi.isConnected() ) {
      // WiFi 斷線時，指示 Blink_task 以每 80ms 的頻率快速閃爍
      xBlink_Delay = 80;  //ms
      Blink_task.start( Blink_func );
  
      while(!WiFi.isConnected() ) DELAY(400);
      Serial.print("[WiFi] WiFi Connected, IP : ");
      Serial.println(WiFi.localIP());
      
      // WiFi 連線成功後，停止 Blink_task 的警示運作
      Blink_task.stop();  

      // 啟動 AsyncWebServer_task 開始提供網頁服務
      AsyncWebServer_task.start(AsycWebServer_func);
    }
    
    DELAY( WIFI_INTERVAL * 60000); // 等待指定分鐘後，再次檢查 WiFi 狀態
  }
}
