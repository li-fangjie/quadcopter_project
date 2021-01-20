EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 9 12
Title "MCU"
Date ""
Rev ""
Comp "FJ"
Comment1 "Adapted from Arduino Pro mini Configuration."
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	5450 2100 5450 2000
Wire Wire Line
	5550 2100 5550 2000
$Comp
L Device:C C5
U 1 1 5EB7E9DB
P 4100 2550
F 0 "C5" H 3900 2600 50  0000 L CNN
F 1 "0.1 uF" H 3750 2500 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4138 2400 50  0001 C CNN
F 3 "~" H 4100 2550 50  0001 C CNN
	1    4100 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 2800 4100 2700
Wire Wire Line
	4100 2400 4850 2400
Wire Wire Line
	6050 3700 6250 3700
Wire Wire Line
	6050 3800 6250 3800
Text HLabel 6250 3800 2    50   Output ~ 0
SCL
Wire Wire Line
	6050 4100 6250 4100
Wire Wire Line
	6050 4300 6250 4300
Text HLabel 6250 4300 2    50   BiDi ~ 0
D2_INT0
Wire Wire Line
	6050 2700 6250 2700
Text HLabel 6250 2700 2    50   Output ~ 0
MOSI
Wire Wire Line
	6050 2800 6250 2800
Text HLabel 6250 2800 2    50   Input ~ 0
MISO
Text HLabel 6250 2900 2    50   Output ~ 0
SCK
$Comp
L Device:R R4
U 1 1 5EBAE5FF
P 6900 3900
F 0 "R4" H 6970 3946 50  0000 L CNN
F 1 "10 K" H 6970 3855 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 6830 3900 50  0001 C CNN
F 3 "~" H 6900 3900 50  0001 C CNN
	1    6900 3900
	1    0    0    -1  
$EndComp
Text HLabel 5450 5300 3    50   UnSpc ~ 0
H_GND
Wire Wire Line
	6050 4600 6250 4600
Text HLabel 6250 4600 2    50   Output ~ 0
PWM_0
Text HLabel 6250 4700 2    50   Output ~ 0
PWM_1
Wire Wire Line
	6050 4700 6250 4700
Wire Wire Line
	6050 2500 6250 2500
Wire Wire Line
	6050 2600 6250 2600
Text HLabel 6250 2600 2    50   Output ~ 0
PWM_2
$Comp
L MCU_Microchip_ATmega:ATmega328P-AU U2
U 1 1 5EB7271E
P 5450 3600
F 0 "U2" H 5700 2050 50  0000 C CNN
F 1 "ATmega328P-AU" H 5900 2150 50  0000 C CNN
F 2 "Package_QFP:TQFP-32_7x7mm_P0.8mm" H 5450 3600 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega328_P%20AVR%20MCU%20with%20picoPower%20Technology%20Data%20Sheet%2040001984A.pdf" H 5450 3600 50  0001 C CNN
	1    5450 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	6050 2400 6250 2400
Text HLabel 6250 2400 2    50   Output ~ 0
RF_CSN
Wire Wire Line
	6050 4800 6250 4800
Text HLabel 6250 4800 2    50   Output ~ 0
RF_CE
Wire Wire Line
	4850 2600 4650 2600
NoConn ~ 4850 2700
Text HLabel 4650 2600 0    50   Input ~ 0
BAT_VAL
NoConn ~ 6050 4500
Wire Wire Line
	6050 4400 6250 4400
Text HLabel 6250 4400 2    50   Output ~ 0
PWM_4
NoConn ~ 6050 3600
NoConn ~ 6050 3500
$Comp
L Device:LED D5
U 1 1 5EC1CF02
P 8650 2850
F 0 "D5" V 8689 2928 50  0000 L CNN
F 1 "LED_G" V 8598 2928 50  0000 L CNN
F 2 "LED_SMD:LED_0805_2012Metric" H 8650 2850 50  0001 C CNN
F 3 "~" H 8650 2850 50  0001 C CNN
	1    8650 2850
	0    1    -1   0   
$EndComp
$Comp
L Device:R R8
U 1 1 5EC1CEFB
P 8650 2550
F 0 "R8" H 8720 2596 50  0000 L CNN
F 1 "330" H 8720 2505 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 8580 2550 50  0001 C CNN
F 3 "~" H 8650 2550 50  0001 C CNN
	1    8650 2550
	1    0    0    1   
$EndComp
Wire Wire Line
	6050 2900 6250 2900
Wire Wire Line
	6050 3900 6450 3900
