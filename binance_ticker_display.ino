// Display
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Connection
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <HttpClient.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_MOSI   13
#define OLED_CLK   14
#define OLED_DC    5
#define OLED_CS    15
#define OLED_RESET 4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

String ssid = "";
String password = "";

String label = "USTC-USDT";
String ticker = "USTCUSDT";

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC);
  WifiConnect();
  ClearBackground();
  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(12,10);
  display.println(label);
}

void loop()
{
  String rateValue = GetBinanceTickerRate();
  display.setCursor(5,35);
  display.println(rateValue);
  display.display();
  delay(100);
}

String GetBinanceTickerRate() 
{
  WiFiClientSecure client;
  client.setInsecure(); // do not validate server SSL certificate
  HttpClient http = HttpClient(client, "api.binance.com", 443);
  http.get("/api/v1/ticker/price?symbol=" + ticker);

  int statusCode = http.responseStatusCode();
  String response = http.responseBody();
  if (statusCode != 200)
  {
    Serial.printf("Error occurred during request. Response status code: %d\n", statusCode);
    return " --ERROR--";
  }
  else 
  {
    int labelStart = response.indexOf("price\":");
    int rateValueStart = response.indexOf("\"", labelStart);
    int rateValueEnd = response.indexOf("\"}", rateValueStart);
    return response.substring(rateValueStart + 3, rateValueEnd);
  }
}

void WifiConnect()
{
  // Set WiFi to station mode and disconnect from an AP if it was Previously
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
}

void ClearBackground()
{
  int i;
  int j;
  display.clearDisplay();
  for(i=0;i<SCREEN_WIDTH;i++)
    for(j=0;j<SCREEN_HEIGHT;j++)
      display.drawPixel(i, j, SSD1306_WHITE);
  display.display();
}
