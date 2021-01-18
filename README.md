


## Configuration

In order for the project to build and run properly, some configuration information needs to be proviced.

### WiFi setup
The code connect to an existing WiFi network. Network name and password can be delivered in a few ways:


Environment variable:
```
export ADDITIONAL_BUILD_FLAGS='-DWIFI_SSID=\"My ssid name\" -DWIFI_PASS=\"My password\"'
```

(For my WSL setup I needed to add the line to .bashrc, because the IDE didn't recognize the variables.)

Build flag in platform.ini, for example:
```
[env]
build_flags =
  -DWIFI_SSID="\"My ssid name\""
  -DWIFI_PASS="\"My password\""
  -DDEBUG_ON
```

Explicit definition in the code (main.cpp):

Remove:
```
#ifndef WIFI_SSID
#error wifi not defined
#endif
```
Add:
```
#ifndef WIFI_SSID
#define WIFI_SSID "My ssid name"
#define WIFI_PASS "My password"
#endif
```

## Communication
In my environment, I use port `COM10` (that uses CP210x driver for my ESP devices). The equivalent port for WSL is `/dev/ttyS10`.
(Please, take a look at https://www.instructables.com/id/ESP32-Development-on-Windows-Subsystem-for-Linux/ for more details, if needed.)


Set up the port for firmware uploads and serial monitor according to your needs in `platform.ini`, for example:
```
[env]
; upload_port = COM10
; monitor_port = COM10
upload_port = /dev/ttyS10
monitor_port = /dev/ttyS10
monitor_speed = 115200
```


# Roadmap

## Implemented features
* Connect to given WiFi
* Prototype web ui to press/release remote control button
* Automatically turn off the controler after given timeout (not drain bateries in case of a bug)

## TODO
* remove unused (valve) code
* in readme you mention com ports used in wsl. is wsl2 even able to see com ports?
* src\web\handlers_status.cpp:21:28: error: 'class ESP8266WiFiClass' has no member named 'getHostname'; fix .hostname() vs .getHostname for esp32 or esp8266; String hostname = WiFi.getHostname();
* clean up the OTA code: https://github.com/lbernstone/asyncUpdate/blob/master/AsyncUpdate.ino
