/*
 * develop by uriel wenceslao
 */

#include <EEPROM.h>
 /* 
  *  initializing pins
  *  Digital pin 0 - 4 as input
  */
  int coinPin = 0; // IC PIN 2 coin slot
  int waterPin = 1; // IC PIN 3 Water wash button
  int soapPin = 2; // IC PIN 4 soap washing button
  int airPin = 3; // IC PIN 5 Air service button
  int userdefinePin = 4; //IC PIN 6 anything the user wants but not open to public. This is for personal used
  

  int CoinIndicatorLight = 6; // Pin 12 notify the user if the coin is accepted else light is off.
  int ServoDataControl = 7; // Pin 13 control the servo motors during mixing
  
  int relayWater = 8; // IC pin 14 turning the relay control on and off
  int relaySoap = 9; // IC 15 relay soap
  int relayAir = 10; // IC 16 relay Air

  
    
  int resetpasskey = 11; // IC Pin 17 Keyinput for EEPROM reset
  int EEPROMFlag = false; // This will control the EEPROM DATA. true = write, false = not
  int limitUsage = 5; // the limit of usage on how many times you can use the service else device is unusable.
  int EEPROM_Transaction_ADR = 10; // EEPROM address where data transaction counter is stored.
  int EPROM_Protect = 519; // uriel m wenceslao

  

  int solinoid_WaterIn_from_waterTank = A0;// analog IC PIN23 solinoid to control the open and close of the water tank
  int solinoid_WaterOut_WaterWash = A1;// analog IC PIN24  solinoid to control the water out for washing
  
  int solinoidWaterOut_To_Soap_Tank_for_Mixing = A2; // analog IC PIN25  solinoid to feed in water to soap tank

  int solinoid_Soap_Out_for_Washing = A3; // analog IC PIN26 solinoid to control soap for washing
  
  int solinoid_Water_Out_To_SoapTank = A4; // analog IC PIN27  solinoid to pour out water to soaptank mixing
  int solinoid_Soap_Out_To_SoapTank = A5; // Analog IC PIN28 solinoid to pour out soap to soaptank for mixing
  

void setup() {
  pinMode(coinPin, INPUT);
  pinMode(waterPin, INPUT);
  pinMode(soapPin, INPUT);
  pinMode(airPin, INPUT);
  pinMode(userdefinePin, INPUT);
  pinMode(CoinIndicatorLight, OUTPUT);
  pinMode(relayWater, OUTPUT);
  pinMode(relaySoap, OUTPUT);
  pinMode(relayAir, OUTPUT);
  pinMode(ServoDataControl,OUTPUT);
  
  pinMode(solinoid_WaterIn_from_waterTank,OUTPUT); //A0
  pinMode(solinoid_WaterOut_WaterWash,OUTPUT); //A1
  pinMode(solinoidWaterOut_To_Soap_Tank_for_Mixing,OUTPUT); //A2
  pinMode(solinoid_Soap_Out_for_Washing,OUTPUT); //A3
  pinMode(solinoid_Water_Out_To_SoapTank,OUTPUT); //A4
  pinMode(solinoid_Soap_Out_To_SoapTank,OUTPUT); //A5

  digitalWrite(solinoid_WaterIn_from_waterTank,LOW); //A0
  digitalWrite(solinoid_WaterOut_WaterWash,LOW); //A1
  digitalWrite(solinoidWaterOut_To_Soap_Tank_for_Mixing,LOW); //A2
  digitalWrite(solinoid_Soap_Out_for_Washing,LOW); //A3
  digitalWrite(solinoid_Water_Out_To_SoapTank,LOW); //A4
  digitalWrite(solinoid_Soap_Out_To_SoapTank,LOW); //A5

 // Serial.begin(9600); 
}
 
void optionSelect(int inputcoin)
  {
    int transactionCounter = EEPROM.read(EEPROM_Transaction_ADR);
    int defwater = 0,defair = 0;
    digitalWrite(CoinIndicatorLight,inputcoin);
    
    if (limitUsage == transactionCounter)
      {
      // beep buzzer 
      for (int i = 0; i < 5; i++)
        {
          digitalWrite(CoinIndicatorLight,1);
          delay(50);
          digitalWrite(CoinIndicatorLight,0);
          delay(20);
          digitalWrite(CoinIndicatorLight,1);
          delay(50);
          digitalWrite(CoinIndicatorLight,0);        
        }    
      }
    else 
      {
      while (inputcoin)
        {    
        if (digitalRead(waterPin) == 1 && inputcoin == 1)
          {
            if(EEPROMFlag)
            EEPROM.update(EEPROM_Transaction_ADR, EEPROM.read(EEPROM_Transaction_ADR) + 1);
            digitalWrite(CoinIndicatorLight,0);
            Water_drain();
            inputcoin = 0;
            break;
          }
        
        if (digitalRead(soapPin) == 1 && inputcoin == 1)
          {
            if(EEPROMFlag)
            EEPROM.update(EEPROM_Transaction_ADR, EEPROM.read(EEPROM_Transaction_ADR) + 1);
            digitalWrite(CoinIndicatorLight,0);
            Soap_drain();
            inputcoin = 0;
            break;
          }
        
        if (digitalRead(airPin) == 1 && inputcoin == 1)
          {
            if(EEPROMFlag)
            EEPROM.update(EEPROM_Transaction_ADR, EEPROM.read(EEPROM_Transaction_ADR) + 1);
            digitalWrite(CoinIndicatorLight,0);
            Air_drain();
            inputcoin = 0;
            break;
          }
        
        if (digitalRead(userdefinePin) == 1)
          {
          if(EEPROMFlag)
          EEPROM.update(EEPROM_Transaction_ADR, EEPROM.read(EEPROM_Transaction_ADR) + 1);
          digitalWrite(CoinIndicatorLight,0);
            do{
            defwater = digitalRead(waterPin);
            defair = digitalRead(airPin);
              if (defwater == 1)
                {
                  digitalWrite(CoinIndicatorLight,1);
                  break;
                }  
              
              if (defair == 1)
                {
                  digitalWrite(CoinIndicatorLight,1);
                  break;
                }  
            
            }while (digitalRead(userdefinePin));
          
          userdefineValue(defwater,defair);   
          digitalWrite(CoinIndicatorLight,0);   
          inputcoin = 0;
          }        
        }
      }
  }


