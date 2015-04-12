#include "ir.h"

#define LEFT_IR PORTBbits.RB12
#define CENTER_IR PORTBbits.RB13
#define RIGHT_IR PORTBbits.RB14

//initialize the pins for all 3 phototransistors
void initIR(void){
    //set analot input to be digital
    AD1PCFGbits.PCFG12 = 1;//pcfg12 refers to an12 (pin23)
    AD1PCFGbits.PCFG11 = 1;//pcfg11 refers to an11 (pin24)
    AD1PCFGbits.PCFG10 = 1;//pcfg10 refers to an10 (pin25)

    TRISBbits.TRISB12 = 1;//set rb12 as an input (pin23)
    TRISBbits.TRISB13 = 1;//set rb13 as an input (pin24)
    TRISBbits.TRISB14 = 1;//set rb14 as an input (pin25)

    //that should be it for init....
}

//check the values on the photoresistor pins to see which direction we need to
//go to follow the line.
//returns stateType
stateType checkSensorValues(void){
    //these simple checks should work but we can modify them to increase
    //functionality later

    //if left sensor detects line and right doesn't, turn left to correct
    if (LEFT_IR && !RIGHT_IR) return turnLeft;
    //if right sensor detects line and left doesn't, turn right to correct
    if (RIGHT_IR && !LEFT_IR) return turnRight;
    ///if all three are blocked we need to turn
    if (LEFT_IR && RIGHT_IR && CENTER_IR) return turn180;
    //else only the middle sensor detects the line and we can go forward
    if (CENTER_IR && !LEFT_IR && !RIGHT_IR) return forward;
}
