#include "stepperDriver.h"


long programStartTime_ms = 0;

uint32_t last_time_status = 0;

void setup() 
{
  programStartTime_ms = millis();

  Serial.begin(250000); //init serial port and set baudrate
  Serial.println("\nStart...");

  initStepper(500);

  stop();
  
  rotateToAsync(20000);
  delay(1000);
}

uint16_t rms_current(uint8_t CS, float Rsense = 0.11) {
  return (float)(CS+1)/32.0 * (vsense?0.180:0.325)/(Rsense+0.02) / 1.41421 * 1000;
}

void loop() 
{
  long ms = millis();

  if(isRunning == 0)
  {
    Serial.println();
    Serial.println("Reversing...");
    delay(200);
    rotateToAsync(-counterTarget);
    Serial.println();
    delay(200);
  }

  if((ms-last_time_status) > 1000) //run every 1s
  {
    last_time_status = ms;

    Serial.print("Running time: ");
    Serial.print(ms-programStartTime_ms);
    Serial.println("ms");

    uint32_t drv_status = driver.DRV_STATUS();
    Serial.print("0 ");
    uint32_t sgValue = (drv_status & SG_RESULT_bm)>>SG_RESULT_bp;

    Serial.print(sgValue, DEC);
    Serial.print(" ");
    Serial.println(rms_current((drv_status & CS_ACTUAL_bm)>>CS_ACTUAL_bp), DEC);

    Serial.print("currCounter = ");
    Serial.println(getCurrentCounter());

    Serial.println();
  }
}
