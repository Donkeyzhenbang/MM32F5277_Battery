#include "img_process.h"


uint8 Width[MT9V03X_H],LeftEdge_X[MT9V03X_H] ={0};
uint8 RightEdge_X[MT9V03X_H]={MT9V03X_W-1};
uint8 Middle_X[MT9V03X_H]={MT9V03X_W/2-1};
uint8 LeftEdge_GetFlag[MT9V03X_H];
uint8 RightEdge_GetFlag[MT9V03X_H];
uint8 max_left_line = 0,max_left_row = 0,max_right_line = 179,max_right_row = 0;
uint8 image_process[MT9V03X_H][MT9V03X_W];

/********************寻找边界****************/
uint8 StartScanPoint_line= MT9V03X_W/2-1;                       //赛道起始扫描点
uint8 StartScanPoint_row = MT9V03X_H-1;
uint8 LeftStart_line= MT9V03X_W/2-1;                                //赛道起始扫描点
uint8 RightStart_line = MT9V03X_W/2-1;
uint8 L_cross_flag = 0,R_cross_flag = 0;
#define MaxValue 20
uint8 row;  //行数
uint8 line; //列数

int8 B;//斜率计算
int jiezhihang,DUANDIAN;

void Get_Road(void)
{
		ips200_show_gray_image(0, 0, (const uint8 *)mt9v03x_image, MT9V03X_W, MT9V03X_H, 188, 60, 0);
    Read_IMG();
    VariableReset();                //参数清零
    TrackBothEdge();                //找初始左右边缘
		Correction();
}

void Read_IMG (void)
{
    int i,j;
    for(i=0;i<MT9V03X_H;i++)
    {
        for(j=0;j<MT9V03X_W;j++)
        {
            image_process[i][j]=mt9v03x_image[i][j];
        }
    }
}

/********************图像初始化****************/
void VariableReset(void)
{
    for(row=0;row<MT9V03X_H;row++)
    {
        Middle_X[row] = MT9V03X_W/2-1;
        LeftEdge_X[row]=0;
        RightEdge_X[row]=MT9V03X_W-1;

        LeftEdge_GetFlag[row] =0;
        RightEdge_GetFlag[row]=0;
    }
}

void TrackBothEdge(void)
{
    jiezhihang=0;
    int16 value_confirm=0;
    uint8 start=90;

    for(row=StartScanPoint_row;row>=1;row--)
    {
        if(row==StartScanPoint_row)
        {
            for(line=start;line>1;line--)//左边缘
            {
                if((image_process[row][line] - image_process[row][line-2])>MaxValue)
                {
                    LeftEdge_X[row] = line;
                    LeftEdge_GetFlag[row]=1;
                    break;
                }
            }

            for(line=start;line<MT9V03X_W-2;line++)//右边缘
            {
                if((image_process[row][line] - image_process[row][line+2])>MaxValue)
                {
                    RightEdge_X[row] = line;
                    RightEdge_GetFlag[row]=1;
                    break;
                }
            }
        }

        else
        {
            for(line=LeftStart_line;line>1;line--)//左边缘
            {
                if(line>=4)
                  value_confirm = image_process[row][line] - image_process[row][line-4];
                else
                    value_confirm = image_process[row][line] - image_process[row][0];

                if((image_process[row][line] - image_process[row][line-2])>MaxValue && value_confirm>20)
                {
                    LeftEdge_X[row] = line;
                    LeftEdge_GetFlag[row]=1;
                    break;
                }
            }

            for(line=RightStart_line;line<MT9V03X_W-2;line++)//右边缘
            {
                if(line<MT9V03X_W-4)
                  value_confirm = image_process[row][line] - image_process[row][line+4];
                else
                    value_confirm = image_process[row][line] - image_process[row][179];
                if((image_process[row][line] - image_process[row][line+2])>MaxValue && value_confirm>20)
                {
                    RightEdge_X[row] = line;
                    RightEdge_GetFlag[row]=1;
                    break;
                }
            }
        }
        start = Middle_X[MT9V03X_H-1];

        if(row<58)
        {
            if(fabs(image_process[row][(RightEdge_X[row]+LeftEdge_X[row])/2]-image_process[row+2][(RightEdge_X[row+2]+LeftEdge_X[row+2])/2])>40)//||image_process[row][(RightEdge_X[row]+LeftEdge_X[row])/2]<130
            {
                LeftEdge_GetFlag[row]=0;
                RightEdge_GetFlag[row]=0;
            }
        }

        if ((LeftEdge_GetFlag[row] == 1 || RightEdge_GetFlag[row] == 1))  //中线处理
				{
						Middle_X[row] = (RightEdge_X[row] + LeftEdge_X[row]) / 2;
				}

        if(LeftEdge_GetFlag[row]==1)                            //起点设置
        {
            LeftStart_line = LeftEdge_X[row] + 10;
        }
        else
        {
            if(LeftEdge_GetFlag[row+1]==1&&RightEdge_GetFlag[row+1]==1)
            {
                LeftStart_line = Middle_X[row+1];
            }
            else
            {
                LeftStart_line = Middle_X[row];
            }
        }

        if(RightEdge_GetFlag[row]==1)
        {
            RightStart_line = RightEdge_X[row] - 10;
        }
        else
        {
            if(LeftEdge_GetFlag[row+1]==1&&RightEdge_GetFlag[row+1]==1)
            {
                RightStart_line = Middle_X[row+1];
            }
            else
            {
                RightStart_line = Middle_X[row];
            }
        }

        if(LeftStart_line>MT9V03X_W-1)                           //限制出界
        {
            LeftStart_line = MT9V03X_W-1;
        }
        if(LeftStart_line>RightEdge_X[row])
        {
            LeftStart_line = RightEdge_X[row] - 5;
        }
        if(RightStart_line<1)
        {
            RightStart_line = 1;
        }
        if(RightStart_line<LeftEdge_X[row])
        {
            RightStart_line = LeftEdge_X[row] + 5;
        }
        if(LeftStart_line>RightStart_line)
        {
            line = LeftStart_line;
            LeftStart_line=RightStart_line;
            RightStart_line=line;
        }

        if(LeftEdge_X[row]>=max_left_line)
        {
            max_left_line = LeftEdge_X[row];
            max_left_row = row;
        }

        if(RightEdge_X[row]<=max_right_line)
        {
              max_right_line = RightEdge_X[row];
              max_right_row = row;
        }

		}
}


/********************边界显示****************/
void Correction(void)
{
    uint8 i;

    for(i=1;i<=59;i++)
    {
        ips200_draw_point(LeftEdge_X[i],i,RGB565_RED);
        ips200_draw_point(RightEdge_X[i],i,RGB565_BLUE);
        ips200_draw_point(Middle_X[i],i,RGB565_BLACK);
    }

//    for(i=0;i<=50;i++)
//    {
//        ips200_draw_point(i,30,RGB565_RED);
//        ips200_draw_point(i,15,RGB565_RED);
//    }
    for(i=1;i<=60;i++)
    {
        ips200_draw_point(90,i,RGB565_RED);

    }



}