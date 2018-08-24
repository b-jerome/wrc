/*
+---------------------------------------------------------------------------
 |   > Copyright (c) 2013
 |   > Bourgeois Jérôme
 +---------------------------------------------------------------------------
 |   > Web Reef Controller
 |   > Module written by Bourgeois Jérôme
 |   > mail.bjerome@gmail.com
 ----------------------------------------------
 |   > Date started: Aug 2, 2012
 |   > Release updated: Mar 29, 2013 - 0:34 AM
 +---------------------------------------------
 |   > Module Version Number: 0.0.2.9 rc5
 +--------------------------------------------------------------------------
 */

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <EEPROM.h>
#include <DS1307.h>
#include <Wire.h>
#include <OneWire.h>
#include <Servo.h> 
#include "lang.h"

//RTC
DS1307 rtc;
OneWire ds(8);

//servos
Servo servo1, servo2;
// MAC and IP address.
byte mac[] = {  
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// holds received packet's originating IP
byte remoteIp[4];
// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
char  ReplyBuffer[] = "acknowledged";

/*** START CONFIG ***/

byte ip[] = { 
  192,168,1,10 };
char serverName[] = "www.mywebsite.com";
String host       = "www.mywebsite.com";
String subdir     = "/led"; //If your project is hosted in sub directory (example : www.mysite.com/led )

// local port to listen on
unsigned int remotePort = 8812;

//Assign LEDS 
const int led1 = 2; //Pin channel 1
const int led2 = 3; //Pin channel 2
const int led3 = 4; //Pin channel 3
const int led4 = 5; //Pin channel 4
const int led5 = 6; //Pin channel 5
const int led6 = 7; //Pin channel 6
const int ctrlAtx = 22; // Pin ATX power supply
const int Servo1Pin = 39; // Pin Servo 1
const int Servo2Pin = 40; // Pin Servo 2
const int AlarmPin  = 41; // Pin Alarm Pin

//Number of temperatures captor (0 - 1 or 2)
int tpcc = 2;
//temp celsius = 1 or fahrenheit = 2
int tp = 1;
//Servo motor
int svo = 1; //(number of servos 0, 1 or 2)

//channels gradation length in sec
long tempsMax = 900; //send temps to mysql every X secondes
long c1Max = 15;     //grad channel +/- 1% every 15 sec
long c2Max = 15; 
long c3Max = 15;
long c4Max = 15;
long c5Max = 15; 
long c6Max = 15; 
long svoMax = 100; //millisec (minimum 10)

/*** END CONFIG ***/

//variables
long tempsNow,c1Now,c2Now,c3Now,c4Now,c5Now,c6Now = 1;
long nxcs,nxce,nxss,nxse,servo1A,servo2A,svot,sect,tmpa = 0;
long note = 2093;
long length = 500;
int acp1,acp2,alarmIsOn,alarmIsOn2,dodata,dostorm,docloud,randLed,randTime,randWa,randH,val,func,to,randCs,c1,c2,c3,c4,c5,c6,startTime,endTime,curMin,rest,mau,dsend,atx,stormp,g1,g2,g3,g4,g5,g6,servo1V,servo2V,pos,pos2,servo1S,servo2S,sens1,sens2,itp,disableS,disableC,toserver,tpons=0;
int initialise = 1;
unsigned long s1,s2,s3,s4,s5,s6,e1,e2,e3,e4,e5,e6;
float cur1,cur2,cur3,cur4,cur5,cur6,tp1,tp2,celsius,fahrenheit,mt1,mt2,mnt1,mnt2;

//Text
String DateTxt,dow,mth,t1s,t2s,SetDate,mt1s,mnt1s,mt2s,mnt2s;
//Where to store in eeprom
int eled1=1,eled2=2,eled3=3,eled4=4,eled5=5,eled6=6,eled1s=7,eled1e=11,eled2s=15,eled2e=19,eled3s=23,eled3e=27,eled4s=31,eled4e=35,eled5s=39,eled5e=43,eled6s=47,eled6e=51,st=55,cl=56,ma=57,servo1Saddr=58,servo2Saddr=59,servo1Aaddr=60,servo2Aaddr=61,servo1Vaddr=62,servo2Vaddr=63,disableSaddr=64,disableCaddr=65,aison=66,aison2=67,am1=68,amn1=70,am2=72,amn2=74;
//used to send UPD packet
char toS[128], buf[16], tt1[4], tt2[4], mtt1[4], mntt1[4], mtt2[4], mntt2[4];

//Horloge
uint8_t sec, min, hour, date, day, month;
uint16_t year;
long curSec;

EthernetUDP Udp;
EthernetClient client;

void setup() {

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // start the Ethernet connection:
  Serial.println("Start ethernet");

  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    while(true);
  }
  else{
    Serial.print("DHCP OK IP FOUND :");
    Serial.println(Ethernet.localIP());
  }

  // give the Ethernet shield a second to initialize:
  delay(2000);
  Serial.println("Start UDP...");
  Udp.stop();
  Udp.begin(remotePort);

  Serial.println("Loading eeprom values");
  // Read LED channels value
  c1 = EEPROM.read(eled1);
  c2 = EEPROM.read(eled2);
  c3 = EEPROM.read(eled3);
  c4 = EEPROM.read(eled4);
  c5 = EEPROM.read(eled5);
  c6 = EEPROM.read(eled6);
  //LED START TIME
  s1 = EEPROM_readlong(eled1s);
  s2 = EEPROM_readlong(eled2s);
  s3 = EEPROM_readlong(eled3s);
  s4 = EEPROM_readlong(eled4s);
  s5 = EEPROM_readlong(eled5s);
  s6 = EEPROM_readlong(eled6s);
  //LED END TIME
  e1 = EEPROM_readlong(eled1e);
  e2 = EEPROM_readlong(eled2e);
  e3 = EEPROM_readlong(eled3e);
  e4 = EEPROM_readlong(eled4e);
  e5 = EEPROM_readlong(eled5e);
  e6 = EEPROM_readlong(eled6e);
  //Servo
  servo1S   = EEPROM.read(servo1Saddr);
  servo2S   = EEPROM.read(servo2Saddr);
  servo1A   = EEPROM.read(servo1Aaddr);
  servo2A   = EEPROM.read(servo2Aaddr);
  servo1V   = EEPROM.read(servo1Vaddr);
  servo2V   = EEPROM.read(servo2Vaddr);
  //OPTS
  //dostorm = EEPROM.read(st);
  //docloud = EEPROM.read(cl);
  //mau     = EEPROM.read(ma);
  //Alarm
  mt1   = EEPROM.read(am1);
  mnt1  = EEPROM.read(amn1);
  mt2   = EEPROM.read(am2);
  mnt2  = EEPROM.read(amn2);
  
  mt1s = dtostrf(mt1,0,1,mtt1);
  mnt1s = dtostrf(mnt1,0,1,mntt1);
  mt2s = dtostrf(mt2,0,1,mtt2);
  mnt2s = dtostrf(mnt2,0,1,mntt2);
  
  alarmIsOn  = EEPROM.read(aison);
  alarmIsOn2  = EEPROM.read(aison2);
  //Storm & Cloud 
  disableS = EEPROM.read(disableSaddr);
  disableC = EEPROM.read(disableCaddr);
  //Atx power supply
  pinMode(ctrlAtx, OUTPUT);
  digitalWrite(ctrlAtx, HIGH);

  //Init Servo motors
  servo1A = servo1A <= 180 ? servo1A : 10;
  servo2A = servo2A <= 180 ? servo2A : 10;
  servo1V = servo1V > 0 && servo1V <= 10 ? servo1V : 1;
  servo2V = servo2V > 0 && servo2V <= 10 ? servo2V : 1;

  if( svo > 0 )
  {
    servo1.attach(Servo1Pin);
  }

  if ( svo > 1 )
  {
    servo2.attach(Servo2Pin);
  }

  Serial.println("Setup Real-time clock.");
  //Définir la date et l'heure (décommenté pour la premiere utilisation)
  //rtc.set(0, 56, 14, 6, 26, 1, 2013); //sec, min, heur, jour de la semaine, jour, mois, annee
  rtc.start();
  Serial.println("Start controller...");
}

