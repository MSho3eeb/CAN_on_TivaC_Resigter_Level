
#ifndef   _BIT_MATHS_H_
#define 	_BIT_MATHS_H_

#define SET_BIT(var , bit)   	 var|=(1<<bit)
#define CLR_BIT(var , bit)    	 var&=~(1<<bit)
#define TOGGLE_BIT(var , bit)    var^=(1<<bit)
#define GET_BIT(var , bit)    	 ((var>>bit)&1)
/* Check if a specific bit is set in any register and return true if yes */
#define BIT_IS_SET(var,bit) (( var & (1<<bit) )>>bit)

/* Check if a specific bit is cleared in any register and return true if yes */
#define BIT_IS_CLEAR(var,bit) ( !(var & (1<<bit)) )


#endif
