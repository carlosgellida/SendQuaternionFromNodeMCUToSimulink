#include <Arduino.h>
#include <MPU_personalized_functions.h>

uint32_t prev = 0;
byte Quaternion[6] ; 

byte quat2Byte(float q){
  return byte(q*255) ; 
}

void sendQuaternionByte(MPU9250 &mpu){
  byte Quaternion[6] ;
  Quaternion[0] = 'R'; 
  Quaternion[1] = quat2Byte(mpu.getQuaternionX()) ; 
  Quaternion[2] = quat2Byte(mpu.getQuaternionY()) ; 
  Quaternion[3] = quat2Byte(mpu.getQuaternionZ()) ;
  Quaternion[4] = quat2Byte(mpu.getQuaternionW());
  Quaternion[5] = '\n' ; 
  //Serial.print(Quaternion[1]); 
  Serial.write(Quaternion, 6); // 4 es el número de bytes que se envían
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  inicializeMPU9250(mpu); 
}

void loop() {
  uint32_t now = micros();
    if(now - prev >= 10000){ // Send each 10 ms
      while(true){
      if (mpu.update()) { // If mpu is ready, continue
        prev = micros();

        sendQuaternionByte(mpu); // Read and send current quaternion
        break ; 
    } //end if
    } // end while
    } // end if
}