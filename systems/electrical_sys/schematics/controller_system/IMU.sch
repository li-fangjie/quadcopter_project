EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 3
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
L Sensor_Motion:MPU-6050 U2
U 1 1 5ECEDD32
P 5500 3700
F 0 "U2" H 5100 3150 50  0000 C CNN
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
Text HLabel 5600 2800 1    50   UnSpc ~ 0
H_3.3V
$Comp
L Device:C C6
U 1 1 5ECEFDE5
P 5150 3000
F 0 "C6" V 4898 3000 50  0000 C CNN
F 1 "10 nF" V 4989 3000 50  0000 C CNN
F 2 "" H 5188 2850 50  0001 C CNN
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
L Device:C C7
U 1 1 5ECF20C1
P 5850 3000
F 0 "C7" V 5598 3000 50  0000 C CNN
F 1 "0.1 uF" V 5689 3000 50  0000 C CNN
F 2 "" H 5888 2850 50  0001 C CNN
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
H_INT
Wire Wire Line
	4800 3500 4600 3500
Text HLabel 4600 3500 0    50   UnSpc ~ 0
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
L Device:C C8
U 1 1 5ECF57FD
P 6400 3850
F 0 "C8" V 6300 3750 50  0000 C CNN
F 1 "2.2 nF" V 6300 4050 50  0000 C CNN
F 2 "" H 6438 3700 50  0001 C CNN
F 3 "~" H 6400 3850 50  0001 C CNN
	1    6400 3850
	0    1    1    0   
$EndComp
Text HLabel 6750 3900 2    50   UnSpc ~ 0
H_GND
$Comp
L Device:C C9
U 1 1 5ECF6E40
P 6400 4050
F 0 "C9" V 6300 3950 50  0000 C CNN
F 1 "0.1 uF" V 6300 4250 50  0000 C CNN
F 2 "" H 6438 3900 50  0001 C CNN
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
$EndSCHEMATC
