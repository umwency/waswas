

#include <Wire.h>
#define eeprom 0x50 //defines the base address of the EEPROM

int resetEPROMContent = 4;
int ViewEPROMContent = 5;
int pinInput = 6;
int pinOutput = 8;
int initValue = 0;
int EEPROM_Read_And_Write_address = 0;
boolean buttonCheck = false;
int pressCount = 0;


void setup() {
  Wire.begin(); 
  Serial.begin(9600);
  pinMode(resetEPROMContent, INPUT);
  pinMode(ViewEPROMContent, INPUT);
  pinMode(pinInput, INPUT);
  pinMode(pinOutput, OUTPUT);
  digitalWrite(pinOutput, initValue);
  if(digitalRead(resetEPROMContent) == 1) // if no data available else skip
    EEprom_WriteData(initValue,eeprom,EEPROM_Read_And_Write_address);
}


void loop() {

 if(digitalRead(pinInput) == 1){
   buttonCheck = true;

    if (buttonCheck && !pressCount)   
     {
       EEprom_WriteData(EEprom_ReadData(eeprom,EEPROM_Read_And_Write_address) + 1,eeprom,EEPROM_Read_And_Write_address);
       Serial.print("\n EEProm Data: ");
       Serial.print(EEprom_ReadData(eeprom,EEPROM_Read_And_Write_address));
       digitalWrite(pinOutput,1); 
       pressCount = 1;
       buttonCheck = false;
        
     }
 }   
 else
    {  
      digitalWrite(pinOutput,0);
      pressCount = 0;
        
    }

   if(digitalRead(ViewEPROMContent) == 1) 
   {
      int content = EEprom_ReadData(eeprom,EEPROM_Read_And_Write_address);
      for (int i = 0; i < content; i++) {
      digitalWrite(pinOutput,1); 
      delay(1000);
      digitalWrite(pinOutput,0);
      delay(1000); 
      }
    
   }

  
}
void EEprom_WriteData(int data, int EpromBase_Address, int WritingAddress){
  Wire.beginTransmission(EpromBase_Address);
  Wire.write((int)(WritingAddress >> 8));  //writes the MSB
  Wire.write((int)(WritingAddress & 0xFF)); //writes the LSB
  Wire.write(data);
  Wire.endTransmission();
  delay(5);
}


int EEprom_ReadData(int EpromBase_Address, int ReadingAddress){
  int readdata = 0;
  Wire.beginTransmission(EpromBase_Address);
  Wire.write((int)(ReadingAddress >> 8)); //writes the MSB
  Wire.write((int)(ReadingAddress & 0xFF)); //writes the LSB
  Wire.endTransmission();
  Wire.requestFrom(EpromBase_Address,1);
  readdata = Wire.read();
  return readdata;
}
