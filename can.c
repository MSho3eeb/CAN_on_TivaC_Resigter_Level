#include "can.h"

static const uint16 CANBitsValues[]={
                                     CAN_BIT_VALUE(2,1,1),
                                     CAN_BIT_VALUE(3,1,1),
                                     CAN_BIT_VALUE(3,2,2),
                                     CAN_BIT_VALUE(4,2,2),
                                     CAN_BIT_VALUE(4,3,3),
                                     CAN_BIT_VALUE(5,3,3),
                                     CAN_BIT_VALUE(5,4,4),
                                     CAN_BIT_VALUE(6,4,4),
                                     CAN_BIT_VALUE(6,5,4),
                                     CAN_BIT_VALUE(7,5,4),
                                     CAN_BIT_VALUE(7,6,4),
                                     CAN_BIT_VALUE(8,6,4),
                                     CAN_BIT_VALUE(8,7,4),
                                     CAN_BIT_VALUE(9,7,4),
                                     CAN_BIT_VALUE(9,8,4),
                                     CAN_BIT_VALUE(10,8,4)
};

void CANInit(CanIndexType can){
    if(can == CAN0){
        uint32 ratio;
        uint32 CANBits;
        uint32 PSDivide;
        uint32 RegVal;
        uint32 msg;

        SET_BIT(SYSCTL_RCGCGPIO_R, GPIO_PORTB_INDEX);
        SET_BIT(SYSCTL_RCGCCAN_R, CAN0_INDEX);
        SET_BIT(GPIO_PORTB_DEN_R, PORTB4);
        SET_BIT(GPIO_PORTB_DEN_R, PORTB5);
        GPIO_PORTB_CR_R = 0xFF;
        SET_BIT(GPIO_PORTB_AFSEL_R, PORTB4);
        SET_BIT(GPIO_PORTB_AFSEL_R, PORTB5);

        GPIO_PORTB_PCTL_R = 0x00;
        SET_BIT(GPIO_PORTB_PCTL_R, 19);
        SET_BIT(GPIO_PORTB_PCTL_R, 23);
        GPIO_PORTB_PCTL_R |= 3 << 8;
        GPIO_PORTB_PCTL_R |= 3 << 12;
        GPIO_PORTB_DR8R_R = 0x30;

        SET_BIT(CAN0_CTL_R, 0);
        while(GET_BIT(CAN0_IF1CRQ_R, 15) == 1);

        SET_BIT(CAN0_IF1CMSK_R, 7); // WRNRD
        SET_BIT(CAN0_IF1CMSK_R, 5); // ARB
        SET_BIT(CAN0_IF1CMSK_R, 4); // CONTROL

        CAN0_IF1ARB2_R = 0;
        CAN0_IF1MCTL_R = 0;

        for(msg = 1; msg <= 32; msg++){
            while(GET_BIT(CAN0_IF1CRQ_R, 15) == 1);
            CAN0_IF1CRQ_R = msg;
        }
        CAN0_IF1CMSK_R = 0x00;
        SET_BIT(CAN0_IF1CMSK_R, 2); // NEWDAT
        SET_BIT(CAN0_IF1CMSK_R, 3); // CLRINTPND

        for(msg = 1; msg <= 32; msg++){
            while(GET_BIT(CAN0_IF1CRQ_R, 15) == 1);
            CAN0_IF1CRQ_R = msg;
        }

        ratio = 32; // 16M / 500K to work on 500KHz

        while(ratio <= (CAN_MAX_BIT_DIVISOR * CAN_MAX_PRE_DIVISOR)){
            for(CANBits = CAN_MAX_BIT_DIVISOR; CANBits >= 4; CANBits--){
                PSDivide = ratio/CANBits;

                if((PSDivide * CANBits) == ratio){

                    RegVal = CANBitsValues[CANBits - 4];
                    RegVal |= ((PSDivide - 1) & 0x3F);

                    SET_BIT(CAN0_CTL_R, 6);
                    CAN0_BIT_R = RegVal;
                    CAN0_BRPE_R = ((PSDivide - 1) >> 6) & 0xF;
                    CLR_BIT(CAN0_CTL_R, 6);
                    break;
                }
            }
            ratio++;
            break;
        }

    }else if(can == CAN1){
        /*Not supported yet*/

    }
}

