In this project, the data comimg from lis302 dl accelerometer sensor is transmitted from stm32f407 to computer via USB-TLL by using USART comminication protocol. Reading data from lis302 dl by using stm32f407 is explained [here](https://github.com/yasinsulhan/stm32f4-USART-receiving-transmitting-string-data). The SPI comminication protocol is used. So, in this project SPI and USART is used together. Also, you can check [SPI project tutorial](https://github.com/yasinsulhan/SPI-stm32f4-between-lis302dl). 

Notice that, **the comminication between stm32f407 and COM port in the PC is provided via USART. The comminication between stm32f407 and lis302 dl sensor is provided via SPI to do reading and writing process.** 

The designed application using MATLAB GUI shows the x, y and z axis variable in real time graph and text. Also, the available COM port can be chosen using popup menu upper left-hand corner. Then the comminication is started if the COM port is available.

