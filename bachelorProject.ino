#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ESP32_MailClient.h"
#include <WiFiManager.h>
#include "esp_adc_cal.h"
#include "logos.h"
#include "sending_mail.h"
#include "loading_animation.h"

#define lm35Sensor    35
#define TRIGGER_PIN_START 18
#define GMAIL_SMTP_SEVER "smtp.gmail.com"
#define GMAIL_SMTP_USERNAME "esp32smolskyi@gmail.com"
#define GMAIL_SMTP_PASSWORD "xlzmhhomiwblzkhi"
#define GMAIL_SMTP_PORT 587  
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int sensorData = 0;
float lm35TempC = 0.0;
float voltage = 0.0;
float temperature = 0.0;
const int arraySize = 10;
int highThreshold = 0;
int lowThreshold = 0;

int timeout = 90;
bool isConnecting = false;

/*Функція для розрахунку середнього значення температури*/
float runMiddleArifmOptim(float newValues) {
  static int i = 0;
  static float values[arraySize];
  static float average = 0;
  if (++i >= arraySize) i = 0; 
  average -= values[i];         
  average += newValues;          
  values[i] = newValues;           
  return ((float)average / arraySize);
}

void temperatureFunction() {
  // Зчитування виводу АЦП lm35Sensor
  sensorData = analogRead(lm35Sensor);  
  // Калібрування АЦП та отримання напруги (у мВ)
  voltage = readADC_Cal(sensorData);
  // Градуси Цельсія
  lm35TempC = voltage / 10;
  // Фільтрування даних
  float filteredTemp = runMiddleArifmOptim(lm35TempC);

  temperature = filteredTemp;
  delay(1500);
}

uint32_t readADC_Cal(int ADC_Raw) {
  esp_adc_cal_characteristics_t adc_chars;
  
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
  return(esp_adc_cal_raw_to_voltage(ADC_Raw, &adc_chars));
}

SMTPData data;
WiFiManager wm;
char hThreshold[4];
char lThreshold[4];
/*Функція для підключення до мережі*/
void connectToWiFi() {
  if (digitalRead(TRIGGER_PIN_START) == LOW) {
    int numIterations = 8;
    for(int i = 0; i < numIterations; i++) {
      display.clearDisplay(); 
      showBootLogo(loading1);
      showBootLogo(loading2);
      showBootLogo(loading3);
      showBootLogo(loading4);
      showBootLogo(loading5);
      showBootLogo(loading6);
      showBootLogo(loading7);
      showBootLogo(loading8);
      showBootLogo(loading9);
      showBootLogo(loading10);
      showBootLogo(loading11);
      showBootLogo(loading12);
      showBootLogo(loading13);
      showBootLogo(loading14);
      showBootLogo(loading15);
      showBootLogo(loading16);
      showBootLogo(loading17);
      showBootLogo(loading18);
      showBootLogo(loading19);
      showBootLogo(loading20);
      showBootLogo(loading21);
      showBootLogo(loading22);
      showBootLogo(loading23);
      showBootLogo(loading24);
      showBootLogo(loading25);
      showBootLogo(loading26);
      showBootLogo(loading27);
      showBootLogo(loading28);
      delay(100);
    }
      display.clearDisplay();
      display.display();
      WiFiManagerParameter highTemperatureThreshold("key_num1", "Високий поріг", hThreshold, 5);
      WiFiManagerParameter lowTemperatureThreshold("key_num2", "Низький поріг", lThreshold, 5);
      wm.addParameter(&highTemperatureThreshold);
      wm.addParameter(&lowTemperatureThreshold);

      wm.setConfigPortalTimeout(timeout);
      bool startPort = wm.startConfigPortal("TemperatureMonitoringSystem", "Smolskyi");
      strcpy(hThreshold, highTemperatureThreshold.getValue());
      strcpy(lThreshold, lowTemperatureThreshold.getValue());
      highThreshold = atoi(hThreshold);
      lowThreshold = atoi(lThreshold);
      if(startPort) {
        display.clearDisplay();
        showBootLogo(myAcc);
        isConnecting = true;
        delay(1000);
    }
    else {
      display.clearDisplay();
      showBootLogo(myErr);
    }
  }
}
String sendEmail(char *subject, char *sender, char *body, char *recipient, boolean htmlFormat) {
  data.setLogin(GMAIL_SMTP_SEVER, GMAIL_SMTP_PORT, GMAIL_SMTP_USERNAME, GMAIL_SMTP_PASSWORD);
  data.setSender(sender, GMAIL_SMTP_USERNAME);
  data.setSubject(subject);
  data.setMessage(body, htmlFormat);
  data.addRecipient(recipient);
  if (!MailClient.sendMail(data)) 
    return MailClient.smtpErrorReason();
  return "";
}
void showBootLogo(const unsigned char* logoData) {
  display.clearDisplay();
  display.drawBitmap(0, 0, logoData, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
  display.display();
}
void animationOfSendingEmail() {
  showBootLogo(sendingMail1);
  showBootLogo(sendingMail2);
  showBootLogo(sendingMail3);
  showBootLogo(sendingMail4);
  showBootLogo(sendingMail5);
  showBootLogo(sendingMail6);
  showBootLogo(sendingMail7);
  showBootLogo(sendingMail8);
  showBootLogo(sendingMail9);
  showBootLogo(sendingMail10);
  showBootLogo(sendingMail11);
  showBootLogo(sendingMail12);
  showBootLogo(sendingMail13);
  showBootLogo(sendingMail14);
  showBootLogo(sendingMail15);
  showBootLogo(sendingMail16);
  showBootLogo(sendingMail17);
  showBootLogo(sendingMail18);
  showBootLogo(sendingMail19);
  showBootLogo(sendingMail20);
  showBootLogo(sendingMail21);
  showBootLogo(sendingMail22);
  showBootLogo(sendingMail23);
  showBootLogo(sendingMail24);
  showBootLogo(sendingMail25);
  showBootLogo(sendingMail26);
  showBootLogo(sendingMail27);
  showBootLogo(sendingMail28);
}

void setup() {
  WiFi.mode(WIFI_STA); 
  Serial.begin(115200);
  pinMode(TRIGGER_PIN_START, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;) {

    }
  }
  delay(200);
  
  display.clearDisplay();
  display.display();
}

