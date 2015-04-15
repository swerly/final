#include "ir.h"
#include "p24FJ64GA002.h"

//Comparator will output 1.4V - 2.7V, so we set MID_VOLTAGE_VAL the middle
//and then we can easily see if our sensors are sensing the line or not
//620 => ~2V
#define MID_VOLTAGE_VAL 620

#define FLOOR 0
#define LINE 1


//initialize the pins for all 3 phototransistors
void initIR(void){
    //configure pins as outputs
    TRISBbits.TRISB12 = 1;
    TRISBbits.TRISB13 = 1;
    TRISBbits.TRISB14 = 1;
    //TODO: convert commands to hex

    //set pins to be analog
    AD1PCFGbits.PCFG12 = 0;  //an12 (rb12) pin 23
    AD1PCFGbits.PCFG11 = 0;  //an11 (rb13) pin 24
    AD1PCFGbits.PCFG10 = 0;  //an10 (rb14) pin 25

    AD1CON1bits.FORM = 0; //integer form
    AD1CON1bits.SSRC = 7; //auto convert
    AD1CON1bits.ASAM = 0; //DONT AUTOSAMPLE
    AD1CON2bits.VCFG = 0; //3.3V and Ground reference
    AD1CON2bits.CSCNA = 0; //Scan inputs selected by AD1CSSL reg
    //AD1CON2bits.SMPI = 0b1111; //no interrupt so won't matter
    AD1CON2bits.ALTS = 0; //always use mux a
    AD1CON3bits.ADRC = 0; //conversion clock derived from system clock
    AD1CON3bits.SAMC = 1; //auto-sample bits, shouldnt matter cause we aren't auto sampling
    AD1CON3bits.ADCS = 10;
    AD1CHS = 0;             //Configure input channels,

    //configure our inputs to be scanned
    AD1CSSLbits.CSSL12;  //an12 (rb12) pin 23
    AD1CSSLbits.CSSL11;  //an11 (rb13) pin 24
    AD1CSSLbits.CSSL10;  //an10 (rb14) pin 25

    AD1CON1bits.ADON = 1; // Turn on A/D

    IFS0bits.AD1IF = 0;     //Put the flag down, even though we wont be using it
}

//check the values on the photoresistor pins to see which direction we need to
//go to follow the line.
//returns stateType
stateType checkSensorValues(void){
    //these simple checks should work but we can modify them to increase
    //functionality later

    //basically used as bool to see where we need to go to correct our path
    int leftIR = 0, rightIR = 0, midIR = 0;

    //readings from the respective adc buffers will be held here
    int leftRead, rightRead, midRead;

    //need to start sampling so we can read adc vals
    AD1CON1bits.SAMP = 1;
    while (!AD1CON1bits.DONE); //wait until conversion is done
    leftRead = ADC1BUF0; //put respective values in their place
    rightRead = ADC1BUF1;
    midRead = ADC1BUF2;

    //if our input is reading logic high, then no light is getting into the
    //phototransistor which means its sensing the line
    if (leftRead > MID_VOLTAGE_VAL) leftIR = LINE;
    //else our comparator circuit is outputting "logic low" which means we
    //are getting a reflection so we are sensing the floor
    else leftIR = FLOOR;
    if (rightRead > MID_VOLTAGE_VAL) rightIR = LINE;
    else rightIR = FLOOR;
    if (midRead > MID_VOLTAGE_VAL) rightIR = LINE;
    else rightIR = FLOOR;

    //if left sensor detects line and right doesn't, turn left to correct
    if (leftIR && !rightIR) return turnLeft;
    //if right sensor detects line and left doesn't, turn right to correct
    if (rightIR && !leftIR) return turnRight;
    ///if all three are blocked we need to turn
    if (leftIR && rightIR && midIR) return turn180;
    //else only the middle sensor detects the line and we can go forward
    if (midIR && !leftIR && !rightIR) return forward;
}
