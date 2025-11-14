#include <avr/io.h>
#include <util/delay.h>

#define DIN PB3
#define CS  PB2
#define CLK PB5 

// 8x8 "A" Symbol
const uint8_t symbol_A[8] = {
    0x18, //   **   
    0x24, //  *  *  
    0x42, // *    * 
    0x7E, // ****** 
    0x42, // *    * 
    0x42, // *    * 
    0x42, // *    * 
    0x00  //        
};

// Bit-banged SPI send
void spi_send(uint8_t data){
    for(int i=0; i<8; i++){
        if(data & 0x80) 
            PORTB |= (1<<DIN);
        else 
            PORTB &= ~(1<<DIN);

        PORTB |= (1<<CLK);
        _delay_us(1);
        PORTB &= ~(1<<CLK);
        data <<= 1;
    }
}

// Write to MAX7219
void max7219_write(uint8_t addr, uint8_t data){
    PORTB &= ~(1<<CS);
    spi_send(addr);
    spi_send(data);
    PORTB |= (1<<CS);
}

// Initialize MAX7219
void max7219_init(void){
    DDRB |= (1<<DIN)|(1<<CS)|(1<<CLK);

    max7219_write(0x0F, 0x00); // display test OFF
    max7219_write(0x09, 0x00); // decode mode OFF
    max7219_write(0x0B, 0x07); // scan limit: all digits
    max7219_write(0x0C, 0x01); // normal operation
    max7219_write(0x0A, 0x0F); // brightness max

    // Clear display
    for(int i=1; i<=8; i++)
        max7219_write(i, 0x00);  
}

int main(void){
    max7219_init();

    while(1){
        // Display "A"
        for(int row=0; row<8; row++){
            max7219_write(row+1, symbol_A[row]);
        }
        _delay_ms(500);
    }
}
