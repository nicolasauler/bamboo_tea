#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display

int blue_led = 12;
int red_led = 27;
int rele = 32;

void setup() {
    // Serial.begin(115200);
    Serial.begin(9600); // Initialize serial communication for debugging
    Serial.println("initialized");
    lcd.init();
    lcd.clear();
    lcd.backlight();      // Make sure backlight is on
    lcd.setCursor(2,0);
    lcd.print("bamboo: soil umidity");

    pinMode(blue_led, OUTPUT);
    pinMode(red_led, OUTPUT);
    pinMode(rele, OUTPUT);
}

void loop() {
    // lcd.setCursor(2,1);
    // lcd.print("Dst: ");
    // lcd.setCursor(11,1);
    // lcd.print("  ");
    // delay(2000);

    int sensorValue = analogRead(A0);
    Serial.print("Soil Moisture Level: ");
    Serial.println(sensorValue);
    lcd.setCursor(2,1);
    lcd.print(sensorValue);

    if (sensorValue > 3000) {
        digitalWrite(blue_led, HIGH);
        digitalWrite(rele, HIGH);
    } else {
        digitalWrite(blue_led, LOW);
        digitalWrite(rele, LOW);
    }

    if (sensorValue < 1400) {
        digitalWrite(red_led, HIGH);
    } else {
        digitalWrite(red_led, LOW);
    }

    delay(1000);
}
