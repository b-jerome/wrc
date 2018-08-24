# wrc
WRC Arduino Web Reef Controller (Porject closed!)

This code allows you to control HIGH POWER LEDS via a web interface. So you can control the LEDs via Computer, iPhone, iPad, etc ... 

You can also monitor temperatures sensors and record information via mysql. The temperatures are tracked on a graph of the web interface. You will need a web hosting provider to use the web interface outside of your home. 

You can use a local wamp environement if you don't want to use the web interface outside your home. The web interface use PHP MYSQL and AJAX (jquery, jqueryui, Highcharts JS). Mysql part is necessary to monitor and save datas temperatures and lightning / clouds effects. 

Main Features:

    6 LED Channels.
    2 Temperatures Sensors with alarm settings
    2 servos motor with angle & speed settings
    Random clouds effect
    Random lightning effect.
    Settings stored in EEPROM

	HARDWARE:

    Arduino Mega 2560 R3 / R2
    I2C RTC Mini DS1307
    Ethernet Shield W5100
    2 x sensor ds18b20
    22x10W POWER LED
    Power Driver shield from sparkfun ( 6 channels ) http://www.sparkfun.com/products/10618
    POWER PC SUPPLY to drive the above shield and LEDs

See it in action :

http://www.youtube.com/watch?v=2cTAq23LqoM&hd=1
