//Proyecto Puerta Seguridad Maxima
//Manolo Iniguez
//Sebastian Mena
//Jossua Guevara

#include <Keypad.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

//DispayLCD
LiquidCrystal_I2C lcd(0x3F, 16,2);

//Servomotor
Servo servomotor;

//Definiciones puertos
const int ledVERDE = 0;
const int ledROJO = 1;

const int triggerULTRA1 = 2;
const int echoULTRA1 = 3;
//const int triggerULTRA2 = ?;
//const int echoULTRA2 = ?;

const int buzzer = 9;

const int servo = 4;

const byte rowsTECLADO = 4; //four rows
const byte colsTECLADO = 4; //four columns

char keys[rowsTECLADO][colsTECLADO] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[rowsTECLADO] = {13, 12, 11, 10}; //connect to the row pinouts of the keypad
byte colPins[colsTECLADO] = {8, 7, 6, 5}; //connect to the column pinouts of the keypad

//Teclado
Keypad teclado = Keypad( makeKeymap(keys), rowPins, colPins, rowsTECLADO, colsTECLADO);
char TECLA;
char CLAVE[5];
char CLAVE_CORRECTA[5] = "0000";
byte indice = 0;
int estado = 0;

void setup() {
  Serial.begin(9600);

  servomotor.attach(servo);
  
  pinMode(buzzer,OUTPUT);
  
  pinMode(triggerULTRA1,OUTPUT);
  pinMode(echoULTRA1, INPUT);
  digitalWrite(triggerULTRA1, LOW);

  //pinMode(triggerULTRA2,OUTPUT);
  //pinMode(echoULTRA2, INPUT);
  //digitalWrite(triggerULTRA2, LOW);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  delay(3000);

  pinMode(ledVERDE, OUTPUT);
  pinMode(ledROJO, OUTPUT);

}

void loop(){
  
  //ULTRASONICO 1
  digitalWrite(triggerULTRA1, HIGH);
  delayMicroseconds(10);          //Enviamos un pulso de 10us
  digitalWrite(triggerULTRA1, LOW);

  long tiempo1;
  long distancia1;
  
  tiempo1 = pulseIn(echoULTRA1, HIGH);
  distancia1 = tiempo1/59;

  //ULTRASONICO 2
  //digitalWrite(triggerULTRA2, HIGH);
  //delayMicroseconds(10);          //Enviamos un pulso de 10us
  //digitalWrite(triggerULTRA2, LOW);

  //long tiempo2;
  //long distancia2;
  
  //tiempo2 = pulseIn(echoULTRA2, HIGH);
  //distancia2 = tiempo2/59;


  //TECLADO
  TECLA = teclado.getKey();
  
  if(TECLA){
    if(distancia1 <= 6){
      //if(distancia2 <= 6){
        CLAVE[indice] = TECLA;
        indice++;
        lcd.print(TECLA);
  
        if(indice == 4){
          if(!strcmp(CLAVE, CLAVE_CORRECTA)){
            abierto();
          }else{
            errorCLAVE();
           }
          indice = 0;
        }
        
      //}else{
        //errorDistanciaPuerta();
        //}
      
    }else{
      errorDistanciaTeclado();
      }
  }  
  

  //Cerrar puerta
  if(TECLA == "C" and estado == 1){
    cerrar();
    limpiar();
  }else{
    errorPuertaAbierta();
    }


  
}


void limpiar(){
  lcd.setCursor(0,0);
  lcd.print("CLAVE : ");
  lcd.setCursor(9,0); 
}

void errorCLAVE(){
  lcd.setCursor(0,0);
  lcd.print("CLAVE INCORRECTA");
  buzzerSuena();
  limpiar();
}

void abierto(){
  lcd.setCursor(0,1);
  lcd.print("PUERTA ABIERTA");
  ledVerdeON();
  servomotor.write(180);
  estado = 1;
}

void cerrar(){
  lcd.setCursor(0,0);
  lcd.print("PUERTA CERRADA");
  servomotor.write(0);
  estado = 0;
  ledVerdeON();
  limpiar();
}

void errorDistanciaTeclado(){
  lcd.setCursor(0,0);
  lcd.print("NADIE CERCA"); 
  lcd.setCursor(0,1);
  lcd.print("DEL TECLADO");
  buzzerSuena();
  ledRojoON();
  limpiar();
   
}

void errorDistanciaPuerta(){
  lcd.setCursor(0,0);
  lcd.print("ALGUIEN CERCA");
  lcd.setCursor(0,1);
  lcd.print("DEL OTRO LADO");
  buzzerSuena();
  ledRojoON();
  limpiar();
}

void errorPuertaAbierta(){
  lcd.setCursor(0,0);
  lcd.print("ERROR PUERTA");
  lcd.setCursor(0,1);
  lcd.print("ABIERTA");
  ledRojoON();
  buzzerSuena();
}

void buzzerSuena(){
  digitalWrite(buzzer,HIGH);
  delay(2000);
  digitalWrite(buzzer,LOW);
}

void ledVerdeON(){
  digitalWrite(ledVERDE, HIGH);
  delay(3000);
  digitalWrite(ledVERDE, LOW);
}

void ledRojoON(){
  digitalWrite(ledROJO, HIGH);
  delay(3000);
  digitalWrite(ledROJO, LOW);
}
