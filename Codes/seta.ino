#define clockPin  2
#define latchPin  3
#define dataPin   4
#define num_de_chip  5

//----- teste millis--------//
unsigned long tempoAtiva = 0;
unsigned long tempoDesativa = 0;
unsigned long clock = 1000;

int x = 0;
int trava = 0;
//--------------------------//

void Leds(byte pino, bool estado); // carrega função criada lá em baixo 🙂
void Seta_Esquerda();
void Descanco();
void Freio();

void setup() {
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  //pinMode(botao1, INPUT_PULLUP);
  //pinMode(botao2, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
 if (trava == 0) {
    if ((millis() - tempoAtiva) >= 1000) {
      if ((millis() -  clock) >= 1000){
      for (int led = 0; led < 4; led++) { //Vai ligando e somando 1
        Leds(led, HIGH);
        
      }
    }else { return;}
      clock = millis();
      tempoAtiva = millis();
      x++;
      if (x == 4) {
        trava = 1;
      }
    }
  }
}

void Freio() {
  Leds(8, HIGH);
  Leds(9, HIGH);
  Leds(10, HIGH);
  Leds(11, HIGH);
  Leds(12, HIGH);
  Leds(8, HIGH);
}

void Descanco() {
  Leds(13, HIGH);
  Leds(14, HIGH);
  Leds(15, HIGH);
  Leds(16, HIGH);
  Leds(17, HIGH);
  Leds(18, HIGH);
  Leds(19, HIGH);
  Leds(20, HIGH);
  Leds(21, HIGH);
  Leds(22, HIGH);
  Leds(23, HIGH);


}

void Alerta() {

}

void Seta_esquerda() {
  
/*
  if (trava == 1) {
    if ((millis() - tempoDesativa) >= 1000) {
      for (int led = 4; led >= 0; led--) { // Vai desligando e subtraindo 1
        Leds(led, LOW);
        
      }
      tempoDesativa = millis();
      x--;
      if (x == 0) {
        trava = 0;
      }
    }
  }
  */
}

// =-=-=-= Funçãozinha pika das galaxia de faz a porra todo e kibei mesmo =-=-=-=-=-= //
void Leds(byte pino, bool estado) {
  static byte ciBuffer[num_de_chip];               // define o valor maximo de acordo com o numero de chip

  bitWrite(ciBuffer[pino / 8], pino % 8, estado);  // pega um Bite e separa em oito bits 000000000

  digitalWrite(latchPin, LOW); //Inicia a Transmissão

  digitalWrite(dataPin, LOW);    //Apaga Tudo para Preparar Transmissão
  digitalWrite(clockPin, LOW);

  for (int nC = num_de_chip - 1; nC >= 0; nC--) {
    for (int nB = 7; nB >= 0; nB--) {

      digitalWrite(clockPin, LOW);  //Baixa o Clock

      digitalWrite(dataPin,  bitRead(ciBuffer[nC], nB) );     //Escreve o BIT

      digitalWrite(clockPin, HIGH);   //Eleva o Clock
      digitalWrite(dataPin, LOW);     //Baixa o Data para Previnir Vazamento
    }
  }

  digitalWrite(latchPin, HIGH);  //Finaliza a Transmissão

}