# air-machine
This program is written in C language, Using two 16bit pic for interrupt handling, screen control,...and hashing for Motor Control L298.

Main components used:
 + 2 pic 16bit PIC16F877A
 + Motor Controll L298
 + Temperature and Humidity sensor DHT11
 + LCD 16x4 LM041L
 
 ![alt text](https://github.com/hmcofetel/air-machine/blob/master/screenshot/screenshot1.png?raw=true)
 
 ![alt text](https://github.com/hmcofetel/air-machine/blob/master/screenshot/sreenshot2.png?raw=true)
 
 This circuit works in both **Automatic** and **Non-automatic** modes:
  + In automatic mode:
    The temperature and humidity control motor will gradually increase the capacity when the environment is not satisfactory and gradually decrease the capacity when the environment exceeds the requirements.
![alt text](https://github.com/hmcofetel/air-machine/blob/master/screenshot/demo.gif?raw=true)

  + In non-automatic mode: 
    The temperature and humidity control motor will run at a fixed capacity that the user needs.
![alt text](https://github.com/hmcofetel/air-machine/blob/master/screenshot/demo2.gif?raw=true)    
    
