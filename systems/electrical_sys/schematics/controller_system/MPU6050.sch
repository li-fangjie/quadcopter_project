EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 12
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
L Sensor_Motion:MPU-6050 U1
U 1 1 5ECEDD32
P 5500 3700
AR Path="/5ECE82AB/5ECEDD32" Ref="U1"  Part="1" 
AR Path="/5ED66D06/5ECEDD32" Ref="U?"  Part="1" 
AR Path="/5ED670C4/5ECEDD32" Ref="U?"  Part="1" 
F 0 "U1" H 5100 3150 50  0000 C CNN
F 1 "MPU-6050" H 5200 3050 50  0000 C CNN
F 2 "Sensor_Motion:InvenSense_QFN-24_4x4mm_P0.5mm" H 5500 2900 50  0001 C CNN
F 3 "https://store.invensense.com/datasheets/invensense/MPU-6050_DataSheet_V3%204.pdf" H 5500 3550 50  0001 C CNN
	1    5500 3700
	1    0    0    -1  
$EndComp
NoConn ~ 6200 3600
NoConn ~ 6200 3700
Wire Wire Line
	5600 2900 5400 2900
Connection ~ 5600 2900
Wire Wire Line
	5600 2900 5600 2800
Text HLabel 5600 2800 1    50   Input ~ 0
H_3V3
$Comp
L Device:C C1
U 1 1 5ECEFDE5
P 5150 3000
AR Path="/5ECE82AB/5ECEFDE5" Ref="C1"  Part="1" 
AR Path="/5ED66D06/5ECEFDE5" Ref="C?"  Part="1" 
AR Path="/5ED670C4/5ECEFDE5" Ref="C?"  Part="1" 
F 0 "C1" V 4898 3000 50  0000 C CNN
F 1 "10 nF" V 4989 3000 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 5188 2850 50  0001 C CNN
F 3 "~" H 5150 3000 50  0001 C CNN
	1    5150 3000
	0    1    1    0   
$EndComp
Wire Wire Line
	5300 3000 5400 3000
Connection ~ 5400 3000
Wire Wire Line
	5400 3000 5400 2900
Wire Wire Line
	5000 3000 4900 3000
Text HLabel 4900 3000 0    50   UnSpc ~ 0
H_GND
Wire Wire Line
	5600 3000 5700 3000
Connection ~ 5600 3000
Wire Wire Line
	5600 3000 5600 2900
$Comp
L Device:C C2
U 1 1 5ECF20C1
P 5850 3000
AR Path="/5ECE82AB/5ECF20C1" Ref="C2"  Part="1" 
AR Path="/5ED66D06/5ECF20C1" Ref="C?"  Part="1" 
AR Path="/5ED670C4/5ECF20C1" Ref="C?"  Part="1" 
F 0 "C2" V 5598 3000 50  0000 C CNN
F 1 "0.1 uF" V 5689 3000 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 5888 2850 50  0001 C CNN
F 3 "~" H 5850 3000 50  0001 C CNN
	1    5850 3000
	0    1    1    0   
$EndComp
Wire Wire Line
	6000 3000 6100 3000
Text HLabel 6100 3000 2    50   UnSpc ~ 0
H_GND
Wire Wire Line
	4800 3600 4600 3600
Text HLabel 4600 3600 0    50   UnSpc ~ 0
H_GND
Wire Wire Line
	6200 3400 6400 3400
Text HLabel 6400 3400 2    50   UnSpc ~ 0
INT
Wire Wire Line
	4800 3500 4600 3500
Text HLabel 4600 3500 0    50   Input ~ 0
SCL
Wire Wire Line
	4800 3400 4600 3400
Text HLabel 4600 3400 0    50   UnSpc ~ 0
SDA
Wire Wire Line
	4800 4000 4600 4000
Text HLabel 4600 4000 0    50   UnSpc ~ 0
H_GND
Text HLabel 4600 3900 0    50   UnSpc ~ 0
H_GND
Wire Wire Line
	4600 3900 4800 3900
$Comp
L Device:C C3
U 1 1 5ECF57FD
P 6400 3850
AR Path="/5ECE82AB/5ECF57FD" Ref="C3"  Part="1" 
AR Path="/5ED66D06/5ECF57FD" Ref="C?"  Part="1" 
AR Path="/5ED670C4/5ECF57FD" Ref="C?"  Part="1" 
F 0 "C3" V 6300 3750 50  0000 C CNN
F 1 "2.2 nF" V 6300 4050 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 6438 3700 50  0001 C CNN
F 3 "~" H 6400 3850 50  0001 C CNN
	1    6400 3850
	0    1    1    0   
$EndComp
Text HLabel 6750 3900 2    50   UnSpc ~ 0
H_GND
$Comp
L Device:C C4
U 1 1 5ECF6E40
P 6400 4050
AR Path="/5ECE82AB/5ECF6E40" Ref="C4"  Part="1" 
AR Path="/5ED66D06/5ECF6E40" Ref="C?"  Part="1" 
AR Path="/5ED670C4/5ECF6E40" Ref="C?"  Part="1" 
F 0 "C4" V 6300 3950 50  0000 C CNN
F 1 "0.1 uF" V 6300 4250 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 6438 3900 50  0001 C CNN
F 3 "~" H 6400 4050 50  0001 C CNN
	1    6400 4050
	0    1    1    0   
$EndComp
Wire Wire Line
	6200 4000 6200 4050
Wire Wire Line
	6200 4050 6250 4050
Wire Wire Line
	6200 3900 6200 3850
Wire Wire Line
	6200 3850 6250 3850
Text HLabel 6750 4000 2    50   UnSpc ~ 0
H_GND
Wire Wire Line
	6550 3850 6600 3850
Wire Wire Line
	6600 3850 6600 3900
Wire Wire Line
	6600 3900 6750 3900
Wire Wire Line
	6550 4050 6600 4050
Wire Wire Line
	6600 4050 6600 4000
Wire Wire Line
	6600 4000 6750 4000
Wire Wire Line
	5500 4400 5500 4500
Text HLabel 5500 4500 3    50   UnSpc ~ 0
H_GND
Text HLabel 4150 1600 0    50   UnSpc ~ 0
SDA
Text HLabel 3650 1600 0    50   UnSpc ~ 0
SCL
Wire Wire Line
	4150 1600 4200 1600
Wire Wire Line
	4200 1600 4200 1500
$Comp
L Device:R R2
U 1 1 5ED86E00
P 4200 1350
F 0 "R2" H 4270 1396 50  0000 L CNN
F 1 "4.7 K" H 4270 1305 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 4130 1350 50  0001 C CNN
F 3 "~" H 4200 1350 50  0001 C CNN
	1    4200 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 1200 4200 1100
Text HLabel 4200 1100 1    50   UnSpc ~ 0
H_3V3
Wire Wire Line
	3650 1600 3700 1600
Wire Wire Line
	3700 1600 3700 1500
$Comp
L Device:R R1
U 1 1 5ED8A39A
P 3700 1350
F 0 "R1" H 3770 1396 50  0000 L CNN
F 1 "4.7 K" H 3770 1305 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 3630 1350 50  0001 C CNN
F 3 "~" H 3700 1350 50  0001 C CNN
	1    3700 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 1200 3700 1100
Text HLabel 3700 1100 1    50   UnSpc ~ 0
H_3V3
$EndSCHEMATC