//Funtionc Timer
void DoTimers()
{  
  //temperatures
  if ( tpcc > 0 )
  {
    tempsNow = tempsNow >= tempsMax ? 1 : tempsNow+1; 

    if ( tempsNow == tempsMax )
    {
      toserver = 1;
      //temps();
      dsend = 3;
      dodata = 1;
    }
  }
}

//long integer EEPROM
void EEPROM_writelong(int address, unsigned long value) 
{
  //tronquer la partie supérieure et écrit dans l'EEPROM la partie inférieure
  EEPROM_writeint(address+2, word(value));
  //déplacement
  value = value >> 16;
  //tronque et ecrit
  EEPROM_writeint(address, word(value));
}

// long in EEPROM
unsigned long EEPROM_readlong(int address) 
{
  // function lecture double mot
  unsigned long dword = EEPROM_readint(address);
  //deplacement lecture mot
  dword = dword << 16;
  // lecture simple ou double
  dword = dword | EEPROM_readint(address+2);
  return dword;
}

//write in EEPROM
void EEPROM_writeint(int address, int value) 
{
  EEPROM.write(address,highByte(value));
  EEPROM.write(address+1 ,lowByte(value));
}

//read eeprom
unsigned int EEPROM_readint(int address) 
{
  unsigned int word = word(EEPROM.read(address), EEPROM.read(address+1));
  return word;
} 

// Function date heure
void SetRtc()
{ 
  rtc.set(sec, min, hour, date, day, month, year);
}

// Function date heure
void DrawRtc()
{ 
  rtc.get(&sec, &min, &hour, &date, &day, &month, &year);

  //Jour
  dow = getDayOfWeek(date);
  //mois
  mth = getMonth(month);
  DateTxt = dow + " " + int(day) + " " + mth + " " + year;
  //Heure 
  sprintf(buf, "%02i:%02i:%02i", hour, min, sec);

  DateTxt = DateTxt + " - " + buf;

  //curent seconds past today
  curSec = hour*3600L + min*60L + sec;

  //Serial.println(DateTxt);
}

//Jour de la semaine
String getDayOfWeek(uint8_t d)
{
  String dy[8]={
    "Erreur 0x01", lang_d1,lang_d2,lang_d3,lang_d4,lang_d5,lang_d6,lang_d7                                                           };
  return dy[d];
}    

//mois
String getMonth(uint8_t d)
{
  String mth[13]={
    "Erreur 0x02", lang_m1,lang_m2,lang_m3,lang_m4,lang_m5,lang_m6,lang_m7,lang_m8,lang_m9,lang_m10,lang_m11,lang_m12                                                           };
  return mth[d];
}  

//Servomoteurs
void DrawServosMode()
{
  if( servo1S == 1 )
  {  
    if( sens1 == 0 && pos + servo1V < servo1A )
    {
      pos += servo1V;
      sens1 = 0;
    }
    else
    {
      pos -= servo1V;
      sens1 = pos > 0 ? 1 : 0;
    }
    servo1.write(pos);
  }  

  if( servo2S == 1 )
  {  
    if( sens2 == 0 && pos2 + servo2V < servo2A )
    {
      pos2 += servo2V;
      sens2 = 0;
    }
    else
    {
      pos2 -= servo2V;
      sens2 = pos2 > 0 ? 1 : 0;
    }
    servo2.write(pos2);
  }  
}

