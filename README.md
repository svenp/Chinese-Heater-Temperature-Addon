# Chinese-Heater-Temperature-Addon
Addon for chinese diesel heaters to enable better (or different) temperature control

#Modified by SvenP

#######################

Function changed to implement frost protection. So that a garage or garden shed can be kept frost-free.
When the temperature falls below 2 degrees Celsius, the switch-on signal is sent. If the temperature is exceeded, it is switched off again.

Wiring:

D2 -> Bus blue

5V -> + 5V red

GND -> GND black

D8 -> GND optional inhibit switch


########################

This addon is written with the arduino nano as the target but should work with most 5v atmega chips.

it connects directly to the data line between the heater and existing controller using D2 for data and a gnd connection to the heaters negative
(in practice you can also pick up power from the same point on the loom as the existing controller is powered by 5v from the heater)

Once connected the addon will remain dormant until a start command is sent from the standard controller
once it has seen the start command the module monitors temperature (desired temperature and actual temperature) data from the controller to the heater.
If the actual temperature rises 2 degrees above the desired temperature and the heater has reached its normal running state the module will send a stop command to the heater, the command sent triggers the normal shutdown on the heater as if you had pressed the off button on the controller so all of the normal cooldown procedures are followed.

If the heater has been shut down by the module and the temperature falls 1 degree below the desired temperature the module will issue a start command to the heater, again this start command triggers the same startup as you pressing the On button so all normal startup procedures are followed by the heater.

The reason for 2 degrees above being chosen as the cutoff is to allow the heater to try and maintain temperature on its own by going to it's low setting (which it does at 1 degree above) and only shut it off if the amount of heat being produced on low is still raising the temperature
