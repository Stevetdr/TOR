EESchema Schematic File Version 2
LIBS:Ln_IC
LIBS:Ln_R-C-L
LIBS:LnArduino_uno
LIBS:LnConnectors
LIBS:LnDevice
LIBS:LnDiodiTransistors
LIBS:LnMicroController
LIBS:LnRegulators
LIBS:LnSample
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:rs485-cache
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date "5 oct 2015"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L ARDUINO_NANO_IC U?
U 1 1 555F2B4C
P 7700 1100
F 0 "U?" H 8200 1250 60  0000 C CNN
F 1 "ARDUINO_NANO_IC" V 8300 -50 60  0000 C CNN
F 2 "~" H 8400 -1150 60  0000 C CNN
F 3 "~" H 8400 -1150 60  0000 C CNN
	1    7700 1100
	1    0    0    -1  
$EndComp
$Comp
L ARDUINO_RS485 U?
U 1 1 55AE4ABA
P 4200 2150
F 0 "U?" H 4650 1950 60  0000 C CNN
F 1 "ARDUINO_RS485" H 5050 1450 60  0000 C CNN
F 2 "~" H 4900 -100 60  0000 C CNN
F 3 "~" H 4900 -100 60  0000 C CNN
	1    4200 2150
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4200 4250 4650 4250
Wire Wire Line
	4650 1700 4650 4400
Wire Wire Line
	4750 2600 4200 2600
Wire Wire Line
	4650 4400 4200 4400
Connection ~ 4650 4250
$Comp
L GND #PWR?
U 1 1 55AE4EC8
P 3150 2700
F 0 "#PWR?" H 3150 2700 30  0001 C CNN
F 1 "GND" H 3150 2630 30  0001 C CNN
F 2 "" H 3150 2700 60  0000 C CNN
F 3 "" H 3150 2700 60  0000 C CNN
	1    3150 2700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 55AE4ED7
P 4250 3750
F 0 "#PWR?" H 4250 3750 30  0001 C CNN
F 1 "GND" H 4250 3680 30  0001 C CNN
F 2 "" H 4250 3750 60  0000 C CNN
F 3 "" H 4250 3750 60  0000 C CNN
	1    4250 3750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 55AE4EE6
P 4300 4700
F 0 "#PWR?" H 4300 4700 30  0001 C CNN
F 1 "GND" H 4300 4630 30  0001 C CNN
F 2 "" H 4300 4700 60  0000 C CNN
F 3 "" H 4300 4700 60  0000 C CNN
	1    4300 4700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 55AE4EF5
P 7600 1600
F 0 "#PWR?" H 7600 1600 30  0001 C CNN
F 1 "GND" H 7600 1530 30  0001 C CNN
F 2 "" H 7600 1600 60  0000 C CNN
F 3 "" H 7600 1600 60  0000 C CNN
	1    7600 1600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 55AE4F04
P 9050 1300
F 0 "#PWR?" H 9050 1300 30  0001 C CNN
F 1 "GND" H 9050 1230 30  0001 C CNN
F 2 "" H 9050 1300 60  0000 C CNN
F 3 "" H 9050 1300 60  0000 C CNN
	1    9050 1300
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR?
U 1 1 55AE4F13
P 3150 2050
F 0 "#PWR?" H 3150 2140 20  0001 C CNN
F 1 "+5V" H 3150 2140 30  0000 C CNN
F 2 "" H 3150 2050 60  0000 C CNN
F 3 "" H 3150 2050 60  0000 C CNN
	1    3150 2050
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR?
U 1 1 55AE4F22
P 4250 3050
F 0 "#PWR?" H 4250 3140 20  0001 C CNN
F 1 "+5V" H 4250 3140 30  0000 C CNN
F 2 "" H 4250 3050 60  0000 C CNN
F 3 "" H 4250 3050 60  0000 C CNN
	1    4250 3050
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR?
U 1 1 55AE4F31
P 4450 4050
F 0 "#PWR?" H 4450 4140 20  0001 C CNN
F 1 "+5V" H 4450 4140 30  0000 C CNN
F 2 "" H 4450 4050 60  0000 C CNN
F 3 "" H 4450 4050 60  0000 C CNN
	1    4450 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 3650 4250 3650
Wire Wire Line
	4250 3650 4250 3750
