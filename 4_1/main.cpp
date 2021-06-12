#include "mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"
#include <vector>
#include <chrono>

using namespace std;
Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
BufferedSerial xbee(D1, D0);
BBCar car(pin5, pin6, servo_ticker);

float d1 = 0;
float d2 = 0;
char dir;
bool setVar = 0;


void move();

int main() {
   char buf[256], outbuf[256];
   FILE *devin = fdopen(&xbee, "r");
   FILE *devout = fdopen(&xbee, "w");
   double pwm_table0[] = {-150, -120, -90, -60, -30, 0, 30, 60, 90, 120, 150};
   double speed_table0[] = {-16.741, -16.343, -15.307, -12.118, -5.820, 0.000, 6.138, 12.118, 15.227, 16.423, 16.821};
   double pwm_table1[] = {-150, -120, -90, -60, -30, 0, 30, 60, 90, 120, 150};
   double speed_table1[] = {-16.052, -16.263, -15.147, -11.639, -5.581, 0.000, 6.138, 12.197, 15.307, 16.263, 16.742};
   car.setCalibTable(11, pwm_table0, speed_table0, 11, pwm_table1, speed_table1);// please contruct you own calibration table with each servo

   while (1) {
      memset(buf, 0, 256);
      for( int i = 0; ; i++ ) {
         char recv = fgetc(devin);
         if(recv == '\n') {
            printf("\r\n");
            dir = buf[i-1]; 
            move();
            break;
         }else if (recv==' '){
            if (setVar == 0){ 
               d1 = atof(buf);
               setVar = 1;
               memset(buf, 0, 256);
               break;
            }else{
               d2 = atof(buf);
               setVar = 0;
               memset(buf, 0, 256);
               break;
            }
            break;
         } 
         buf[i] = fputc(recv, devout);
         printf("buf %s", buf);
      }
      printf("enter: %f, %f, %c\n", d1, d2, dir);
   }

}



void move(){
   if (dir=='w'){
      if (d1>=0){
         d1 = (d1+ 13.5) /5;
         car.goStraightCalib(5, 5);
         wait_us(d1*1000000);
         car.goStraightCalib(-5, 0);
         wait_us(3.80*1000000);
         d2 = (d2+ 3.5) /5;
         car.goStraightCalib(-5, -5);
         wait_us(d2*1000000);
         car.stop();
      }else if (d1<0){
         d1 = (-d1) /5;
         car.goStraightCalib(-5, -5);
         wait_us(d1*1000000);
         car.goStraightCalib(-3, 0);
         wait_us(1*1000000);
         d2 = (d2+ 3.5) /5;
         car.goStraightCalib(-5, -5);
         wait_us(d2*1000000);
         car.stop();
      }
   }else if (dir=='e'){
      if (d1<=0){
         d1 = (-d1+ 13.5) /5;
         car.goStraightCalib(5, 5);
         wait_us(d1*1000000);
         car.goStraightCalib(0, -5);
         wait_us(3.8*1000000);
         d2 = (d2+ 3.5) /5;
         car.goStraightCalib(-5, -5);
         wait_us(d2*1000000);
         car.stop();
      }else if (d1>0){
         d1 = (d1) /5;
         car.goStraightCalib(-5, -5);
         wait_us(d1*1000000);
         car.goStraightCalib(0, -5);
         wait_us(3.80*1000000);
         d2 = (d2+ 3.5) /5;
         car.goStraightCalib(-5, -5);
         wait_us(d2*1000000);
         car.stop();
      }
   }else if (dir=='n'){
      if (d1>=0){
         car.goStraightCalib(5, 0);
         wait_us(4.05*1000000);
         d1 = (d1+ 13.5) /5;
         car.goStraightCalib(5, 5);
         wait_us(d1*1000000);
         car.goStraightCalib(-5, 0);
         wait_us(3.80*1000000);
         d2 = (d2+ 3.5) /5;
         car.goStraightCalib(-5, -5);
         wait_us(d2*1000000);
         car.stop();
      }else if (d1<0){
         car.goStraightCalib(0, 5);
         wait_us(4.05*1000000);
         d1 = (-d1+ 13.5) /5;
         car.goStraightCalib(5, 5);
         wait_us(d1*1000000);
         car.goStraightCalib(0, -5);
         wait_us(3.8*1000000);
         d2 = (d2+ 3.5) /5;
         car.goStraightCalib(-5, -5);
         wait_us(d2*1000000);
         car.stop();
      }
   }
}