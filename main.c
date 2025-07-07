/*************************************************************************************************************************
	Filename:			        main.c	                                                                                            
**************************************************************************************************************************
        Project Reference: 			DM3600 MKII Mothercard FLT 								
  	Microcontroller:			Texas MSP430FR5969 )											
    	Software Language:			C Code.																			
        Compiler/Linker:			Crossworks for the MSP430,						
	Microncontroller Tools:		        Debugging and Programming via Texas spy by wire
	PROGRAMMER                              M.Payne									
*************************************************************************************************************************/

#include <__cross_studio_io.h>
#include "device.h"

void Init_MSP(void);
void Display_Test(void);
void refresh_display(void);


int FirstPower = 1;
int i,j;
int p5temp = 1;
int p7temp = 1;
int Release = 0;
int p4inTemp;
int bpressed[3] = {0,0,0};
int btnidx = 0x0080;

int disDig[6] = {0,0,0,0,0,0}; //Used to hex of each didget on display - auto update
int di = 0, dj =0;             //Dedicated i and j for refresh display loops


void main(void)
{
  //debug_printf("hello world\n");
  //debug_exit(0);
  if(FirstPower == 1){
      Init_MSP();

      
      FirstPower = 0;
  }
  _EINT();

  disDig[0] = 0x77;
  disDig[1] = 0x7c;
  disDig[2] = 0x39;
  disDig[3] = 0x5e;
  disDig[4] = 0x79;
  disDig[5] = 0x71;
  //TA3CCTL0 &= ~CCIE; //Disable timer for automatic display refresh
  //Display_Test();

}




