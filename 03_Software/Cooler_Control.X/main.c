/** 
 *  ===================================================
 *  DIY PROJECT NAME: COOLER CONTROL
 *  ===================================================
 * 
 *  @Filename           :   main.c 
 *  @Date               :   01.12.2022
 *  @Reference          :   Power supply/converter 12V DC to 220V AC
 *                          Temperature control
 *                          Over and under Voltage control
 *                          Cooler control
 *  @HW Compatibility   :   Microchip PIC12F675, 8bit, 8 Pin
 *  @Code reference     :   PIC12F675 Stack by Microchip 
 *  @SW Owner           :   Privat authors
 *                          Albert Ziatdinov ziatdinovar@gmail.com
 *                          Alex Talko  
 *  @Authors            :   Albert Ziatdinov
 *                          Alex Talko
 * 
 *
 * @section Introduction
 * =====================
 * This application makes for Power supply/converter 12V DC to 220V AC to 
 * control:
 *  - over and under voltage
 *  - temperature
 *  - instied temperture turn on the cooler and generate PWM signal
 * 
 * Important User-Inputs and Variables:
 * ====================================
 *  
 * ***********************************
 * System clock is configured to 4MHz
 * ***********************************
 *
 *
 */

#include <xc.h>
#include <stdint.h>
#include "interrupts.h"
#include "initPeriphery.h"
#include "sounds.h"

#define _XTAL_FREQ   4000000 

//#define SIMULATION

#ifndef SIMULATION
// CONFIG
#pragma config FOSC = INTRCIO   // Oscillator Selection bits (INTOSC oscillator: I/O function on GP4/OSC2/CLKOUT pin, I/O function on GP5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-Up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // GP3/MCLR pin function select (GP3/MCLR pin function is MCLR)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config CP = OFF         // Code Protection bit (Program Memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)

#else 
// Config word
__CONFIG(FOSC_INTRCIO & WDTE_OFF & PWRTE_ON & MCLRE_OFF & BOREN_ON & CP_OFF & CPD_OFF);

#endif

uint8_t errorType = 0;

void DataProcessing(void);

// Main function
void main()          
{	
    __delay_ms(100);
    Init_uC();
    InitTimer0();
    InitTimer1();
    errorType   = ERROR_OK;
    
    while(1){ 
     ADCProcessing();    
     DataProcessing(); 
    }  
}

void DataProcessing(void){
    
    switch(errorType){
	 
	 case ERROR_OK:
	   break;
	 
	 case ERROR_UNDER_VOLTAGE:	 
	   TwoShortOneLong();
	   break;
	    
	 case ERROR_OVER_VOLTAGE:
       TwoShortTwoLong();
	   break;
	 
	 case ERROR_TMP_HIGH:
       ThreeShort();
	   break;      
	    
	 default:
       break;	 
	}
}