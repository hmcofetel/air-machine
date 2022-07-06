#include <stdio.h>
#include <stdlib.h>
#include <xc.h>


// BEGIN CONFIG
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF // Flash Program Memory Code Protection bit (Code protection off)
//END CONFIG


void Timer0_Init(){
    OPTION_REGbits.T0CS =0;
    OPTION_REGbits.PSA = 0;
    
    
    OPTION_REGbits.PS2 = 0;
    OPTION_REGbits.PS1 = 0;
    OPTION_REGbits.PS0 = 0;
    
   
    
}
void DelayTimer0_01ms(unsigned int t){
    while(t--){
        TMR0 = 156;
        INTCONbits.TMR0IF = 0;
        while(!INTCONbits.TMR0IF);
    }
}
int x = 0, y = 0;
int mem1 = 0, mem2 =0;
int temp,temp1,temp2,temp3;
void control_fan(int x, int y){
    if (x < y){   
        temp = x;
        temp1 = 0b100; 
        temp2 = y - x;
        temp3 = 100 - y;
    }
    else {
        temp = y;
        temp1 = 0b001; 
        temp2 = x - y;
        temp3 = 100 - x;
    }
}

void main(void)
{
    TRISB = 0x0;
    PORTB = 0x0;
    TRISD = 0xff;
    
    Timer0_Init();
    
  while(1)
  {
      if(((PORTD&0x80)== 0x80)&&((PORTD&0x7f)!= mem1)){
          x = PORTD&0x7f; 
          mem1 = x;
      }
      else if(((PORTD&0x80)== 0)&&((PORTD&0x7f)!= mem2)) {
          y = PORTD&0x7f;
          mem2 = y;
      }
      control_fan(x, y);
      PORTB = 0x5;
      DelayTimer0_01ms(temp);
      PORTB = temp1 ;
      DelayTimer0_01ms(temp2);
      PORTB = 0b0;
      DelayTimer0_01ms(temp3);
      
    
    
 }
}