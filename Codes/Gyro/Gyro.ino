
#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 mpu6050(Wire);

#define R 3                                 // Constante da seta da direita
#define L 4                                 // Constante da seta da esquerda 

bool R1 = 0, L1 = 0, t = 0, T = 1;          // Variaveis de trava

int VN = 0, VA = 0;                         // Variaveis de controle

long timer = 7294;                          // Começo da Millis()

void setup() {
  Serial.begin(9600);                       // Inicia o monitor serial
  Wire.begin();                             // Inicia a lib "Wire"
  mpu6050.begin();                          // Inicia a lib "MPU6050-tockn"
  mpu6050.calcGyroOffsets(true);            // Diminui irregularidades do Chip;
  pinMode(R, OUTPUT);                       // Define "R" como saida
  pinMode(L, OUTPUT);                       // Define "L" como saida 
}

void loop() {
  mpu6050.update();                         // Atualiza o valor do sensor
  VN = mpu6050.getAngleZ();                 // Armazena o angulo do eixo "Z"
  if (VN >= VA + 45) {                      // Verifica se o valor atual é 45º maior que o anterior    
    if(R1 ==0){
    //Serial.println("Direita Ligada");
    Serial.write('1');
   // digitalWrite(R, HIGH);                  // É? Aciona a seta da dieita
    }
    R1 = 1;                                 // Avisa que a seta está ligada 
    t = 1;                                  // Pede que o timer inicie de 0.
    Reset();
  } else {
    //Serial.println(VN);                          // Angulo em que o giroscópio está.
    mpu6050.update();                            // Atualiza o valor.
  }

  if (R1 == 1) {                                      // Se a seta da direita estiver ligada
    t = 0;                                            // Não pode mais resetar o timer
    T = 0;                                            // Não pode mair verificar se pode resetar o timer
    if (millis() - timer > 3000) {                    // Após 3 segundos com a seta ligada
      T = 1;                                          // Pode perguntar se o timer deve ser resetado
      if (VN <= VA + 95 && VN >= VA + 45) {           // Caso o angulo lido enteja entre " 90 e 45 "
    //    Serial.println("Direita Desligada");
       // digitalWrite(R, LOW);                         // Apaga a seta
        R1 = 0;                                       // Avisa que a seta está desligada
        VA = VN;                                      // Valor antigo passa a ser o valor novo
      }
      timer = millis();                           // Reseta o timer
    }else {                                       // Caso ainda não tenham se passados 3 segundos após a seta ligar
    //Serial.println((millis() - timer) / 1000);  // Imprime o timer na serial
      mpu6050.update();                           // Atualiza o ângulo lido
    }
  }
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=-=-=   // Executa as mesmas funções para a seta da Esquerda
if (VN <= VA - 45) {
  if(L1 == 0){
   // Serial.println("Esquerda Ligada");
    Serial.write('2');
   // digitalWrite(L, HIGH);
  }
    L1 = 1;
    t = 1;
    Reset();
  } else {
   // Serial.println(VN);
    mpu6050.update();
  }

  if (L1 == 1) {
    t = 0;
    T = 0;
    if (millis() - timer > 3000) {
      T = 1;
      if (VN >= VA - 90 && VN <= VA - 45) {
      //  Serial.println("Esquerda Desligada");
       // digitalWrite(L, LOW);
        L1 = 0;
        VA = VN;
      }
      timer = millis();
    } else {
     // Serial.println((millis() - timer) / 1000);
      mpu6050.update();
    }
  }
}
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=-=-=   // Fim do código

// =-=-=-=-=-=-=-=- Funções -=-=-=-=-=-==-=-=-=   //

void Reset(){                                // Criação da função "Reset"
      if (T == 1) {
                                             // Pode resetar o timer?
      if ( t == 1) {
        timer = millis();                    // Timer resetado.
      }
    }
}
// =-=-=-=-=-=-=-=-=-= Fim =-=-=-=-=-=-==-=-=-=   //
