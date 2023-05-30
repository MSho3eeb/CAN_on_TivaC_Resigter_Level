#ifndef CAN_H__
#define CAN_H__

#include "tm4c123gh6pm.h"
#include <stdbool.h>
#include "Platform_Types.h"
#include "Bit_Maths.h"

#define CAN0_IF1DA1 0x4004003C

#define GPIO_PORTA_INDEX 0
#define GPIO_PORTB_INDEX 1

#define CAN0_INDEX 0
#define CAN1_INDEX 1

#define PORTB4 4
#define PORTB5 5

#define CAN_MAX_PRE_DIVISOR 1024
#define CAN_MAX_BIT_DIVISOR 19

/*******FLAGS**********/

#define MSG_TX_INT_ENABLE  0x0001
#define MSG_RX_INT_ENABLE  0x0002
#define MSG_EXTENDED_ID    0x0004
#define MSG_USE_ID_FILTER  0x0008
#define MSG_NEW_DATA       0x0080
#define MSG_DATA_LOST      0x0100
#define MSG_USE_DIR_FILTER (0x10 | MSG_USE_ID_FILTER)
#define MSG_USE_EXT_FILTER (0x20 | MSG_USE_ID_FILTER)
#define MSG_REMOTE_FRAME   0x0040
#define MSG_FIFO           0x0200
#define MSG_NO_FLAGS       0x0000
#define MSG_STATUS_MASK    (MSG_NEW_DATA | MSG_DATA_LOST)

#define CAN0_IF1DA1 0x4004003C
#define CAN0_IF2DA1 0x4004009C

#define HWREG(c) (*((volatile unsigned long *)c))


#define CAN_BIT_VALUE(seg1, seg2, sjw)                                        \
                                ((((seg1 - 1) << CAN_BIT_TSEG1_S) &           \
                                  CAN_BIT_TSEG1_M) |                          \
                                 (((seg2 - 1) << CAN_BIT_TSEG2_S) &           \
                                  CAN_BIT_TSEG2_M) |                          \
                                 (((sjw - 1) << CAN_BIT_SJW_S) &              \
                                  CAN_BIT_SJW_M))


typedef enum{
    CAN0 = 0,
    CAN1
}CanIndexType;

typedef enum{
    MSG_TX = 0,
    MSG_RX
}MsgTypeType;

typedef struct{
    uint32 MsgID;
    uint32 MsgIDMask;
    uint32 Flags;
    uint32 MsgLen;
    uint8* MsgDataPtr;
}CanMsg;

void CANInit(CanIndexType can);
void CANEnable(CanIndexType can);
void CANDisable(CanIndexType can);
void CanMessageSet(CanIndexType can, uint32 ObjID, MsgTypeType msgType, CanMsg *Msg);
void CanMessageGet(CanIndexType can, uint32 ObjID, char pendingInt, CanMsg *Msg);
void CANIntEnable(CanIndexType can);




#endif /* CAN_H__ */
