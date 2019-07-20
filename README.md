# SHARKSKY
Ein Quadrocopter Flightcontroller für eine Maturaarbeit.

## Aufbau
Hier wird der Aufbau des Repos beschrieben.

### 01_Flightcontroller
In diesem Ordner ist der Code, und der Schaltplan des Flightcontrollers zu finden. Im Ordner Code sind die Eclipse Projekte die mit der STM32CubeIDE erstellt worden sind. Im Ordner ST32F722ZE-Board sind noch die Versuche auf dem NucleoBoard zu finden.

### 02_RCTransmitter
In diesem Ordner ist der Code, der Schaltplan und der Konstruktion der Fernbedienung zu finden.
### 03_Datasheets
Hier sind die nötigen Datasheets gelagert um nicht immer im Internet dannach zu suchen.

### 04_Schriftliche Arbeit
Dort ist das LaTex file für den schriftlichen Teil und die Codefokumentation mit Doxygen zu finden.

## Benutzte Libraries
* [nRF24/RF24](https://github.com/nRF24/RF24) - Bibliothek für den Sender bzw. Empfänger
* [EminAtes/NRF24_STM32](https://github.com/EminAtes/NRF24_STM32) - Bibliothek für den Sender bzw. Empfänger übersetzt für kompitabilität mit STM32
* [jrowberg/i2cdevlib](https://github.com/jrowberg/i2cdevlib) - Bibliothek für den MPU6050

## Dank
* [fboris](https://github.com/fboris/STM32Cube_FW_F4/blob/master/Projects/STM32F4-Discovery/Examples/SPI/SPI_FullDuplex_ComDMA/Src/main.c) - Für das erkennen einer vollständigen SPI Transaktion.
