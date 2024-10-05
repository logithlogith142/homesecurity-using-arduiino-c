#define BLYNK_TEMPLATE_ID "TMPL3bum-vJW1"
#define BLYNK_TEMPLATE_NAME "home"
#define BLYNK_AUTH_TOKEN "R6ylvL7dGB4p__HFgmXasp_hDN1nfTdx"//Include the library files
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

char auth[] = "R6ylvL7dGB4p__HFgmXasp_hDN1nfTdx"; // Enter your Auth token
char ssid[] = "vivo"; // Enter your WIFI name
char pass[] = "11223344"; // Enter your WIFI password

DHT dht(D3, DHT11); //(sensor pin, sensor type)
BlynkTimer timer;
bool pirbutton = 0;

// Define component pins
#define Buzzer D0
//#define MQ2 A0
#define trig D4
#define echo D5
#define PIR D6
#define relay1 D7
#define relay2 D8

// Get buttons values
BLYNK_WRITE(V0) {
  pirbutton = param.asInt();
}

void setup() {
  Serial.begin(9600);

  pinMode(Buzzer, OUTPUT);
  pinMode(PIR, INPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  dht.begin();

  // Call the functions
  // timer.setInterval(100L, gassensor);
  timer.setInterval(100L, DHT11sensor);
  timer.setInterval(100L, pirsensor);
  timer.setInterval(100L, ultrasonic);
}

// Get the DHT11 sensor values
void DHT11sensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
   // Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V2, t);
  Blynk.virtualWrite(V3, h);
  Serial.println(t);
  Serial.println(h);
}

// Get the PIR sensor values
void pirsensor() {
  bool value = digitalRead(PIR);
  if (pirbutton == 1) {
   
    if (value == 0) {
      
      digitalWrite(Buzzer, LOW);
    } else if (value == 1) { Serial.println("Warning! Please check your security system");
      Blynk.virtualWrite(V10, "Warning! Please check your security system"); // Change V10 to the appropriate virtual pin where you want to receive the notification
      digitalWrite(Buzzer, HIGH);
    }
  }

else{
  digitalWrite(Buzzer, LOW);
}}

// Get the ultrasonic sensor values
void ultrasonic() {
  digitalWrite(trig, LOW);
  delayMicroseconds(4);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long t = pulseIn(echo, HIGH);
  long cm = t / 29 / 2;
  Blynk.virtualWrite(V4, cm);
  if (cm >= 900) {
      
      digitalWrite(Buzzer, HIGH);
    }
    else{
            digitalWrite(Buzzer, LOW);

    }
     Serial.print("cm");
 Serial.println(cm);
}

// Get buttons values
BLYNK_WRITE(V5) {
  bool RelayOne = param.asInt();
  if (RelayOne == 1) {
    digitalWrite(relay1, LOW);
  } else {
    digitalWrite(relay1, HIGH);
  }
}

// Get buttons values
BLYNK_WRITE(V6) {
  bool RelayTwo = param.asInt();
  if (RelayTwo == 1) {
    digitalWrite(relay2, LOW);
     Serial.println("off");
  } else {
    digitalWrite(relay2, HIGH);
    Serial.println("on");
  }
}

void loop() {
  Blynk.run(); // Run the Blynk library
  timer.run(); // Run the Blynk timer
}
