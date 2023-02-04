#ifndef __IMG_PROCESS_H
#define __IMG_PROCESS_H
#include "zf_common_headfile.h"

#define fabs(x) ( (x)>0?(x):-(x) )

void Read_IMG (void);
void TrackBothEdge(void);
void VariableReset(void);
void Get_Road(void);
void Correction(void);


#endif