//light
void DrawLed()
{   
  //cloud simulation if channel 4 & 5 ON
  if ( nxcs <= curSec && nxce > curSec && cur4 > 0 && cur5 > 0 && docloud == 0 )
  {
    docloud = 3;
    cloud(); 
  }
  else if ( nxce < curSec && docloud == 3 )
  {
    docloud = 2; 
    initialise = 1;
  }

  //storm simulation
  if ( nxss <= curSec && nxse > curSec && dostorm == 0 && stormp == 1 )
  {
    Serial.println("Storm started whoo");

    dostorm = 3;
    storm(); 
  }
  else if ( nxse < curSec && dostorm == 3 )
  {
    Serial.println("Let's get sunshine ! or not :o");
    dostorm = 2; 
    initialise = 1;
  }

  //Led channel 1
  if ( ( s1 * 60 < curSec && ( s1 + e1 ) * 60 > curSec ) && cur1 < c1 * 2.55 )
  {  
    if ( initialise == 1 )
    {  
      if ( atx == 0 )
      {
        digitalWrite(ctrlAtx, LOW);
        atx = 1;
      }
      for(cur1; cur1 < c1 * 2.55; cur1 +=1)
      {
        analogWrite(led1, cur1);
        delay(5);
      }
    }
    else
    {
      c1Now = c1Now >= c1Max ? 1 : c1Now+1;

      if ( c1Now == c1Max )
      { 
        cur1 = cur1 + 1;
        analogWrite(led1, cur1);
      }
    }
    g1 = 1;
  }
  else if ( cur1 > 0 )
  {
    c1Now = c1Now >= c1Max ? 1 : c1Now+1;

    if ( c1Now == c1Max )
    { 
      cur1 = cur1 - 1;
      analogWrite(led1, cur1);
    }
    g1 = 0;
  }
  //Led channel 2
  if ( ( s2 * 60 < curSec && ( s2 + e2 ) * 60 > curSec ) && cur2 < c2 * 2.55 )
  {  
    if ( initialise == 1 )
    {  
      if ( atx == 0 )
      {
        digitalWrite(ctrlAtx, LOW);
        atx = 1;
      }
      for(cur2; cur2 < c2 * 2.55; cur2 +=1)
      {
        analogWrite(led2, cur2);
        delay(5);
      }
    }
    else
    {
      c2Now = c2Now >= c2Max ? 1 : c2Now+1;

      if ( c2Now == c2Max )
      { 
        cur2 = cur2 + 1;
        analogWrite(led2, cur2);
      }
    }
    g2 = 1;
  }
  else if ( cur2 > 0 )
  {
    c2Now = c2Now >= c2Max ? 1 : c2Now+1;

    if ( c2Now == c2Max )
    { 
      cur2 = cur2 - 1;
      analogWrite(led2, cur2);
    }
    g2 = 0;
  }
  //Led channel 3
  if ( ( s3 * 60 < curSec && ( s3 + e3 ) * 60 > curSec ) && cur3 < c3 * 2.55 )
  {
    if ( initialise == 1 )
    {
      if ( atx == 0 )
      {
        digitalWrite(ctrlAtx, LOW);
        atx = 1;
      }  
      for(cur3; cur3 < c3 * 2.55; cur3 +=1)
      {
        analogWrite(led3, cur3);
        delay(5);
      }
    }
    else
    {
      c3Now = c3Now >= c3Max ? 1 : c3Now+1;

      if ( c3Now == c3Max )
      { 
        cur3 = cur3 + 1;
        analogWrite(led3, cur3);
      }
    }
    g3 = 1;
  }
  else if ( cur3 > 0 )
  {
    c3Now = c3Now >= c3Max ? 1 : c3Now+1;

    if ( c3Now == c3Max )
    { 
      cur3 = cur3 - 1;
      analogWrite(led3, cur3);
    }
    g3 = 0;
  }
  //Led channel 4
  if ( ( s4 * 60 < curSec && ( s4 + e4 ) * 60 > curSec ) && cur4 < c4 * 2.55 )
  {  
    if ( initialise == 1 )
    {
      if ( atx == 0 )
      {
        digitalWrite(ctrlAtx, LOW);
        atx = 1;
      }  
      for(cur4; cur4 < c4 * 2.55; cur4 +=1)
      {
        analogWrite(led4, cur4);
        delay(5);
      }
    }
    else
    {
      c4Now = c4Now >= c4Max ? 1 : c4Now+1;

      if ( c4Now == c4Max )
      { 
        cur4 = cur4 + 1;
        analogWrite(led4, cur4);
      }
    }
    g4 = 1;
  }
  else if ( cur4 > 0 )
  {
    c4Now = c4Now >= c4Max ? 1 : c4Now+1;

    if ( c4Now == c4Max )
    { 
      cur4 = cur4 - 1;
      analogWrite(led4, cur4);
    }
    g4 = 0;
  }
  //Led channel 5
  if ( ( s5 * 60 < curSec && ( s5 + e5 ) * 60 > curSec ) && cur5 < c5 * 2.55 )
  {  
    if ( initialise == 1 )
    { 
      if ( atx == 0 )
      {
        digitalWrite(ctrlAtx, LOW);
        atx = 1;
      } 
      for(cur5; cur5 < c5 * 2.55; cur5 +=1)
      {
        analogWrite(led5, cur5);
        delay(5);
      }
    }
    else
    {
      c5Now = c5Now >= c5Max ? 1 : c5Now+1;

      if ( c5Now == c5Max )
      { 
        cur5 = cur5 + 1;
        analogWrite(led5, cur5);
      }
    }
    g5 = 1;
  }
  else if ( cur5 > 0 )
  {
    c5Now = c5Now >= c5Max ? 1 : c5Now+1;

    if ( c5Now == c5Max )
    { 
      cur5 = cur5 - 1;
      analogWrite(led5, cur5);
    }
    g5 = 0;
  }
  //Led channel 6
  if ( ( s6 * 60 < curSec && ( s6 + e6 ) * 60 > curSec ) && cur6 < c6 * 2.55 )
  {  
    if ( initialise == 1 )
    {  
      if ( atx == 0 )
      {
        digitalWrite(ctrlAtx, LOW);
        atx = 1;
      }
      for(cur6; cur6 < c6 * 2.55; cur6 +=1)
      {
        analogWrite(led6, cur6);
        delay(5);
      }
    }
    else
    {
      c6Now = c6Now >= c6Max ? 1 : c6Now+1;

      if ( c6Now == c6Max )
      { 
        cur6 = cur6 + 1;
        analogWrite(led6, cur6);
      }
    }
    g6 = 1;
  }
  else if ( cur6 > 0 )
  {
    c6Now = c6Now >= c6Max ? 1 : c6Now+1;

    if ( c6Now == c6Max )
    { 
      cur6 = cur6 - 1;
      analogWrite(led6, cur6);
    }
    g6 = 0;
  }
  //Serial.println("Led Checked");
  initialise = 0;
}

