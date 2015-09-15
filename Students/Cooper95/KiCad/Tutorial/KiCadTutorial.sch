EESchema Schematic File Version 2
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
LIBS:KiCadTutorial-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L BS170 Q2
U 1 1 55F19AEA
P 4250 3350
F 0 "Q2" H 4450 3425 50  0000 L CNN
F 1 "BS170" H 4450 3350 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-92_Inline_Narrow_Oval" H 4450 3275 50  0000 L CIN
F 3 "" H 4250 3350 50  0000 L CNN
	1    4250 3350
	1    0    0    -1  
$EndComp
$Comp
L BS170 Q3
U 1 1 55F19E72
P 5200 3350
F 0 "Q3" H 5400 3425 50  0000 L CNN
F 1 "BS170" H 5400 3350 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-92_Inline_Narrow_Oval" H 5400 3275 50  0000 L CIN
F 3 "" H 5200 3350 50  0000 L CNN
	1    5200 3350
	1    0    0    -1  
$EndComp
$Comp
L BS170 Q5
U 1 1 55F19ED0
P 6850 3350
F 0 "Q5" H 7050 3425 50  0000 L CNN
F 1 "BS170" H 7050 3350 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-92_Inline_Narrow_Oval" H 7050 3275 50  0000 L CIN
F 3 "" H 6850 3350 50  0000 L CNN
	1    6850 3350
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X11 P1
U 1 1 55F1A145
P 5400 4850
F 0 "P1" H 5400 5450 50  0000 C CNN
F 1 "CONN_01X11" V 5500 4850 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x11" H 5400 4850 60  0001 C CNN
F 3 "" H 5400 4850 60  0000 C CNN
	1    5400 4850
	0    1    1    0   
$EndComp
$Comp
L BS170 Q1
U 1 1 55F1A305
P 3550 3350
F 0 "Q1" H 3750 3425 50  0000 L CNN
F 1 "BS170" H 3750 3350 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-92_Inline_Narrow_Oval" H 3750 3275 50  0000 L CIN
F 3 "" H 3550 3350 50  0000 L CNN
	1    3550 3350
	1    0    0    -1  
$EndComp
$Comp
L BS170 Q4
U 1 1 55F1A37E
P 5900 3350
F 0 "Q4" H 6100 3425 50  0000 L CNN
F 1 "BS170" H 6100 3350 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-92_Inline_Narrow_Oval" H 6100 3275 50  0000 L CIN
F 3 "" H 5900 3350 50  0000 L CNN
	1    5900 3350
	1    0    0    -1  
$EndComp
$Comp
L BS170 Q6
U 1 1 55F1A3D6
P 7550 3350
F 0 "Q6" H 7750 3425 50  0000 L CNN
F 1 "BS170" H 7750 3350 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-92_Inline_Narrow_Oval" H 7750 3275 50  0000 L CIN
F 3 "" H 7550 3350 50  0000 L CNN
	1    7550 3350
	1    0    0    -1  
$EndComp
Text Notes 3300 3100 0    60   ~ 0
Aux
Text Notes 4900 3100 0    60   ~ 0
RasPi
Text Notes 6600 3100 0    60   ~ 0
Radio
Wire Wire Line
	4900 4300 4900 4650
Wire Wire Line
	3350 4300 4900 4300
Wire Wire Line
	3350 4300 3350 3400
Wire Wire Line
	4050 3400 4050 4300
Connection ~ 4050 4300
Wire Wire Line
	5200 4000 5200 4650
Wire Wire Line
	5000 4000 5700 4000
Wire Wire Line
	5000 4000 5000 3400
Wire Wire Line
	5700 4000 5700 3400
Connection ~ 5200 4000
Wire Wire Line
	5000 4650 5000 4200
Wire Wire Line
	5000 4200 3650 4200
Wire Wire Line
	3650 4200 3650 3550
Wire Wire Line
	5100 4650 5100 4100
