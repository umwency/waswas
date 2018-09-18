/*
 * develop by uriel wenceslao
 */

#include <EEPROM.h>
 /* 
  *  initializing pins
  *  Digital pin 0 - 4 as input
  */
  int coin = 0;
  int water = 1;
  int soap = 2;
  int air = 3;
  int userdefine = 4;
  int relay = 8;
  int EEPROM_ADR = 0;
  int EEPROM_Transaction_ADR = 10;
  int EPROM_Protect = 519;

void setup() {
  pinMode(coin, INPUT);
  pinMode(water, INPUT);
  pinMode(soap, INPUT);
  pinMode(air, INPUT);
  pinMode(userdefine, INPUT);
  EEPROM.write(EEPROM_ADR,0);  
 // EEPROM.write(EEPROM_Transaction_ADR,0); // initializing transaction address on the first time.

 // Serial.begin(9600); 
}
 
void optionSelect(int inputcoin)
{
  int outputcoinPin = 8;
  pinMode(outputcoinPin, OUTPUT);
  digitalWrite(outputcoinPin,inputcoin);
  while (inputcoin){
    
    if (digitalRead(water) == 1)
    {
      EEPROM.update(EEPROM_Transaction_ADR, EEPROM.read(EEPROM_Transaction_ADR) + 1);
      digitalWrite(outputcoinPin,0);
      Water_drain();
      inputcoin = 0;
    }

   if (digitalRead(soap) == 1)
    {
      EEPROM.update(EEPROM_Transaction_ADR, EEPROM.read(EEPROM_Transaction_ADR) + 1);
      digitalWrite(outputcoinPin,0);
      Soap_drain();
      inputcoin = 0;
    }

   if (digitalRead(air) == 1)
    {
      EEPROM.update(EEPROM_Transaction_ADR, EEPROM.read(EEPROM_Transaction_ADR) + 1);
      digitalWrite(outputcoinPin,0);
      Air_drain();
      inputcoin = 0;
    }

     if (digitalRead(userdefine) == 1)
    {
      digitalWrite(outputcoinPin,0);
      userdefineValue();
      inputcoin = 0;
    }
    
  }
    
 
}


void Water_drain() //////////////////////////////////////////////////////////////////////////////////////
{
  
  int timePIN1 = 12;
  int timePIN2 = 13;
  int relayOn = 1;
  int relayOff = 0;
  
  pinMode(relay, OUTPUT);
  pinMode(timePIN1, INPUT);
  pinMode(timePIN2, INPUT);
  /*
   * Pin Combinatio
   * 0 0 - 2 minutes
   * 0 1 - 2.5 minutes
   * 1 0 - 3  minutes
   * 1 1 - 3. 5 minutes
   */
  float delaycount = 20; // ratio of 1:1 means dealay count is the default one minute duration
  float switchtime;

  if (digitalRead(timePIN1) == 0 && digitalRead(timePIN2) == 0)
    switchtime =  2;
  if (digitalRead(timePIN1) == 0 && digitalRead(timePIN2) == 1)
    switchtime = 2.5;
  if (digitalRead(timePIN1) == 1 && digitalRead(timePIN2) == 0)
    switchtime = 3;
  if (digitalRead(timePIN1) == 1 && digitalRead(timePIN2) == 1)
    switchtime = 3.5;
  
  for (int i = 0; i < 7 ; i++) {
  digitalWrite(relay,relayOn);
  delay(delaycount*switchtime); 
  digitalWrite(relay,relayOff);
  delay(delaycount); 
  }

 }

void Soap_drain() /////////////////////////////////////////////////////////////////////////////////////////////////
{

  pinMode(11, OUTPUT);
  pinMode(5, OUTPUT); //pass pin
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  
  int FirstDigit = 0,SecondDigit = 0, ThirdDigit = 0;
  int pass1 = 5, pass2 = 1, pass3 = 9;
  int passcomplete  = 0;


  if (digitalRead(11) == 1)
  {
    do { // first digit
        if (digitalRead(5) == 1){
          FirstDigit = FirstDigit + 1;
          digitalWrite(10,1);
          delay(80);
          digitalWrite(10,0);
        }
      } while (FirstDigit != pass1);
    
    }
        if(FirstDigit == pass1)
        {
           digitalWrite(8,1);
           passcomplete = 500;
        }

    do { // 2nd digit
        if (digitalRead(5) == 1){
          SecondDigit = SecondDigit + 1;
          digitalWrite(10,1);
          delay(80);
          digitalWrite(10,0);
        }
      } while (SecondDigit != pass2);
    
    if(SecondDigit == pass2)
    {
      digitalWrite(9,1);
      passcomplete = passcomplete + 10;
    }

    do { // 3rd digit
        if (digitalRead(5) == 1){
          ThirdDigit = ThirdDigit + 1;
          digitalWrite(10,1);
          delay(80);
          digitalWrite(10,0);
        }
      } while (ThirdDigit != pass3);
    
    if(ThirdDigit == pass3)
    {
      passcomplete = passcomplete + 9;
    }

if (passcomplete == EPROM_Protect)
  {
  EEPROM.update(EEPROM_Transaction_ADR, 0);
      for (int i = 0; i < 10; i++) {
          digitalWrite(9,1);
          digitalWrite(10,1);
          digitalWrite(8,1);
          delay (40);
          digitalWrite(9,0);
          digitalWrite(10,0);
          digitalWrite(8,0);
          delay (50);
      }

}

    

}

void Air_drain() /////////////////////////////////////////////////////////////////////////////////////
{
  
  int relayOn = 1;
  int relayOff = 0;
  int airDrainTime = 30; 
  pinMode(relay, OUTPUT);
  pinMode(9, OUTPUT);
  for (int i = 0; i < 7 ; i++) {
  digitalWrite(9,relayOn);
  delay(airDrainTime); 
  digitalWrite(9,relayOff);
  delay(airDrainTime); 
  }


  
 }


void userdefineValue() //////////////////////////////////////////////////////////////////////
{
pinMode(9, OUTPUT);  
 int x;
 int epromData = EEPROM.read(EEPROM_Transaction_ADR);

  for (x = 0; x < epromData ; x++) {
  digitalWrite(9,1);
  delay(40); 
  digitalWrite(9,0);
  delay(40); 
  }
  
}









void loop() {
  int coinslot = 0;
  coinslot = digitalRead(coin);
  if (coinslot == 1)  
  optionSelect(coinslot);
}
