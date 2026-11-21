/*****************************************************
 *
 * AsycWebServer_func
 *
 * 此 task 負責建立非同步 WebServer。
 * WebServer 不需要等待單一客戶端的完整請求處理完畢，
 * 即可服務下一個客戶端。
 * 這使得 Multitask 運作更加流暢，不阻塞其他執行緒。
 *
 * 備註
 * task 內部請全部使用大寫的 DELAY()
 * 建立非同步 WebServer 請安裝這兩個 Library:
 *    https://github.com/me-no-dev/AsyncTCP
 *    https://github.com/me-no-dev/ESPAsyncWebServer
 *
 ****************************************************/

#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

void AsycWebServer_func(void*) {
  //----此區塊用於宣告僅在此 task 內部使用的變數----

  //-----------------------------------------------
  VOID SETUP() {                       // task 內的 VOID SETUP() 使用大寫
    server.on("/heap", HTTP_GET, [](AsyncWebServerRequest * request) {
      request->send(200, "text/plain", String(ESP.getFreeHeap()));
    });
    
    server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
      char buf[50];
      sprintf(buf, "<H2>Temp  : %.2f<br>\nHumid : %.2f<br> YAW= %.2f<br> PITCH= %.2f<br> ROLL= %.2f<br></H2> \n", xDHT_temp, xDHT_humid,xax,xay,xaz);     
      request->send(200, "text/html", String(buf));
    });
    
    server.onNotFound([](AsyncWebServerRequest * request) {
      Serial.println("[AsyncWebServer] Page NOT FOUND ");
      request->send(404, "text/plain", "Page not found");
    });

    Serial.println("[AsyncWebServer] start!");
    Serial.printf ("[AsyncWebServer] Open http://%s on the browser.\n", WiFi.localIP().toString().c_str());
    server.begin();
  }

  VOID LOOP() {                        // task 內的 VOID LOOP() 使用大寫
    DELAY(1);                          // task 內的 DELAY(..) 使用大寫
  }
}
