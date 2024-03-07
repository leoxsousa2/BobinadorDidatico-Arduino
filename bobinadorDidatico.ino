// C++ code
//
/*                                                        */
/*     driver A4988 e DRV8825                             */
/*         ,-----------,                                  */
/*  ENABLE |o         o| V_MOTORE                         */
/*     MS1 |o ____    o| GND                              */
/*     MS2 |o|    |   o| 2B                               */
/*     MS3 |o|____|   o| 2A                               */
/*   RESET |o         o| 1A                               */
/*   SLEEP |o         o| 1B                               */
/*    STEP |o         o| VDD                              */
/*     DIR |o         o| GND                              */
/*         `-----------'                                  */
/*                                                        */
/*    ==========================================          */
/*     MS1 | MS2  |  MS3 |  Microstep resolution          */
/*    -----|------|------|----------------------          */
/*    Low  | Low  | Low  |  Full step |  200              */
/*    High | Low  | Low  |  1/2 step  |  400              */
/*    Low  | High | Low  |  1/4 step  |  800              */
/*    High | High | Low  |  1/8 step  |  1600             */
/*    High | High | High |  1/16 step |  3200             */
/*    =========================================           */
/*########################################################*/

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Stepper.h>
#include <Servo.h>

Servo myservo; 


int Espiras = 0;
int tela = 0;
int imprimir = "0";
int vel = 0;
int contservo = 0;
int contServInit = 0;
int angServoFinal = 0;

//Config inicial para tenpo de clique para botoes
unsigned long tIni = 0;
unsigned long tBt1 = 0;
unsigned long tBt2 = 0;
unsigned long tBt3 = 0;
unsigned long tBt4 = 0;
bool Bt1;
bool Bt2;
bool Bt3;
bool Bt4;
bool BtAnt1;
bool BtAnt2;
bool BtAnt3;
bool BtAnt4;

LiquidCrystal_I2C lcd(0x27,16,2);

const int stepsPerRevolution = 3200;
Stepper myStepper(stepsPerRevolution, 8, 7, 9, 6);

void (*funcReset) () = 0;
//Para resetar use fun funcReset();

