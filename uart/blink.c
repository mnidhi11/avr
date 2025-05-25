//install sudo apt install avr-libc avrdude binutils-avr gcc-avr
/*Makefile
default: build

build:
	avr-gcc -0s -DF_CPU=16000000UL -mmcu=atmega328p -c -o main.c main.o
    avr-gcc -o main.bin main.o
	avr-objcopy -O ihex -R .eeprom main.bin main.hex
	avrdude -F -V -c arduino -p m328p -P /dev/ttyACM0 -b 115200 -U flash:w:main.hex*/

// ls /dev/ |grep ACM


#include <avr/io.h>
#include <util/delay.h>

#include <stdint.h>
#include <stdio.h>

#define F_CPU 16000000UL

#define MAX 100
#define uart_udr   UDR0 //starts from 0x0026 to 0x0028
#define uart_ucsra UCSR0A
#define uart_ucsrb UCSR0B
#define uart_ucsrc UCSR0C
#define uart_ubrrl  UBRR0L
#define uart_ubrrh  UBRR0H

void uart_init()
{
    uint16_t ubrr = 8;  // Hardcoded for 115200 at 16 MHz
    UBRR0H = (ubrr >> 8);
    UBRR0L = ubrr;
    uart_ucsrb = (1 << RXEN0) | (1 << TXEN0);
    uart_ucsrc = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_txchar(uint16_t data)
{
    /*while(uart_ucsra & (1 << UDRE0))
    {
        uart_udr = data;
    }*/
    while(!(uart_ucsra & (1 << UDRE0))); //if the UDRE0 is 0 then performing polling until its set to 1 -> copy the data into udr register
    uart_udr = data;
}

char uart_rxchar()
{
    while(!(uart_ucsra & ( 1 << RXC0)));//if rxc0 is set there is unread data, while evaluates to 0 and terminates
    return uart_udr;
 
        
}

void printstring(const char *s)
{
    while(*s)
    {
        uart_txchar(*s++);
       
    }
}

void rxstring(char *buf)
{
    uint8_t i = 0;
    char myval;
    
    if(uart_ucsra & (1 << RXC0))
    {
        printstring("in receiver\n");
    }
   
  
    while (!(uart_ucsra & (1 << RXC0))) {
            myval = uart_rxchar();
            if(myval != '\n')
            {
                buf[i] = myval;
               // uart_txchar(buf[i]);
                i++;
            }
           else {
               buf[i] = '\0';
               break;
            }
        if(uart_ucsra & (1 << RXC0))
        {
            printstring("set rx");
        }
            
    } //IF RXC0 IS SET there is pending data in udr - cannot read more data so exit while
    //if rxc0 is 0 then udr isof receiver is empty to read data
    

    

}

int main(void)
{
    //INitialize uart
    uart_init();
    printstring("hello\n");
    char rev[MAX] = {0};

    while(1)
    {
        printstring(">");
        _delay_ms(10000);
        printstring("\n");
       
        rxstring(rev);
        
        printstring("echo: \n");
    
        printstring(rev);
        _delay_ms(10000);
        
    }
  
    
}
