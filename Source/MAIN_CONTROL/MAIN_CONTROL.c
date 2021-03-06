#include <stdio.h>
#include <stdlib.h>
#define _XTAL_FREQ 8000000

#include <xc.h>
#include "HEADER_FILE/dht11.h"
#include "HEADER_FILE/lcd.h"

// BEGIN CONFIG
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF // Flash Program Memory Code Protection bit (Code protection off)

#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF // Flash Program Memory Code Protection bit (Code protection off)
//END CONFIG
unsigned int  temp_button,temperature,humidity;
unsigned int LCD_Control = 0b00000000,lcd_temp,FAN1 = 90,FAN2 = 20,fan1_control = 50, fan2_control = 50;
unsigned int TEM = 32, HUM = 86, ENABLE_AUTO = 1; 
void Button_Init(){
    OPTION_REGbits.INTEDG = 0; //Ngat canh len
    INTCONbits.INTE =1; //Cho phep ngat ngoai
    INTCONbits.GIE =1; // Cho phep ngat toan cuc
}
void Sensor_Init(){
    dht11_init();
    find_response();
    if(Check_bit == 1){
        RH_byte_1 = read_dht11();
        RH_byte_2 = read_dht11();
        Temp_byte_1 = read_dht11();
        Temp_byte_2 = read_dht11();
        Sumation = read_dht11();
        if(Sumation == ((RH_byte_1+RH_byte_2+Temp_byte_1+Temp_byte_2) & 0XFF)){
            temperature = Temp_byte_1;
            humidity = RH_byte_1;                        
        }
        else{
            Lcd_Write_String("Check sum error");
        }
    }
    else {
        Lcd_Clear();
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("Error!!!");
        
     }
}
void Show_Menu(){
    Lcd_Set_Cursor(1,1);               
    Lcd_Write_String("------MENU------");
    Lcd_Set_Cursor(2,2);  
        Lcd_Write_String("SET Temp: ");
        Lcd_Write_Number(TEM);
        Lcd_Write_Char(0xDF);
        Lcd_Write_String("C");
        
        Lcd_Set_Cursor(3,2);  
        Lcd_Write_String("SET Hum: ");
        Lcd_Write_Number(HUM);
        Lcd_Write_String("% ");
        
        Lcd_Set_Cursor(4,2);  
        Lcd_Write_String("SET Auto");
}
void Show_Menu_Auto(){
    Lcd_Set_Cursor(1,2);               
    Lcd_Write_String("SET AUTO: ");
    if (ENABLE_AUTO == 1 ){

        Lcd_Write_String("Yes");
    }
    else {
        Lcd_Write_String("No ");
    }
    

    Lcd_Set_Cursor(2,2);               
    Lcd_Write_String("FAN1: ");
    Lcd_Write_Number(FAN1);
    Lcd_Write_String("%");

    Lcd_Set_Cursor(3,2);               
    Lcd_Write_String("FAN2: ");
    Lcd_Write_Number(FAN2);
    Lcd_Write_String("%");

    Lcd_Set_Cursor(4,2);               
    Lcd_Write_String("Back");
}
void LCD_INFOR(){           
    Lcd_Set_Cursor(1,1);               
    Lcd_Write_String("Temp: ");
    Lcd_Write_Number(temperature);
    Lcd_Write_Char(0xDF);
    Lcd_Write_String("C");
    
    Lcd_Set_Cursor(2,1);  
    Lcd_Write_String("Humidity: ");
    Lcd_Write_Number(humidity);
    Lcd_Write_String("%  ");
    
    Lcd_Set_Cursor(3,1);  
    Lcd_Write_String("Fan TEM: ");
    Lcd_Write_Number(fan1_control);
    Lcd_Write_String("%  ");
    
    Lcd_Set_Cursor(4,1);  
    Lcd_Write_String("Fan HUM: ");
    Lcd_Write_Number(fan2_control);
    Lcd_Write_String("%  ");
 
}
void LCD_MENU_AUTO(){
    if ((LCD_Control&0b110001) == 0b10001){
        Lcd_Set_Cursor((LCD_Control&0b110)/2+1,1);
        Lcd_Write_String(">");
        Show_Menu_Auto();
    }
    else if ((LCD_Control&0b110111) != 0b110111){
        Lcd_Set_Cursor((LCD_Control&0b110)/2+1,1);
        Lcd_Write_String("<");
        Show_Menu_Auto();
    }
    else {
        LCD_Control -= 0b1000;
        LCD_Control = LCD_Control&0b1;
        LCD_Control += 0b100;
        Lcd_Clear();
        
    }
}
void LCD_MENU(){
    
   
    
    if ((LCD_Control&0b1001) == 1){ // N???u ??ang trong menu nh??ng ch??a ch???n
        Lcd_Set_Cursor((LCD_Control&0b110)/2+2,1);
        Lcd_Write_String(">");
        Show_Menu();
    }
    else if((LCD_Control&0b1111) != 0b1101){ // N???u ??ang trong menu v?? ???? ch???n kh??c v???i menu auto
        Lcd_Set_Cursor((LCD_Control&0b110)/2+2,1);
        Lcd_Write_String("<");
        Show_Menu();
    }

    else {
        LCD_Control += 0b10000;
        LCD_Control = LCD_Control&0b11001;
        Lcd_Clear();
        

    }           
}