unsigned long previousMillisHe = 0;
unsigned long previousMillisAc = 0;

void loop() {
  char body[2048];
  temperatureFunction();

  if (isConnecting) {
    if (temperature > highThreshold) {
      display.clearDisplay();
      showBootLogo(temperatureImg);
      unsigned long currentMillisHe = millis();
      if (currentMillisHe - previousMillisHe >= 60000) {
        previousMillisHe = currentMillisHe;
        sprintf(body, "<h2>Температура підвищилась!</h2><p><b>Температура = </b> <font color='red'>%.2f</font> °C</p>", temperature);
        String result = sendEmail("Warning sensor", "ESP32Home", body, "andrii.smolskyi.tr.2019@lpnu.ua", true);
        display.clearDisplay();
        animationOfSendingEmail();
        display.clearDisplay();
      }
    } 
    else if (temperature < lowThreshold) {
      display.clearDisplay();
      showBootLogo(temperatureImg);
      unsigned long currentMillisAc = millis();
      if (currentMillisAc - previousMillisAc >= 60000) {
        previousMillisAc = currentMillisAc;
        sprintf(body, "<h2>Температура знизилась!</h2><br/><p><b>Температура = </b> <font color='blue'>%.2f</font> °C</p>", temperature);
        String result = sendEmail("Warning sensor", "ESP32Home", body, "andrii.smolskyi.tr.2019@lpnu.ua", true);
        display.clearDisplay();
        animationOfSendingEmail();
        display.clearDisplay();
        }
      } 
    else {
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.print(wm.getWiFiSSID());
      // температура
      display.setTextSize(1.5);
      display.setTextColor(WHITE);
      display.setCursor(0, 10);
      display.print("Temperature: ");
      display.setTextSize(2);
      display.setCursor(0, 25);
      display.print(temperature);
      display.print(" ");
      display.setTextSize(1);
      display.cp437(true);
      display.write(167);
      display.setTextSize(2);
      display.print("C");
      display.display();
    }
  }
  else {
    display.clearDisplay();
    showBootLogo(ITPE);
  }
  connectToWiFi();
}
