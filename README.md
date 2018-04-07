# Irrigation control system
Small private project for controlling garden irrigation with OH2 and MQTT.
***

## Description

The water level is measured with maintenance-free ultrasonic sensors. Float switches are not recommended due to deposits and dirt in the service water. 
In the following the level is published with MQTT under the topic "waterLevel".
If OH2 was configured correctly, rules can now be applied to the items.
In my case I have a large groundwater tank which serves as a storage for the garden irrigation system. Irrigation should run for as long as a minimum level is maintained. If this is too low, the groundwater pump should refill the tank in parallel. If the water level falls below the minimum level, the irrigation system is deactivated and the reservoir is filled up.

The pumps can be controlled via topics "groundWater" and "gardenIrrigation". 



## Buzzwords

* MQTT
* Mosquitto
* OpenHAB 2
* Ultrasonic



## Prerequisites

* NodeMCU V3
* [Mosquitto or any other MQTT Broker](https://mosquitto.org/download/)
* [HC-SR04 Ultrasonic sensor](http://www.micropik.com/PDF/HCSR04.pdf)
* Running OpenHAB2

    * [MQTT Binding](https://docs.openhab.org/addons/bindings/mqtt1/readme.html)
    * [MQTT Actions](https://docs.openhab.org/addons/actions/mqtt/readme.html)



## OpenHAB 2 Configuration

### MQTT.cfg

```
openhabianpi.url=tcp://localhost:1883
```



### .items

```
Number waterLevel "Waterlevel [%.2f m]" {mqtt="<[openhabianpi:/italy/lestans/garden/irrigation/sensors/waterLevel:state:default]"}

Switch groundwater_pump "Groundwater pump" {mqtt=">[openhabianpi:/italy/lestans/garden/irrigation/pumps/groundWater:command:ON:1], >[openhabianpi:/italy/lestans/garden/irrigation/pumps/groundWater:command:OFF:0]"}  
Switch irrigation_pump "Garden irrigation pump" {mqtt=">[openhabianpi:/italy/lestans/garden/irrigation/pumps/gardenIrrigation:command:ON:1], >[openhabianpi:/italy/lestans/garden/irrigation/pumps/gardenIrrigation:command:OFF:0]"}
```


### .sitemap

```
Text item = "waterLevel"
Switch item = "irrigation_pump"
Switch item = "groundwater_pump"
```


### .rules
```
Rules
```
