#ifndef __DEBUG_H
#define __DEBUG_H
#include "zf_common_headfile.h"



#define BYTE0(dwTemp)       (*(char *)(&dwTemp))	 
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))	 
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))


void sent_data(uint16_t A,uint16_t B,uint8_t C);



#endif
