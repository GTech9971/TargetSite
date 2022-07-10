#include "ir_trans.h"

/**
 * 0ビット点滅
 */
void low_bit(){
    blink();
    LATA &= (0xFF - 0x10);
    __delay_us(600);
}

/**
 * 1ビット点滅
 */
void high_bit(){
    blink();
    LATA &= (0xFF - 0x10);
    __delay_us(1800);
}

/**
 * 赤外線点滅
 */
void blink(){
    for(uint8_t i = 0; i < 12; i++){
        LATA |= 0x10;
        __delay_us(25);
        LATA &= (0xFF - 0x10);
        __delay_us(25);
    }
}


/**
 * データ送信
 * @param data 上位2bit 識別ID, 下位6bit データ
 */
void send_bit(uint8_t data){
    uint8_t mask = 0x80;
    for(uint8_t i = 0; i < 8; i++){
        if(data & mask){
            high_bit();
        }else{
            low_bit();
        }
        mask = mask >> 0x01;
    }
}