void Water_drain() //////////////////////////////////////////////////////////////////////////////////////
  {
  
  int timePIN1 = 12; // pin18
  int timePIN2 = 13; //pin19
  int relayOn = 1;
  int relayOff = 0;
  pinMode(timePIN1, INPUT);
  pinMode(timePIN2, INPUT);
  /*
  * Pin Combinatio
  * 0 0 - 2 minutes
  * 0 1 - 2.5 minutes
  * 1 0 - 3  minutes
  * 1 1 - 3. 5 minutes
  */
  float delaycount = 10; // ratio of 1:1 means dealay count is the default one minute duration
  float switchtime;
  
  if (digitalRead(timePIN1) == 0 && digitalRead(timePIN2) == 0)
    switchtime =  1;
  if (digitalRead(timePIN1) == 0 && digitalRead(timePIN2) == 1)
    switchtime = 5;
  if (digitalRead(timePIN1) == 1 && digitalRead(timePIN2) == 0)
   switchtime = 10;
  if (digitalRead(timePIN1) == 1 && digitalRead(timePIN2) == 1)
    switchtime = 15;
  
  // Turning On Solinoid A0  and A4
  digitalWrite(solinoid_WaterOut_WaterWash,1); //A1
  delay(200);
  digitalWrite(solinoid_WaterIn_from_waterTank,1); //A4
  delay(80);
  for (int i = 0; i < 7 ; i++)
    {
      digitalWrite(relayWater,relayOn);
      delay(delaycount*switchtime); 
      digitalWrite(relayWater,relayOff);
      delay(delaycount); 
    }
  
  // Turning Off Solinoid A0  and A4
  digitalWrite(solinoid_WaterOut_WaterWash,0); //A1
  delay(10);
  digitalWrite(solinoid_WaterIn_from_waterTank,0); //A4
  delay(10);  
  }

void Soap_drain() /////////////////////////////////////////////////////////////////////////////////////////////////
{
  int On = 1, Off = 0;
  int solinoidDalay_On_Off = 100;
  int SoapDrainTime = 300; 
  int analogPinValueOn  = HIGH;
  int analogPinValueOff  = LOW;
  
/*
  pinMode(solinoid_WaterIn_from_waterTank,OUTPUT); //A0
  pinMode(solinoid_WaterOut_WaterWash,OUTPUT); //A1
  pinMode(solinoidWaterOut_To_Soap_Tank_for_Mixing,OUTPUT); //A2
  pinMode(solinoid_Soap_Out_for_Washing,OUTPUT); //A3
  pinMode(solinoid_Water_Out_To_SoapTank,OUTPUT); //A4
  pinMode(solinoid_Soap_Out_To_SoapTank,OUTPUT); //A5
 */

    // Open water conenction to SoapTank  
      digitalWrite(solinoid_Water_Out_To_SoapTank,analogPinValueOn); // A4
      delay(solinoidDalay_On_Off); // time to open the valve
      digitalWrite(solinoid_WaterIn_from_waterTank,analogPinValueOn); // A0
      delay(solinoidDalay_On_Off);
    
    // Fill soap tank with water     
      digitalWrite(relayWater,On);
      delay(500); // time to fill the water on the soap tank
      digitalWrite(relayWater,Off);  
      delay(10);

    // Off the valve
      digitalWrite(solinoid_Water_Out_To_SoapTank,analogPinValueOff);
      delay(solinoidDalay_On_Off); // time to open the valve
      digitalWrite(solinoid_WaterIn_from_waterTank,analogPinValueOff);
      delay(solinoidDalay_On_Off);      

    // at this stage soap tank already contains water and valve are closed
  
    // Fill soap tank with Soap     
      digitalWrite(solinoid_Soap_Out_To_SoapTank,analogPinValueOn); // A5
      delay(50); // time to fill the water on the soap tank
      digitalWrite(solinoid_Soap_Out_To_SoapTank,analogPinValueOff); // A5
      delay(solinoidDalay_On_Off);
   
   // at this stage soap tank already contains Soap
        
   // Turn the servo motor on to start mixing
      for (int i = 0; i < 50; i++)
      {     
        digitalWrite(ServoDataControl,On);
        delay(10); // Turn the servo motors on then mixing soap and water
        digitalWrite(ServoDataControl,Off);
        delay(5);
      }
  // Despense soap for washing
       digitalWrite(solinoid_Soap_Out_for_Washing,analogPinValueOn); // A3
       delay(solinoidDalay_On_Off);
       digitalWrite(solinoidWaterOut_To_Soap_Tank_for_Mixing,analogPinValueOn); // A2
       delay(solinoidDalay_On_Off);       
       digitalWrite(relaySoap,On);
       delay(SoapDrainTime);
       digitalWrite(relaySoap,Off);      
       delay(15);
       digitalWrite(solinoid_Soap_Out_for_Washing,analogPinValueOff);
       delay(solinoidDalay_On_Off);
       digitalWrite(solinoidWaterOut_To_Soap_Tank_for_Mixing,analogPinValueOff);
       delay(solinoidDalay_On_Off);

       ////Happy day
        
  
}

