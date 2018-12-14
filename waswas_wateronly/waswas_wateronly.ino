//updated 12.12.18

#include <EEPROM.h>
    int TimeSettingPerWas = 1; // Total washing duration time - change on how many munites the total washing during per cycle
    int StandardSeconds = 5; // 1 minutes in 60 seconds // change it to 60 on actual hardware
    int StandardMilliseconds = 50; // 1 seconds is 1000 millisecods and the loop is rotating ones every milliseconds;
    
    

    int milliseconds_realTime = 1; // the loop method execute at exactly .0001 milliseconds per cycle
    
    /*
     this setting will set on what is hardware that will run
     */
    boolean waterFlag = true;
    boolean airFlag = false;
    boolean soapFlag = true;
   
    
    boolean coinFlag = false; // this will detect if there is coin inserted
    int coinIndicator = A0; // will turn on if coin is detected and off once user select any of the services offered (water,air,soap)
    int on = 1, off = 0;

    /*
     * This is the input buttons of the user to select services (water, air, soap).
     */
    int coinPin = 0; // IC PIN 2 coin slot
    int waterPin_one = 1; // IC PIN 3 Water wash button 1
    int waterPin_two = 2; // IC PIN 4 Water wash button 2
    int waterPin_three = 3; // IC PIN 5 Water wash button 3
    int soapPin = 4; // soap button  - IC pin# 6 
    int airPin = 5;  // air button  - IC pin# 11 
    
    /*
     * Pin configuration for the water depensing (open and closing the solinoid faucet)
     */
    int waterPin_Despense_one = 8; // IC PIN 11 Water wash despensing 1
    int waterPin_Despense_two = 9; // IC PIN 12 Water wash despensing 2
    int waterPin_Despense_three = 10; // IC PIN 13 Water wash despensing 3
    


  /*
   * Pin configuration for soap mixing 
   */
   int WaterSoap = A3; // water despens to mixing container
   int LiquidSoap = A4; // liquid detergent despens to mixing container
   int MixingProcess = A5; // mixing process ( dynamo or servo motor )

   int Mixingindicator = A2; // turn the lights on when mixing is in progress
    

    // controls the open and closing the relay for water pump
    // this contains all the pump out of the services
    int relayPin = 13; // IC PIN 19 This controls the relay
    int soapPin_Despense = 12; // IC PIN 18
    int airPin_Despense = 11; // IC PIN 17
    
    
    boolean waterIndicator_one = false;
    boolean waterIndicator_two = false;
    boolean waterIndicator_three = false;
    boolean soapIndicator = false;

    /*
     * EEPROM address to determine the number of use before the system locked out.
     */
    int eePromAddress_Water_1 = 10;
    int eePromAddress_Water_2 = 11;
    int eePromAddress_Water_3 = 12;
    int eePromAddress_CoinDropADR = 15;
    

    int minutes_delay_1,minutes_delay_2,minutes_delay_3 = 0;
    int seconds_delay_1,seconds_delay_2,seconds_delay_3 = 0;
    int milliseconds_delay_1,milliseconds_delay_2,milliseconds_delay_3 = 0;

    /*
     * Soap Time despensing
     */
    int soapdespenseTime = 2; // this is express in secondes
    int soapSeconds_delay = 0;
    int soapMillisencods_delay = 0; // this should be set to 1000 in actual hardware - 1 seconds is equal to 1000 ms

//This is a 3 input 3 out water wash
    void setup() {
    // Setting up pin input/output
     pinMode(coinPin, INPUT); // coin - IC pin# 2 
     pinMode(waterPin_one, INPUT); // water button 1 - IC pin# 3 
     pinMode(waterPin_two, INPUT);// water button 2 - IC pin# 4
     pinMode(waterPin_three, INPUT);// water button 3 - IC pin# 5 
     pinMode(soapPin, INPUT);// soap button  - IC pin# 6 
     pinMode(airPin, INPUT);// air button  - IC pin# 11 

     /* Mixing process I/O */
     pinMode(WaterSoap, OUTPUT); // 
     pinMode(LiquidSoap, OUTPUT); //
     pinMode(MixingProcess, OUTPUT); //
     pinMode(Mixingindicator, OUTPUT); // 

        
     pinMode(waterPin_Despense_one, OUTPUT); // Solinoid faucet 1 - IC pin# 14
     pinMode(waterPin_Despense_two, OUTPUT); // Solinoid faucet 2 - IC pin# 15
     pinMode(waterPin_Despense_three, OUTPUT); // Solinoid faucet 3 - IC pin# 16
     
     pinMode(relayPin, OUTPUT); // relay water control - IC pin# 19
     pinMode(soapPin_Despense, OUTPUT); //  soap despense - IC pin# 18
     pinMode(airPin_Despense, OUTPUT); // sir desponse - IC pin# 17
     
     

     /*
     EEPROM.write(eePromAddress_Water_1, 0);
     EEPROM.write(eePromAddress_Water_2, 0);
     EEPROM.write(eePromAddress_Water_3, 0);
     EEPROM.write(eePromAddress_CoinDropADR, 0);
     */
     pinMode(coinIndicator,OUTPUT);
}   

