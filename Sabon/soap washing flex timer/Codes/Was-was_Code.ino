/*
 * develop by uriel wenceslao
 * umwency@gmail.com
 */

#include <EEPROM.h>
  
    // Standard time settings
    int milliseconds_realTime = 1; // 1 milliseconds delay each program cycles.
    
    int StandardSeconds = 60; // 1 minutes in 60 secods
    int StandardMilliseconds = 60; // 1 seconds is 1000 millisecods and the loop is rotating ones every milliseconds;

    
    // Timer Settings for soap
    int SoapDespenseDurationTime = 3; // express in seconds. (1 min = 60 seconds) 
    int Soap_milliseconds = 0; // initiate soap milliseconds timer
    int Soap_Seconds = 0; // inititate soap seconds timer    

 ////////////////////////////////////////////////////////////////////////////////////
     
    // Timer Settings for Air
    int AirDespenseDurationTime = 10; // express in seconds. (1 min = 60 seconds) 
    int Air_milliseconds = 0; // initiate soap milliseconds timer
    int Air_Seconds = 0; // inititate soap seconds timer   

//////////////////////////////////////////////////////////////////////////////////
    // declaration for services selected.
  boolean Soap_Indicator = false;
  boolean Air_Indicator = false;
    

    boolean coinFlag = false;
    int coinIndicator = A0;
    int on = 1, off = 0;

    // Service input pins
    int coinPin = 2; // IC PIN #4 coin slot
    int soapPin = 1; // IC PIN #3 Soap pin button
    int airPin = 0; // IC PIN #2 Air pin button

    int userDefine = 3; // IC PIN #5 Userdefine button
    
  

    //Service output pins (despense product)
    int despenseSoapPin = 13;// IC Pin #19  
    int despenseAirPin = 12; // IC Pin #18

    // Soap mixing services
    int EEPROM_Transaction_ADR = 10; // EEPROM address where data transaction counter is stored.
    int waterSoapToBowl = 8; // IC Pin #14
    int liquidSoapToBowl = 9; // IC Pin #15
    int mixingMotor = 10; // IC Pin #16
    
    int transactionCounter = 4; //number of soap service use before it create mixing 
 
    int liquidSoaptoBowlTime = 3; // in seconds (despensing liquid soap to mixing bowl)
    int watertoBowlTime = 5; // in seconds (despensing water from tank to mixing bowl)
    int mixingMotorTime = 4; // in seconds mixing soap and water
    int mixing_milliseconds = 0; // initiate soap milliseconds timer
    int mixing_Seconds = 0; // inititate soap seconds timer   
    boolean Trigger_liquidSoaptoBowlTime = false;
    boolean Trigger_watertoBowlTime = false;
    boolean Trigger_mixingMotorTime = false;
    boolean mexingComplete = false;
    

//This is the main setup for pins washing main controller
    void setup() {
    // Setting up pin input pins
     pinMode(coinPin, INPUT);
     pinMode(soapPin, INPUT); // Soap Button
     pinMode(airPin, INPUT); // Air Button

     // Setting up  output pins
     pinMode(despenseSoapPin, OUTPUT);
     pinMode(despenseAirPin, OUTPUT);
     pinMode(coinIndicator,OUTPUT);

     // setting up soap mixing pin
     pinMode(waterSoapToBowl, OUTPUT);
     pinMode(liquidSoapToBowl, OUTPUT);
     pinMode(mixingMotor, OUTPUT);
     pinMode(11,OUTPUT); // Counter indicator
     pinMode(userDefine,OUTPUT); // user define button
     
     //EEPROM.write(EEPROM_Transaction_ADR,0);  to be enabled during initial setup
}   