Wire Wire Line
	6450 3900 6450 4100
Wire Wire Line
	6050 3300 6250 3300
Text Label 6250 3300 0    50   ~ 0
FCS_LED_0
Text Label 8650 2300 0    50   ~ 0
FCS_LED_0
Wire Wire Line
	8650 2300 8650 2400
Wire Wire Line
	6050 3400 6250 3400
Text Label 6250 3400 0    50   ~ 0
FCS_LED_1
Text HLabel 6250 3700 2    50   BiDi ~ 0
SDA
Wire Wire Line
	6050 3000 6700 3000
$Comp
L Device:Crystal Y1
U 1 1 5EB878EA
P 6900 3050
F 0 "Y1" V 6854 3181 50  0000 L CNN
F 1 "16 MHz" V 6945 3181 50  0000 L CNN
F 2 "Crystal:Crystal_SMD_ECS_CSM3X-2Pin_7.6x4.1mm" H 6900 3050 50  0001 C CNN
F 3 "~" H 6900 3050 50  0001 C CNN
	1    6900 3050
	0    1    1    0   
$EndComp
Wire Wire Line
	6700 3000 6700 2900
Wire Wire Line
	6700 2900 6900 2900
Wire Wire Line
	6700 3100 6700 3200
Wire Wire Line
	6050 3100 6700 3100
Wire Wire Line
	6700 3200 6900 3200
Wire Wire Line
	6900 2900 7050 2900
Connection ~ 6900 2900
Wire Wire Line
	6900 3200 7050 3200
Connection ~ 6900 3200
$Comp
L Device:C_Small C7
U 1 1 5EB8EB82
P 7150 2900
F 0 "C7" V 7379 2900 50  0000 C CNN
F 1 "22 pF" V 7288 2900 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 7150 2900 50  0001 C CNN
F 3 "~" H 7150 2900 50  0001 C CNN
	1    7150 2900
	0    1    -1   0   
$EndComp
$Comp
L Device:C_Small C8
U 1 1 5EB905E9
P 7150 3200
F 0 "C8" V 6950 3200 50  0000 C CNN
F 1 "22 pF" V 7050 3200 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 7150 3200 50  0001 C CNN
F 3 "~" H 7150 3200 50  0001 C CNN
	1    7150 3200
	0    1    -1   0   
$EndComp
Wire Wire Line
	7250 2900 7350 2900
Wire Wire Line
	7250 3200 7350 3200
Wire Wire Line
	7350 2900 7350 3050
Wire Wire Line
	7350 3050 7550 3050
Connection ~ 7350 3050
Wire Wire Line
	7350 3050 7350 3200
$Comp
L Device:LED D6
U 1 1 5EC820DB
P 8650 4250
F 0 "D6" V 8689 4328 50  0000 L CNN
F 1 "LED_R" V 8598 4328 50  0000 L CNN
F 2 "LED_SMD:LED_0805_2012Metric" H 8650 4250 50  0001 C CNN
F 3 "~" H 8650 4250 50  0001 C CNN
	1    8650 4250
	0    1    -1   0   
$EndComp
$Comp
L Device:R R9
U 1 1 5EC820E1
P 8650 3950
F 0 "R9" H 8720 3996 50  0000 L CNN
F 1 "330" H 8720 3905 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 8580 3950 50  0001 C CNN
F 3 "~" H 8650 3950 50  0001 C CNN
	1    8650 3950
	1    0    0    1   
$EndComp
Text Label 8650 3700 0    50   ~ 0
FCS_LED_1
Wire Wire Line
	8650 3700 8650 3800
Text Notes 7100 6400 0    50   ~ 0
Note: PWM_4 is only highlighted for future reference, it will not be used for this version of the system.
Wire Wire Line
	5450 5100 5450 5300
Text HLabel 6900 4800 3    50   UnSpc ~ 0
H_GND
Wire Wire Line
	5550 2000 5450 2000
Text HLabel 5450 1900 1    50   Input ~ 0
H_5V
Wire Wire Line
	5450 2000 5450 1900
Connection ~ 5450 2000
Text HLabel 6900 3700 1    50   Input ~ 0
H_5V
Text HLabel 8650 4400 3    50   UnSpc ~ 0
H_GND
Text HLabel 8650 3000 3    50   UnSpc ~ 0
H_GND
Text Notes 7100 6150 0    50   ~ 0
NOTE: There is no need for DTR pin w/ High Pass Filter.\nThis is because the currently used CH340 does not support it.
$Comp
L Device:C C9
U 1 1 5ECDE23B
P 7200 4100
F 0 "C9" V 7250 4000 50  0000 C CNN
F 1 "0.1 uF" V 7050 4100 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 7238 3950 50  0001 C CNN
F 3 "~" H 7200 4100 50  0001 C CNN
	1    7200 4100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7350 4100 7500 4100