//save data
void saveData() {

  String data;

  if ( dsend == 1 )
  {
    //format data before sending
    data = "do=data&data="+String()+c1+","+c2+","+c3+","+c4+","+c5+","+c6+","+s1+","+s2+","+s3+","+s4+","+s5+","+s6+","+e1+","+e2+","+e3+","+e4+","+e5+","+e6+","+dostorm+","+docloud+","+String(DateTxt)+","+mau+","+servo1S+","+servo2S+","+servo1A+","+servo2A+","+servo1V+","+servo2V+","+t1s+","+t2s+","+disableC+","+disableS+","+alarmIsOn+","+alarmIsOn2+","+mt1s+","+mnt1s+","+mt2s+","+mnt2s;
  }
  else if ( dsend == 2 )
  {
    data = "do=data&func=2&cloud_start="+String()+nxcs+"&cloud_end="+nxce+"&storm_start="+nxss+"&storm_end="+nxse;
  }
  else if ( dsend == 3 )
  {
    toserver = 0;
    itp = 0;
    t1s = dtostrf(tp1,0,1,tt1);
    t2s = dtostrf(tp2,0,1,tt2);
    //init done
    tpons = 1;
    data = "do=data&func=3&temp1="+t1s+"&temp2="+t2s;
  }  

  Serial.print("Sending datas:");
  Serial.println(data);

  // if you get a connection, report back via serial:  
  if (client.connect(serverName, 80)) {
    Serial.println("connected");
    client.println( "POST "+subdir+"/functions.php HTTP/1.1" );
    client.println( "Host: "+host );
    client.println( "Content-Type: application/x-www-form-urlencoded" );
    client.println( "Connection: close" );
    client.print( "Content-Length: " );
    client.println( data.length() );
    client.println();
    client.print( data );
    client.println();
  } 
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  } 

  dsend = 0;
}

