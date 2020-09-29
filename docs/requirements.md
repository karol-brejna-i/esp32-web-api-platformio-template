# Water valve controller

## Functional

### Manual opening and closing of a valve

### Scheduling

Should it be included in this component, or should it be a dedicated/external coordinator?
But with the external coordinator, we rely on network/radio connection (scheduling wouldn't work when offline).

## Non-functional/maintainability

### Offline mode
The app should work even if it is not possible for it to connect to a wifi network.
The app includes scheduled maintenance procedures (open/close valves periodically) that need to be able to work even without a connection.

### Event tracking

Persists events locally (so it can be read when there is physical access to the device).

Send the events "outside" -- consume (read/send/remove) locally stored events to store them in some external system (db/broker).

Send Heartbeat events

External heartbeat watcher -- when there is no heartbeat for a given period, the external component should create an alarm.


### Alarms

(Probably some external component...)

Information about the statatus of the device, important state changes should be sent to notify user.

	maybe it is worth listening to wifi events (connected/disconnected -- https://techtutorialsx.com/2019/08/11/esp32-arduino-getting-started-with-wifi-events/


### Auto-check
Verify internal logical state (valve state representation, are the GPIOs configured properly, etc.).

The valve could get stuck from long periods of idle. To prevent that the system should periodically open/close it.

	We have controll over dump valve (the valve that is responsible for getting rid of the water from the pipes)
	and main valve (the valve that is responsible for enabling the water flow),
	but the last element is the crane which (at least for now) is not automated.
	With the crane open we can have undesired "results". Maybe there is a need
	to confirm the state of the crane (closed) with the user berofre starting
	the procedure.

## Problems to solve

### How to make package the components
How to physically place the components
- power supply
- relays (105x50mm)
- microcontroller
- wifi antenna

### How to protect from moisture

### Weak WiFi range

### Leakage control?

### The valves don't report their state (closed/open) -- there is no way to check it


