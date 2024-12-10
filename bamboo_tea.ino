#include <WiFiMulti.h>
WiFiMulti wifiMulti;
#define DEVICE "ESP32"

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define WIFI_SSID "Pedro"
#define WIFI_PASSWORD "pedrolopes123"

#define INFLUXDB_URL "http://192.168.254.180:8086"
#define INFLUXDB_TOKEN "pp-NO2oX4WjRyGy5rqHMrhmNbEFKllozx_zYiReS0AhKwC-cAMz5FvhTgRTazxqUJ1Pgw1haKGsPwJQ0oPuKkQ=="
#define INFLUXDB_ORG "d7faedaf1294696d"
#define INFLUXDB_BUCKET "planta"

#define TZ_INFO "UTC-3"

// Declare InfluxDB client instance with preconfigured InfluxCloud certificate
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);

// Declare Data point
Point sensor("wifi_status");
Point sensor2("umdty");

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display

const int blue_led = 12;
const int red_led = 27;
const int rele = 32;

void setup() {
    Serial.begin(9600); // Initialize serial communication for debugging

    // Setup wifi
    WiFi.mode(WIFI_STA);
    wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("Connecting to wifi");
    while (wifiMulti.run() != WL_CONNECTED) {
      Serial.print(".");
      delay(100);
    }
    Serial.println("Connected to Wifi successfully");

    // Accurate time is necessary for certificate validation and writing in batches
    // We use the NTP servers in your area as provided by: https://www.pool.ntp.org/zone/
    // Syncing progress and the time will be printed to Serial.
    timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");


    // Check server connection
    if (client.validateConnection()) {
      Serial.print("Connected to InfluxDB: ");
      Serial.println(client.getServerUrl());
    } else {
      Serial.print("InfluxDB connection failed: ");
      Serial.println(client.getLastErrorMessage());
    }

    sensor.addTag("device", DEVICE);
    sensor.addTag("SSID", WiFi.SSID());

    sensor2.addTag("device", DEVICE);
    sensor2.addTag("umdty", "umidade do solo");

    Serial.println("initialized");
    lcd.init();
    lcd.clear();
    lcd.backlight();      // Make sure backlight is on
    lcd.setCursor(1,0);
    lcd.print("soil umdty: ");
    lcd.setCursor(1,1);
    lcd.print("light: ");

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
    lcd.setCursor(9,0);
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

    // influxdb

    // Clear fields for reusing the point. Tags will remain the same as set above.
    sensor.clearFields();
    sensor2.clearFields();

    // Store measured value into point
    // Report RSSI of currently connected network
    sensor.addField("rssi", WiFi.RSSI());
    sensor2.addField("umdty", sensorValue);

    // Print what are we exactly writing
    Serial.print("Writing: ");
    Serial.println(sensor.toLineProtocol());
    Serial.println(sensor2.toLineProtocol());

    // Check WiFi connection and reconnect if needed
    if (wifiMulti.run() != WL_CONNECTED) {
      Serial.println("Wifi connection lost");
    }

    // Write point
    // if (!client.writePoint(sensor)) {
    //   Serial.print("InfluxDB write failed: ");
    //   Serial.println(client.getLastErrorMessage());
    // }
    // if (!client.writePoint(sensor2)) {
    //   Serial.print("InfluxDB write failed: ");
    //   Serial.println(client.getLastErrorMessage());
    // }

    Serial.println("Waiting 1 second");
    delay(1000);
}
