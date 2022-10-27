# Dynamixel XL-320 @ STM32
A Robotis XL320 Servo library for STM32

<img src="https://i.imgur.com/6Vg47Ny.png" width=20%>


## Author

Ricardo SILVESTRE at ENSEA
- [@ricardossilvestre](https://www.github.com/ricardossilvestre)

<img src="https://i.imgur.com/V16zoK4.png" width=40%>


# How to use

## Installation

 - Copy and paste the "Inc" files into the "Inc" folder and the "Src" files into the "Src" folders of your STM32CubeMX project

## Hardware

 - Connect the XL320 to a 7.4V source and use the "DATA" pin directly in the UART Half-Duplex port

## Software

 - Set a UART port for half-duplex at 1000000 baudrate (default for the XL320) and make sure to activate the DMA for the USART*_RX

 - Inside the project make sure the DMA is initialized before the other components by placing it in the /* USER CODE BEGIN SysInit */ part of the main.c file

 - Set the UART GPIO parameters as "Alternate Function Push Pull" and turn ON the "GPIO Pull-up"

 - Call the functions described in the dynamixel_xl320.h header using the UART pointer. Ex: to turn on the red LED light
 
 ```c 
 void XL320_set_led_ON(&huart6, 1);
 ``` 
## Acknowledgements
### Inspired on the following repos

 - [@ROBOTIS-GIT](https://github.com/ROBOTIS-GIT/dynamixel2arduino)
 - [@hackerspace-adelaide](https://github.com/hackerspace-adelaide/XL320)
