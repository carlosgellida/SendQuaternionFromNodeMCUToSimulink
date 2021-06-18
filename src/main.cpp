#include <Arduino.h>

byte a[4] ;

byte fractionOfTime(void){
  int time = millis(); 
  int time_fraction; 
  time_fraction = time % 1000; 
  return byte(255*time_fraction/1000) ; 
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  a[0] = 'R'; 
  a[1] = byte(10) ; 
  a[2] = fractionOfTime() ; 
  a[3] = '\n' ; 
  Serial.write(a, 4); // 4 es el número de bytes que se envían
  delay(10) ; 
}