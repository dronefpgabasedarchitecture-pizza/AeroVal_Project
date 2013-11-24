//
//  motors.h
//  
//
//  Created by Valentin Py on 24.11.13.
//
//

#ifndef _motors_h
#define _motors_h

void motors_init(void);
void motors_send(void); //values come from global array
void motor_commandAll(int globalCommand);

void motors_init(void)
{
    DDRB |=0b01100000;  //PB6, PB5 output
    DDRE |=0b00111000;  //PE3,4,5 output
    DDRH |=0b00111000;  //ph3,4,5 output
    
    TCCR1A = 0b10100010; //chanels A+B PWM, non inverting
    TCCR3A = 0b10101010; //chanels A+B+C PWM, non inverting
    TCCR4A = 0b10101010; //chanels A+B+C PWM, non inverting
    
    TCCR1B = 0b00011010; //mode 14 and PS=8
    TCCR3B = 0b00011010; //mode 14 and PS=8
    TCCR4B = 0b00011010; //mode 14 and PS=8
    
    ICR1 = 6666;  // (Fcpu/(PS§Fpwm)) -1
    ICR3 = 6666;  // (Fcpu/(PS§Fpwm)) -1
    ICR4 = 6666;  // (Fcpu/(PS§Fpwm)) -1
    
    //init Thigh=1000us
    OCR1A=2000;
    OCR1B=2000;
    OCR3A=2000;
    OCR3B=2000;
    OCR3C=2000;
    OCR4A=2000;
    OCR4B=2000;
    OCR4C=2000;
}
void motors_send(void)
{
    OCR3B = motorCommand[0] * 2;
    OCR3C = motorCommand[1] * 2;
    OCR3A = motorCommand[2] * 2;
    OCR4A = motorCommand[3] * 2;
    OCR4B = motorCommand[4] * 2;
    OCR4C = motorCommand[5] * 2;
    OCR1B = motorCommand[6] * 2;
    OCR1A = motorCommand[7] * 2;
}

void motor_commandAll(int globalCommand)
{
    globalCommand *=2;
    OCR3B = globalCommand;
    OCR3C = globalCommand;
    OCR3A = globalCommand;
    OCR4A = globalCommand;
    OCR4B = globalCommand;
    OCR4C = globalCommand;
    OCR1B = globalCommand;
    OCR1A = globalCommand;
}

#endif
