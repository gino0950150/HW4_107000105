#include"mbed.h"
#include <vector>
#include "bbcar.h"
using namespace std;

BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(D1,D0); //tx,rx

Ticker servo_ticker;
Ticker encoder_ticker;
PwmOut pin5(D5), pin6(D6);
DigitalIn encoder(D11);

BBCar car(pin5, pin6, servo_ticker);

int i=0;
char pre[1];
int main(){
   uart.set_baud(9600);
   pc.set_baud(9600);
   while(1){
      if(uart.readable()){
            char recv[1];
            uart.read(recv, sizeof(recv));
            char c = recv[0];
            if (c != pre[0]){
               switch (c){
                  printf("c: %c\n", c);
                  case '0':
                   printf("!\n");
                     car.goStraight(20, 20);
                     break;
                  case '1':
                     car.goStraight(20, 0);
                     break;
                  case '2':
                     car.goStraight(0, 20);
                     break;
               }
               pre[0] = c;
            }
      }
   }
}
