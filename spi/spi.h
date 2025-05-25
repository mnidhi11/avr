//
//  spi.h
//  
//
//  Created by Nidhi Mangalvedhekar on 5/16/25.
//
/*SCK PB5
MOSI PB3
CS PB2

REGISTERS
SPCR SET SPE BIT6 I.E SPI ENABLE FLAG
SPCR: CPOL CPHA WILL BE 0 DENOTING SAMPLING AND LATCH ON RISING EDGE OF CLOCK
SPCR : SET THE MASTER MODE MSTR BIT I.E 4TH BIT
SPSR: SET SPI2X TO 1- CLOCK WILL BE FREQ/2 - SPI CLOCK FREQ SCK WILL BE DOUBLED
SPDR: IS DATA REGISTER

IN MAX7219
SELECT INTENSITY REGISTER : 0XA -> BIT 8 TO 11 (12-15 ARE NOOP)
SET VALUE OF INTENSITY : 10 -> BIT 0-3
SELECT DATA REGISTER :
SEND DATA BYTE BY BYTE TO BE DISPALYED*/

#ifndef spi_h
#define spi_h
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>


#define F_CPU 16000000UL
#define SCK  PORTB5
#define MOSI PORTB3
#define MISO PORTB4
#define CS   PORTB2
#define DDR_SPI  DDRB //PORTB DATA DIRECTION REGISTER
#define CS_ENABLE()  PORTB &= ~(1 << CS)
#define CS_DISABLE() PORTB |=  (1 << CS)
#define SPI_DATA SPDR
#define MAX 8

volatile uint8_t tx_done = 0;

int animation[8] =
{
    0b00000000,
    0b01000010,
    0b00011000,
    0b00011000,
    0b01000010,
    0b00100100,
    0b00011000,
    0b00000000,
};

int arr[2][8] =
{
    {0x00,0x42,0x18,0x18,0x42,0x24,0x18,0x00},
    {0x00,0x12,0x20,0x4C,0x4C,0x20,0x12,0x00},
};

#endif /* spi_h */
