/* 
 * File:   ir.h
 * Author: Seth
 *
 * Created on April 11, 2015, 9:39 AM
 */

#ifndef IR_H
#define	IR_H

//i put this here so i could use stateType as a return type
typedef enum stateTypeEnum{
    idle, getState, turnLeft, turnRight, forward, turn180,
    debouncePress, debounceRelease
}stateType;

//initialize the pins for all 3 phototransistors
void initIR(void);

//check the values on the photoresistor pins to see which direction we need to
//go to follow the line.
//returns stateType
stateType checkSensorValues(void);


#endif	/* IR_H */