void loop() {
if (!waterIndicator_one || !waterIndicator_two || !waterIndicator_three || !soapIndicator)
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
        if (digitalRead(waterPin_one) == 1){  // button 1 -  water button 1
          coinFlag = false;  
          waterIndicator_one = true; 
          digitalWrite(coinIndicator,off);
      }
    } 
    if (coinFlag)
      {
        if (digitalRead(waterPin_two) == 1){  // button 2 -  water button 2
          coinFlag = false; 
          waterIndicator_two = true;  
          digitalWrite(coinIndicator,off);
        }
      }
    if (coinFlag)
      {
        if (digitalRead(waterPin_three) == 1){  // button 3 - water button 3 
          coinFlag = false;   
          waterIndicator_three = true;
          digitalWrite(coinIndicator,off);
        }
      }

     if (coinFlag)
      {
        if (digitalRead(soapPin) == 1){  // button 4 - Soap button 3 
          coinFlag = false;   
          soapIndicator = true;
          digitalWrite(coinIndicator,off);
          mixingprocessSoap();
        }
        
      }    

//============START Soap  ========================
//Time counter for button 1
  if(soapIndicator && soapFlag) 
    {
    
     soapMillisencods_delay++;
     digitalWrite(soapPin_Despense,on);
     
    if (soapSeconds_delay == soapdespenseTime)
      {
        digitalWrite(soapPin_Despense,off);
        soapMillisencods_delay = 0;
        soapSeconds_delay = 0;
        soapIndicator = false;
      }
     
    if (soapMillisencods_delay == StandardMilliseconds)
      {
        soapMillisencods_delay = 0;
        soapSeconds_delay++;
      }  
    }
//============END Soap ========================

       
//============START BUTTON 1 ========================
//Time counter for button 1
  if(waterIndicator_one && waterFlag) 
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
    
         milliseconds_delay_1++;  // setting timer for each service in milliseconds counting
     
      if (milliseconds_delay_1 ==  StandardMilliseconds)
        {
          milliseconds_delay_1 = 0;
          seconds_delay_1++;
        }
  }
//============END BUTTON 1 ========================

//============START BUTTON 2 ========================
//Time counter for button 2
  if(waterIndicator_two && waterFlag) 
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
    
      milliseconds_delay_2++;  // setting timer for each service in milliseconds counting
    
    if (milliseconds_delay_2 ==  StandardMilliseconds)
      {
        milliseconds_delay_2 = 0;
        seconds_delay_2++;
      }
    }
//============END BUTTON 2 ========================

//============START BUTTON 3 ========================
//Time counter for button 3
  if(waterIndicator_three && waterFlag) 
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
    

    milliseconds_delay_3++;  // setting timer for each service in milliseconds counting
    
    if (milliseconds_delay_3 ==  StandardMilliseconds)
      {
        milliseconds_delay_3 = 0;
        seconds_delay_3++;
      }
  }
//============END BUTTON 3 ========================




//controlling water relay on/off
  if ((waterIndicator_one || waterIndicator_two || waterIndicator_three) && waterFlag)
  
      digitalWrite(relayPin,on);
  else
      digitalWrite(relayPin,off);

    //Enabling timer delay cycle... each cycle is 1 milliseconds
    delay(milliseconds_realTime);


}

void mixingprocessSoap()
{
  int timerWater_Despense_to_mixing = 100; // 1000 = 1 seconds
  int timerSoap_Despense_to_mixing = 100; // 1000 = 1 seconds
  int timerMixingProcess  = 100; // mixing 

 digitalWrite(Mixingindicator,on);
  
 digitalWrite(WaterSoap,on);
 delay(timerWater_Despense_to_mixing);
 digitalWrite(WaterSoap,off);
 delay(10); 
 /*
 digitalWrite(LiquidSoap,on);
 delay(timerSoap_Despense_to_mixing);
 digitalWrite(LiquidSoap,off);
 delay(10);
 
 digitalWrite(MixingProcess,on);
 delay(timerMixingProcess);
 digitalWrite(MixingProcess,off);
 delay(10);
 */
 digitalWrite(Mixingindicator,off);
 delay(20);


}
