//wyswietlacz
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>
#define OLED_RESET 5
Adafruit_SSD1306 display(OLED_RESET);
//silnik
#include <Arduino.h>
#include "BasicStepperDriver.h"
#define MOTOR_STEPS 200
#define MICROSTEPS 1
#define DIR 8
#define STEP 9
#define LUZ 13
BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP);

//STAŁE
#define BUTTON_TIME 150
#define BUTTON_OK 3 //PRZYCISK PULL_UP 
#define BUTTON_L 2
#define BUTTON_P 4
#define POZYCJE_MENU 12
#define MOTOR 5

//DEFINICJE FUNKCJI
void wyswietl_menu(String);
float wyswietl_podmenu(String,float);
void Go(float,float,float,float,float,float,float,float,float,float,float);
void zapisywanie(void);
void dalej(void);
void wylaczSilnik(void);
void stan(int );

//ZMIENNE:
int menu = 0;
String start="START";
String zap="ZAPISZ";
String warstwa_1="Warstwa 1";
String warstwa_2="Warstwa 2";
String warstwa_3="Warstwa 3";
String warstwa_4="Warstwa 4";
String warstwa_5="Warstwa 5";
String warstwa_6="Warstwa 6";
String warstwa_7="Warstwa 7";
String warstwa_8="Warstwa 8";
String warstwa_9="Warstwa 9";
String warstwa_10="Warstwa 10";
String predkosc="Predkosc";
String wyl_sil = "Wyl.Silnik";
float ustawienia[11];



void setup()   {                
  Serial.begin(9600);

  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  display.display();
  delay(2000);  
  display.clearDisplay();
  
  //przyciski
pinMode(BUTTON_OK, INPUT);
pinMode(BUTTON_L, INPUT);
pinMode(BUTTON_P, INPUT);
pinMode(MOTOR, INPUT);
digitalWrite(BUTTON_OK,HIGH);
digitalWrite(BUTTON_L,HIGH);
digitalWrite(BUTTON_P,HIGH);
digitalWrite(MOTOR,HIGH);

pinMode(LUZ, OUTPUT);

//wczytywanie zapisu
for(int x =0; x <11 ;x++){
  ustawienia[x] = EEPROM.read(x);
  delay(50);
}
}


void loop() {
  
  
  for(int c = 0;c<5;c++){     //WYSWIETLANIE PAMIECI
    Serial.println(EEPROM.read(c));
  }  
 
  if(digitalRead(BUTTON_L) == LOW){
    delay(BUTTON_TIME);
    if(menu == 0){
      menu=POZYCJE_MENU;
    }
    else{                               
      menu--;
    }
  }
  if(digitalRead(BUTTON_P) == LOW){
    delay(BUTTON_TIME);
    if(menu == POZYCJE_MENU){
      menu=0;
    }
    else{
      menu++;
    }    
  }  
   switch(menu){       //MENU
    case 0: wyswietl_menu(start); break;   
    case 1: wyswietl_menu(warstwa_1); break;
    case 2: wyswietl_menu(warstwa_2); break;
    case 3: wyswietl_menu(warstwa_3); break;
    case 4: wyswietl_menu(warstwa_4); break;
    case 5: wyswietl_menu(warstwa_5); break;
    case 6: wyswietl_menu(warstwa_6); break;
    case 7: wyswietl_menu(warstwa_7); break;
    case 8: wyswietl_menu(warstwa_8); break;
    case 9: wyswietl_menu(warstwa_9); break;
    case 10: wyswietl_menu(warstwa_10); break;
    case 11: wyswietl_menu(predkosc); break;
    case 12: wyswietl_menu(zap); break;
    //case 8: wyswietl_menu(wyl_sil); break;
   }   
   
   if(digitalRead(BUTTON_OK)==LOW){ ////PODMENU
     delay(BUTTON_TIME);
     switch(menu){
       case 0: Go(ustawienia[0],ustawienia[1],ustawienia[2],ustawienia[3],ustawienia[4],ustawienia[5],ustawienia[6],ustawienia[7],ustawienia[8],ustawienia[9],ustawienia[10]); break;
       case 1: ustawienia[0] = wyswietl_podmenu(warstwa_1 , ustawienia[0]); break;
       case 2: ustawienia[1] = wyswietl_podmenu(warstwa_2 , ustawienia[1]); break;
       case 3: ustawienia[2] = wyswietl_podmenu(warstwa_3 , ustawienia[2]); break;
       case 4: ustawienia[3] = wyswietl_podmenu(warstwa_4 , ustawienia[3]); break;
       case 5: ustawienia[4] = wyswietl_podmenu(warstwa_5 , ustawienia[4]); break;
       case 6: ustawienia[5] = wyswietl_podmenu(warstwa_6 , ustawienia[5]); break;
       case 7: ustawienia[6] = wyswietl_podmenu(warstwa_7 , ustawienia[6]); break;
       case 8: ustawienia[7] = wyswietl_podmenu(warstwa_8 , ustawienia[7]); break;
       case 9: ustawienia[8] = wyswietl_podmenu(warstwa_9 , ustawienia[8]); break;
       case 10: ustawienia[9] = wyswietl_podmenu(warstwa_10 , ustawienia[9]); break;
       case 11: ustawienia[10] = wyswietl_podmenu(predkosc , ustawienia[10]); break;
       case 12: zapisywanie();
               wyswietl_menu("zapisano!");
               delay(1000);
               break;  
       //case 8: wylaczSilnik(); break;              
     }
     delay(BUTTON_TIME);    
   } 
   
  if(digitalRead(MOTOR) == LOW){            //WYŁACZANIE SILNIKA PRZYCISKIEM NA PINIE 
    wylaczSilnik();
  }
}

