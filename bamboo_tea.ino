#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display

void setup() {
    // Serial.begin(115200);
    Serial.begin(9600); // Initialize serial communication for debugging
    Serial.println("initialized");
    lcd.init();
    lcd.clear();
    lcd.backlight();      // Make sure backlight is on
    lcd.setCursor(2,0);   //Set cursor to character 2 on line 0
    lcd.print("bamboo");
}

void loop() {
    // lcd.setCursor(2,1);   //Set cursor to character 2 on line 0
    // lcd.print("Dst: ");
    // lcd.setCursor(11,1);
    // lcd.print("  ");
    // delay(2000);

    int sensorValue = analogRead(A0); // Read the analog value from the sensor
    Serial.print("Soil Moisture Level: ");
    Serial.println(sensorValue);
    lcd.setCursor(2,1);   //Set cursor to character 2 on line 0
    lcd.print(sensorValue);
    delay(1000); // Delay for one second before the next reading
}