void CANEnable(CanIndexType can){
    if(can == CAN0){
        CLR_BIT(CAN0_CTL_R, 0);
    }else{
        CLR_BIT(CAN1_CTL_R, 0);
    }
}
void CANDisable(CanIndexType can){
    if(can == CAN0){
        SET_BIT(CAN0_CTL_R, 0);
    }else{
        SET_BIT(CAN1_CTL_R, 0);
    }
}

void CanMessageSet(CanIndexType can, uint32 ObjID,  MsgTypeType msgType, CanMsg *Msg){

    uint16 CmdMaskReg = 0;
    uint16 MaskReg0 = 0, MaskReg1 = 0;
    uint16 ArbReg0 = 0, ArbReg1 = 0;
    uint16 MsgCtrl = 0;
    bool bTransData;
    bool bExtendID;
    uint8* pData;
    uint32* pReg;

    bTransData = 0;

    while(GET_BIT(CAN0_IF1CRQ_R, 15) == 1);

    if((Msg->MsgID > 0x7FF) || (Msg->Flags & MSG_EXTENDED_ID)){
        bExtendID = 1;
    }else{
        bExtendID = 0;
    }

    SET_BIT(CmdMaskReg, 0); // DATAB
    SET_BIT(CmdMaskReg, 1); // DATAA
    SET_BIT(CmdMaskReg, 4); // CONTROL
    SET_BIT(CmdMaskReg, 7); // WRNRD

    if(msgType == MSG_TX){
        SET_BIT(MsgCtrl, 8); // TXRQST
        SET_BIT(ArbReg1, 13);// DIR
        bTransData = 1;
    }else if(msgType == MSG_RX){
        ArbReg1 = 0;
    }

    if(Msg->Flags & MSG_USE_ID_FILTER){
        if(bExtendID){
            MaskReg0 = Msg->MsgID & 0xFFFF;
            MaskReg1 = ((Msg->MsgID >> 16) & 0x1FFF);
        }else{
            MaskReg0 = 0;
            MaskReg1 = ((Msg->MsgID << 2) & 0x1FFF);
        }
    }
    if((Msg->Flags & MSG_USE_EXT_FILTER) == MSG_USE_EXT_FILTER){
        SET_BIT(MaskReg1, 15);
    }
    if((Msg->Flags & (MSG_USE_EXT_FILTER | MSG_USE_DIR_FILTER | MSG_USE_ID_FILTER))){
        SET_BIT(MaskReg1, 12); //UMASK
        SET_BIT(CmdMaskReg, 6); //MASK
    }
    SET_BIT(CmdMaskReg, 5); // ARB
    if(bExtendID){
        ArbReg0 |= Msg->MsgID & 0xFFFF;
        ArbReg1 |= (Msg->MsgID >> 16) & 0x1FFF;
    }else{
        ArbReg1 |= (Msg->MsgID << 2) & 0x1FFF;
        SET_BIT(ArbReg1, 15); // MSGVAL
    }
    MsgCtrl |= (Msg->MsgLen & 0x0F);

    if((Msg->Flags & MSG_FIFO) == 0){
        SET_BIT(MsgCtrl, 7); // EOB
    }
    if(Msg->Flags & MSG_TX_INT_ENABLE){
        SET_BIT(MsgCtrl, 11);//TXIE
    }
    if(Msg->Flags & MSG_RX_INT_ENABLE){
        SET_BIT(MsgCtrl, 10);//RXIE
    }

    if(bTransData){
        uint32 Indx, Val;
        pReg = (uint32*)CAN0_IF1DA1;
        pData = Msg->MsgDataPtr;
        for(Indx = 0; Indx< Msg->MsgLen; ){
            Val = pData[Indx++];
            if(Indx < Msg->MsgLen){ // in usual we use 1 byte
                Val |= (pData[Indx++] << 8);
            }
            HWREG(pReg++) = Val;
        }
    }
    CAN0_IF1CMSK_R = CmdMaskReg;
    CAN0_IF1MSK1_R = MaskReg0;
    CAN0_IF1MSK2_R = MaskReg1;
    CAN0_IF1ARB1_R = ArbReg0;
    CAN0_IF1ARB2_R = ArbReg1;
    CAN0_IF1MCTL_R = MsgCtrl;

    CAN0_IF1CRQ_R = ObjID & 0x3F;

}

