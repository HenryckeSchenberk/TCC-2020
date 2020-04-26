#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 mpu6050(Wire);

//#define R 3                                 // Constante da seta da direita
//#define L 4                                 // Constante da seta da esquerda 

//bool R1 = 0, L1 = 0, t = 0, T = 1;          // Variaveis de trava

int VN = 0, VA = 0;                         // Variaveis de controle

long timer = 7294;                          // Começo da Millis()

void setup() {
  Serial.begin(9600);                       // Inicia o monitor serial
  Wire.begin();                             // Inicia a lib "Wire"
  mpu6050.begin();                          // Inicia a lib "MPU6050-tockn"
  mpu6050.calcGyroOffsets(true);            // Diminui irregularidades do Chip;
 // pinMode(R, OUTPUT);                       // Define "R" como saida
 // pinMode(L, OUTPUT);                       // Define "L" como saida
}

void loop() {
  mpu6050.update();
  //Serial.println(VN);
  VN = mpu6050.getAngleZ();                 // Armazena o angulo do eixo "Z"
  if (VN >= VA + 5) {
    Serial.write('1');
    VA = VN;
  } else {
    mpu6050.update();
    if (VN <= VA - 5) {
      Serial.write('2');
      VA = VN;
    } else {
      mpu6050.update();
    }
  }
}