//restore Led to initial state
void restoreLed() {
  if ( mau > 0 )
  {
    Serial.println("Restore Started");

    if ( mau == 2 )
    {
      //Fadeout led if needeed when stopping manuel
      if ( g1 == 0 )
      {
        for(cur1; cur1 > 0; cur1 -=1)
        {
          Serial.println(cur1);
          analogWrite(led1, cur1);
          delay(5);
        }
      }
      if ( g2 == 0 )
      {
        for(cur2; cur2 > 0; cur2 -=1)
        {
          analogWrite(led2, cur2);
          delay(5);
        }
      }
      if ( g3 == 0 )
      {
        for(cur3; cur3 > 0; cur3 -=1)
        {
          analogWrite(led3, cur3);
          delay(5);
        }
      }
      if ( g4 == 0 )
      {
        for(cur4; cur4 > 0; cur4 -=1)
        {
          analogWrite(led4, cur4);
          delay(5);
        }
      }
      if ( g5 == 0 )
      {
        for(cur5; cur5 > 0; cur5 -=1)
        {
          analogWrite(led5, cur5);
          delay(5);
        }
      }
      if ( g6 == 0 )
      {
        for(cur6; cur6 > 0; cur6 -=1)
        {
          analogWrite(led6, cur6);
          delay(5);
        }
      }
    }
    //manuel mode
    else
    {
      if ( cur1 < c1 * 2.55 )
      {
        for(cur1; cur1 < c1 * 2.55; cur1 +=1)
        {
          analogWrite(led1, cur1);
          delay(5);
        }
      }
      else if ( cur1 >= c1 * 2.55 )
      {    
        for(cur1; cur1 >= c1 * 2.55; cur1 -=1)
        {
          analogWrite(led1, cur1);
          delay(5);
        }
      }
      if ( cur2 < c2 * 2.55 )
      {
        for(cur2; cur2 < c2 * 2.55; cur2 +=1)
        {
          analogWrite(led2, cur2);
          delay(5);
        }
      }
      else if ( cur2 >= c2 * 2.55  )
      {    
        for(cur2; cur2 >= c2 * 2.55; cur2 -=1)
        {
          analogWrite(led2, cur2);
          delay(5);
        }
      }
      if ( cur3 < c3 )
      {
        for(cur3; cur3 < c3 * 2.55; cur3 +=1)
        {
          analogWrite(led3, cur3);
          delay(5);
        }
      }
      else if ( cur3 >= c3  )
      {    
        for(cur3; cur3 >= c3 * 2.55; cur3 -=1)
        {
          analogWrite(led3, cur3);
          delay(5);
        }
      }
      if ( cur4 < c4 * 2.55  )
      {
        for(cur4; cur4 < c4 * 2.55; cur4 +=1)
        {
          analogWrite(led4, cur4);
          delay(5);
        }
      }
      else if ( cur4 >= c4 * 2.55 )
      {    
        for(cur4; cur4 >= c4 * 2.55; cur4 -=1)
        {
          analogWrite(led4, cur4);
          delay(5);
        }
      }
      if ( cur5 < c5 * 2.55  )
      {
        for(cur5; cur5 < c5 * 2.55; cur5 +=1)
        {
          analogWrite(led5, cur5);
          delay(5);
        }
      }
      else if ( cur5 >= c5 * 2.55  )
      {    
        for(cur5; cur5 >= c5 * 2.55; cur5 -=1)
        {
          analogWrite(led5, cur5);
          delay(5);
        }
      }
      if ( cur6 < c6 * 2.55 )
      {
        for(cur6; cur6 < c6 * 2.55; cur6 +=1)
        {
          analogWrite(led6, cur6);
          delay(5);
        }
      }
      else if ( cur6 >= c6 * 2.55  )
      {    
        for(cur6; cur6 >= c6 * 2.55; cur6 -=1)
        {
          analogWrite(led6, cur6);
          delay(5);
        }
      }
    }

    cur1 = cur1 > 0 ? cur1 : 0;
    cur2 = cur2 > 0 ? cur2 : 0;
    cur3 = cur3 > 0 ? cur3 : 0;
    cur4 = cur4 > 0 ? cur4 : 0;
    cur5 = cur5 > 0 ? cur5 : 0;
    cur6 = cur6 > 0 ? cur6 : 0;

    Serial.println("Restore Complete");
    rest = 1;

  }
}

//Cloud channels 4 - 5
void cloud() {

  if ( ( mau == 1 && docloud == 1 ) || docloud == 3 )
  {
    randWa   = random(1000);
    randTime = random(50);

    //Fade channel 1,2,3 to 30% = 77
    for(cur1; cur1 > 77; cur1 -=1)
    {
      analogWrite(led1, cur1);
      analogWrite(led2, cur1);
      analogWrite(led3, cur1);
      cur2 = cur3 = cur1;
      delay(randTime);
    }

    randH    = random(1,255);
    for(cur4; cur4 > randH; cur4 -=1)
    {
      analogWrite(led4, cur4);
      delay(randTime);
    }

    for(cur4; cur4 < c4; cur4 +=1)
    {
      analogWrite(led4, cur4);
      delay(randTime);
    }

    delay(randWa);

    randH    = random(1,255);
    for(cur5; cur5 > randH; cur5 -=1)
    {
      analogWrite(led5, cur5);
      delay(randTime);
    }

    for(cur5; cur5 < c5; cur5 +=1)
    {
      analogWrite(led5, cur5);
      delay(randTime);
    }

    delay(randWa);
  }
}

//Storm channels  1 - 2 - 3
void storm() {

  if ( ( mau == 1 && dostorm == 1 ) || dostorm == 3 )
  {
    cur1 = cur2 = cur3 = 0;

    //Random wait time
    randTime = random(100);

    //Fade channel 4 - 5 
    for(cur4; cur4 > 8; cur4 -=1)
    {
      analogWrite(led4, cur4);
      analogWrite(led5, cur4);
      cur5 = cur4;
      delay(randTime);
    }

    //Fade channel 6 
    for(cur6; cur6 > 10; cur6 -=1)
    {
      analogWrite(led6, cur6);
      delay(randTime);
    }

    //Random wait time
    randTime = random(150);
    randWa = random(150);
    //Random ON / OFF
    randLed = random(2);
    //Random led storm (PIN 2 - 3 - 4)
    randCs = random(2,5);

    //Serial.println(randCs);  

    if ( randLed > 0 ) 
    { 
      analogWrite(randCs, 255);
      delay(randTime);
      analogWrite(randCs, 0);
      delay(randTime);
      analogWrite(randCs, 128);
      delay(randTime);
      analogWrite(randCs, 0);
    } 

    //Random ON / OFF second storm
    randLed = random(2);
    //Random led storm (PIN 2 - 3 - 4)
    randCs = random(2,5);

    if ( randLed > 0 ) 
    { 
      analogWrite(randCs, 255);
      delay(randTime);
      analogWrite(randCs, 0);
      delay(randTime);
      analogWrite(randCs, 128);
      delay(randTime);
      analogWrite(randCs, 0);
    } 

    //if cloud is on
    if ( docloud == 1 && randLed > 0 ) 
    { 
      delay(randWa);

      //Random led storm (PIN 2 - 3 - 4)
      randCs = random(2,5);

      analogWrite(randCs, 255);
      delay(randTime);
      analogWrite(randCs, 0);
      delay(randTime);
      analogWrite(randCs, 128);
      delay(randTime);
      analogWrite(randCs, 0);
    } 

    delay(randWa);
  }
}

