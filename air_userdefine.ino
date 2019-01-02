
int coinPin = 2;
int userSwitch = 3;
int relay = 10;

void setup() {

  pinMode(relay, OUTPUT); 

}

void airPumping_userDefine()
{
    while (digitalRead(userSwitch))
      {
        digitalWrite(relay,0);
      }
     
}


void loop() {
 int coinslot = digitalRead(coinPin);
 
 Serial.println(coinslot);
 if (coinslot == 1)
 {
 digitalWrite(relay,0);
 delay(90000); //
 }


 else
   digitalWrite(relay,1);
    

if (digitalRead(userSwitch) == 1)
    airPumping_userDefine();
 }
