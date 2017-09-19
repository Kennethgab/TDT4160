#include "o3.h"
#include "gpio.h"
#include "systick.h"
int s;
int m;
int h;
int state;

/**************************************************************************//**
 * @brief Konverterer nummer til string 
 * Konverterer et nummer mellom 0 og 99 til string
 *****************************************************************************/
void int_to_string(char *timestamp, unsigned int offset, int i) {
    if (i > 99) {
        timestamp[offset]   = '9';
        timestamp[offset+1] = '9';
        return;
    }

    while (i > 0) {
	    if (i >= 10) {
		    i -= 10;
		    timestamp[offset]++;
		
	    } else {
		    timestamp[offset+1] = '0' + i;
		    i=0;
	    }
    }
}

/**************************************************************************//**
 * @brief Konverterer 3 tall til en timestamp-string
 * timestamp-argumentet mÃ¥ vÃ¦re et array med plass til (minst) 7 elementer.
 * Det kan deklareres i funksjonen som kaller som "char timestamp[7];"
 * Kallet blir dermed:
 * char timestamp[7];
 * time_to_string(timestamp, h, m, s);
 *****************************************************************************/
void time_to_string(char *timestamp, int h, int m, int s) {
    timestamp[0] = '0';
    timestamp[1] = '0';
    timestamp[2] = '0';
    timestamp[3] = '0';
    timestamp[4] = '0';
    timestamp[5] = '0';
    timestamp[6] = '\0';

    int_to_string(timestamp, 0, h);
    int_to_string(timestamp, 2, m);
    int_to_string(timestamp, 4, s);
}

typedef struct {
	volatile word CTRL;
	volatile word LOAD;
	volatile word VAL;
	volatile word CALIB;
} gpio_sys_map;


typedef struct {
   volatile word CTRL;
   volatile word MODEL;
   volatile word MODEH;
   volatile word DOUT;
   volatile word DOUTSET;
   volatile word DOUTCLR;
   volatile word DOUTTGL;
   volatile word DIN;
   volatile word PINLOCKN;
   } gpio_port_map_t;


   typedef struct {
       volatile gpio_port_map_t ports[6];
       volatile word unused_space[10];
       volatile word EXTIPSELL;
       volatile word EXTIPSELH;
       volatile word EXTIRISE;
       volatile word EXTIFALL;
       volatile word IEN;
       volatile word IF;
       volatile word IFS;
       volatile word IFC;
       volatile word ROUTE;
       volatile word INSENSE;
       volatile word LOCK;
       volatile word CTRL;
       volatile word CMD;
       volatile word EM4WUEN;
       volatile word EM4WUPOL;
       volatile word EM4WUCAUSE;
       } gpio_map_t;

void GPIO_ODD_IRQHandler(void){

	volatile word mask;

	  volatile gpio_map_t* pinregister;
	    pinregister = (gpio_map_t*) GPIO_BASE;



	  if(state == 0){
		  s = s +1;

		  char timestamp[7];
		  		time_to_string(timestamp, h, m , s);
		  		lcd_write(timestamp);

	  }
	  if(state == 1){
		  m = m +1;

		  char timestamp[7];
		  		time_to_string(timestamp, h, m , s);
		  		lcd_write(timestamp);


	  }
	  if(state == 2){
		  h = h + 1;

		  char timestamp[7];
		  		time_to_string(timestamp, h, m , s);
		  		lcd_write(timestamp);


	  }








   	// setting up Interrupt Flag Clear

   	volatile word interrupt = pinregister->IFC;
   	mask = 1;
   	mask = mask << 9;
   	interrupt = mask | interrupt;
   	pinregister->IFC = interrupt;


   }


void GPIO_EVEN_IRQHandler(void){

	volatile word mask;

		  volatile gpio_map_t* pinregister;
		    pinregister = (gpio_map_t*) GPIO_BASE;

		  volatile gpio_sys_map* sysregister;
		    sysregister = (gpio_sys_map*) SYSTICK_BASE;


		    if(state == 2)
		    {
		    	state = 3;
		    	sysregister->CTRL = 7;


		    }

		    if(state == 1)
		    {
		    	state = 2;
		    }


		    if(state == 0)
		    {
		    	state = 1;
		    }

		    if(state == 4)
		    {

		    	pinregister->ports[GPIO_PORT_E].DOUTCLR = 4;
		    	state = 0;

		    }




	volatile word interrupt = pinregister->IFC;
	   	mask = 1;
	   	mask = mask << 10;
	   	interrupt = mask | interrupt;
	   	pinregister->IFC = interrupt;



}


