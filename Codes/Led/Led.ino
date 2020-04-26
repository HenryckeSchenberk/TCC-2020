#include <SoftwareSerial.h>
#include <TinyGPS.h>
TinyGPS gps;
SoftwareSerial serialGYRO(0, 1);
SoftwareSerial serialGPS(6, 7);
SoftwareSerial serialBTH(8, 9);
//SoftwareSerial serialGSM(10, 11); // RX, TX

// -=-=-=-=-=-=-=-=-=-= Constantes -=-=-=-=-=-=-=-=-=-//
#define clockPin 2
#define latchPin 3
#define dataPin 4
#define num_de_chip 3
//#define senhaGsm "1234"
//#define BLYNK_PRINT SerialBT
//#define BLYNK_USE_DIRECT_CONNECT
//#include <BlynkSimpleSerialBLE.h>
//char auth[] = "2S0pMT0BDb0DxAVU8orRgATKa7JbyNbq";
// -=-=-=-=-=-=-=-=-=-=- ********* -=-=-=-=-=-=-=-=-=-//

// -=-=-=-=-=-=-=-=-=-=- Variáveis -=-=-=-=-=-=-=-=-=-//
 char op = 'x';
 //bool temSMS = false;
 //String telefoneSMS;
 //String dataHoraSMS;
 //String mensagemSMS;
 //String comandoGSM = "";
 //String ultimoGSM = "";
// -=-=-=-=-=-=-=-=-=-=- ********* -=-=-=-=-=-=-=-=-=-//

// -=-=-=-=-=-=-=-=- Funções - Upload -=-=-=-=-=-=-=-//
void Leds(byte pino, bool estado);
//void leGSM();
void leGPS();
//void enviaSMS(String telefone, String mensagem);
//void configuraGSM();
// -=-=-=-=-=-=-=-=-=-=- ********* -=-=-=-=-=-=-=-=-=-//

// -=-=-=-=-=-=-=-=-=-=- SetUP -=-=-=-=-=-=-=-=-=-//
void setup()
{
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
  for (int nl = 0; nl < 24; nl++) {
    Leds(nl, LOW);
  }
  
  Serial.begin(9600);
  serialGPS.begin(9600);
 // serialGSM.begin(9600);
  serialBTH.begin(9600);
  serialGYRO.begin(9600);

 leGPS();
}
// -=-=-=-=-=-=-=-=-=-=- ********* -=-=-=-=-=-=-=-=-=-//

// -=-=-=-=-=-=-=-=-=-=-=- Loop =-=-=-=-=-=-=-=-=-=-=-//
void loop() {
//leGSM();
  serialGYRO.listen();
  if (serialGYRO.available() > 0) {
    op = serialGYRO.read();
  }

  switch (op)  {
    case '1':
     // Serial.print('1');
      for (int r = 0; r < 3; r++) {
        SetaE();
        op = '4';
      }
      serialBTH.listen();
      serialBTH.print("Seta da Esquerda");
      break;

    case '2':
     // Serial.print('2');
      for (int r = 0; r < 3; r++) {
        SetaD();
        op = '4';
      }
      serialBTH.listen();
      serialBTH.print("Seta da Direita");
      break;

    case '3':
     // Serial.print('3');
      for (int r = 0; r < 3; r++) {
        Freio();
        op = '4';
      }
      serialBTH.listen();
      serialBTH.print("Freio acionado");
      break;

   // case '4':
   //   static unsigned long delayLeGPS = millis();
   //  if ( (millis() - delayLeGPS) > 10000 ) {
   //     leGPS();     
   //     delayLeGPS = millis(); 
   //   }
   //   break;
     case '4':
     leGPS();
     op = 'x';
      break;
    default:
      for (int nl = 0; nl < 24; nl++) {
        Leds(nl, LOW);
        op = 'x';
      }
  }
}
// -=-=-=-=-=-=-=-=-=-=- ********* -=-=-=-=-=-=-=-=-=-//

// -=-=-=-=-=-=-=-=-= -Funções - Create -=-=-=-=-=-=-=-//
void SetaE() {
  delay(200);
  for (int nl = 16; nl < 20; nl++)  {
    Leds(nl, HIGH);
    delay(100);
  }
  for (int nl = 23; nl > 19; nl--)  {
    Leds(nl, HIGH);
    delay(100);
  }
  for (int nl = 16; nl < 24; nl++)  {
    Leds(nl, LOW);
  }
}

void Freio() {
  for (int nl = 8; nl < 16; nl++) {
    Leds(nl, HIGH);
  }
  delay(300);
  for (int nl = 8; nl < 16; nl++) {
    Leds(nl, LOW);
  }
  delay(200);
}

void SetaD() {
  delay(200);
  for (int nl = 4; nl < 8; nl++)  {
    Leds(nl, HIGH);
    delay(100);
  }
  for (int nl = 3; nl >= 0; nl--)  {
    Leds(nl, HIGH);
    delay(100);
  }
  for (int nl = 0; nl < 8; nl++)  {
    Leds(nl, LOW);
  }
}

void leGPS(){
  Serial.print('4');
  unsigned long delayGPS = millis();

   serialGPS.listen();
   bool lido = false;
   while ( (millis() - delayGPS) < 1000 ) { 
      while (serialGPS.available()) {
          char cIn = serialGPS.read(); 
          lido = gps.encode(cIn); 
      }

      if (lido) { 
         float flat, flon;
         unsigned long age;
    
         gps.f_get_position(&flat, &flon, &age);
    
         String urlMapa = "Local Identificado: https://maps.google.com/maps/?&z=10&q=";
         urlMapa += String(flat,6);
         urlMapa += ",";
         urlMapa += String(flon,6);
         serialBTH.listen();
         serialBTH.println(urlMapa);
         Serial.println(urlMapa);
         
         break; 
      }
   }
   Serial.print('5');
}
void Leds(byte pino, bool estado) {
  static byte ciBuffer[num_de_chip]; // define o valor maximo de acordo com o numero de chip
  bitWrite(ciBuffer[pino / 8], pino % 8, estado); // pega um Bite e separa em oito bits 000000000
  digitalWrite(latchPin, LOW); // Inicia a Transmissão
  digitalWrite(dataPin, LOW); // Apaga Tudo para Preparar Transmissão
  digitalWrite(clockPin, LOW);
  for (int nC = num_de_chip - 1; nC >= 0; nC--)  {
    for (int nB = 7; nB >= 0; nB--)    {
      digitalWrite(clockPin, LOW); // Baixa o Clock
      digitalWrite(dataPin, bitRead(ciBuffer[nC], nB)); // Escreve o BIT
      digitalWrite(clockPin, HIGH); // Eleva o Clock
      digitalWrite(dataPin, LOW); // Baixa o Data para Previnir Vazamento
    }
  }
  digitalWrite(latchPin, HIGH); // Finaliza a Transmissão
}
