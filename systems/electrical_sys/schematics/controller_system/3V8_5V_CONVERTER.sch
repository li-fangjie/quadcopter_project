EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 12 12
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
L Regulator_Switching:TPS61202DRC U3
U 1 1 5ED5D34A
P 5150 3700
F 0 "U3" H 5450 4050 50  0000 C CNN
F 1 "TPS61202DRC" H 5050 4050 50  0000 C CNN
F 2 "Package_SON:Texas_S-PVSON-N10_ThermalVias" H 5150 3250 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tps61200.pdf" H 5150 3700 50  0001 C CNN
	1    5150 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 3600 4700 3600
Wire Wire Line
	4700 3600 4700 3500
Wire Wire Line
	4700 3500 4750 3500
Wire Wire Line
	4750 3700 4700 3700
Wire Wire Line
	4700 3700 4700 3600
Connection ~ 4700 3600
Wire Wire Line
	4750 3800 4700 3800
Wire Wire Line
	4700 3800 4700 3700
Connection ~ 4700 3700
Connection ~ 4700 3500
Wire Wire Line
	4700 3500 4700 3250
Wire Wire Line
	5600 3250 5600 3500
Wire Wire Line
	5600 3500 5550 3500
$Comp
L Device:L L1
U 1 1 5ED5EDEC
P 5150 3250
F 0 "L1" V 5340 3250 50  0000 C CNN
F 1 "4.7 uH" V 5249 3250 50  0000 C CNN
F 2 "Inductor_SMD:L_Taiyo-Yuden_NR-30xx" H 5150 3250 50  0001 C CNN
F 3 "~" H 5150 3250 50  0001 C CNN
	1    5150 3250
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4700 3250 5000 3250
Wire Wire Line
	5300 3250 5600 3250
Wire Wire Line
	5550 3600 5600 3600
Wire Wire Line
	5550 3900 5600 3900
Wire Wire Line
	5600 3900 5600 3600
Connection ~ 5600 3600
Text HLabel 5700 4400 3    50   UnSpc ~ 0
H_GND
Wire Wire Line
	5050 4200 5050 4250
Wire Wire Line
	5050 4250 5150 4250
Wire Wire Line
	5250 4250 5250 4200
Wire Wire Line
	5150 4200 5150 4250
Connection ~ 5150 4250
Wire Wire Line
	5150 4250 5250 4250
Wire Wire Line
	4550 4250 5050 4250
Connection ~ 5050 4250
Wire Wire Line
	4550 4250 4550 4400
Connection ~ 4550 4250
Wire Wire Line
	6000 3600 6000 3850
$Comp
L Device:C C13
U 1 1 5ED68815
P 6000 4000
F 0 "C13" H 6100 4050 50  0000 L CNN
F 1 "22 uF" H 6100 3900 50  0000 L CNN
F 2 "Capacitor_SMD:C_1210_3225Metric" H 6038 3850 50  0001 C CNN
F 3 "~" H 6000 4000 50  0001 C CNN
	1    6000 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 4150 6000 4250
Wire Wire Line
	6000 4250 5700 4250
Connection ~ 5250 4250
$Comp
L Device:C C10
U 1 1 5ED6DE46
P 4550 4000
F 0 "C10" H 4350 4050 50  0000 L CNN
F 1 "10 uF" H 4300 3900 50  0000 L CNN
F 2 "Capacitor_SMD:C_1210_3225Metric" H 4588 3850 50  0001 C CNN
F 3 "~" H 4550 4000 50  0001 C CNN
	1    4550 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 3500 4550 3850
Wire Wire Line
	4550 4150 4550 4250
Wire Wire Line
	4550 3500 4050 3500
Connection ~ 4550 3500
Wire Wire Line
	5600 3600 6000 3600
Text HLabel 7050 3600 2    50   Output ~ 0
H_5V
Connection ~ 6000 3600
Wire Wire Line
	6000 3600 6350 3600
Wire Wire Line
	5550 3700 5700 3700
Wire Wire Line
	5700 3700 5700 3850
Wire Wire Line
	5700 4150 5700 4250
Connection ~ 5700 4250
Wire Wire Line
	5700 4250 5250 4250
Wire Wire Line
	6000 4250 6350 4250
Wire Wire Line
	6350 4250 6350 4150
Connection ~ 6000 4250
$Comp
L Device:C C14
U 1 1 5ED75AC8
P 6350 4000
F 0 "C14" H 6450 4050 50  0000 L CNN
F 1 "0.1 uF" H 6450 3950 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 6388 3850 50  0001 C CNN
F 3 "~" H 6350 4000 50  0001 C CNN
	1    6350 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6350 3850 6350 3600
Connection ~ 6350 3600
Wire Wire Line
	6350 3600 6800 3600
Wire Wire Line
	5700 4400 5700 4250
Text HLabel 4550 4400 3    50   UnSpc ~ 0
H_PGND
Wire Wire Line
	4550 3500 4700 3500
$Comp
L Device:C C12
U 1 1 5ED736CA
P 5700 4000
F 0 "C12" H 5800 4050 50  0000 L CNN
F 1 "0.1 uF" H 5750 3900 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 5738 3850 50  0001 C CNN
F 3 "~" H 5700 4000 50  0001 C CNN
	1    5700 4000
	1    0    0    -1  
$EndComp
Text HLabel 4050 3500 0    50   Input ~ 0
H_3V8
$Comp
L Device:LED D?
U 1 1 5EE851C8
P 6800 4150
AR Path="/5EB724B6/5EE851C8" Ref="D?"  Part="1" 
AR Path="/5ED5CA68/5EE851C8" Ref="D7"  Part="1" 
F 0 "D7" V 6839 4228 50  0000 L CNN
F 1 "LED_R" V 6748 4228 50  0000 L CNN
F 2 "LED_SMD:LED_0805_2012Metric" H 6800 4150 50  0001 C CNN
F 3 "~" H 6800 4150 50  0001 C CNN
	1    6800 4150
	0    1    -1   0   
$EndComp
$Comp
L Device:R R?
U 1 1 5EE851CE
P 6800 3850
AR Path="/5EB724B6/5EE851CE" Ref="R?"  Part="1" 
AR Path="/5ED5CA68/5EE851CE" Ref="R16"  Part="1" 
F 0 "R16" H 6870 3896 50  0000 L CNN
F 1 "330" H 6870 3805 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 6730 3850 50  0001 C CNN
F 3 "~" H 6800 3850 50  0001 C CNN
	1    6800 3850
	1    0    0    1   
$EndComp
Wire Wire Line
	6800 3600 6800 3700
Text HLabel 6800 4300 3    50   UnSpc ~ 0
H_GND
Connection ~ 6800 3600
Wire Wire Line
	6800 3600 7050 3600
$EndSCHEMATC