void Air_drain() /////////////////////////////////////////////////////////////////////////////////////
{

  int relayOn = 1;
  int relayOff = 0;
  int airDrainTime = 30; 
    for (int i = 0; i < 7 ; i++)
    {
      digitalWrite(relayAir,relayOn);
      delay(airDrainTime); 
      digitalWrite(relayAir,relayOff);  
      delay(airDrainTime); 
    }


  
 }


void userdefineValue(int defWater, int defAir) //////////////////////////////////////////////////////////////////////
  {
  // This will be unlimited option for both air and water 
  // Soap needs to consider on what design is accepted. It will create a huge mixture of soap and water or one at a time depending on user request
  
  if ( defWater == 1)
    {
    while (digitalRead(userdefinePin))
      {
        digitalWrite(relayWater,1);
        delay(30); 
        digitalWrite(relayWater,0);
        delay(30); 
      }
    }
  
  
  if ( defAir == 1)
    {
    while (digitalRead(userdefinePin))
      {
        digitalWrite(relayAir,1);
        delay(40); 
        digitalWrite(relayAir,0);
        delay(40); 
      }
    }
  }

void eepromReadData() //////////////////////////////////////////////////////////////////////
{
 int x;
 int epromData = EEPROM.read(EEPROM_Transaction_ADR);

  for (x = 0; x < epromData; x++) 
    {
      digitalWrite(CoinIndicatorLight,1);
      delay(15); 
      digitalWrite(CoinIndicatorLight,0);
      delay(15); 
    }
  
}


void epromReset() /////////////////////////////////////////////////////////////////////////////////////////////////
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
  do{ // first digit
    if (digitalRead(5) == 1)
      {
        FirstDigit = FirstDigit + 1;
        digitalWrite(CoinIndicatorLight,1);
        delay(80);
        digitalWrite(CoinIndicatorLight,0);
      }
  }while (FirstDigit != pass1);
  
  }
  if(FirstDigit == pass1)
  {
  digitalWrite(8,1);
  passcomplete = 500;
  }
  
  do{ // 2nd digit
    if (digitalRead(5) == 1)
    {
      SecondDigit = SecondDigit + 1;
      digitalWrite(CoinIndicatorLight,1);
      delay(80);
      digitalWrite(CoinIndicatorLight,0);
    }
  }while (SecondDigit != pass2);
  
  if(SecondDigit == pass2)
    {
      digitalWrite(9,1);
      passcomplete = passcomplete + 10;
    }
  
  do{ // 3rd digit
    if (digitalRead(5) == 1)
    {
      ThirdDigit = ThirdDigit + 1;
      digitalWrite(CoinIndicatorLight,1);
      delay(80);
      digitalWrite(CoinIndicatorLight,0);
    }
  }while (ThirdDigit != pass3);
        digitalWrite(10,1);
  
  if(ThirdDigit == pass3)
  {
  passcomplete = passcomplete + 9;
  }
  
  if (passcomplete == EPROM_Protect)
    {
    EEPROM.update(EEPROM_Transaction_ADR, 0);
    for (int i = 0; i < 10; i++) 
      {
        digitalWrite(CoinIndicatorLight,1);
        delay (7);
        digitalWrite(CoinIndicatorLight,0);
        delay (7);
      }
    }
  }


void loop() {
  int coinslot = 0;
  coinslot = digitalRead(coinPin);
  
 if (digitalRead(resetpasskey) == 1 && coinslot == 1)
    eepromReadData();

  if (digitalRead(resetpasskey) == 1 &&  digitalRead(userdefinePin) == 1)
    epromReset();
  
  if (coinslot == 1 && digitalRead(resetpasskey) == 0)  
    optionSelect(coinslot);  
 

   
}