void setup() {
  myservo.attach(11);
  myservo.write(0);
 
  pinMode(A1, OUTPUT);//MS1 do A4988
  pinMode(A2, OUTPUT);//MS2 do A4988
  pinMode(A3, OUTPUT);//MS3 do A4988
  digitalWrite(A1, HIGH);//MS3 do A4988 Ativar
  digitalWrite(A2, HIGH);//MS3 do A4988 Ativar
  digitalWrite(A3, LOW);//MS3 do A4988 Desativar

  
  pinMode(2, INPUT_PULLUP);//botoes
  pinMode(3, INPUT_PULLUP);//
  pinMode(4, INPUT_PULLUP);//
  pinMode(5, INPUT_PULLUP);//
  pinMode(13, OUTPUT);//LEDinterno
  pinMode(10, OUTPUT);//Enable A4988
  pinMode(11, OUTPUT);//Step ServoMotor


  digitalWrite(10, HIGH);
  //digitalWrite(11, LOW);

  lcd.init();
  lcd.backlight();

//parte de Market do projeto
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Projeto:");
  lcd.setCursor(0,1);
  lcd.print("Fisica Na Escola");
  delay(5000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Apoio: SEDUC-SE");
  lcd.setCursor(0,1);
  lcd.print("       FAPITEC");
  delay(5000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Desenvolvedores:");
  lcd.setCursor(0,1);
  lcd.print("Fabio Leo Jane");
  delay(5000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Supervisor:");
  lcd.setCursor(0,1);
  lcd.print("Prof. Edvaldo");
  delay(5000);
//parte de Market do projeto
//*/
  tela = 1;
  imprimir = "sim";

}

void loop() {
  Bt1 = !digitalRead(2); /*Ao apertar o botao*/if (Bt1 && !BtAnt1) {if (tIni == 0) {tIni = millis();/*digitalWrite(led, LOW);*/}}/*Ao soltar o botao*/if (tIni > 50) {if (!Bt1 && BtAnt1) {tBt1 = millis() - tIni; tIni = 0;/*digitalWrite(led, LOW);*/}}
  Bt2 = !digitalRead(3); /*Ao apertar o botao*/if (Bt2 && !BtAnt2) {if (tIni == 0) {tIni = millis();/*digitalWrite(led, LOW);*/}}/*Ao soltar o botao*/if (tIni > 50) {if (!Bt2 && BtAnt2) {tBt2 = millis() - tIni; tIni = 0;/*digitalWrite(led, LOW);*/}}
  Bt3 = !digitalRead(4); /*Ao apertar o botao*/if (Bt3 && !BtAnt3) {if (tIni == 0) {tIni = millis();/*digitalWrite(led, LOW);*/}}/*Ao soltar o botao*/if (tIni > 50) {if (!Bt3 && BtAnt3) {tBt3 = millis() - tIni; tIni = 0;/*digitalWrite(led, LOW);*/}}
  Bt4 = !digitalRead(5); /*Ao apertar o botao*/if (Bt4 && !BtAnt4) {if (tIni == 0) {tIni = millis();/*digitalWrite(led, LOW);*/}}/*Ao soltar o botao*/if (tIni > 50) {if (!Bt4 && BtAnt4) {tBt4 = millis() - tIni; tIni = 0;/*digitalWrite(led, LOW);*/}}

  if ((tela == 1) && (imprimir == "sim")) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Quantas espiras?");
    lcd.setCursor(0,1);
    lcd.print(Espiras);
    imprimir = "nao";
    }
    //Botão 1                 -------------------------------------------//----------------------------------------------
    else if ((tela == 1) && (imprimir == "nao") && (tBt1 > 50) && (tBt1 <= 300)) {tBt1 = 0; 
      Espiras++;
      lcd.setCursor(0,0);
      lcd.print("Quantas espiras?");
      lcd.setCursor(0,1);
      lcd.print(Espiras);
      imprimir = "sim";
    }
    else if ((tela == 1) && (imprimir == "nao") && (tBt1 > 400) && (tBt1 <= 1000)) {tBt1 = 0; 
      Espiras = Espiras - 1;
      lcd.setCursor(0,0);
      lcd.print("Quantas espiras?");
      lcd.setCursor(0,1);
      lcd.print(Espiras);
    }
    //Botão 2                 -------------------------------------------//----------------------------------------------
    else if ((tela == 1) && (imprimir == "nao") && (tBt2 > 50) && (tBt2 <= 300)) {tBt2 = 0; 
      Espiras = Espiras * 10;
      lcd.setCursor(0,0);
      lcd.print("Quantas espiras?");
      lcd.setCursor(0,1);
      lcd.print(Espiras);
    }
    else if ((tela == 1) && (imprimir == "nao") && (tBt2 > 400) && (tBt2 <= 1000)) {tBt2 = 0; 
      Espiras = Espiras / 10;
      lcd.setCursor(0,0);
      lcd.print("Quantas espiras?");
      lcd.setCursor(0,1);
      lcd.print(Espiras);
    }
    //Botão 3                 -------------------------------------------//----------------------------------------------
    else if ((tela == 1) && (imprimir == "nao") && (tBt3 > 50) && (tBt3 <= 300)) {tBt3 = 0; 
      Espiras = 0;
      lcd.setCursor(0,0);
      lcd.print("Quantas espiras?");
      lcd.setCursor(0,1);
      lcd.print("0   ");
    }
    else if ((tela == 1) && (imprimir == "nao") && (tBt3 > 400) && (tBt3 <= 1000)) {tBt3 = 0; 
    //
    }
    //Botão 4                 -------------------------------------------//----------------------------------------------
    else if ((tela == 1) && (imprimir == "nao") && (tBt4 > 50) && (tBt4 <= 300)) {tBt4 = 0; 
      tela = 2;
      imprimir = "sim";
    }
    else if ((tela == 1) && (imprimir == "nao") && (tBt4 > 400) && (tBt4 <= 1000)) {tBt3 = 0; 
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Reiniciando...");
      delay(5000);
      funcReset(); //resetar o arduino
    }

  if ((tela == 2) && (imprimir == "sim")) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Qual velocidade?");
    lcd.setCursor(0,1);
    lcd.print("          max=70");
    lcd.setCursor(0,1);
    lcd.print(vel);
    imprimir = "nao";
    }
    //Botão 1                 -------------------------------------------//----------------------------------------------
    else if ((tela == 2) && (imprimir == "nao") && (tBt1 > 50) && (tBt1 <= 300)) {tBt1 = 0; 
      vel++;
      lcd.setCursor(0,0);
      lcd.print("Qual velocidade?");
      lcd.setCursor(0,1);
      lcd.print(vel);
      imprimir = "sim";
    }
    else if ((tela == 2) && (imprimir == "nao") && (tBt1 > 400) && (tBt1 <= 1000)) {tBt1 = 0; 
      vel = vel - 1;
      lcd.setCursor(0,0);
      lcd.print("Qual velocidade?");
      lcd.setCursor(0,1);
      lcd.print(vel);
    }
    //Botão 2                 -------------------------------------------//----------------------------------------------
    else if ((tela == 2) && (imprimir == "nao") && (tBt2 > 50) && (tBt2 <= 300)) {tBt2 = 0; 
      vel = vel * 10;
      lcd.setCursor(0,0);
      lcd.print("Qual velocidade?");
      lcd.setCursor(0,1);
      lcd.print(vel);
    }
    else if ((tela == 2) && (imprimir == "nao") && (tBt2 > 400) && (tBt2 <= 1000)) {tBt2 = 0; 
      vel = vel / 10;
      lcd.setCursor(0,0);
      lcd.print("Qual velocidade?");
      lcd.setCursor(0,1);
      lcd.print(vel);
    }
    //Botão 3                 -------------------------------------------//----------------------------------------------
    else if ((tela == 2) && (imprimir == "nao") && (tBt3 > 50) && (tBt3 <= 300)) {tBt3 = 0; 
      vel = 0;
      lcd.setCursor(0,0);
      lcd.print("Qual velocidade?");
      lcd.setCursor(0,1);
      lcd.print("0   ");
    }
    else if ((tela == 2) && (imprimir == "nao") && (tBt3 > 400) && (tBt3 <= 1000)) {tBt3 = 0; 
    //
    }
    //Botão 4                 -------------------------------------------//----------------------------------------------
    else if ((tela == 2) && (imprimir == "nao") && (tBt4 > 50) && (tBt4 <= 300)) {tBt4 = 0; 
      tela = 3;
      imprimir = "sim";
    }
    else if ((tela == 2) && (imprimir == "nao") && (tBt4 > 400) && (tBt4 <= 1000)) {tBt3 = 0; 
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Reiniciando...");
      delay(5000);
      funcReset(); //resetar o arduino
    }


if ((tela == 3) && (imprimir == "sim")) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Angulo inicial?");
    lcd.setCursor(0,1);
    lcd.print("     range=0a160");
    lcd.setCursor(0,1);
    lcd.print(contservo);
    imprimir = "nao";
    }
    //Botão 1                 -------------------------------------------//----------------------------------------------
    else if ((tela == 3) && (imprimir == "nao") && (tBt1 > 50) && (tBt1 <= 300)) {tBt1 = 0; 
      contservo++;
      contServInit++;
      myservo.write(contServInit);
      lcd.setCursor(0,0);
      lcd.print("Angulo inicial?");
      lcd.setCursor(0,1);
      lcd.print(contservo);
      imprimir = "sim";
    }
    else if ((tela == 3) && (imprimir == "nao") && (tBt1 > 400) && (tBt1 <= 1000)) {tBt1 = 0; 
      contservo--;
      contServInit--;
      myservo.write(contServInit);
      lcd.setCursor(0,0);
      lcd.print("Angulo inicial?");
      lcd.setCursor(0,1);
      lcd.print(contservo);
    } 
    //Botão 2                 -------------------------------------------//----------------------------------------------
    else if ((tela == 3) && (imprimir == "nao") && (tBt2 > 50) && (tBt2 <= 300)) {tBt2 = 0; 
      contservo = contservo * 10;
      contServInit = contServInit * 10;
      myservo.write(contServInit);
      lcd.setCursor(0,0);
      lcd.print("Angulo inicial?");
      lcd.setCursor(0,1);
      lcd.print(contservo);
    }
    else if ((tela == 3) && (imprimir == "nao") && (tBt2 > 400) && (tBt2 <= 1000)) {tBt2 = 0; 
      contservo = contservo / 10;
      contServInit = contServInit / 10;
      myservo.write(contServInit);
      lcd.setCursor(0,0);
      lcd.print("Angulo inicial?");
      lcd.setCursor(0,1);
      lcd.print(contservo);
    }
    //Botão 3                 -------------------------------------------//----------------------------------------------
    else if ((tela == 3) && (imprimir == "nao") && (tBt3 > 50) && (tBt3 <= 300)) {tBt3 = 0; 
      contservo = 0;
      contServInit = 0;
      myservo.write(contServInit);
      lcd.setCursor(0,0);
      lcd.print("Angulo inicial?");
      lcd.setCursor(0,1);
      lcd.print("0   ");
    }
    else if ((tela == 3) && (imprimir == "nao") && (tBt3 > 400) && (tBt3 <= 1000)) {tBt3 = 0; 
    //
    }
    //Botão 4                 -------------------------------------------//----------------------------------------------
    else if ((tela == 3) && (imprimir == "nao") && (tBt4 > 50) && (tBt4 <= 300)) {tBt4 = 0; 
      tela = 4;
      myservo.write(contServInit);
      imprimir = "sim";
    }
    else if ((tela == 3) && (imprimir == "nao") && (tBt4 > 400) && (tBt4 <= 1000)) {tBt3 = 0; 
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Reiniciando...");
      delay(5000);
      funcReset(); //resetar o arduino
    }



if ((tela == 4) && (imprimir == "sim")) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Angulo final?");
    lcd.setCursor(0,1);
    lcd.print("     range=0a160");
    lcd.setCursor(0,1);
    lcd.print(angServoFinal);
    imprimir = "nao";
    }
    //Botão 1                 -------------------------------------------//----------------------------------------------
    else if ((tela == 4) && (imprimir == "nao") && (tBt1 > 50) && (tBt1 <= 300)) {tBt1 = 0; 
      angServoFinal++;
      myservo.write(angServoFinal);
      lcd.setCursor(0,0);
      lcd.print("Angulo final?");
      lcd.setCursor(0,1);
      lcd.print(angServoFinal);
      imprimir = "sim";
    }
    else if ((tela == 4) && (imprimir == "nao") && (tBt1 > 400) && (tBt1 <= 1000)) {tBt1 = 0; 
      angServoFinal--;
      myservo.write(angServoFinal);
      lcd.setCursor(0,0);
      lcd.print("Angulo final?");
      lcd.setCursor(0,1);
      lcd.print(angServoFinal);
    }
    //Botão 2                 -------------------------------------------//----------------------------------------------
    else if ((tela == 4) && (imprimir == "nao") && (tBt2 > 50) && (tBt2 <= 300)) {tBt2 = 0; 
      angServoFinal = angServoFinal * 10;
      myservo.write(angServoFinal);
      lcd.setCursor(0,0);
      lcd.print("Angulo final?");
      lcd.setCursor(0,1);
      lcd.print(angServoFinal);
    }
    else if ((tela == 4) && (imprimir == "nao") && (tBt2 > 400) && (tBt2 <= 1000)) {tBt2 = 0; 
      angServoFinal = angServoFinal / 10;
      myservo.write(angServoFinal);
      lcd.setCursor(0,0);
      lcd.print("Angulo final?");
      lcd.setCursor(0,1);
      lcd.print(angServoFinal);
    }
    //Botão 3                 -------------------------------------------//----------------------------------------------
    else if ((tela == 4) && (imprimir == "nao") && (tBt3 > 50) && (tBt3 <= 300)) {tBt3 = 0; 
      angServoFinal = 0;
      myservo.write(angServoFinal);
      lcd.setCursor(0,0);
      lcd.print("Angulo final?");
      lcd.setCursor(0,1);
      lcd.print("0   ");
    }
    else if ((tela == 4) && (imprimir == "nao") && (tBt3 > 400) && (tBt3 <= 1000)) {tBt3 = 0; 
    //
    }
    //Botão 4                 -------------------------------------------//----------------------------------------------
    else if ((tela == 4) && (imprimir == "nao") && (tBt4 > 50) && (tBt4 <= 300)) {tBt4 = 0; 
      tela = 5;
      myservo.write(angServoFinal);
      imprimir = "sim";
    }
    else if ((tela == 4) && (imprimir == "nao") && (tBt4 > 400) && (tBt4 <= 1000)) {tBt3 = 0; 
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Reiniciando...");
      delay(5000);
      funcReset(); //resetar o arduino
    }



  if ((tela == 5) && (imprimir == "sim")) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Executar?");
    imprimir = "nao";
    }
  //Botão 4                 -------------------------------------------//----------------------------------------------
    else if ((tela == 5) && (imprimir == "nao") && (tBt4 > 50) && (tBt4 <= 300)) {tBt4 = 0;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Executanto...");
      lcd.setCursor(8,1);
      lcd.print("esp= 0");

      digitalWrite(10, LOW);
      
      myservo.write(contServInit);

      int espiras = Espiras - 1;
      int passosParaAcelerar = 49;
      for (int volta = 0; volta < 1; volta++) {
        for (int aceleracao = 5; aceleracao < vel;) {
          aceleracao += 1;
          lcd.setCursor(0,1);
          lcd.print("Vel= ");
          lcd.print(aceleracao);
          myStepper.setSpeed(aceleracao);
          myStepper.step(1 * passosParaAcelerar);
        }
      }

      myservo.write(contServInit);

      lcd.setCursor(8,1);
      lcd.print("esp= 1");

      
      int contSinal = "+";
      for (int volta = 0; volta < espiras; volta++) {
        lcd.setCursor(8,1);
        lcd.print("esp= ");
        lcd.print(volta + 2);
        myStepper.setSpeed(vel);
        myStepper.step(1 * stepsPerRevolution);
        //Vai e vem do servo motor
        if (contSinal == "+") {contservo = contservo + 2; myservo.write(contservo);}
        if (contSinal == "-") {contservo = contservo - 2; myservo.write(contservo);}
        if (contservo == angServoFinal)  {contSinal = "-";} if (contservo == contServInit) {contSinal = "+";}
        // fim
      }
      lcd.setCursor(0,0);
      lcd.print("Pronto!      ");
      lcd.setCursor(0,1);
      lcd.print("Vel= 0 ");
      myservo.write(0);

      delay(2000);
      digitalWrite(10, HIGH);
      

    }
    if ((tBt4 > 400) && (tBt4 <= 1000)) {tBt3 = 0; 
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Reiniciando...");
      delay(5000);
      funcReset(); //resetar o arduino
    }

  BtAnt1 = Bt1;
  BtAnt2 = Bt2;
  BtAnt3 = Bt3;
  BtAnt4 = Bt4;
  delay(20);
}