//////////////FUNKCJE

void wyswietl_menu(String napis) {       ////////MENU
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(2,5);
  display.clearDisplay();
  display.println(napis);
  display.fillTriangle(0,28,   10,26,   10,30, WHITE);
  display.fillTriangle(128,28,   118,26,   118,30, WHITE);
  display.fillTriangle(64,30,   69,26,   59,26, WHITE);
  display.display();
  delay(1);
}



float wyswietl_podmenu(String napis, float wartosc) {  ///// PODMENU
  while(digitalRead(BUTTON_OK) == HIGH){    
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(2,7);
    display.clearDisplay();
    display.println(napis);
    display.setTextSize(1);
    display.setCursor(75,7);
    if(menu==11){
    display.println(wartosc);
    }
    else{
      display.println(wartosc/2);
    }
    display.fillTriangle(0,28,   10,26,   10,30, WHITE);
    display.fillTriangle(128,28,   118,26,   118,30, WHITE);
    display.fillTriangle(64,26,   69,30,   59,30, WHITE);
    display.display();
    delay(1);
    
    if(digitalRead(BUTTON_L) == LOW){
      delay(BUTTON_TIME-80);
      if(wartosc == 0){
        wartosc = 250;
      }
      else{                               
        wartosc--;
      }
    }
    if(digitalRead(BUTTON_P) == LOW){
      delay(BUTTON_TIME-80);
      if(wartosc == 250){
        wartosc=0;
      }
      else{
        wartosc++;
      }
    }
   }
  return wartosc;
}
void zapisywanie(void){
  for(int z = 0; z<11 ;z++){
  EEPROM.write(z, ustawienia[z]);
  delay(50);
  }
}
  
