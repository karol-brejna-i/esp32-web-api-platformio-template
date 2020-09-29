


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
  -DDEBUG
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

## To be implemented
* Add responsive web ui
* Add websocket/server side events communication -- inform the clients about state changes
* Add minimal time teshold -- turn the remote on only when the button is pressed (held) for at leaset n seconds