void DrawController() {

  // read packet
  int packetSize = Udp.parsePacket();

  if(packetSize)
  {
    // subtract the 8 byte header
    packetSize = packetSize - 8;
    memset(packetBuffer, 0, UDP_TX_PACKET_MAX_SIZE);

    // read packet into packetBufffer and get senders IP addr and port
    Udp.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);

    //parse packetBuffer
    char *record = packetBuffer;
    char *l, *i, *v, *f, *ss, *mm, *hh, *w, *d, *m, *y; 

    //  First strtok iteration find function to switch
    l = strtok_r(record,",",&i);
    to = atoi(l);

    //  strtok iteration 2
    v = strtok_r(NULL,",",&i);
    val = atoi(v);

    // strtok iteration 3
    f = strtok_r(NULL,",",&i);
    func = atoi(f);  

    ss = strtok_r(NULL,",",&i);
    sec = atoi(ss);

    mm = strtok_r(NULL,",",&i);
    min = atoi(mm);

    hh = strtok_r(NULL,",",&i);
    hour = atoi(hh);

    w = strtok_r(NULL,",",&i);
    date = atoi(w);

    m = strtok_r(NULL,",",&i);
    month = atoi(m);

    d = strtok_r(NULL,",",&i);
    day = atoi(d);

    Serial.print("function:");
    Serial.println(func);
    Serial.print("to:");
    Serial.println(to);
    Serial.print("Value:");
    Serial.println(val);

    //Switch function
    switch (func) { 
    //LED CHANNEL VALUES
    case 1: 
      if ( to == 1 )
      {
        EEPROM.write(eled1,val);
        c1 = val;
      }
      else if ( to == 2 )
      {
        EEPROM.write(eled2,val);
        c2 = val;        
      }
      else if ( to == 3 )
      {
        EEPROM.write(eled3,val);
        c3 = val;
      }
      else if ( to == 4 )
      {     
        EEPROM.write(eled4,val);
        c4 = val;    
      }
      else if ( to == 5 )
      {
        EEPROM.write(eled5,val);
        c5 = val;
      }
      else
      {
        EEPROM.write(eled6,val);
        c6 = val;
      }
      restoreLed();
      break;
    //LED STRAT TIME
    case 2:
      if ( to == 1 )
      {
        EEPROM_writelong(eled1s,val);
        s1 = val;
      }
      else if ( to == 2 )
      {
        EEPROM_writelong(eled2s,val);
        s2 = val;
      }
      else if ( to == 3 )
      {
        EEPROM_writelong(eled3s,val);
        s3 = val;
      }
      else if ( to == 4 )
      {
        EEPROM_writelong(eled4s,val);
        s4 = val;
      }
      else if ( to == 5 )
      {
        EEPROM_writelong(eled5s,val);
        s5 = val;
      }
      else if ( to == 6 )
      {
        EEPROM_writelong(eled6s,val);
        s6 = val;
      }
      break;   
    //LED END TIME
    case 3:
      if ( to == 1 )
      {
        EEPROM_writelong(eled1e,val);
        e1 = val;
      }
      else if ( to == 2 )
      {
        EEPROM_writelong(eled2e,val);
        e2 = val;
      }
      else if ( to == 3 )
      {
        EEPROM_writelong(eled3e,val);
        e3 = val;
      }
      else if ( to == 4 )
      {
        EEPROM_writelong(eled4e,val);
        e4 = val;
      }
      else if ( to == 5 )
      {
        EEPROM_writelong(eled5e,val);
        e5 = val;
      }
      else if ( to == 6 )
      {
        EEPROM_writelong(eled6e,val);
        e6 = val;
      }
      break; 
    //MANUEL STORM
    case 4:
      dostorm = val;
      rest = 0;
      break; 
    // MANUEL CLOUD
    case 5:
      docloud = val;
      rest = 0;
      break; 
      //send current value to PHP 
    case 6:
      dsend = 1;
      dodata = 1;
      break;  
    //MANUEL ON / OFF
    case 7:
      mau = val;
      rest = 0;
      break; 
      //restart controller  
    case 8:
      softReset();
      break;
      //Servos ON / OFF
    case 9:
      if ( to == 1 )
      {
        servo1S = val;
        EEPROM.write(servo1Saddr,val);
      }
      else if ( to == 2 )
      {
        servo2S = val;
        EEPROM.write(servo2Saddr,val);
      }
      break;
    //SERVO ANGLE
    case 10:
      if ( to == 1 )
      {
        servo1A = val;
        EEPROM.write(servo1Aaddr,val);
      }
      else if ( to == 2 )
      {
        servo2A = val;
        EEPROM.write(servo2Aaddr,val);
      }
      break;  
    //SERVO SPEED
    case 11:
      if ( to == 1 )
      {
        servo1V = val;
        EEPROM.write(servo1Vaddr,val);
      }
      else if ( to == 2 )
      {
        servo2V = val;
        EEPROM.write(servo2Vaddr,val);
      }
      break;  
    //SET DATE TIME
    case 12:
      year = to + 2000;
      SetRtc();
      break;
    // DISABLE AUTO CLOUD / STORM
    case 13:
      if ( to == 1 )
      {
        disableC = val;
        EEPROM.write(disableCaddr,val);
      }
      else if ( to == 2 )
      {
        disableS = val;
        EEPROM.write(disableSaddr,val);
      }
      break;
    //alarm ON / OFF
    case 14:
       if ( to == 1 )
      {
        alarmIsOn = val;
        EEPROM.write(aison,val);
      }
      else if ( to == 2 )
      {
        alarmIsOn2 = val;
        EEPROM.write(aison2,val);
      }   
      break;
     //Alarm mix / max
     case 15:
      if ( to == 1 )
      {
        mnt1 = val;
        mnt1s = dtostrf(mnt1,0,1,mntt1);
        EEPROM.write(amn1,val);
      }
      else if ( to == 12 )
      {
        mt1 = val;
        mt1s = dtostrf(mt1,0,1,mtt1);
        EEPROM.write(am1,val);
      }
      if ( to == 2 )
      {
        mnt2 = val;
        mnt2s = dtostrf(mnt2,0,1,mntt2);
        EEPROM.write(amn2,val);
      }
      else if ( to == 22 )
      {
        mt2 = val;
        mt2s = dtostrf(mt2,0,1,mtt2);
        EEPROM.write(am2,val);
      }
      break;
    }
    // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(ReplyBuffer);
    Udp.endPacket();
  }
}

