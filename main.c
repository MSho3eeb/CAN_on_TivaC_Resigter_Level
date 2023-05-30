#define TX  0

#if TX == 1
#include "can.h"

volatile uint32 MsgSentCount = 0;

void INTCanHandler(){
    MsgSentCount++;
}

int main(void)
{
    CanMsg Msg;
    uint32 MsgData = 'd';
    uint8* MsgPtr;
    MsgPtr = (uint8*)&MsgData;
    CANInit(CAN0);
    CANIntEnable(CAN0);
    CANEnable(CAN0);

    Msg.MsgID = 1;
    Msg.MsgIDMask = 0;
    Msg.Flags = MSG_TX_INT_ENABLE;
    Msg.MsgLen = sizeof(MsgData);
    Msg.MsgDataPtr = MsgPtr;

    while(1){
        CanMessageSet(CAN0, 1, MSG_TX, &Msg);
    }

	return 0;
}
#else
#include "can.h"

bool msgRecieved = 0;
void INTCanHandler(){
    msgRecieved = 1;
}
int main(void){
    CanMsg Msg;
    uint32 MsgData = 'd';
    uint8* MsgPtr;
    MsgPtr = (uint8*)&MsgData;
    CANInit(CAN0);
    CANIntEnable(CAN0);
    CANEnable(CAN0);

    Msg.MsgID = 1;
    Msg.MsgIDMask = 0;
    Msg.Flags = MSG_RX_INT_ENABLE;
    Msg.MsgLen = sizeof(MsgData);
    Msg.MsgDataPtr = MsgPtr;
    CanMessageSet(CAN0, 1, MSG_RX, &Msg);
    while(1){
        if(msgRecieved){
            CanMessageGet(CAN0, 1, 0, &Msg);
        }
    }
}

#endif
