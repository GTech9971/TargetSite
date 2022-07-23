/*
 * File:   target_site_check.c
 * Author: georg
 *
 * Created on 2022/07/23, 15:24
 */

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include <stdbool.h>
#include "target_site_check.h"

/** シューティング開始フラグ */
bool is_start_shooting;

/**
 * ターゲットの初期化
 * @param target_list
 */
void init_target(target_model target_list[]){
    //RA3
    target_list[0].device_id = 0x08;
    target_list[0].is_hit = false;
    target_list[0].sw_count = 0;            
    //RA4    
    target_list[1].device_id = 0x10;
    target_list[1].is_hit = false;
    target_list[1].sw_count = 0;
    //RA5    
    target_list[2].device_id = 0x20;
    target_list[2].is_hit = false;
    target_list[2].sw_count = 0;
    //RB7    
    target_list[3].device_id = 0x80;
    target_list[3].is_hit = false;
    target_list[3].sw_count = 0;
    
    is_start_shooting = false;
}

/**
 * ヒット情報の確認
 * ヒットしていれば、赤色LEDを消灯し、緑色LEDを点灯する
 * @param target_list
 */
void check_target(target_model target_list[]){
    if(is_start_shooting == false){
        return;
    }
    
    //カウントアップ
    if(RA3 == 0 && target_list[0].sw_count < SW_LIMIT){
        target_list[0].sw_count++;
    }
    
    if(RA4 == 0 && target_list[1].sw_count < SW_LIMIT){
        target_list[1].sw_count++;
    }
    
    if(RA5 == 0 && target_list[2].sw_count < SW_LIMIT){
        target_list[2].sw_count++;
    }
    
    if(RB7 == 0 && target_list[3].sw_count < SW_LIMIT){
        target_list[3].sw_count++;
    }
    
    
    // ヒット判定    
    if(target_list[0].sw_count == SW_LIMIT && target_list[0].is_hit == false){
        target_list[0].sw_count = 0;
        target_list[0].is_hit = true;
        LATC |= 0x01;
        LATC &= ~0x02;
    }
    
    if(target_list[1].sw_count == SW_LIMIT && target_list[1].is_hit == false){
        target_list[1].sw_count = 0;
        target_list[1].is_hit = true;
        LATC |= 0x04;
        LATC &= ~0x08;
    }
    
    if(target_list[2].sw_count == SW_LIMIT && target_list[2].is_hit == false){
        target_list[2].sw_count = 0;
        target_list[2].is_hit = true;
        LATC |= 0x10;
        LATC &= ~0x20;
    }
    
    if(target_list[3].sw_count == SW_LIMIT && target_list[3].is_hit == false){
        target_list[3].sw_count = 0;
        target_list[3].is_hit = true;
        LATC |= 0x40;
        LATC &= ~0x80;
    }
}


/**
 * シューティング開始
 */
void start_shooting(target_model target_list[]){
    init_target(target_list);
    
    is_start_shooting = true;
    //全消灯
    LATC = 0x00;
    //赤色点灯
    LATC |= 0x02;
    LATC |= 0x08;
    LATC |= 0x20;
    LATC |= 0x80;
}

/**
 * シューティング終了
 */
void stop_shooting(target_model target_list[]){
    //すでに停止していたら無視する
    if(is_start_shooting == false){
        return;
    }
    
    is_start_shooting = false;
    // ターゲット情報の初期化   
    //RA3
    target_list[0].device_id = 0x08;
    target_list[0].is_hit = false;
    target_list[0].sw_count = 0;            
    //RA4    
    target_list[1].device_id = 0x10;
    target_list[1].is_hit = false;
    target_list[1].sw_count = 0;
    //RA5    
    target_list[2].device_id = 0x20;
    target_list[2].is_hit = false;
    target_list[2].sw_count = 0;
    //RB7    
    target_list[3].device_id = 0x80;
    target_list[3].is_hit = false;
    target_list[3].sw_count = 0;
}
