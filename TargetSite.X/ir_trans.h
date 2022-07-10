#define _XTAL_FREQ 48000000

#include <xc.h> // include processor files - each processor file is guarded.  

/**
 * 0ビット点滅
 */
void low_bit();

/**
 * 1ビット点滅
 */
void high_bit();

/**
 * 赤外線点滅
 */
void blink();

/**
 * データ送信
 * @param data 上位2bit 識別ID, 下位6bit データ
 */
void send_bit(uint8_t data);