static void IntCan(){
    SET_BIT(NVIC_EN1_R, 7);
}

void CANIntEnable(CanIndexType can){
    if(can == CAN0){
        IntCan();
        SET_BIT(CAN0_CTL_R, 1);
        SET_BIT(CAN0_CTL_R, 2);
        SET_BIT(CAN0_CTL_R, 3);
        __asm(" CPSIE i");
    }else if(can == CAN1){
        /**/
    }
}

void CanMessageGet(CanIndexType can, uint32 ObjID, char pendingInt, CanMsg *Msg){

    uint16 CmdMaskReg = 0, MaskReg0 = 0, MaskReg1 = 0;
    uint16 ArbReg0 = 0, ArbReg1 = 0, MsgCtrl = 0;

    SET_BIT(CmdMaskReg, 0); // DATAB
    SET_BIT(CmdMaskReg, 1); // DATAA
    SET_BIT(CmdMaskReg, 4); // CONTROL
    SET_BIT(CmdMaskReg, 7); // WRNRD
    SET_BIT(CmdMaskReg, 6); // MASK

    if(pendingInt){
        SET_BIT(CmdMaskReg, 3); //CLRINTPND
    }

    CAN0_IF1MSK2_R = CmdMaskReg;
    CAN0_IF2CRQ_R = ObjID & 0x3F;

    while(GET_BIT(CAN0_IF2CRQ_R, 15) == 1);

    MaskReg0 = CAN0_IF2MSK1_R;
    MaskReg1 = CAN0_IF2MSK2_R;
    ArbReg0  = CAN0_IF2ARB1_R;
    ArbReg1  = CAN0_IF2ARB2_R;
    MsgCtrl  = CAN0_IF2MCTL_R;

    Msg->Flags = 0x00;

    if(GET_BIT(ArbReg1, 14)){
        Msg->MsgID = (((ArbReg1 & 0x1FFF) << 16) | ArbReg0);
        Msg->Flags = MSG_EXTENDED_ID;
    }else{
        Msg->MsgID = (ArbReg1 & 0x1FFF) >> 2;
    }

    if(GET_BIT(MsgCtrl, 14)){
        Msg->Flags = MSG_DATA_LOST;
    }

    if(GET_BIT(MsgCtrl, 12)){
        if(GET_BIT(ArbReg1, 14)){
            Msg->MsgIDMask = ((MaskReg1 & 0x1FFF) << 16) | MaskReg0;
            if((Msg->MsgIDMask |= 0x1FFFFFFF)){
                Msg->Flags |= MSG_USE_ID_FILTER;
            }
        }
    }else{
        Msg->MsgIDMask = (MaskReg1 & 0x1FFF) >> 2;

        if(Msg->MsgIDMask != 0x7FF){
            Msg->Flags |= MSG_USE_ID_FILTER;
        }
    }

    if(GET_BIT(MaskReg1, 15)){
        Msg->Flags |= MSG_USE_EXT_FILTER;
    }

    if(GET_BIT(MaskReg1, 14)){
        Msg->Flags |= MSG_USE_DIR_FILTER;
    }

    if(GET_BIT(MsgCtrl, 11)){
        Msg->Flags |= MSG_TX_INT_ENABLE;
    }
    if(GET_BIT(MsgCtrl, 10)){
        Msg->Flags |= MSG_RX_INT_ENABLE;
    }
    if(GET_BIT(MsgCtrl, 15)){
        uint32 Indx, Val;
        uint32* pReg;
        pReg = (uint32*)CAN0_IF2DA1;

        Msg->MsgLen = MsgCtrl & 0x0F;

        for(Indx = 0; Indx < Msg->MsgLen; ){
            Val = HWREG(pReg++);
            Msg->MsgDataPtr[Indx++] = (uint8)Val;
            if(Indx < Msg->MsgLen){
                Msg->MsgDataPtr[Indx++] = (uint8)(Val >> 8);
            }
        }
        CAN0_IF2CMSK_R = 0;
        SET_BIT(CAN0_IF2CMSK_R, 2); // NEWDAT
        CAN0_IF2CRQ_R = ObjID & 0x3F;
        while(GET_BIT(CAN0_IF2CRQ_R, 15));
    }else{
        Msg->MsgLen = 0;
    }
}















