// 必須安裝 I2Cdev 與 MPU6050 Library
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps_V6_12.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

// 預設 I2C 地址為 0x68 (AD0 接地)
MPU6050 mpu;

// 輸出 Yaw, Pitch, Roll 角度格式 (以度為單位)
#define OUTPUT_READABLE_YAWPITCHROLL

#define INTERRUPT_PIN 2  // 使用 Arduino Uno 或多數開發板的 Pin 2 作為外部中斷腳位
#define LED_PIN 13       
bool blinkState = false;

// MPU 控制與狀態變數
bool dmpReady = false;  // 若 DMP 初始化成功則設為 true
uint8_t mpuIntStatus;   // 存放 MPU 實際的中斷狀態
uint8_t devStatus;      // 每次設備操作的回傳狀態 (0 = 成功, !0 = 錯誤)
uint16_t packetSize;    // 預期的 DMP 封包大小 (預設為 42 bytes)
uint16_t fifoCount;     // 目前 FIFO 中的位元組總數
uint8_t fifoBuffer[64]; // FIFO 儲存緩衝區

// 姿態與運動變數
Quaternion q;           // [w, x, y, z]         四元數容器
VectorFloat gravity;    // [x, y, z]            重力向量
float ypr[3];           // [yaw, pitch, roll]   Yaw/Pitch/Roll 容器


// ================================================================
// ===                   外部中斷偵測常式                   ===
// ================================================================

volatile bool mpuInterrupt = false;     // 指示 MPU 中斷腳位是否已觸發 High
void dmpDataReady() {
  mpuInterrupt = true;
}


void MPU_func(void*) {
 
// ================================================================
// ===                      初始化設定                      ===
// ================================================================

VOID SETUP() { 
  // 加入 I2C 匯流排
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
  Wire.setClock(400000); // 400kHz I2C 時脈
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif

  Serial.begin(115200);
  while (!Serial); 

  // 初始化設備
  Serial.println(F("Initializing I2C devices..."));
  mpu.initialize();
  pinMode(INTERRUPT_PIN, INPUT);

  // 驗證連線
  Serial.println(F("Testing device connections..."));
  Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

  // 載入並設定 DMP
  Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();

  // 提供陀螺儀與加速度計的偏移量 (Offsets)
  mpu.setXGyroOffset(51);
  mpu.setYGyroOffset(8);
  mpu.setZGyroOffset(21);
  mpu.setXAccelOffset(1150);
  mpu.setYAccelOffset(-50);
  mpu.setZAccelOffset(1060);
  
  // 確認設定成功 (回傳 0 代表成功)
  if (devStatus == 0) {
    // 校準 MPU6050
    mpu.CalibrateAccel(6);
    mpu.CalibrateGyro(6);
    Serial.println();
    mpu.PrintActiveOffsets();
    
    // 啟動 DMP
    Serial.println(F("Enabling DMP..."));
    mpu.setDMPEnabled(true);

    // 啟用外部中斷偵測
    Serial.print(F("Enabling interrupt detection (Arduino external interrupt "));
    Serial.print(digitalPinToInterrupt(INTERRUPT_PIN));
    Serial.println(F(")..."));
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();

    // 標記 DMP 已準備就緒，讓主迴圈知道可以開始讀取資料
    Serial.println(F("DMP ready! Waiting for first interrupt..."));
    dmpReady = true;

    packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    // 錯誤處理 (1 = 初始記憶體載入失敗, 2 = DMP 設定更新失敗)
    Serial.print(F("DMP Initialization failed (code "));
    Serial.print(devStatus);
    Serial.println(F(")"));
  }

  pinMode(LED_PIN, OUTPUT);
}



// ================================================================
// ===                    主程式迴圈                        ===
// ================================================================

VOID LOOP() {
  // 若初始化失敗，則不執行任何動作
  if (!dmpReady) return;
  
  // 從 FIFO 讀取封包
  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) { // 取得最新封包

#ifdef OUTPUT_READABLE_YAWPITCHROLL
    // 顯示 Euler 角度 (以度為單位)
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

    // 寫入全域變數供 WebServer 讀取
    xax=ypr[0];
    xay=ypr[1];
    xaz=ypr[2];
    
    Serial.print("ypr\t");
    Serial.print(ypr[0] * 180 / M_PI);
    Serial.print("\t");
    Serial.print(ypr[1] * 180 / M_PI);
    Serial.print("\t");
    Serial.print(ypr[2] * 180 / M_PI);
    Serial.println();
#endif

    // 閃爍 LED 指示資料正在讀取
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);
  }
}
}
