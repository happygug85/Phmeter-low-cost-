#include <EEPROM.h>
int stato = 0;
int pulsanteAGG = 7;
int pulsanteSOTT = 6;
int pulsanteS = 5;
bool FIRST = true;
bool PRIMO = true;
bool PRIMAVOLTA = true;
float misura;
float ph;
float phnew;
float offset=21.56;
unsigned long before = 0;


void setup() {
  Serial.begin(9600);
pinMode(7, INPUT); //pin pulsante per aumentare il valore nel menu calibrazione
pinMode(6, INPUT); //pin pulsante per diminuire il valore nel menu calibrazione
pinMode(5, INPUT); //pin pulsante per salvare dati nella eeprom nel menu calibrazione
  pinMode(8, INPUT); //pin pulsante per cambiare menu

}

void loop() {
  delay(200);
  switch (stato) { //menu iniziale di benvenuto
    case 0:
      welcome();
    PRIMO = true;
        break; 
    case 10:        //menu di lettura ph
      lettura_ph();
    break; 
    case 100:       //menu di calibrazione manuale
    calibrazione();
    break; 
  
  }
}

void welcome() {
if (FIRST) {
Serial.println("benvenuto");
FIRST = false;
  
}

if (PRIMAVOLTA) {
EEPROM.put(0,offset); //salva il valore di offset 21.56 nella eeprom, solo all'accensione
PRIMAVOLTA = false;
  
}
  if (digitalRead(8)) {
stato = 10; //premendo il tasto pin 8 passa al menu lettura
    delay(100);
  
}
}

void misph() {

// codice che fa la media
   for(int i=0; i<10; i++) { 
    misura += analogRead(A0);
    delay (10);  }
  
  misura = misura / 10.0; 

ph = (float)misura * (5.0/1024)*0.897;
  
  
  phnew= -5.70 * ph + offset;
  Serial.print("    pH:");  
  Serial.print(phnew, 2);
  Serial.println(" "); 
    
/* codice che legge il ph da singola misurazione della tensione analogica
misura = analogRead(A0);
 ph = (float)misura * (5.0/1024)*0.897;
  
  
  phnew= -5.70 * ph + offset;
  Serial.print("    pH:");  
  Serial.print(phnew, 2);
  Serial.println(" ");
  
  delay(200); */
}

void lettura_ph() {
  
  if (FIRST==false) {
Serial.println("lettura ph");
EEPROM.get(0,offset); //legge il valore salvato nella eeprom
Serial.println(offset); //stampa il valore di offset salvato nella eeprom, alla prima accensione dovrebbe uscire 21.56 o comunque senza aver eseguito la calibrazione
FIRST = true; //stampa solo 1 volta il titolo del menu
  
}
  
  if (digitalRead(8)) {
stato = 100; //passa allo stato del menu calibrazione
    delay(100);
  
}

  if (digitalRead(5)) {
Serial.println(offset); //premendo il tasto collegato al pin 5 stampa il valore di offset
    delay(100);
  
}
  int diff;
  diff = millis() - before;
 
if (diff >= 1000)  {
 //inserire il codice per la lettura ph (inizio)
  misph();

  // codice lettura ph (fine)
  before = millis();
 } 
 } 
void scrittura(){ //funzione incaricata di scrivere sulla eeprom
 
 EEPROM.put(0, offset);
 Serial.println("scrittura completata");
stato=0;
}


void calibrazione() {

if (PRIMO) {
  Serial.println("calibrazione");
        Serial.println("offset=");
    Serial.println(offset); //stampa a terminale il valore preimpostato    
        PRIMO = false;      
        
    }
    



  if (digitalRead(8)) {
stato = 0;
    delay(200);
 
}
 
 if (digitalRead(7) == HIGH) {
delay(100);
   offset = offset + 0.05;
   Serial.println(offset);
   
  }

 if (digitalRead(6) == HIGH) {
delay(100);
   offset = offset - 0.05;
   Serial.println(offset);
    
  
  }
   
  if (digitalRead(5)==HIGH){

 scrittura();
 delay(200);
 


}
 
 

}

  