void SysTick_Handler(void){

	volatile gpio_sys_map* sysregister;
	sysregister = (gpio_sys_map*) SYSTICK_BASE;

	volatile gpio_map_t* pinregister;
	pinregister = (gpio_map_t*) GPIO_BASE;





if(s != 0)
{
	s = s-1;
}
else if(s == 0 && m != 0)
{
	s = 59;
	m = m -1;

}
else if( s == 0 && m == 00 && h != 0)
{
	s = 59;
	m = 59;
	h = h -1;

}




char timestamp[7];
				time_to_string(timestamp, h, m , s);
				lcd_write(timestamp);



if(s == 0 && m == 0 && h == 0)
{
	sysregister->CTRL = 0;
	state = 4;
				pinregister->ports[GPIO_PORT_E].DOUTSET = 4;


}












}







int main(void) {
    init();
    

    // initializing global screen variables

    s = 0;
    m = 0;
    h = 0;
    state = 0;

    char timestamp[7];
    		time_to_string(timestamp, h, m , s);
    		lcd_write(timestamp);



    volatile gpio_map_t* pinregister;
    pinregister = (gpio_map_t*) GPIO_BASE;

    volatile gpio_sys_map* sysregister;
    sysregister = (gpio_sys_map*) SYSTICK_BASE;


    volatile word mask = 4;
    pinregister->ports[GPIO_PORT_E].DOUTCLR = mask;


    volatile word interrupt = pinregister->IFC;
        	mask = 512;
        	interrupt = mask | interrupt;
        	pinregister->IFC = interrupt;


        	// setting up MODE pin 2


    mask = 15; // binær 1111
    mask = mask << 8; // leftshift -> 1111 0000 0000
    mask = ~mask;  // NOT: 0000 1111 1111
    volatile word model = pinregister->ports[GPIO_PORT_E].MODEL;
    model = model & mask; // MODEL : 0000 XXXX XXXX
    mask = 4;			// BINÆR 0100
    mask = mask << 8; // 0100 0000 0000
    model = model | mask; // MODEL: 0100 XXXX XXXX
    pinregister->ports[GPIO_PORT_E].MODEL = model;



    // setting up MODE pin 9

    mask = 15; // binær 1111
    mask = mask << 4; // leftshift -> 1111 0000
    mask = ~mask; // not: 0000 1111
    volatile word modeh = pinregister->ports[GPIO_PORT_B].MODEH;
    modeh = modeh & mask; // MODEH: 0000 XXXX
    mask = 1; // Binær 0001
    mask = mask << 4;
    modeh = modeh | mask;
    pinregister->ports[GPIO_PORT_B].MODEH = modeh;

   // Setting up MODE pin 10

    mask = 15; // binær 1111
    mask = mask << 8; // leftshift -> 1111 0000 0000
    mask = ~mask; // not: 0000 1111 1111
    modeh = pinregister->ports[GPIO_PORT_B].MODEH;
    modeh = modeh & mask; // MODEH: 0000 XXXX XXXX
    mask = 1; // Binær 0001
    mask = mask << 8;
    modeh = modeh | mask;  // 0001 XXXX XXXX
    pinregister->ports[GPIO_PORT_B].MODEH = modeh;






    // Setting up EXTIPSELH, PORT B PIN 9



    mask = 15; // binær 1111
    mask = mask << 4; // leftshift -> 1111 0000
    mask = ~mask; // not: 0000 1111
    volatile word extipselh = pinregister->EXTIPSELH;
    extipselh = extipselh & mask;
    mask = 1;
    mask = mask << 4;
    extipselh = extipselh | mask;
    pinregister->EXTIPSELH = extipselh;

    // Setting up EXTIFALL, PIN 9

    volatile word extifall = pinregister->EXTIFALL;
    mask = 1;
    mask = mask << 9;
    extifall = extifall | mask;
    pinregister->EXTIFALL = extifall;



    // Setting up Interrupt ENable, PIN 9

    volatile word enable = pinregister->IEN;
    mask = 1;
    mask = mask << 9;
    enable = mask | enable;
    pinregister->IEN = enable;




    // Setting up EXTIPSELH, PORT B PIN 10



    mask = 15; // binær 1111
    mask = mask << 8; // leftshift -> 1111 0000 0000
    mask = ~mask; // not: 0000 1111 1111
    extipselh = pinregister->EXTIPSELH;
    extipselh = extipselh & mask; // 0000 XXXX XXXX
    mask = 1; // BINÆR 0001
    mask = mask << 8; // 0001 0000 0000
    extipselh = extipselh | mask; // 0001 XXXX XXXX
    pinregister->EXTIPSELH = extipselh;

        // Setting up EXTIFALL, PIN 10

    extifall = pinregister->EXTIFALL;
    mask = 1;
    mask = mask << 10;
    extifall = extifall | mask;
    pinregister->EXTIFALL = extifall;



        // Setting up Interrupt ENable, PIN 10

   enable = pinregister->IEN;
    mask = 1;
    mask = mask << 10;
    enable = mask | enable;
    pinregister->IEN = enable;









    //Setting up SysTick Load

     sysregister->LOAD = FREQUENCY;





















while(1){


    }
    return 0;
}



