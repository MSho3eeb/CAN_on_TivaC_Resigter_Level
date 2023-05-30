
#ifndef         PLATFORM_TYPES_H_

#define     PLATFORM_TYPES_H_


/**********************************************************************************************************
* This define satisfies requirment [SWS_Platform_00044], [SWS_Platform_00064] :
* For each platform the register width of the CPU used shall be indicated by defining CPU_TYPE. 
*********************************************************************************************************/

#define CPU_TYPE CPU_TYPE_32 /* Our Platform (TM4C132GH6PM) is 32 bit Data Bus*/


/**********************************************************************************************************
* This define satisfies requirment [SWS_Platform_00045] : 
* According to the register width of the CPU used,  
* CPU_TYPE shall be assigned to one of the symbols CPU_TYPE_8, CPU_TYPE_16, CPU_TYPE_32 or CPU_TYPE_64. ⌋
*********************************************************************************************************/
#define CPU_TYPE_8 8
#define CPU_TYPE_16 16
#define CPU_TYPE_32 32
#define CPU_TYPE_64 64


/**********************************************************************************************************
* This define satisfies requirment SWS_Platform_00043] , [SWS_Platform_00038]:
* For each platform the appropriate bit order on register level 
* shall be indicated in the platform types header file using the symbol CPU_BIT_ORDER.
*********************************************************************************************************/
#define CPU_BIT_ORDER LSB_FIRST /* Our Platform (TM4C132GH6PM) is Little endian bit ordering*/


/**********************************************************************************************************
* This define satisfies requirment [SWS_Platform_00048]:
* In case of big endian bit ordering CPU_BIT_ORDER 
* shall be assigned to MSB_FIRST in the platform types header file.
*********************************************************************************************************/
#define MSB_FIRST 0x0

/**********************************************************************************************************
* This define satisfies requirment [SWS_Platform_00049]:
* In case of little endian bit ordering CPU_BIT_ORDER 
* shall be assigned to LSB_FIRST in the platform types header file.
*********************************************************************************************************/
#define LSB_FIRST 0x1


/**********************************************************************************************************
* This define satisfies requirment [SWS_Platform_00046] , [SWS_Platform_00039]:
* For each platform the appropriate byte order on memory level 
* shall be indicated in the platform types header file using the symbol CPU_BYTE_ORDER.
*********************************************************************************************************/
#define CPU_BYTE_ORDER LOW_BYTE_FIRST /* Our Platform (TM4C132GH6PM) is Little endian byte ordering*/


/**********************************************************************************************************
* This define satisfies requirment [SWS_Platform_00050]:
* In case of big endian byte ordering CPU_BYTE_ORDER 
shall be assigned to HIGH_BYTE_FIRST in the platform types header file.
*********************************************************************************************************/
#define HIGH_BYTE_FIRST 0x0

/**********************************************************************************************************
* This define satisfies requirment [SWS_Platform_00051]:
* In case of little endian byte ordering CPU_BYTE_ORDER 
shall be assigned to LOW_BYTE_FIRST in the platform types header file.
*********************************************************************************************************/
#define LOW_BYTE_FIRST 0x1

/* This typedef satisfies requirment [SWS_Platform_00026],[SWS_Platform_00027] */ 
typedef unsigned char                 boolean;

/* This typedef satisfies requirment [SWS_Platform_00013] */ 
typedef unsigned char                uint8;

/* This typedef satisfies requirment [SWS_Platform_00016] */ 
typedef signed char                  sint8;

/* This typedef satisfies requirment [SWS_Platform_00014] */ 
typedef unsigned short               uint16;

/* This typedef satisfies requirment [SWS_Platform_00017] */ 
typedef signed short                 sint16;

/* This typedef satisfies requirment [SWS_Platform_00015] */ 
typedef unsigned long                uint32;

/* This typedef satisfies requirment [SWS_Platform_00018] */ 
typedef signed long                  sint32;

/* This typedef satisfies requirment [SWS_Platform_00066] */ 
typedef unsigned long long           uint64;

/* This typedef satisfies requirment [SWS_Platform_00067] */ 
typedef signed long long             sint64;

/* This typedef satisfies requirment [SWS_Platform_00041]*/ 
typedef float                        float32; 

/* This typedef satisfies requirment [SWS_Platform_00042]*/ 
typedef double                       float64;  

/* This typedef satisfies requirment [SWS_Platform_00020]*/ 
typedef unsigned char                uint8_least;

/* This typedef satisfies requirment [SWS_Platform_00023]*/ 
typedef signed char                  sint8_least;

/* This typedef satisfies requirment [SWS_Platform_00021]*/ 
typedef unsigned short               uint16_least;

/* This typedef satisfies requirment [SWS_Platform_00024]*/ 
typedef signed short                 sint16_least;

/* This typedef satisfies requirment [SWS_Platform_00022]*/ 
typedef unsigned long                uint32_least;

/* This typedef satisfies requirment [SWS_Platform_00025]*/ 
typedef signed long                  sint32_least;



/**********************************************************************************************************
* This define satisfies requirment [SWS_Platform_00056]: 
* The standard AUTOSAR type boolean shall be implemented as an unsigned integer 
* with a bit length that is the shortest one
* natively supported by the platform (in general 8 bits).
**********************************************************************************************************/
typedef enum {FALSE,TRUE} TRUE_FALSE;

/**********************************************************************************************************
* This define satisfies requirment [SWS_Platform_00034], [SWS_Platform_00056], [SWS_Platform_00054] 
**********************************************************************************************************/
/* is this defines needed as anyway the above ENUM gives the true and false */
//#ifndef TRUE
//#define TRUE  1
//#endif
//
//#ifndef FALSE
//#define FALSE 0
//#endif


#endif /* PLATFORM_TYPES_H_ */
