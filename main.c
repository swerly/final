#include "p24fj64ga002.h"
#include "timer.h"
#include "pwm.h"
#include "ir.h"
#include "robot.h"
#include "sw1.h"
#include <stdio.h>

#define DEBOUNCE_DELAY_US 5000

_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_ON & COE_OFF & ICS_PGx1 &
          FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )

_CONFIG2( IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
          IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT )

volatile stateType curState, next;


int main(void){
    initIR();
    int leftRead, rightRead, midRead, i;

    while(1){
        //need to start sampling so we can read adc vals
        AD1CON1bits.SAMP = 1;
        while (!AD1CON1bits.DONE); //wait until conversion is done

        //put respective values in their place
        //an10 -> an11 -> an12 fill buffer in that order
        rightRead = ADC1BUF0;
        midRead = ADC1BUF1;
        leftRead = ADC1BUF2;

        //delay and debug to see if sensor vals are what they should be
        for(i = 0; i < 1000; i++) delayUs(1000);
    }

    /*
    initMotorOnePWM();
    initMotorTwoPWM();
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
                 curState = checkSensorValues();
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
    */
}

void _ISR _CNInterrupt(){
    IFS1bits.CNIF = 0; //put down flag

    if (SW1PORT == PRESSED){
        //just debounce and continue until released
        next = curState;
        curState == debouncePress;
    }
    else if (SW1PORT == RELEASED){
        //debounce and start or stop motion
        curState = debounceRelease;

        //if we are idling we want to start motion
        //else we are in motion and we want to stop
        //idk how we are going to press the button while the robot is moving tho...
        if (curState == idle) next = getState;
        else curState = idle;

    }
}