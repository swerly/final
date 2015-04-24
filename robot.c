#include "p24FJ64GA002.h"
#include "robot.h"
#include "pwm.h"

void fwd(void){
    setDutyCycle(BOTH, 90);
}
void softRight(void){
    setDutyCycle(MOTOR_ONE, 100);
    setDutyCycle(MOTOR_TWO, 0);
}
void softLeft(void){
    setDutyCycle(MOTOR_ONE, 0);
    setDutyCycle(MOTOR_TWO, 100);
}
void hardRight(void){
    setDutyCycle(MOTOR_ONE, 100);
    setDutyCycle(MOTOR_TWO, 0);
}
void hardLeft(void){
    setDutyCycle(MOTOR_ONE, 0);
    setDutyCycle(MOTOR_TWO, 100);
}
void t180(void){
    int i = 0;
    setDutyCycle(MOTOR_ONE, 100);
    setDutyCycle(MOTOR_TWO, 0);
    for(i = 0; i < 2400; i++) delayUs(1000);

}
void stop(void){
    setDutyCycle(BOTH, 0);
}
