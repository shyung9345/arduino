/*****************************************************
 * 
 *  Blink_func 
 *
 *  此 task 負責控制 LED 閃爍。
 *  可透過外部變數動態調整閃爍的快慢。
 *  
 *  備註
 *  task 內部請全部使用大寫的 DELAY()
 *  
 ****************************************************/

#define BLINK_LED_PIN       2
#define DEFAULT_BLINK_DELAY 1000

// Task 共享變數說明
// xBlink_Delay   : 供外部 task 寫入，以控制此 task 的閃爍延遲時間

void Blink_func(void* arg){  // Blink_task 在此執行
  VOID SETUP(){                       // task 內的 VOID SETUP() 使用大寫
    pinMode(BLINK_LED_PIN,OUTPUT);
    if( xBlink_Delay <= 0) xBlink_Delay = DEFAULT_BLINK_DELAY;
  }

  VOID LOOP() {                       // task 內的 VOID LOOP() 使用大寫
    digitalWrite(BLINK_LED_PIN,HIGH);
    DELAY(xBlink_Delay);              // task 內的 DELAY(..) 使用大寫
    digitalWrite(BLINK_LED_PIN,LOW);
    DELAY(xBlink_Delay);
  }
}
