#include "p24fj64ga002.h"
#include "timer.h"
#include "pwm.h"
#include "ir.h"
#include "robot.h"
#include "sw1.h"
#include <stdio.h>

#define DEBOUNCE_DELAY_US 5000

#define MID_VOLTAGE_VAL 450

#define FLOOR 0
#define LINE 1

_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_ON & COE_OFF & ICS_PGx1 &
          FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )

_CONFIG2( IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
          IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT )

volatile stateType curState, next;
volatile int adcValLeft = 0;
volatile int adcValMiddle = 0;
volatile int adcValRight = 0;


volatile int done = 0;
volatile int leftIR = 0;
volatile int rightIR = 0;
volatile int midIR = 0;

int main(void){
    initMotorOnePWM();
    initMotorTwoPWM();
    initTimerPWM();
    initIR();
    initSW1();
    curState = idle;

    while(1){
         switch (curState)
        {
            case idle:
                stop();
                break;

             case getState:
                 curState = checkSensorValues(adcValLeft, adcValRight, adcValMiddle);
                 break;

             case forward:
                 fwd();
                 curState = getState;
                 break;

             case turnLeft:
                 softLeft();
                 curState = getState;
                 break;

             case turnRight:
                 softRight();
                 curState = getState;
                 break;

             case turn180:
                 t180();
                 curState = getState;
                 break;

             case debouncePress:
                 delayUs(DEBOUNCE_DELAY_US);
                 curState = next;
                 break;

             case debounceRelease:
                 delayUs(DEBOUNCE_DELAY_US);
                 curState = next;
                 break;
         }
    }
    
}

void _ISR _CNInterrupt(){
    IFS1bits.CNIF = 0; //put down flag

    if (SW1PORT == PRESSED){
        //just debounce and continue until released
        next = curState;
        curState = debouncePress;
    }
    else if (SW1PORT == RELEASED){
        curState = debounceRelease;

        //if we are idling we want to start motion
        //else we are in motion and we want to stop
        //idk how we are going to press the button while the robot is moving tho...
        if (next == idle) next = getState;
        else next = idle;

        

    }
}

void _ISR _ADC1Interrupt(void){
    IFS0bits.AD1IF = 0;

    unsigned int *adcPtr;
    adcValLeft = 0;
    adcValMiddle = 0;
    adcValRight = 0;
    adcPtr = (unsigned int *) (&ADC1BUF0);
    adcValRight = *adcPtr;
    adcPtr++;
    adcValMiddle = *adcPtr;
    adcPtr++;
    adcValLeft = *adcPtr;
    adcPtr++;

    done = 1;
}