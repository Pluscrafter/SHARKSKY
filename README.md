# SHARKSKY
Ein Quadrocopter Flightcontroller für eine Maturaarbeit.

## Aufbau
Hier wird der Aufbau des Repos beschrieben.

### 01_Flightcontroller
In diesem Ordner ist der Code, und der Schaltplan des Flightcontrollers zu finden. Im Ordner Code sind die Eclipse Projekte die mit der STM32CubeIDE erstellt worden sind.

### 02_RCTransmitter
In diesem Ordner ist der Code und die Konstruktion der Fernbedienung zu finden.
### 03_Datasheets
Hier sind die nötigen Datasheets gelagert um nicht immer im Internet dannach zu suchen.

### 04_Schriftliche Arbeit
Dort ist das LaTex file für den schriftlichen Teil.

### 05_Simulatoren
Dort sind die Programme um die Filter zu testen.

### 06_Videos und Bilder
Dort sind alle Bilder zur MA.

## Benutzte Libraries
* [nRF24/RF24](https://github.com/nRF24/RF24) - Bibliothek für den Sender bzw. Empfänger
* [EminAtes/NRF24_STM32](https://github.com/EminAtes/NRF24_STM32) - Bibliothek für den Sender bzw. Empfänger übersetzt für kompitabilität mit STM32
* [jrowberg/i2cdevlib](https://github.com/jrowberg/i2cdevlib) - Bibliothek für den MPU6050
* [firebull/STM32-LCD-HD44780-I2C](https://github.com/firebull/STM32-LCD-HD44780-I2C) - Bibliothek für Hitachi LCD HD44780 