Wire Wire Line
	4200 4550 4300 4550
Wire Wire Line
	4300 4550 4300 4700
Wire Wire Line
	4200 4100 4450 4100
Wire Wire Line
	4450 4100 4450 4050
Wire Wire Line
	3300 2600 3150 2600
Wire Wire Line
	3150 2600 3150 2700
Wire Wire Line
	3150 2050 3150 2150
Wire Wire Line
	3150 2150 3300 2150
$Comp
L +5V #PWR?
U 1 1 55AE5015
P 9200 1450
F 0 "#PWR?" H 9200 1540 20  0001 C CNN
F 1 "+5V" H 9200 1540 30  0000 C CNN
F 2 "" H 9200 1450 60  0000 C CNN
F 3 "" H 9200 1450 60  0000 C CNN
	1    9200 1450
	1    0    0    -1  
$EndComp
Text Label 6750 2300 0    60   ~ 0
RS485-TX
Text Label 6750 2150 0    60   ~ 0
RS485-RX
Text Label 6750 2450 0    60   ~ 0
RS485-TX_ena
Text Notes 3250 1600 0    60   ~ 0
DI   (data in)         TX\nRO   (receive out)    RX\njumpered together:\n     DE (data enable) \n     RE (receive enable)\n
Wire Wire Line
	4200 3350 4250 3350
Wire Wire Line
	4250 3350 4250 3050
$Comp
L 433MHZ_RX U?
U 1 1 55AFB4D4
P 4200 4100
F 0 "U?" H 4775 4000 60  0000 C CNN
F 1 "433MHZ_RX" H 4800 3800 40  0000 C CNN
F 2 "~" H 4900 1850 60  0000 C CNN
F 3 "~" H 4900 1850 60  0000 C CNN
	1    4200 4100
	-1   0    0    -1  
$EndComp
$Comp
L 433MHZ_TX U?
U 1 1 55AFB4E3
P 4200 3400
F 0 "U?" H 4675 3450 60  0000 C CNN
F 1 "433MHZ_TX" H 4675 3225 40  0000 C CNN
F 2 "~" H 4900 1150 60  0000 C CNN
F 3 "~" H 4900 1150 60  0000 C CNN
	1    4200 3400
	-1   0    0    -1  
$EndComp
Wire Wire Line
	7700 2150 4200 2150
Wire Wire Line
	7700 2300 4750 2300
Wire Wire Line
	4750 2300 4750 2600
Wire Wire Line
	4200 2450 7700 2450
Wire Wire Line
	4200 2300 4400 2300
Connection ~ 4400 2450
Text Label 6750 1700 0    60   ~ 0
VirtualWire-RX
Text Label 6750 2000 0    60   ~ 0
VirtualWire-TX
Wire Wire Line
	7700 1700 4650 1700
Wire Wire Line
	7700 2000 4900 2000
Wire Wire Line
	4900 2000 4900 3500
Wire Wire Line
	4900 3500 4200 3500
$Comp
L RESISTOR R?
U 1 1 5604E745
P 7300 3550
F 0 "R?" H 7375 3650 60  0000 C CNN
F 1 "680" H 7550 3550 60  0000 C CNN
F 2 "~" H 7300 3550 60  0000 C CNN
F 3 "~" H 7300 3550 60  0000 C CNN
	1    7300 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	7700 3200 7300 3200
Wire Wire Line
	7300 3200 7300 3400
$Comp
L GND #PWR?
U 1 1 5604E797
P 7300 3900
F 0 "#PWR?" H 7300 3900 30  0001 C CNN
F 1 "GND" H 7300 3830 30  0001 C CNN
F 2 "" H 7300 3900 60  0000 C CNN
F 3 "" H 7300 3900 60  0000 C CNN
	1    7300 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	7300 3700 7300 3900
Text Notes 7400 3750 0    60   ~ 0
Solo per Arduino Master
Wire Wire Line
	7700 1550 7600 1550
Wire Wire Line
	7600 1550 7600 1600
Wire Wire Line
	4400 2300 4400 2450
Wire Wire Line
	8950 1250 9050 1250
Wire Wire Line
	9050 1250 9050 1300
Wire Wire Line
	8950 1550 9200 1550
Wire Wire Line
	9200 1550 9200 1450
$EndSCHEMATC
