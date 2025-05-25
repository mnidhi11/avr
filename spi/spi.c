


#include "spi.h"

ISR(SPI_STC_vect)
{
    // Transmission complete interrupt
     // Prepare next byte or handle post-send logic
     // For example, toggle a bit or send the next pattern
     PORTB ^= (1 << PB1);  // Toggle LED to show interrupt
     tx_done = 1;
}

void spi_init()
{
    DDR_SPI = (1 << MOSI) | (1 << SCK) | (1 << CS);
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPIE) ;
    SPSR = (1 << SPI2X); //DOUBLES THE SCK FREQ
}

void spi_transfer(uint8_t data)
{
    SPI_DATA = data;
    
    //while(!(SPSR & (1 << SPIF)));//use this when you want to  use polling mechanism
    while(tx_done == 0);
    
    tx_done = 0;
  //  return SPI_DATA;
}

void clear_screen(uint8_t *tx_data)
{
    for(uint8_t col = 1; col <= MAX; col++)
    {
        CS_ENABLE();
        *tx_data = col;
        spi_transfer(*tx_data);
        *tx_data = 0b00000000;
        spi_transfer(*tx_data);
        CS_DISABLE();
        
    }
}

void set_led_intensity(uint8_t *tx_data)
{
     CS_ENABLE();
     *tx_data = 0b00001010; //REG 0XA OF MAX LED MATRIX REPRESENTS INTENSITY REG (MEMORY MAPPED)
     spi_transfer(*tx_data);
     *tx_data = 0b00000011; //SET TO MINIMUM
     spi_transfer(*tx_data);
     CS_DISABLE();
}

void set_decode_mode(uint8_t *tx_data)
{
    CS_ENABLE();
    *tx_data = 0b00001001;
    spi_transfer(*tx_data);
    *tx_data = 0b00000000; //SET TO MINIMUM
    spi_transfer(*tx_data);
    CS_DISABLE();
}

void clear_shutdown(uint8_t *tx_data)
{
    CS_ENABLE();
    *tx_data = 0b00001100;
    spi_transfer(*tx_data);
    *tx_data = 0b00000001;
    spi_transfer(*tx_data);
    CS_DISABLE();
}

void set_scan_limit(uint8_t *tx_data)
{
    CS_ENABLE();
    *tx_data = 0b00001011;
    spi_transfer(*tx_data);
    *tx_data = 0b00000111; //SET TO ALL DIGITS
    spi_transfer(*tx_data);
    CS_DISABLE();
}
void send_data(uint8_t *tx_data)
{
    
    // set the screen data
    for (int j = 1; j<9; j++)
    {
        for (int i = 0; i<256; ++i)
        {
            CS_ENABLE();
            *tx_data = j; // use digit J (COLUMN)
            spi_transfer (*tx_data);
            *tx_data = i; // binary count
            spi_transfer (*tx_data);
            CS_DISABLE();
           _delay_ms(1);
        }
    }
}

void display(uint8_t *tx_data)
{
    for(int i = 1; i < 3;i++)
    {
       
       
        for(int j = 1; j < 9; j++)
        {
            CS_ENABLE();
            *tx_data = j; // use digit J (COLUMN)
            spi_transfer (*tx_data);
            *tx_data = arr[i-1][j-1]; // binary count
            spi_transfer (*tx_data);
            CS_DISABLE();
           // _delay_ms(10);
        }
      
        _delay_ms(1000);
    }
}

int main()
{
    sei();
    spi_init();
    DDRB |= (1 << PB1); //led toggle at interrupt
    uint8_t tx_data;
    
    //clear the max7219 screen on boot up
    //each column is connected to segments in 8x8 led matrix
    //select each col first -> D0-D7 bits address i/e MSB byte
    //The LSB 8 bit represents the data
    
 
    clear_screen(&tx_data);
    
    //SET MAX7129 INTENSITY WITH MINIMUM ON
  
    set_led_intensity(&tx_data);
    //SELETCT NO DECODE REGISTER
    set_decode_mode(&tx_data);
    
    //END SHUTDOWN MODE
    clear_shutdown(&tx_data);
    
    //SELECT SCAN LIMIT REGISTER AND SET IT TO MAX 1 (DISPLAY ALL 0-7 DIGITS)
    set_scan_limit(&tx_data);
    

   // send_data(&tx_data); //Lights up all leds
    clear_screen(&tx_data);
    
    display(&tx_data);
   

    
}
