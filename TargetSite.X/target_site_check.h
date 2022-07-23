
// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#define SW_LIMIT 50

typedef enum{
    /** ターゲット情報取得 */
    GET_TARGET_INFO = 0x70,
    /** ヒット情報取得*/
    GET_HIT_INFO = 0x60,
    /** シューティング開始*/
    START_SHOOTING = 0x20,
    /** シューティング終了*/        
    STOP_SHOOTING = 0x10,    
} TARGET_SITE_COMMANDS;

/**
 * ターゲットモデル
 */
typedef struct{
    /** デバイスID */
    uint8_t device_id;
    /** ヒットしたかどうか*/
    bool is_hit;
    /** スイッチのカウント保持(チャタリング防止)*/
    uint8_t sw_count;
} target_model;

/**
 * ターゲットモデルの初期化
 */
void init_target(target_model target_list[]);

/**
 * ターゲットのヒット判定
 */
void check_target(target_model target_list[]);

/**
 * シューティング開始
 */
void start_shooting(target_model target_list[]);

/**
 * シューティング終了
 */
void stop_shooting(target_model target_list[]);




#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

