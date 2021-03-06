EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 11 12
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 3750 3300 2    50   Output ~ 0
H_3V8
Text HLabel 3750 3400 2    50   UnSpc ~ 0
H_PGND
Wire Wire Line
	3750 3300 3650 3300
Wire Wire Line
	3750 3400 3550 3400
$Comp
L Connector_Generic:Conn_01x02 J?
U 1 1 5EDA3A56
P 3350 3400
AR Path="/5ED5CA68/5EDA3A56" Ref="J?"  Part="1" 
AR Path="/5EDA1D08/5EDA3A56" Ref="J6"  Part="1" 
F 0 "J6" H 3350 3650 50  0000 C CNN
F 1 "Lipo_Bat_Con" H 3350 3550 50  0000 C CNN
F 2 "Connector_JST:JST_PH_B2B-PH-K_1x02_P2.00mm_Vertical" H 3350 3400 50  0001 C CNN
F 3 "~" H 3350 3400 50  0001 C CNN
	1    3350 3400
	-1   0    0    1   
$EndComp
Connection ~ 3650 3300
Wire Wire Line
	3650 3300 3550 3300
Wire Wire Line
	3650 3300 3650 3200
$Comp
L Device:R R15
U 1 1 5EDA51A0
P 3650 3050
F 0 "R15" H 3720 3096 50  0000 L CNN
F 1 "100 K" H 3720 3005 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 3580 3050 50  0001 C CNN
F 3 "~" H 3650 3050 50  0001 C CNN
	1    3650 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 2900 3650 2850
$Comp
L Device:R R14
U 1 1 5EDA5765
P 3650 2650
F 0 "R14" H 3720 2696 50  0000 L CNN
F 1 "100 K" H 3720 2605 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 3580 2650 50  0001 C CNN
F 3 "~" H 3650 2650 50  0001 C CNN
	1    3650 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 2850 3900 2850
Connection ~ 3650 2850
Wire Wire Line
	3650 2850 3650 2800
Wire Wire Line
	3650 2500 3650 2400
Text HLabel 3650 2400 1    50   UnSpc ~ 0
H_PGND
Text HLabel 3900 2850 2    50   Output ~ 0
BAT_VAL
$EndSCHEMATC