void prevision() {

  Serial.println("Start prevision");
  stormp = 0;
  //Jan
  if ( month == 1 && ( day == 1 || day == 3 || day == 4 || day == 6 || day == 7 || day == 8 || day == 10 || day == 12 || day == 13 || day == 14 || day == 15 || day == 17 || day == 18 || day == 20 || day == 22 || day == 23 || day == 25 || day == 26 || day == 28 || day == 29 || day == 30 ) )
  {
    stormp = 1;
  }
  //Fev
  else if ( month == 2 && ( day == 1 || day == 2 || day == 4 || day == 5 || day == 6 || day == 9 || day == 10 || day == 11 || day == 15 || day == 16 || day == 17 || day == 19 || day == 20 || day == 21 || day == 24 || day == 25 || day == 26 || day == 27 || day == 29 || day == 30 ) )
  {
    stormp = 1;
  }
  //Mar
  else if ( month == 3 && ( day == 1 || day == 2 || day == 5 || day == 6 || day == 7 || day == 8 || day == 12 || day == 13 || day == 14 || day == 17 || day == 18 || day == 20 || day == 21 || day == 23 || day == 24 || day == 26 || day == 28 || day == 29 || day == 31 ) )
  {
    stormp = 1;
  }
  //Apr
  else if ( month == 4 && ( day == 3 || day == 7 || day == 16 || day == 19 || day == 22 || day == 25 || day == 27 || day == 28 || day == 31 ) )
  {
    stormp = 1;
  }
  //May
  else if ( month == 5 && ( day == 10 || day == 20 ) )
  {
    stormp = 1;
  }
  //Jun
  else if ( month == 6 && day == 10 )
  {
    stormp = 1;
  }
  //jul
  else if ( month == 7 && day == 15 )
  {
    stormp = 1;
  }
  //Aug
  else if ( month == 8 && day == 21 )
  {
    stormp = 1;
  }
  //Sep
  else if ( month == 9 &&  ( day == 12 || day == 24 ) )
  {
    stormp = 1;
  }
  //Oct
  else if ( month == 10 &&  ( day == 2 || day == 5 || day == 8 || day == 16 || day == 25 || day == 28 || day == 31 ) )
  {
    stormp = 1;
  }
  //Nov
  else if ( month == 11 &&  ( day == 1 || day == 3 || day == 7 || day == 9 || day == 10 || day == 14 || day == 15 || day == 18 || day == 22 || day == 26 || day == 28 || day == 29 ) )
  {
    stormp = 1; 
  }
  //Dec
  else if ( month == 12 && ( day == 2 || day == 4 || day == 5 || day == 8 || day == 9 || day == 11 || day == 12 || day == 16 || day == 19 || day == 21 || day == 22 || day == 23 || day == 26 || day == 27 || day == 29 || day == 30 ) )
  {
    stormp = 1;
  }

  //Storm prevision
  if ( stormp == 1 && disableS == 0 )
  {
    //Start / end storm 
    nxss = random(82799); //start sec in the day 86400 - 3601 (-1h)
    nxse = random((nxss + 600),(nxss + 3599)); //end nxss (start) + 10 minutes minimum to 59m59sec max
  }
  else
  {
    nxss = 0;
    nxse = 0;
  }

  //cloud prevision based on channel 4 start & end time (you may change s4 and e4 if you don't use the channel 4).
  if ( disableC == 0 )
  {
      //start / end cloud
      nxcs = random( (s4*60),( (s4*60) + ( e4*60 ) ) - 3601 ); //between period of channel4 ( -1h, 1sec )
      nxce = random((nxcs + 600),(nxcs + 3599)); //end nxcs (start) + 10 minutes minimum to 59m59sec max
  }
  else
  {
     nxcs = 0;
     nxce = 0;
  }
  
  if ( disableC == 0 || disableS == 0 )
  {
      //Send pevision 
      dsend = 2;
      dodata = 1;
  }
}

