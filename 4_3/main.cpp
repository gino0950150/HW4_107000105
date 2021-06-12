#include"mbed.h"
#include <vector>
#include "bbcar.h"

using namespace std;
BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(D1,D0); //tx,rx
DigitalInOut ping(D13);
Ticker servo_ticker;
Ticker encoder_ticker;
Timer t;
PwmOut pin5(D5), pin6(D6);
DigitalIn encoder(D11);
volatile int steps;
volatile int last;
BBCar car(pin5, pin6, servo_ticker);
void encoder_control() {
   int value = encoder;
   if (!last && value) steps++;
   last = value;
}
int i=0;

void Pinging(){
      float val;
      ping.output();
      ping = 0; wait_us(200);
      ping = 1; wait_us(5);
      ping = 0; wait_us(5);
      ping.input();
      while(ping.read() == 0);
      t.start();
      while(ping.read() == 1);
      val = t.read();
      printf("Ping = %lf\r\n", val*17700.4f);
      t.stop();
      t.reset();
      ThisThread::sleep_for(20s);
}

int main(){
   uart.set_baud(9600);
   pc.set_baud(9600);
   encoder_ticker.attach(&encoder_control, 10ms);
   steps = 0;
   last = 0;
   pc.set_baud(9600);
   double pwm_table0[] = {-150, -120, -90, -60, -30, 0, 30, 60, 90, 120, 150};
   double speed_table0[] = {-16.741, -16.343, -15.307, -12.118, -5.820, 0.000, 6.138, 12.118, 15.227, 16.423, 16.821};
   double pwm_table1[] = {-150, -120, -90, -60, -30, 0, 30, 60, 90, 120, 150};
   double speed_table1[] = {-16.052, -16.263, -15.147, -11.639, -5.581, 0.000, 6.138, 12.197, 15.307, 16.263, 16.742};
   car.setCalibTable(11, pwm_table0, speed_table0, 11, pwm_table1, speed_table1);// please contruct you own calibration table with each servo
   char pre = 'X';
   while(1){
      if(uart.readable()){
         char recv[1];
         uart.read(recv, sizeof(recv));
         pc.write(recv, sizeof(recv));
         char c = recv[0];
               switch (c){
               case '0':
                  car.stop();
                  ThisThread::sleep_for(5s);
                  Pinging();
                  break;
               case '1':
                  car.goStraightCalib(0, -2);
                  break;
               case '2':
                  car.goStraightCalib(-2, 0);
                  break;
               default:
                  if (pre!='X' && pre!='0'&& pre!='3'  ){
                     if (pre=='2')car.goStraightCalib(0, -2);
                     if (pre=='1')car.goStraightCalib(-2, 0);
                     ThisThread::sleep_for(1500ms);
                     car.stop();
                     Pinging();
                     ThisThread::sleep_for(5s);
                  }
                  break;
               }
               pre = c;
      }
   }

}


