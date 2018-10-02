

#include <EEPROM.h>
    int TimeSettingPerWas = 2; // Total washing duration time
    int StandardSeconds = 5; // 1 minutes in 60 secods
    int StandardMilliseconds = 60; // 1 seconds is 1000 millisecods and the loop is rotating ones every milliseconds;

    boolean coinFlag = false;
    int coinIndicator = 6;
    int on = 1, off = 0;

    int milliseconds_realTime = 1;

    int coinPin = 0; // IC PIN 2 coin slot
    int waterPin_one = 1; // IC PIN 3 Water wash button 1
    int waterPin_two = 2; // IC PIN 4 Water wash button 2
    int waterPin_three = 3; // IC PIN 5 Water wash button 3

    int waterPin_Despense_one = 8; // IC PIN 11 Water wash button 1
    int waterPin_Despense_two = 9; // IC PIN 12 Water wash button 2
    int waterPin_Despense_three = 10; // IC PIN 13 Water wash button 3

    int relayPin = 13; // IC PIN 19 This controls the relay
    boolean waterIndicator_one = false;
    boolean waterIndicator_two = false;
    boolean waterIndicator_three = false;

    int eePromAddress_Water_1 = 10;
    int eePromAddress_Water_2 = 11;
    int eePromAddress_Water_3 = 12;
    int eePromAddress_CoinDropADR = 15;

    int minutes_delay_1,minutes_delay_2,minutes_delay_3 = 0;
    int seconds_delay_1,seconds_delay_2,seconds_delay_3 = 0;
    int milliseconds_delay_1,milliseconds_delay_2,milliseconds_delay_3 = 0;

//This is a 3 input 3 out water wash
    void setup() {
    // Setting up pin input/output
     pinMode(coinPin, INPUT);
     pinMode(waterPin_one, INPUT);
     pinMode(waterPin_two, INPUT);
     pinMode(waterPin_three, INPUT);

     pinMode(waterPin_Despense_one, OUTPUT);
     pinMode(waterPin_Despense_two, OUTPUT);
     pinMode(waterPin_Despense_three, OUTPUT); 
     pinMode(relayPin, OUTPUT);
     
     EEPROM.write(eePromAddress_Water_1, 0);
     EEPROM.write(eePromAddress_Water_2, 0);
     EEPROM.write(eePromAddress_Water_3, 0);
     EEPROM.write(eePromAddress_CoinDropADR, 0);
     pinMode(coinIndicator,OUTPUT);
}   

void loop() {
if (!waterIndicator_one || !waterIndicator_two || !waterIndicator_three)
  {
    if (digitalRead(coinPin) == 1)
    {
      coinFlag = true;
      digitalWrite(coinIndicator,on);
    }
  }
 
   //Searhing for button to be press.
    if (coinFlag )
      {   
        if (digitalRead(waterPin_one) == 1){  // button 1 
          coinFlag = false;  
          waterIndicator_one = true; 
          digitalWrite(coinIndicator,off);
      }
    } 
    if (coinFlag)
      {
        if (digitalRead(waterPin_two) == 1){  // button 2 
          coinFlag = false; 
          waterIndicator_two = true;  
          digitalWrite(coinIndicator,off);
        }
      }
    if (coinFlag)
      {
        if (digitalRead(waterPin_three) == 1){  // button 2 
          coinFlag = false;   
          waterIndicator_three = true;
          digitalWrite(coinIndicator,off);
        }
      }
      
//============START BUTTON 1 ========================
//Time counter for button 1
  if(waterIndicator_one) 
  {
      if (seconds_delay_1 == StandardSeconds)
        {
          minutes_delay_1++;
          seconds_delay_1 = 0;
          milliseconds_delay_1 = 0;    
        }
    
      if (minutes_delay_1 != TimeSettingPerWas)
        {
         digitalWrite(waterPin_Despense_one,on);    
        }
      
      else if (minutes_delay_1 == TimeSettingPerWas)
        {
         digitalWrite(waterPin_Despense_one,off); 
         minutes_delay_1 = 0;
         seconds_delay_1 = 0;
         milliseconds_delay_1 = 0;
         waterIndicator_one = false;       
        }
    
       //Enabling timer for button 1
         delay(milliseconds_realTime);
         milliseconds_delay_1++;  
     
      if (milliseconds_delay_1 ==  StandardMilliseconds)
        {
          milliseconds_delay_1 = 0;
          seconds_delay_1++;
        }
  }
//============END BUTTON 1 ========================

//============START BUTTON 2 ========================
//Time counter for button 2
  if(waterIndicator_two) 
  {
    if (seconds_delay_2 == StandardSeconds)
      {
        minutes_delay_2++;
        seconds_delay_2 = 0;
        milliseconds_delay_2 = 0;    
      }
    
    if (minutes_delay_2 != TimeSettingPerWas)
      {
        digitalWrite(waterPin_Despense_two,on);    
      }
    
    else if (minutes_delay_2 == TimeSettingPerWas)
    {
      digitalWrite(waterPin_Despense_two,off); 
      minutes_delay_2 = 0;
      seconds_delay_2 = 0;
      milliseconds_delay_2 = 0;
      waterIndicator_two = false; 
    }
    
    //Enabling timer for button 1
    delay(milliseconds_realTime);
    milliseconds_delay_2++;  
    
    if (milliseconds_delay_2 ==  StandardMilliseconds)
      {
        milliseconds_delay_2 = 0;
        seconds_delay_2++;
      }
    }
//============END BUTTON 2 ========================

//============START BUTTON 3 ========================
//Time counter for button 3
  if(waterIndicator_three) 
  {
    if (seconds_delay_3 == StandardSeconds)
      {
        minutes_delay_3++;
        seconds_delay_3 = 0;
        milliseconds_delay_3 = 0;    
      }
    
    if (minutes_delay_3 != TimeSettingPerWas)
      {
        digitalWrite(waterPin_Despense_three,on);    
      }
    
    else if (minutes_delay_3 == TimeSettingPerWas)
      {
        digitalWrite(waterPin_Despense_three,off); 
        minutes_delay_3 = 0;
        seconds_delay_3 = 0;
        milliseconds_delay_3 = 0;
        waterIndicator_three = false; 
      }
    
    //Enabling timer for button 1
    delay(milliseconds_realTime);
    milliseconds_delay_3++;  
    
    if (milliseconds_delay_3 ==  StandardMilliseconds)
      {
        milliseconds_delay_3 = 0;
        seconds_delay_3++;
      }
  }
//============END BUTTON 3 ========================

//controlling relay on/off
  if (waterIndicator_one || waterIndicator_two || waterIndicator_three)
      digitalWrite(relayPin,on);
  else
      digitalWrite(relayPin,off);
}