void Init_MSP(void){
    /*****************************************************************************************************************************************
    init_ports   PxDIR bits 0-input/1-output      PxSEL bits 0-normal/1-secondary function
    *****************************************************************************************************************************************/

    /* PORT J

    PJ.0  = Output - TXEN 
    PJ.1  = Output - COMMEN 
    PJ.2  = Output 
    PJ.3  = Output 
    PJ.4  = Output
    PJ.5  = Output 
    PJ.6  = XT2 
    PJ.7  = XT2 

    BIT         7       6       5       4       3       2       1       0
    PJDIR       1       1       1       1       1       1       1       1           0xFF
    PJSEL0      1       1       0       0       0       0       0       0           0xC0
    PJSEL1      0       0       0       0       0       0       0       0           0x00
    PJOUT       0       0       0       0       0       0       1       1
    PJREN       0       0       0       0       0       0       0       0
    */

    PJDIR =  0xFF;
    PJSEL0 = 0xC0;
    PJSEL1 = 0x00;
    PJOUT =  0x00;
    PJREN =  0x00;


    /* PORT 1
    P1.0  = Output- PWM1
    P1.1  = Input CJ veREF+
    P1.2  = Output- 
    P1.3  = Output
    P1.4  = Output
    P1.5  = Output
    P1.6  = Output
    P1.7  = output

    BIT         7       6       5       4       3       2       1       0
    P1DIR       1       1       1       1       1       1       1       1           0xFF
    P1SEL0      0       0       0       0       0       0       1       0           0x02
    P1SEL1      1       1       0       0       0       0       1       0           0x02
    P1OUT       0       0       0       0       0       0       0       0           0x00
    P1REN       0       0       0       0       0       0       0       0           0x00
    P1IES       x       x       x       1       x       0       x       x           0x00
    */

    P1DIR =  0xFF;
    P1SEL0 = 0x02;
    P1SEL1 = 0x02; 
    P1OUT =  0x00;
    P1REN =  0x00;

    /*PORT 2
    P2.0  = USB RXD
    P2.1  = USB TXD
    P2.2  = USB SENSE
    P2.3  = Output
    P2.4  = MOD_EN1  set as input at start up to detect output card with pull up
    P2.5  = MOD_EN2  set as input at start up to detect output card with pull up
    P2.6  = ADC_EN1  
    P2.7  = ADC_EN2  controls thermistor

    BIT             7       6       5       4       3       2       1       0
    P2DIR           0       1       0       0       1       0       0       0           0x08
    P2SEL0          0       0       0       0       0       0       0       0           0x00
    P2SEL1          0       0       0       0       0       0       0       0           0x00
    P2OUT           1       1       0       0       0       0       0       0           0x00
    P2REN           1       1       1       1       0       1       1       1           0xF7
    */
    P2DIR =  0x48;
    P2SEL0 = 0x00;
    P2SEL1 = 0x00;
    P2OUT =  0x40;
    P2REN =  0xF7;

    /* PORT 3

    P3.0  = Out - 
    P3.1  = Output - SCL ADC
    P3.2  = Output
    P3.3  = DISOUT_1B
    P3.4  = DISOUT_2A
    P3.5  = DISOUT_2B
    P3.6  = Output Dc to DC push pull 150 KHz
    P3.7  = Output DC to DC push pull 150 KHz

    BIT         7       6       5       4       3       2       1       0
    P3DIR       1       1       1       1       1       1       1       1           0xFE
    P3SEL0      0       0       0       0       0       0       0       0           0x00
    P3SEL1      1       1       0       0       0       0       0       0           0xC0
    P3OUT       0       0       0       0       0       0       0       0
    */

    P3DIR =  0xFF;
    P3SEL0 = 0x00;
    P3SEL1 = 0x00;
    P3OUT =  0x00;
    P3REN =  0x00;

    /* PORT 4

    /* PORT 4

    P4.0  = Output 
    P4.1  = Output  
    P4.2  = Output RXD enabled when USB not connected connects to RS485 interface start as input week pull down
    P4.3  = Output TXD enabled when USB not connected connects to RS485 interface start as input week pull down 
    P4.4  = Input TA1_CLK (for future freq version set as output
    P4.5  = Input SW3 
    P4.6  = Input SW2
    P4.7  = Input SW1 

    BIT         7       6       5       4       3       2       1       0
    P4DIR       0       0       0       0       0       0       1       1           0x13
    P4SEL0      0       0       0       0       0       0       0       0           0x00
    P4SEL1      0       0       0       0       0       0       0       0           0x00
    P4OUT       1       1       1       0       0       1       0       0           0xE0
    P4REN       1       1       1       0       1       1       0       0           0xEC
    */

    P4DIR =  0x03;
    P4SEL0 = 0x00;
    P4SEL1 = 0x00;
    P4OUT =  0xE0;
    P4REN =  0xEC;

     /* PORT 5
    port 5.0 not available on 80 pin version used by display
    P5.0  = Output-seg a  
    P5.1  = Output-seg b 
    P5.2  = Output-seg c 
    P5.3  = Output-seg d  
    P5.4  = Output-seg e  
    P5.5  = Output-seg f   
    P5.6  = Output-seg g 
    P5.7  = Output-seg DP  

    BIT         7       6       5       4       3       2       1       0
    P5DIR       1       1       1       1       1       1       1       1           0xFF
    P5SEL0      0       0       0       0       0       0       0       0           0x00
    P5SEL1      0       0       0       0       0       0       0       0           0x00
    P5OUT       0       0       0       0       0       0       0       0
    P5REN       0       0       0       0       0       0       0       0
    */

    P5DIR =  0xFF;
    P5SEL0 = 0x00;
    P5SEL1 = 0x00;
    P5OUT =  0x00;
    P5REN =  0x00;

    /* PORT 6
    Set as output low, port 6  
    P6.0  = Output
    P6.1  = Output
    P6.2  = Output
    P6.3  = Output - LED1
    P6.4  = Output - LED2 
    P6.5  = Output - LED3
    P6.6  = Output - LED4 
    P6.7  = Output - 

    BIT         7       6       5       4       3       2       1       0
    P6DIR       1       1       1       1       1       1       1       1           0xFF
    P6SEL0      0       0       0       0       0       0       0       0           0x00
    P6SEL1      0       0       0       0       0       0       0       0           0x00
    P6OUT       0       1       1       1       1       0       0       0
    */

    P6DIR =  0xFF;
    P6SEL0 = 0x00;
    P6SEL1 = 0x00;
    P6OUT =  0x78;
    P6REN =  0x00;

    /* PORT 7
    Set as output low, port 7 is used by display
    /* PORT 7
    Set as output low, port 7 is used by display
    P7.0  = Output - dig1
    P7.1  = Output - dig2
    P7.2  = Output - dig3
    P7.3  = Output - dig4
    P7.4  = Output - dig5
    P7.5  = Output - dig6
    P7.6  = Output - EN1
    P7.7  = Output DISOUT1A

    BIT         7       6       5       4       3       2       1       0
    P7DIR       1       0       1       1       1       1       1       1           0xBF
    P7SEL0      0       0       0       0       0       0       0       0           0x00
    P7SEL1      0       0       0       0       0       0       0       0           0x00
    P7OUT       0       0       0       0       0       0       0       0
    P7REN       0       1       0       0       0       0       0       0
    */

    P7DIR =  0xBF;
    P7SEL0 = 0x00;
    P7SEL1 = 0x00;
    P7OUT =  0x00;
    P7REN =  0x40;

    /* PORT 8  Not available on 80 pin device

    P8.0  = Output - SW1 
    P8.1  = Output - SW2
    P8.2  = Output - SW3 
    P8.3  = Output 
    P8.4  = Output
    P8.5  = Output 
    P8.6  = Output 
    P8.7  = input adc A4  CJ reading

    BIT         7       6       5       4       3       2       1       0
    P8DIR       0       1       1       1       1       0       0       0           0x78
    P8SEL0      1       0       0       0       0       0       0       0           0x80
    P8SEL1      1       0       0       0       0       0       0       0           0x80
    P8OUT       0       0       0       0       0       1       1       1           0x03
    P8REN       0       0       0       0       0       1       1       1           0x03
    */

    P8DIR =  0x58;
    P8SEL0 = 0x80;
    P8SEL1 = 0x80;
    P8OUT =  0x03;
    P8REN =  0x03;




    /* PORT 9

    P9.0  = Output/Input SDA_1
    P9.1  = Output/Input SDA_2
    P9.2  = Output/Input SDA_3
    P9.3  = Output/Input ADC_EN3 (switch VmA to resitance mode)
    P9.4  = set 4K7 shunt 
    P9.5  = input comparator for power  detect 2V threshold
    P9.6  = Output  
    P9.7  = Output 

    BIT         7       6       5       4       3       2       1       0
    P9DIR       1       1       0       1       1       0       0       0           0xF8
    P9SEL0      0       0       1       0       0       0       0       0           0x10
    P9SEL1      0       0       1       0       0       0       0       0           0x10
    P9OUT       0       0       0       0       0       0       0       0           0x00
    P9REN       0       1       0       0       0       0       0       0           0x40
    */

    P9DIR =  0xD8;
    P9SEL0 = 0x20;
    P9SEL1 = 0x20;
    P9OUT =  0x08;
    P9REN =  0x07;

    /* PORT 10

    P10.0  = Output 
    P10.1  = Output EN2
    P10.2  = Output -


    BIT          2       1       0
    P10DIR       1       0       1           0xFE
    P10SEL0      0       0       0           0x00
    P10SEL1      0       0       0           0x00
    P10OUT       0       0       0
    P10REN       0       1       0
    */

    P10DIR =  0x05;
    P10SEL0 = 0x00;
    P10SEL1 = 0x00;
    P10OUT =  0x00;
    P10REN =  0x02;   



    // Clock System Setup
    CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
    CSCTL4 =   0;
    CSCTL4 |=   HFXTOFF + LFXTOFF;
    CSCTL1 |= DCOFSEL_6;                       // Set DCO to 8MHz

    CSCTL2 = SELA__LFMODOSC | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = 0;     // Set all dividers off


    // interrupt every 62uS  clock 8 Mhz / 8 = 1000KHz  50000 counts for 0.05 S
    TA3CCTL0 = CCIE; // TACCR3 interrupt enabled
    TA3CCR0 = 500;// 0.5 mS
    TA3CTL = TASSEL_2 + MC_1 + TACLR + ID__8; // SMCLK, continuous mode

}

