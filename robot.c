#include "p24FJ64GA002.h"
#include "robot.h"
#include "pwm.h"

void fwd(void){
    setDutyCycle(BOTH, 100);
}
void softLeft(void){
    setDutyCycle(MOTOR_ONE, 100);
    setDutyCycle(MOTOR_TWO, 0);
}
void softRight(void){
    setDutyCycle(MOTOR_ONE, 0);
    setDutyCycle(MOTOR_TWO, 100);
}
void hardLeft(void){
    setDutyCycle(MOTOR_ONE, 100);
    setDutyCycle(MOTOR_TWO, 0);
}
void hardRight(void){
    setDutyCycle(MOTOR_ONE, 0);
    setDutyCycle(MOTOR_TWO, 100);
}
void t180(void){
    setDirection(MOTOR_ONE, REVERSE);
    setDutyCycle(MOTOR_ONE, 100);
    setDutyCycle(MOTOR_TWO, 100);

    setDirection(MOTOR_ONE, FORWARD);
}
void stop(void){
    setDutyCycle(BOTH, 0);
}
