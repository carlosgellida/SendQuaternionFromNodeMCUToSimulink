#include <Arduino.h>
#include <MPU_personalized_functions.h>

uint32_t prev = 0;
byte Quaternion[6] ; 

byte quat2Byte(float q){
  return byte(q*127+127) ; 
}
byte euler2Byte(float e){
  return byte((e+180)*255/360) ; 
}

void sendQuaternionByte(MPU9250 &mpu){
  byte Quaternion[6] ;
  Quaternion[0] = 'R'; 
  Quaternion[1] = quat2Byte(mpu.getQuaternionW());
  Quaternion[2] = quat2Byte(mpu.getQuaternionX()) ; 
  Quaternion[3] = quat2Byte(mpu.getQuaternionY()) ; 
  Quaternion[4] = quat2Byte(mpu.getQuaternionZ()) ;
  Quaternion[5] = '\n' ; 
  Serial.write(Quaternion, 6); // 4 es el número de bytes que se envían
}

void sendEulerByte(MPU9250 &mpu){
  byte Euler[5] ;
  Euler[0] = 'R'; 
  Euler[1] = euler2Byte(mpu.getEulerX()) ; 
  Euler[2] = euler2Byte(mpu.getEulerY()) ; 
  Euler[3] = euler2Byte(mpu.getEulerZ()) ;
  Euler[4] = '\n' ; 
  Serial.write(Euler, 5); // 4 es el número de bytes que se envían
}

void sendRollPitchYawByte(MPU9250 &mpu){
  byte Euler[5] ;
  Euler[0] = 'R'; 
  Euler[1] = euler2Byte(mpu.getRoll()) ; 
  Euler[2] = euler2Byte(mpu.getPitch()) ; 
  Euler[3] = euler2Byte(mpu.getYaw()) ;
  Euler[4] = '\n' ; 
  Serial.write(Euler, 5); // 4 es el número de bytes que se envían
}

void sendEuler(MPU9250 &mpu){
  float Euler[3] ;
  Euler[0] = mpu.getEulerX() ; 
  Euler[1] = mpu.getEulerY() ; 
  Euler[2] = mpu.getEulerZ() ;
  Serial.print(Euler[0]);
  Serial.print(" ") ;
  Serial.print(Euler[1]);
  Serial.print(" ") ;
  Serial.println(Euler[2]);
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  inicializeMPU9250(mpu);

  loadCalibration(); // Charge calibration data from EPROM 
  mpu.verbose(false);
  delay(500);
}

void loop() {
  uint32_t now = micros();
    if(now - prev >= 10000){ // Send each 10ms
      while(true){
      if (mpu.update()) { // If mpu is ready, continue
        prev = micros();
        sendQuaternionByte(mpu); // Read and send current quaternion
        //sendRollPitchYawByte(mpu) ; // Read and send current Euler-Angles
        break ; 
    } //end if
    } // end while
    } // end if
}