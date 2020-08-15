/************************ Adafruit IO Config *******************************/

// visit io.adafruit.com if you need to create an account
//https://io.adafruit.com/
// or if you need your Adafruit IO key.
#define IO_USERNAME "username"
#define IO_KEY "yourKey"

/******************************* WIFI **************************************/

//This code is designed for:
//   - Feather M0 WiFi with WINC1500 -> https://www.adafruit.com/products/3010

//enter your WiFi name and password
#define WIFI_SSID "WiFiName"
#define WIFI_PASS "yourPassword"


#define USE_WINC1500
#include <AdafruitIO_WiFi.h>

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