void Go(float w1,float w2,float w3,float w4,float w5,float w6,float w7,float w8,float w9,float w10, float rpm){  /////// SILNIK 
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(2,5);
  display.clearDisplay();
  display.println("Uruchomic nawijanie ?");
  display.fillTriangle(0,28,   10,26,   10,30, WHITE);
  display.fillTriangle(128,28,   118,26,   118,30, WHITE);
  //display.fillTriangle(64,30,   69,26,   59,26, WHITE);
  display.setCursor(17,24);
  display.println("NIE");
  display.setCursor(95,24);
  display.println("TAK");
  display.display();
  delay(1);
  
  while(digitalRead(BUTTON_P)==HIGH){
      if(digitalRead(BUTTON_L) == LOW){
        return;
      }
    }
    
  int licznik = 1;
  digitalWrite(LUZ,LOW);
  stepper.begin(rpm, 1);
  if(w1 != 0){
    stan(licznik);
    stepper.rotate(w1*180);
    stepper.disable();
    dalej();
    while(digitalRead(BUTTON_OK)==HIGH){
      if(digitalRead(BUTTON_L) == LOW){
        return;
      }
    }
    }
  licznik ++;
   delay(BUTTON_TIME);   
   if(w2 != 0){ 
    stan(licznik);
    stepper.rotate(w2*180);
    stepper.disable();
    dalej();
    while(digitalRead(BUTTON_OK)==HIGH){
      if(digitalRead(BUTTON_L) == LOW){
        return;
      }
  }
  }
  licznik ++;
    delay(BUTTON_TIME);
  if(w3 != 0){   
    stan(licznik);
    stepper.rotate(w3*180);
    stepper.disable();
    dalej();
    
  while(digitalRead(BUTTON_OK)==HIGH){
    if(digitalRead(BUTTON_L) == LOW){
      return;
    }
  }
  }
  licznik ++;
    delay(BUTTON_TIME);  
  if(w4 != 0){ 
    stan(licznik);
    stepper.rotate(w4*180);
    stepper.disable();
    dalej();
    
   while(digitalRead(BUTTON_OK)==HIGH){
    if(digitalRead(BUTTON_L) == LOW){
      return;
    }
  }
  }
  licznik ++;
   delay(BUTTON_TIME);   
   if(w5 != 0){ 
    stan(licznik);
    stepper.rotate(w5*180);
    stepper.disable();
    dalej();
    while(digitalRead(BUTTON_OK)==HIGH){
      if(digitalRead(BUTTON_L) == LOW){
        return;
      }
  }
  }
  licznik ++;
   delay(BUTTON_TIME);   
   if(w6 != 0){ 
    stan(licznik);
    stepper.rotate(w6*180);
    stepper.disable();
    dalej();
    while(digitalRead(BUTTON_OK)==HIGH){
      if(digitalRead(BUTTON_L) == LOW){
        return;
      }
  }
  }
  licznik ++;
   delay(BUTTON_TIME);   
   if(w7 != 0){ 
    stan(licznik);
    stepper.rotate(w7*180);
    stepper.disable();
    dalej();
    while(digitalRead(BUTTON_OK)==HIGH){
      if(digitalRead(BUTTON_L) == LOW){
        return;
      }
  }
  }
  licznik ++;
   delay(BUTTON_TIME);   
   if(w8 != 0){ 
    stan(licznik);
    stepper.rotate(w8*180);
    stepper.disable();
    dalej();
    while(digitalRead(BUTTON_OK)==HIGH){
      if(digitalRead(BUTTON_L) == LOW){
        return;
      }
  }
  }
  licznik ++;
   delay(BUTTON_TIME);   
   if(w9 != 0){ 
    stan(licznik);
    stepper.rotate(w9*180);
    stepper.disable();
    dalej();
    while(digitalRead(BUTTON_OK)==HIGH){
      if(digitalRead(BUTTON_L) == LOW){
        return;
      }
  }
  }
  licznik ++;
    delay(BUTTON_TIME); 
   if(w10 != 0){   
    stan(licznik);
    stepper.rotate(w10*180);
    stepper.disable();
    dalej();
   }
    licznik ++;
    
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10,5);
  display.clearDisplay();
  display.println("UKONCZONO");
  display.display();
  delay(1000); 
  display.clearDisplay(); 
}



void wylaczSilnik(void){
  //stepper.disable();
  digitalWrite(LUZ,HIGH);
}

void dalej(void){
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(2,5);
  display.clearDisplay();
  display.println("DALEJ");
  display.fillTriangle(0,28,   10,26,   10,30, WHITE);
  //display.fillTriangle(128,28,   118,26,   118,30, WHITE);
  display.fillTriangle(64,30,   69,26,   59,26, WHITE);
  display.display();
  delay(1);
}
void stan(int licznik){
      display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(2,7);
    display.clearDisplay();
    display.println("CEWKA ");
    display.setTextSize(2);
    display.setCursor(95,7);
    display.println(licznik);
    display.display();
    delay(1);
}
  
  
