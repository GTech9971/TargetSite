
#include "ir_receive.h"

bool check_start_bit(){
    uint8_t head = 0;
    if(RA0 == 0){ return false; }
    
    while((RA0 == 1) && (head < 100)){
        head++;
        __delay_us(100);
    }
    
    return true;
}

uint8_t bit_find(){
    uint16_t width = 0;
    
    while(RA0 == 0){
        width++;
        __delay_us(5);
    }
    
    if(width < 60){ return 0xFF; }
    
    width = 0;    
    while(RA0 == 1 && width < 1000){
        width++;
        __delay_us(5);
    }
        
    return width > 200 ? 1 : 0;
}

uint8_t recv_data(){
    uint8_t data = 0;
    uint8_t mask = 0x80;
    for(uint8_t i = 0; i < 8; i++){
        if(bit_find() == 1){
            data |= mask;
        }
        mask >>= 1;
    }
    
    return data;
}