//INTERRUPTS
#pragma vector = TIMER3_A0_VECTOR
  __interrupt void Timer3_A0_ISR(void)
  {
  refresh_display ();
  }


void Display_Test(void){
  //    7 segment display + dp  bitmap:
  //    PORT   5.7 |5.6 |5.5 |5.4 |5.3 |5.2 |5.1 |5.0   
  //    SEG   |DP  |G   |F   |E   |D   |C   |B   |A

  //    PORT7 digit1 to 6 enables using discrete transistor drive to boost current sink so is PORT active high.
  //    PORT   7.7   |7.6   |7.5     |7.4    |7.3    |7.2     |7.1     |7.0   
  //    SEG          |      |digit6  |digit5 |digit4 |digit 3 |digit 2 |digit1
  P5OUT = 1;
  P7OUT = 1;


  //Individual Segments - loop x times? 
  for(i = 0; i<6 ; i++){
      P7OUT = p7temp << i;

      for(j=0;j<8;j++){
        P5OUT = p5temp << j;
  
        __delay_cycles(2000000); //250ms delay
      } 
      j=0;
  }


  __delay_cycles(2000000);
  //Block segments
  P5OUT = 0xff;
  p7temp = 1;

  for(i = 0;i<6;i++){
    P7OUT = p7temp << i;
    __delay_cycles(2000000);
  }

  __delay_cycles(2000000);
  //Button Test
  p7temp = 1;
  P5OUT = 0x08;

  while(Release == 0){ //While all the buttons havent been pressed
    for(i=0; i<3;i++){  //For each unit (3 buttons)
      P5OUT = 0x08;     //Turn off segments
      P7OUT = p7temp << i;
      if(bpressed[i] == 1){
      P5OUT = 0xff;
      }
    
      p4inTemp = P4IN;
      if(~p4inTemp & (btnidx>>i)) { //if button i has been pressed
        bpressed[i] = 1;
      }
                   
    }

    if(bpressed[0]+bpressed[1]+bpressed[2] == 3){
        Release = 1;
        __delay_cycles(2000000);
      }
  }

}


//Ran from timer A3
void refresh_display(void){

  //Loop through each didget and update segments according to array disDig[]
  //Individual Segments - loop x times? 
  int p7distemp = 1;
  for(di = 0; di<6 ; di++){
      P7OUT = p7distemp << di;
      P5OUT = disDig[di];
  }

}

