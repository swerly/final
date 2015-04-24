#include "ir.h"
#include "p24FJ64GA002.h"

//Comparator will output 1.4V - 2.7V, so we set MID_VOLTAGE_VAL the middle
//and then we can easily see if our sensors are sensing the line or not
//620 => ~2V
#define MID_VOLTAGE_VAL 450

#define FLOOR 0
#define LINE 1
#define SET 1
#define RESET 0


//initialize the pins for all 3 phototransistors
void initIR(void){
    
    //pins to be input
    TRISBbits.TRISB12 = 1;
    TRISBbits.TRISB13 = 1;
    TRISBbits.TRISB14 = 1;

    //set pins to be analog
    AD1PCFGbits.PCFG12 = 0;  //an12 (rb12) pin 23
    AD1PCFGbits.PCFG11 = 0;  //an11 (rb13) pin 24
    AD1PCFGbits.PCFG10 = 0;  //an10 (rb14) pin 25

    AD1CON1bits.FORM = RESET;   // Data Output set to interger
    AD1CON1bits.SSRC = 7;       // Conversion trigger set to auto convert
    AD1CON1bits.ASAM = SET;     // Sampling begins after the last one is finished
    AD1CON2bits.VCFG = RESET;   // voltage reference configuration bits aVdd <--> AVss
    AD1CON2bits.CSCNA = 1;      // uses the mux a channels to be the input
    AD1CON2bits.SMPI = 0b0010;  // Convert sequence Interrupts at the completion of conversion for each sample/convert sequence
    AD1CON2bits.ALTS = RESET;   // alternate input sample mode select bit always uses mux A
    AD1CON3bits.ADRC = RESET;   // uses system clock
    AD1CON3bits.SAMC = SET;     // auto sample 1 TAD
    AD1CON3bits.ADCS = 10;      // A/D conversion clock period select bits (set to 10)
    AD1CHS = RESET;             //Configure input channels,

    //configure our inputs to be scanned
    AD1CSSL = 7168;

    AD1CON1bits.ADON = SET;             // Turn on A/D
    IEC0bits.AD1IE = SET;               //Enable the interrupt
    IFS0bits.AD1IF = RESET;             //Put the flag down
    
}

//check the values on the photoresistor pins to see which direction we need to
//go to follow the line.
//returns stateType
stateType checkSensorValues(unsigned int left, unsigned int right, unsigned int center){
    int flag = 0, count = 0;
    //these simple checks should work but we can modify them to increase
    //functionality later

    //basically used as bool to see where we need to go to correct our path
    int leftIR = 0, rightIR = 0, midIR = 0;
    int i = 0;

    if (left > MID_VOLTAGE_VAL) leftIR = LINE;
    //else our comparator circuit is outputting "logic low" which means we
    //are getting a reflection so we are sensing the floor
    else leftIR = FLOOR;

    if (right > MID_VOLTAGE_VAL) rightIR = LINE;
    else rightIR = FLOOR;
    if (center > MID_VOLTAGE_VAL) midIR = LINE;
    else midIR = FLOOR;

    //if left sensor detects line and right doesn't, turn left to correct
    if (leftIR && !rightIR && !midIR){
        return turnLeft;
    }
    //if right sensor detects line and left doesn't, turn right to correct
    else if (rightIR && !leftIR && !midIR){
        i = 0;
        return turnRight;
    }
    else if (rightIR && midIR &&!leftIR) {
        i = 0;
        return forward;
    }
    else if (leftIR && midIR &&!rightIR)
    {
        i = 0;
        return forward;
    }
    else if (rightIR && leftIR && midIR) {
        

        for (i = 0; i >= 3500; i++){

        }
        if (rightIR && leftIR && midIR) return turn180;

    }
    

    else return forward;


}