Wire Wire Line
	5100 4100 4350 4100
Wire Wire Line
	4350 4100 4350 3550
Wire Wire Line
	5300 4650 5300 3550
Wire Wire Line
	5400 4650 5400 4100
Wire Wire Line
	5400 4100 6000 4100
Wire Wire Line
	6000 4100 6000 3550
Wire Wire Line
	6950 3550 6950 4300
Wire Wire Line
	7650 3550 7650 4400
Wire Notes Line
	4750 3000 4750 3700
Wire Notes Line
	4750 3700 3250 3700
Wire Notes Line
	3250 3700 3250 3000
Wire Notes Line
	3250 3000 4750 3000
Wire Notes Line
	4850 3000 4850 3700
Wire Notes Line
	4850 3700 6450 3700
Wire Notes Line
	6450 3700 6450 3000
Wire Notes Line
	6450 3000 4850 3000
Wire Notes Line
	6550 3000 6550 3700
Wire Notes Line
	6550 3700 8050 3700
Wire Notes Line
	8050 3700 8050 3000
Wire Notes Line
	8050 3000 6550 3000
Wire Wire Line
	4350 3150 4350 2800
Wire Wire Line
	4350 2800 8250 2800
Wire Wire Line
	3650 3150 3650 2900
Wire Wire Line
	3650 2900 8150 2900
Wire Wire Line
	8150 2900 8150 4500
Wire Wire Line
	8150 4500 5800 4500
Wire Wire Line
	5800 4500 5800 4650
Wire Wire Line
	5900 4600 5900 4650
Wire Wire Line
	8250 2800 8250 4600
Wire Wire Line
	8250 4600 5900 4600
Wire Wire Line
	5300 3150 5300 2900
Connection ~ 5300 2900
Wire Wire Line
	6000 3150 6000 2800
Connection ~ 6000 2800
Wire Wire Line
	6950 3150 6950 2900
Connection ~ 6950 2900
Wire Wire Line
	7650 3150 7650 2800
Connection ~ 7650 2800
Text Notes 5800 5100 0    60   ~ 0
L
Text Notes 5900 5100 0    60   ~ 0
R
Text Notes 7350 7500 0    60   ~ 0
Amplifier Source Mux
Wire Wire Line
	5500 4650 5500 4200
Wire Wire Line
	5500 4200 7350 4200
Wire Wire Line
	7350 4200 7350 3400
Wire Wire Line
	6650 3400 6650 4200
Connection ~ 6650 4200
Wire Wire Line
	5600 4650 5600 4300
Wire Wire Line
	5600 4300 6950 4300
Wire Wire Line
	5700 4650 5700 4400
Wire Wire Line
	5700 4400 7650 4400
Text Notes 5000 5100 0    60   ~ 0
L
Text Notes 5300 5100 0    60   ~ 0
L
Text Notes 5400 5100 0    60   ~ 0
R
Text Notes 5100 5100 0    60   ~ 0
R
Text Notes 5600 5100 0    60   ~ 0
L
Text Notes 5700 5100 0    60   ~ 0
R
Text Notes 4900 5100 0    60   ~ 0
S
Text Notes 5200 5100 0    60   ~ 0
S
Text Notes 5500 5100 0    60   ~ 0
S
Wire Notes Line
	4850 5000 4850 5150
Wire Notes Line
	4850 5150 6000 5150
Wire Notes Line
	5200 5150 5200 5000
Wire Notes Line
	4850 5000 6000 5000
Wire Notes Line
	5500 5000 5500 5150
Wire Notes Line
	5800 5000 5800 5150
Wire Notes Line
	6000 5000 6000 5150
Text Notes 4900 5250 0    60   ~ 0
Aux
Text Notes 5200 5250 0    60   ~ 0
RasPi
Text Notes 5500 5250 0    60   ~ 0
Radio
Text Notes 5800 5350 0    60   ~ 0
Speaker\nOut
$EndSCHEMATC