Text HLabel 7550 3050 2    50   UnSpc ~ 0
H_GND
Text HLabel 4100 2800 3    50   UnSpc ~ 0
H_GND
$Comp
L Device:C C6
U 1 1 5ED94464
P 6100 1450
AR Path="/5EB724B6/5ED94464" Ref="C6"  Part="1" 
AR Path="/5ED94464" Ref="C?"  Part="1" 
F 0 "C6" H 6215 1496 50  0000 L CNN
F 1 "0.1 uF" H 6215 1405 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 6138 1300 50  0001 C CNN
F 3 "~" H 6100 1450 50  0001 C CNN
	1    6100 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 1300 6100 1250
Wire Wire Line
	6100 1600 6100 1650
Text HLabel 6100 1250 1    50   Input ~ 0
H_5V
Text HLabel 6100 1650 3    50   UnSpc ~ 0
H_GND
Text HLabel 6250 2500 2    50   Output ~ 0
PWM_3
Text Label 6250 4100 0    50   ~ 0
RXI
Text Label 6250 4200 0    50   ~ 0
TXO
Text Label 6550 3900 0    50   ~ 0
RESET
Wire Wire Line
	6900 4100 7050 4100
Wire Wire Line
	6450 4100 6900 4100
Connection ~ 6900 4100
Wire Wire Line
	6900 4100 6900 4050
Wire Wire Line
	6450 3900 6550 3900
Connection ~ 6450 3900
Text Label 7500 4100 0    50   ~ 0
DTR
Wire Wire Line
	6900 3750 6900 3700
Wire Wire Line
	2950 4050 3050 4050
Wire Wire Line
	2950 4250 3050 4250
Wire Wire Line
	2950 4350 3050 4350
Wire Wire Line
	2950 4450 3050 4450
Wire Wire Line
	2950 3500 3050 3500
Wire Wire Line
	2950 3600 3050 3600
Wire Wire Line
	2950 3700 3050 3700
Text HLabel 3050 3500 2    50   Output ~ 0
MOSI
Text HLabel 3050 3600 2    50   Input ~ 0
MISO
Text HLabel 3050 3700 2    50   Output ~ 0
SCK
Text Label 3050 4050 0    50   ~ 0
DTR
Text Label 3050 4150 0    50   ~ 0
TXO
Text Label 3050 4250 0    50   ~ 0
RXI
Text HLabel 3050 4350 2    50   Input ~ 0
H_5V
Text HLabel 3050 4450 2    50   UnSpc ~ 0
H_GND
$Comp
L FCS_LIBS:FTDI_Conn_01x05 J8
U 1 1 5EE65ED8
P 2750 4250
F 0 "J8" H 2792 3835 50  0000 C CNN
F 1 "FTDI_Conn_01x05" H 2792 3926 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 2750 4250 50  0001 C CNN
F 3 "~" H 2750 4250 50  0001 C CNN
	1    2750 4250
	-1   0    0    1   
$EndComp
$Comp
L FCS_LIBS:SPI_Conn_01x04 J7
U 1 1 5EE67091
P 2750 3600
F 0 "J7" H 2817 3185 50  0000 C CNN
F 1 "SPI_Conn_01x04" H 2817 3276 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 2750 3600 50  0001 C CNN
F 3 "~" H 2750 3600 50  0001 C CNN
	1    2750 3600
	-1   0    0    1   
$EndComp
Wire Wire Line
	2950 3400 3050 3400
Text Label 3050 3400 0    50   ~ 0
RESET
Wire Wire Line
	6050 4200 6250 4200
Wire Wire Line
	2950 4150 3050 4150
Wire Wire Line
	6900 4750 6900 4800
Wire Wire Line
	6900 4100 6900 4150
$Comp
L Switch:SW_DIP_x01 SW1
U 1 1 5EBB4D4E
P 6900 4450
F 0 "SW1" V 6854 4580 50  0000 L CNN
F 1 "MCU_RESET_SW" V 6945 4580 50  0000 L CNN
F 2 "Button_Switch_SMD:SW_SPST_B3S-1000" H 6900 4450 50  0001 C CNN
F 3 "~" H 6900 4450 50  0001 C CNN
	1    6900 4450
	0    1    1    0   
$EndComp
$EndSCHEMATC