void temps()
{         
  for(itp; itp < tpcc; itp +=1)
  {
    byte i;
    byte present = 0;
    byte type_s;
    byte data[12];
    byte addr[8];

    if ( !ds.search(addr)) {
      ds.reset_search();
      delay(250);
      return;
    }

    if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return;
    }

    ds.reset();
    ds.select(addr);
    ds.write(0x44,1);         // start conversion, with parasite power on at the end

    //delay(1000); 

    present = ds.reset();
    ds.select(addr);    
    ds.write(0xBE);         // Read Scratchpad

    for ( i = 0; i < 9; i++) {           // we need 9 bytes
      data[i] = ds.read();
    }

    // convert the data to actual temperature

    unsigned int raw = (data[1] << 8) | data[0];
    if (type_s) {
      raw = raw << 3; // 9 bit resolution default
      if (data[7] == 0x10) {
        // count remain gives full 12 bit resolution
        raw = (raw & 0xFFF0) + 12 - data[6];
      }
    } 
    else {
      byte cfg = (data[4] & 0x60);
      if (cfg == 0x00) raw = raw << 3;  // 9 bit resolution, 93.75 ms
      else if (cfg == 0x20) raw = raw << 2; // 10 bit res, 187.5 ms
      else if (cfg == 0x40) raw = raw << 1; // 11 bit res, 375 ms
      // default is 12 bit resolution, 750 ms conversion time
    }
    celsius = (float)raw / 16.0;
    fahrenheit = celsius * 1.8 + 32.0;

    //Capteur 1
    if ( itp == 1 )
    {
      tp1 = tp == 1 ? celsius : fahrenheit;

      if ( tp1 <= 0 || ( tp == 1 && tp1 >= 40 ) || ( tp != 1 && tp1 >= 104 ) )
      { 
        ds.reset();
        itp = 0;
        temps();
      }  
          
      if ( tp1 > mt1 || tp1 < mnt1 )
      {
         acp1 = 1; 
      }
      else
      {
         acp1 = 0; 
      }
     
    }
    //Capteur 2
    else
    {
      tp2 = tp == 1 ? celsius : fahrenheit;

      if ( tp2 <= 0 || ( tp == 1 && tp2 >= 40 ) || ( tp != 1 && tp2 >= 104 ) )
      { 
        ds.reset();
        itp = 0;
        temps();
      }
      else
      {
        if ( tp2 > mt2 || tp2 < mnt2 )
        {
           acp2 = 1; 
        }
        else
        {
           acp2 = 0; 
        }
      }
    }
  }
    Serial.print("temperature sensor 1:");
    Serial.print(tp1);
    Serial.print(" max:");
    Serial.print(mt1);
    Serial.print(" min:");
    Serial.println(mnt1); 
    Serial.print("temperature sensor 2:");
    Serial.print(tp2);
    Serial.print(" max:");
    Serial.print(mt2);
    Serial.print(" min:");
    Serial.println(mnt2); 
}

void DoFunc()
{   
  //ATX power Supply
  if ( ( cur1 > 0 || cur2 > 0 || cur3 > 0 || cur4 > 0 || cur5 > 0 || cur6 > 0 || mau > 0 || dostorm == 3 ) && atx == 0 )
  {
    digitalWrite(ctrlAtx, LOW);
    atx = 1;
    Serial.println("ATX ON"); 
  }
  else if ( atx == 1 && ( cur1 <= 0 && cur2 <= 0 && cur3 <= 0 && cur4 <= 0 && cur5 <= 0 && cur6 <= 0 && dostorm == 0 ) && mau == 0 )
  {
    digitalWrite(ctrlAtx, HIGH);
    atx = 0;
    Serial.println("ATX OFF"); 
  }

  if (  mau == 1 && rest == 0 )
  {
    EEPROM.write(ma,1);
    restoreLed();
    Serial.println("Manuel ON");
  }
  else if ( mau == 2 )
  {
    EEPROM.write(ma,0);
    restoreLed();
    mau = 0;
    Serial.println("Manuel OFF");
  }
  if ( ( dostorm == 1 || dostorm == 3 ) && ( disableS == 0 || mau == 1 ) )
  {
    storm();
    EEPROM.write(st,1);
    Serial.println("Storm ON");
  }
  else if ( dostorm == 2 )
  {
    dostorm = 0;
    EEPROM.write(st,0);
    Serial.println("Storm OFF");
  }

  if ( ( docloud == 1 || docloud == 3 ) && ( disableC == 0 || mau == 1 ) )
  {
    cloud();
    EEPROM.write(cl,1);
    Serial.println("Cloud ON");
  }
  else if ( docloud == 2 )
  {
    docloud = 0;
    EEPROM.write(cl,0);
    Serial.println("Cloud OFF");
  }
    
  if ( dodata == 1 )
  {
    // if the server's disconnected, stop the client:
    if (!client.connected()) {
      saveData();
      Serial.println("disconnecting.");
      client.stop();
    } 
    dodata = 0;
  } 
}

//redémarrage
void softReset()
{
  asm volatile ("  jmp 0");
} 

void controller()
{        
  //Start controller
  DrawController();

  //Start / stop led
  if ( mau == 0 )
  {
    DrawLed();
  }

  //Do prevision everyday at 0:00:01.
  if ( curSec == 1 )
  {
    prevision();
  }

  //timers
  DoTimers();

  //check functions
  DoFunc();
  
  //Alarm
  DrawAlarm();
}

void DrawAlarm()
{                   
  if ( alarmIsOn == 1 || alarmIsOn2 == 1 )
  {
    if( acp1 == 1 || acp2 == 1 )
    {
      //pin note length
      tone(AlarmPin, note, length);
      delay(100);
    }
    else if( acp1 == 0 && acp2 == 0 )
    {
      noTone(AlarmPin);
    }  
  }
  else
  {
    acp1 = 0; acp2 = 0;
    noTone(AlarmPin);
  }
}

void loop() {
  //do every sec
  sect = sect + 10;

  if ( sect == 990 ) // 1sec
  {
    //reset
    sect = 0;
    //controller
    controller();
    //Serial.println("controller");
  }
  //servos
  svot = svot + 10;

  if ( svot == svoMax && svo > 0 )
  {
    //reset
    svot = 0;
    DrawServosMode();
  }
  
    //do every sec
  tmpa = tmpa + 10;

  if ( ( tmpa >= 2990 && toserver == 0 && itp != 1  ) || ( tpons == 0 && dodata == 0 ) ) // 3sec - not sending to server - all sensor done
  {
    //reset
    tmpa = 0;
    itp =0;
    //temp every 10sec
    temps();

    if ( tpons == 0 )
    {
        dsend = 3;
        dodata = 1; 
    }
  }

  //Init Horloge RTC
  DrawRtc();

  delay(10);
}