void loop() {
  int coinslot = digitalRead(coinPin); // detect if there is coin inserted
  int userdefinedButton = digitalRead(userDefine);

  if (userdefinedButton == 1)
     userDefineFuctions();  

    if (coinslot == 1)
      {
        coinFlag = true;
        digitalWrite(coinIndicator,on);
      }

   //Searhing for button to be press.
    if (coinFlag )
      {   
        if (digitalRead(soapPin) == 1){ // Soap Button 
          coinFlag = false;  
          Soap_Indicator = true; 
          digitalWrite(coinIndicator,off);
      }
    } 
    if (coinFlag)
      {
        if (digitalRead(airPin) == 1){  // Air Button
          coinFlag = false; 
          Air_Indicator = true;  
          digitalWrite(coinIndicator,off);
        }
      }

// universal loop delay. ( delay will only be executed if there is a service selected)
//if (Soap_Indicator || Air_Indicator || EEPROM.read(EEPROM_Transaction_ADR) == transactionCounter-1)
delay(milliseconds_realTime);
      
//============SOAP TIMER ========================
//Time counter counter 
  if(Soap_Indicator) 
  {
    Soap_milliseconds++; 
    digitalWrite(despenseSoapPin,on);
    
    if (Soap_milliseconds ==  StandardMilliseconds)
    {
      Soap_milliseconds = 0;
      Soap_Seconds++;
    }    

    if (Soap_Seconds == SoapDespenseDurationTime)
    {
      digitalWrite(despenseSoapPin,off);
      EEPROM.write(EEPROM_Transaction_ADR, EEPROM.read(EEPROM_Transaction_ADR) + 1);
      Soap_Indicator = false;
      Soap_milliseconds = 0;
      Soap_Seconds = 0;
    }     
  }
//============END SOAP TIMER ========================

if (EEPROM.read(EEPROM_Transaction_ADR) == transactionCounter - 1) // warining signal turn on 
  digitalWrite(11,on);
else
  digitalWrite(11,off);  

//============AIR TIMER ========================
//Time counter counter 
  if(Air_Indicator) 
  {
    Air_milliseconds++; 
    digitalWrite(despenseAirPin,on);
    if (Air_milliseconds ==  StandardMilliseconds)
    {
      Air_milliseconds = 0;
      Air_Seconds++;
    }    

    if (Air_Seconds == AirDespenseDurationTime)
    {
      digitalWrite(despenseAirPin,off);
      Air_Indicator = false;
      Air_milliseconds = 0;
      Air_Seconds = 0;
    }     
  }

//============END AIR TIMER ========================



//============ Soap Mixing Fuction ====================

    if (EEPROM.read(EEPROM_Transaction_ADR) == transactionCounter)
     {
       EEPROM.write(EEPROM_Transaction_ADR,0);
       Trigger_watertoBowlTime = true;
     }
    
    if ( Trigger_watertoBowlTime )
    {
      mixing_milliseconds++;
      digitalWrite(waterSoapToBowl,on); // IC Pin #14 despensing water
      if (mixing_milliseconds ==  StandardMilliseconds)
      {
        mixing_milliseconds = 0;
        mixing_Seconds++;
      }  
     
       if (mixing_Seconds == watertoBowlTime) // end despensing water
        {
          digitalWrite(waterSoapToBowl,off); 
          Trigger_watertoBowlTime = false;
          mixing_Seconds = 0;
          mixing_milliseconds = 0;
          Trigger_liquidSoaptoBowlTime = true;          
        }    
    }
  
    if ( Trigger_liquidSoaptoBowlTime )
    {
      mixing_milliseconds++;
      digitalWrite(liquidSoapToBowl,on); // IC Pin #15 despensing liquid soap
      if (mixing_milliseconds ==  StandardMilliseconds)
      {
        mixing_milliseconds = 0;
        mixing_Seconds++;
      }  
     
       if ( mixing_Seconds == liquidSoaptoBowlTime) // end despensing liquid soap
        {
          digitalWrite(liquidSoapToBowl,off);
          Trigger_liquidSoaptoBowlTime = false;
          mixing_Seconds = 0;
          mixing_milliseconds = 0;
          Trigger_mixingMotorTime = true;          
        }
    
    }

    if ( Trigger_mixingMotorTime )
    {
      mixing_milliseconds++;
      digitalWrite(mixingMotor,on); // IC Pin #16 mixing motor
      if (mixing_milliseconds ==  StandardMilliseconds)
      {
        mixing_milliseconds = 0;
        mixing_Seconds++;
      }  
     
       if ( mixing_Seconds == mixing_milliseconds) // end mixing motor
        {
          digitalWrite(mixingMotor,off);
          Trigger_mixingMotorTime = false;
          mixing_Seconds = 0;
          mixing_milliseconds = 0;
        }
    
    }

}


void userDefineFuctions ()
{

  if (digitalRead(soapPin) == 1) // soap Button
  {
       while (digitalRead(userDefine))
         digitalWrite(despenseSoapPin,on);
      
      digitalWrite(despenseSoapPin,off);
  }
  if (digitalRead(airPin) == 1){  // Air Button
  {
           while (digitalRead(userDefine))
          digitalWrite(despenseAirPin,on);
      
       digitalWrite(despenseAirPin,off);
  }

}

}
