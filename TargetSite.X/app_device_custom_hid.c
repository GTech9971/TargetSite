/** INCLUDES *******************************************************/
#include "usb.h"
#include "usb_device_hid.h"

#include <string.h>

#include "system.h"

#include "target_site_check.h"

target_model target_list[4];

/** VARIABLES ******************************************************/
/* Some processors have a limited range of RAM addresses where the USB module
 * is able to access.  The following section is for those devices.  This section
 * assigns the buffers that need to be used by the USB module into those
 * specific areas.
 */
#if defined(FIXED_ADDRESS_MEMORY)
    #if defined(COMPILER_MPLAB_C18)
        #pragma udata HID_CUSTOM_OUT_DATA_BUFFER = HID_CUSTOM_OUT_DATA_BUFFER_ADDRESS
        unsigned char ReceivedDataBuffer[64];
        #pragma udata HID_CUSTOM_IN_DATA_BUFFER = HID_CUSTOM_IN_DATA_BUFFER_ADDRESS
        unsigned char ToSendDataBuffer[64];
        #pragma udata

    #elif defined(__XC8)
        unsigned char ReceivedDataBuffer[64] HID_CUSTOM_OUT_DATA_BUFFER_ADDRESS;
        unsigned char ToSendDataBuffer[64] HID_CUSTOM_IN_DATA_BUFFER_ADDRESS;
    #endif
#else
    unsigned char ReceivedDataBuffer[64];
    unsigned char ToSendDataBuffer[64];
#endif

volatile USB_HANDLE USBOutHandle;    
volatile USB_HANDLE USBInHandle;


void APP_DeviceCustomHIDInitialize()
{
    //initialize the variable holding the handle for the last
    // transmission
    USBInHandle = 0;

    //enable the HID endpoint
    USBEnableEndpoint(CUSTOM_DEVICE_HID_EP, USB_IN_ENABLED|USB_OUT_ENABLED|USB_HANDSHAKE_ENABLED|USB_DISALLOW_SETUP);

    //Re-arm the OUT endpoint for the next packet
    USBOutHandle = (volatile USB_HANDLE)HIDRxPacket(CUSTOM_DEVICE_HID_EP,(uint8_t*)&ReceivedDataBuffer[0],64);
}


void APP_DeviceCustomHIDTasks()
{   
    /* If the USB device isn't configured yet, we can't really do anything
     * else since we don't have a host to talk to.  So jump back to the
     * top of the while loop. */
    if( USBGetDeviceState() < CONFIGURED_STATE )
    {
        return;
    }

    /* If we are currently suspended, then we need to see if we need to
     * issue a remote wakeup.  In either case, we shouldn't process any
     * keyboard commands since we aren't currently communicating to the host
     * thus just continue back to the start of the while loop. */
    if( USBIsDeviceSuspended()== true )
    {
        return;
    }
    
    //Check if we have received an OUT data packet from the host
    if(HIDRxHandleBusy(USBOutHandle) == false)
    {   
        switch(ReceivedDataBuffer[0])
        {
            //シューティング開始
            case START_SHOOTING:{                
                //シューティング開始
                start_shooting(target_list);
                
                ToSendDataBuffer[0] = START_SHOOTING;
                //結果送信
                USBInHandle = HIDTxPacket(CUSTOM_DEVICE_HID_EP, (uint8_t*)&ToSendDataBuffer[0],64);            
                break;
            }
            //シューティング終了
            case STOP_SHOOTING:{
                //シューティング終了
                stop_shooting(target_list);
                ToSendDataBuffer[0] = STOP_SHOOTING;
                //結果送信
                USBInHandle = HIDTxPacket(CUSTOM_DEVICE_HID_EP, (uint8_t*)&ToSendDataBuffer[0],64);            
                break;
            }
            //ターゲット情報取得
            case GET_TARGET_INFO:{
                //1byte目 コマンド
                ToSendDataBuffer[0] = GET_TARGET_INFO;
                //2byte目 ターゲット数
                ToSendDataBuffer[1] = 4;
                //3byte目 ターゲットデバイスID RA3,RA4,RA5,RB7
                ToSendDataBuffer[2] = 0xB8;
                //結果送信
                USBInHandle = HIDTxPacket(CUSTOM_DEVICE_HID_EP, (uint8_t*)&ToSendDataBuffer[0],64);            
                break;
            }
            //ヒット情報取得
            case GET_HIT_INFO:{
                check_target(target_list);
                //1byte目 コマンド
                ToSendDataBuffer[0] = GET_HIT_INFO;
                //2byte目 ターゲット数
                ToSendDataBuffer[1] = 4;
                //3byte目 ヒットしたターゲットはビットを立てる
                uint8_t hid_data = 0x00;                
                //デバイスの数だけループし、ビットを立てる
                for(uint8_t i = 0; i < 4; i++){
                    if(target_list[i].is_hit){
                        hid_data |= target_list[i].device_id;
                    }
                }
                ToSendDataBuffer[2] = hid_data;
                
                //結果送信
                USBInHandle = HIDTxPacket(CUSTOM_DEVICE_HID_EP, (uint8_t*)&ToSendDataBuffer[0],64);            
                break;
            }
            
        }
        
        
        //Re-arm the OUT endpoint, so we can receive the next OUT data packet 
        //that the host may try to send us.
        USBOutHandle = HIDRxPacket(CUSTOM_DEVICE_HID_EP, (uint8_t*)&ReceivedDataBuffer[0], 64);
    }
}