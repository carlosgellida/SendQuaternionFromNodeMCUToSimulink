#include <Arduino.h>
#include <SparkFunMPU9250-DMP.h>


MPU9250_DMP imu;

byte quat2Byte(float q){
  return byte(q*127+127) ; 
}

void printIMUData(void)
{  
  // After calling dmpUpdateFifo() the ax, gx, mx, etc. values
  // are all updated.
  // Quaternion values are, by default, stored in Q30 long
  // format. calcQuat turns them into a float between -1 and 1
  float q0 = imu.calcQuat(imu.qw);
  float q1 = imu.calcQuat(imu.qx);
  float q2 = imu.calcQuat(imu.qy);
  float q3 = imu.calcQuat(imu.qz);

  Serial.println("Q: " + String(q0, 4) + ", " +
                    String(q1, 4) + ", " + String(q2, 4) + 
                    ", " + String(q3, 4));
  Serial.println("R/P/Y: " + String(imu.roll) + ", "
            + String(imu.pitch) + ", " + String(imu.yaw));
  Serial.println("Time: " + String(imu.time) + " ms");
  Serial.println();
}

void sendQuaternionByte(MPU9250_DMP &imu){
  byte Quaternion[6] ;
  Quaternion[0] = 'R'; 
  Quaternion[1] = quat2Byte(imu.calcQuat(imu.qw));
  Quaternion[2] = quat2Byte(imu.calcQuat(imu.qx)) ; 
  Quaternion[3] = quat2Byte(imu.calcQuat(imu.qy)) ; 
  Quaternion[4] = quat2Byte(imu.calcQuat(imu.qz)) ;
  Quaternion[5] = '\n' ; 
  Serial.write(Quaternion, 6); // 4 es el número de bytes que se envían
}

void setup() 
{
  Serial.begin(115200);

  // Call imu.begin() to verify communication and initialize
  if (imu.begin() != INV_SUCCESS)
  {
    while (1)
    {
      Serial.println("Unable to communicate with MPU-9250");
      Serial.println("Check connections, and try again.");
      Serial.println();
      delay(5000);
    }
  }
  
  imu.dmpBegin(DMP_FEATURE_6X_LP_QUAT | // Enable 6-axis quat
               DMP_FEATURE_GYRO_CAL, // Use gyro calibration
              10); // Set DMP FIFO rate to 10 Hz
  // DMP_FEATURE_LP_QUAT can also be used. It uses the 
  // accelerometer in low-power mode to estimate quat's.
  // DMP_FEATURE_LP_QUAT and 6X_LP_QUAT are mutually exclusive
}

void loop() 
{
  // Check for new data in the FIFO
  if ( imu.fifoAvailable() )
  {
    // Use dmpUpdateFifo to update the ax, gx, mx, etc. values
    if ( imu.dmpUpdateFifo() == INV_SUCCESS)
    {
      // computeEulerAngles can be used -- after updating the
      // quaternion values -- to estimate roll, pitch, and yaw
      imu.computeEulerAngles();
      sendQuaternionByte(imu);
    }
  }
}

