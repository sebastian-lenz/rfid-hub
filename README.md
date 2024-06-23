# RFID Hub

The RFID Hub is a C++ project designed to utilize the Raspberry Pi Pico 
microcontroller to interface with up to four RFID readers. Each RFID reader 
acts as a unique keyboard input, allowing  users to simulate keystrokes by
presenting RFID tags to the readers. 


## Compiling

This project uses Platform.IO, please visit https://platformio.org/ to find
out more. After installing Platform.IO this project can be build using the 
following command:

```
$ pio run
```

If a Raspberry Pico is connected the project can be build and uploaded to
the board using the following command:

```
$ pio run -t upload -e pico
```