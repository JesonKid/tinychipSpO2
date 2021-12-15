#ifndef _UG_2864_DRIVER_H_
#define _UG_2864_DRIVER_H_

#include "DataType.h"



//GUI interface direction(level-1)
typedef enum
{
    GUI_DIR_HORIZON_POS = 0,           //Horizontal positive direction
    GUI_DIR_VERTICAL_POS,              //Vertical positive direction
    GUI_DIR_HORIZON_NEG,               //Horizontal negative direction
    GUI_DIR_VERTICAL_NEG,              //Vertical negative direction
    GUI_DIR_MARGIN,

} GUI_DIR_E;




extern const UCHAR gBrightnessTab[];


extern void UG2864_Set_Start_Column(UCHAR d);
extern void UG2864_Set_Addressing_Mode(UCHAR d);
extern void UG2864_Set_Column_Address(UCHAR a, UCHAR b);
extern void UG2864_Set_Page_Address(UCHAR a, UCHAR b);
extern void UG2864_Set_Start_Line(UCHAR d);
extern void UG2864_Set_Contrast_Control(UCHAR d);
extern void UG2864_Set_Area_Brightness(UCHAR d);
extern void UG2864_Set_Pump(UCHAR d);
extern void UG2864_Set_Segment_Remap(UCHAR d);
extern void UG2864_Set_Entire_Display(UCHAR d);
extern void UG2864_Set_Inverse_Display(UCHAR d);
extern void UG2864_Set_Multiplex_Ratio(UCHAR d);
extern void UG2864_Set_Dim_Mode(UCHAR a, UCHAR b);
extern void UG2864_Set_Master_Config(UCHAR d);
extern void UG2864_Set_Display_On_Off(UCHAR d);
extern void UG2864_Set_Start_Page(UCHAR d);
extern void UG2864_Set_Common_Remap(UCHAR d);
extern void UG2864_Set_Display_Offset(UCHAR d);
extern void UG2864_Set_Display_Clock(UCHAR d);
extern void UG2864_Set_Area_Color(UCHAR d);
extern void UG2864_Set_Precharge_Period(UCHAR d);
extern void UG2864_Set_Common_Config(UCHAR d);
extern void UG2864_Set_VCOMH(UCHAR d);
extern void UG2864_Set_Read_Modify_Write(UCHAR d);
extern void UG2864_Set_NOP(void);
extern void UG2864_Fill_RAM(UCHAR Data);

extern void UG2864_OLED_Init(void);
extern void UG2864_OLED_Init_Remap(void);

extern void UG2864_Deactivate_Scroll(void);

extern void UG2864_Horizontal_Scroll(UCHAR a, UCHAR b, UCHAR c, UCHAR d,
                                     UCHAR e, UCHAR f);
extern void UG2864_Continuous_Scroll(UCHAR a, UCHAR b, UCHAR c, UCHAR d,
                                     UCHAR e, UCHAR f, UCHAR g, UCHAR h, UCHAR i);
extern void UG2864_Show_Pattern(UCHAR *Data_Pointer, UCHAR a, UCHAR b, UCHAR c,
                                UCHAR d);
extern void UG2864_Show_ASCII(UCHAR *Data_Pointer, UCHAR a, UCHAR b, UCHAR c,
                              UCHAR d);


/*
********************************************************************************
 * [FUNC.name  ] void UG2864_Display(... ...)                                 *
 * [FUNC.spec. ] Interface display (parameters/menu/trend), refresh frequency *
 *               is 50Hz, must guaranteed by  calling function.               *
 * [FUNC.input ]                                                              *
 *               UCHAR *pData       Display data                      *
 *               UCHAR  startPage   Start page                        *
 *               UCHAR  endPage     End page                          *
 *               UCHAR  startColumn Start column                      *
 *               UCHAR  endColumn   End column                        *
 *               UCHAR  mDirect     Display direction                 *
 * [FUNC.output]                                                              *
 *               void                                                         *
 * [FUNC.return]                                                              *
 *               Void                                                         *
 *----------------------------------------------------------------------------*
********************************************************************************
 */
extern void UG2864_Display(UCHAR *pData, UCHAR beginPage, UCHAR stopPage,
                           UCHAR beginColumn, UCHAR stopColumn, GUI_DIR_E dir);

/*
********************************************************************************
 * [FUNC.name  ] void UG2864_ClearRegion(... ...)                             *
 * [FUNC.spec. ] Interface display (parameters/menu/trend), refresh frequency *
 *               is 50Hz, must guaranteed by  calling function.               *
 * [FUNC.input ]                                                              *
 *               UCHAR  startPage   Start page                        *
 *               UCHAR  endPage     End page                          *
 *               UCHAR  startColumn Start column                      *
 *               UCHAR  endColumn   End column                        *
 *               UCHAR  mDirect     Display direction                 *
 * [FUNC.output]                                                              *
 *               void                                                         *
 * [FUNC.return]                                                              *
 *               Void                                                         *
 *----------------------------------------------------------------------------*
********************************************************************************
 */
extern void UG2864_ClearRegion(UCHAR beginPage, UCHAR stopPage,
                               UCHAR beginColumn, UCHAR stopColumn, GUI_DIR_E dir);


#endif  //_UG_2864_DRIVER_H_