// C??c h??m n??t nh???n
void UP_BN(){
    if (temp_button == 0b1110){ //UP
    //-----------------------------------------       
        if(((LCD_Control&0b110) != 0b0)&&((LCD_Control&0b1000)==0)){ // Trong Menu ch??nh
            LCD_Clear_Char((LCD_Control&0b110)/2+2,1);
            LCD_Control -= 0b10;    
        }
        else if((LCD_Control&0b1000) == 0b1000){ // N???u ???? ch???n 
            if ((LCD_Control&0b110) == 0){ //N???u ch???n set tem
                TEM ++;
            }
            else if ((LCD_Control&0b110) == 0b10){ // N???u ch???n set hum
                HUM ++;
            }
        }
        
    //////////////////////////////////////
        if(((LCD_Control&0b110) != 0b0)&&((LCD_Control&0b110000)==0b10000)){ // Trong Menu auto
            LCD_Clear_Char((LCD_Control&0b110)/2+1,1);
            LCD_Control -= 0b10;
        }
        else if((LCD_Control&0b100000) == 0b100000){  // N???u ch???n OK
            if ((LCD_Control&0b110) == 0b010){  //Ok ??? FAN1
                FAN1 ++;
            }
            else if ((LCD_Control&0b110) == 0b100){ // OK ??? FAN2
                FAN2 ++;
            }
            else if ((LCD_Control&0b110) == 0b0){ // OK ??? SET auto
                ENABLE_AUTO = !ENABLE_AUTO;
            }
        } 
    }
}
void DOWN_BN(){
    if(temp_button == 0b1100) { //DOWN
    //----------------------------------------------------------------------
        if(((LCD_Control&0b110) != 0b100)&&((LCD_Control&0b1000)==0)){ // Trong Menu ch??nh
            LCD_Clear_Char((LCD_Control&0b110)/2+2,1);
            LCD_Control += 0b10;
        }
        else if((LCD_Control&0b1000) == 0b1000){ // N???u  ch???n ok
            if ((LCD_Control&0b110) == 0){
                TEM --;
            }
            else if ((LCD_Control&0b110) == 0b10){
                HUM --;
            }

        }
    ////////////////////////////////////////////////////////
        if(((LCD_Control&0b110) != 0b110)&&((LCD_Control&0b110000)==0b10000)){ // Trong Menu auto
            LCD_Clear_Char((LCD_Control&0b110)/2+1,1);
            LCD_Control += 0b10;
        }
        else if((LCD_Control&0b100000) == 0b100000){  
            if ((LCD_Control&0b110) == 0b10){ 
                FAN1 --;
            }
            else if ((LCD_Control&0b110) == 0b100){ 
                FAN2 --;
            }
            else if ((LCD_Control&0b110) == 0b0){ // OK ??? SET auto
                ENABLE_AUTO = !ENABLE_AUTO;
            }
        }

    }
}
void MENU_BN(){
    if (temp_button == 0b1010){
        
        
        if ((LCD_Control&0b1)== 1){
            LCD_Control --;
            
        }
        else {
            LCD_Control ++;
            
        }
        Lcd_Clear();
      
        
        
    }
}
void OK_BN(){
    if (temp_button == 0b0110){
    //---------------------------

    // ??ang trong Menu ch??nh 
        if ((LCD_Control&0b111001)==1){ // N???u nh???n OK v?? kh??ng ch???n m???c Menu
            LCD_Control += 0b1000;
        }
        else if((LCD_Control&0b111001) == 0b1001){ // N???u ch???n OK khi  set  trong menu
            LCD_Control -= 0b1000;
        }
    /////////////////////////////////////
    //??ang trong menu auto
        else if((LCD_Control&0b110001) == 0b10001){  
            LCD_Control += 0b100000;
        }


        else if((LCD_Control&0b110001) == 0b110001){// N???u ch???n OK trong Menu Auto
            LCD_Control -= 0b100000;
        }
    } 
}


//H??m th???c hi???n khi ng???t
void __interrupt() ISR(void){
    temp_button =  PORTB&0xf;
    UP_BN();
    DOWN_BN();
    MENU_BN();
    OK_BN();
    INTCONbits.INTF = 0;   
}
// H??m ??i???u khi???n FAN
void Control_Fan(int x, int y){
    PORTC = 0x80 + x;
    __delay_ms(100);
    PORTC =  y;
}
//H??m ??i???u khi???n AUTO
void Control_Auto(){
    if ( ENABLE_AUTO == 1){
        if (temperature > TEM){
            if (fan1_control < 100 ){
                fan1_control += 1;
            }
            
        }
        else if(temperature < TEM){
            if (fan1_control > 0){
                fan1_control -= 1;
            }
            
        }

        if (humidity > HUM){
            if (fan2_control > 0 ){
                fan2_control -= 1;
            }
        }
        else if(humidity < HUM){
            if (fan2_control < 100){
                fan2_control += 1;
            }
        }
        
        Control_Fan(fan1_control,fan2_control);
    }
    else {
        fan1_control = FAN1;
        fan2_control = FAN2;
        Control_Fan(fan1_control,fan2_control);
    }
}


void main(void)
{
    TRISD = 0x00;
    Button_Init();
    Lcd_Init();
    TRISBbits.TRISB0 = 1;
    PORTB = 0x0;
    TRISC = 0x00;
    PORTC = 0x00;
    while(1)
    {
        Sensor_Init();
        Control_Auto();
        __delay_ms(500);
        if ((LCD_Control&0b10001) == 1){
            LCD_MENU();
        }
        else if ((LCD_Control&0b10001) == 0b10001){
            LCD_MENU_AUTO();
        }
        else {
            LCD_INFOR();
        }
    }
}