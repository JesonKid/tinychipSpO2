#include "UG2864Driver.h"
#include "DefaultSetup.h"
#include "gpio.h"
#include "spi.h"


//One byte reverse mapping array
const UCHAR gInvertMap[] =
{
    // 0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
    0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0,
    0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8,
    0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,
    0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC,
    0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2,
    0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
    0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6,
    0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
    0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
    0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9,
    0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
    0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
    0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,
    0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
    0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
    0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF,
};



const UCHAR gBrightnessTab[] =
{
    0x01, 0x1E, 0x3C, 0x5A, 0x78, 0x96,
};



//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// UG2864_Delay Time
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
static void UG2864_uDelay(UCHAR l)
{
    while(l--);
}



static void UG2864_Delay(UCHAR n)
{
    UCHAR i, j, k;
    for(k = 0; k < n; k++)
    {
        for(i = 0; i < 131; i++)
        {
            for(j = 0; j < 15; j++)
            {
                UG2864_uDelay(203);
            }
        }
    }
}

/*
********************************************************************************
 * [Func.Name  ] void UG2864_Write_Command(UCHAR Data)                *
 * [Func.Spec. ] OLED command write, must initialized by user.                *
 * [Func.Input ]                                                              *
 *               UCHAR Data   send data                               *
 * [Func.Output]                                                              *
 *               N/A                                                          *
 * [Func.Return]                                                              *
 *               N/A                                                          *
 *----------------------------------------------------------------------------*
********************************************************************************
*/
static void UG2864_Write_Command(UCHAR Data)
{
    UINT32 tmp = Data;

    //CS LOW D/C LOW
    (GPIO_PIN_0_15->data_out) &= 0xFFFF3FFF;
    //Send command data
    SPI_SendData_Host(SPI0, &tmp, 1);
    //CS HIGH D/C HIGH
    (GPIO_PIN_0_15->data_out) |= 0x0000C000;
}

/*
********************************************************************************
 * [Func.Name  ] void UG2864_Write_Data(UCHAR Data)                   *
 * [Func.Spec. ] OLED data write, must initialized by user.                   *
 * [Func.Input ]                                                              *
 *               UCHAR Data   send data                               *
 * [Func.Output]                                                              *
 *               N/A                                                          *
 * [Func.Return]                                                              *
 *               N/A                                                          *
 *----------------------------------------------------------------------------*
********************************************************************************
*/
static void UG2864_Write_Data(UCHAR Data)
{
    UINT32 tmp = Data;

    //CS LOW D/C HIGH
    (GPIO_PIN_0_15->data_out) &= 0xFFFFBFFF;
    //Send command data
    SPI_SendData_Host(SPI0, &tmp, 1);
    //CS HIGH D/C HIGH
    (GPIO_PIN_0_15->data_out) |= 0x00004000;
}



void UG2864_Set_Start_Column(UCHAR d)
{
    // Default => 0x00
    UG2864_Write_Command(0x00 + d %
                         16); // Set Lower Column Start Address for Page Addressing Mode
    // Default => 0x10
    UG2864_Write_Command(0x10 + d /
                         16); // Set Higher Column Start Address for Page Addressing Mode
}



void UG2864_Set_Addressing_Mode(UCHAR d)
{
    UG2864_Write_Command(0xb0 | d); // Set Page Addressing Mode,Default => 0xb0
}



void UG2864_Set_Column_Address(UCHAR a, UCHAR b)
{
    UG2864_Write_Command(0x21); // Set Column Address
    UG2864_Write_Command(a);        // Default => 0x00 (Column Start Address)
    UG2864_Write_Command(b);        // Default => 0x83 (Column End Address)
}



void UG2864_Set_Page_Address(UCHAR a, UCHAR b)
{
    UG2864_Write_Command(0x22); // Set Page Address
    UG2864_Write_Command(a);        // Default => 0x00 (Page Start Address)
    UG2864_Write_Command(b);        // Default => 0x07 (Page End Address)
}



void UG2864_Set_Start_Line(UCHAR d)
{
    // Default => 0x40 (0x00)
    UG2864_Write_Command(0x40 | d); // Set Display Start Line
}



void UG2864_Set_Contrast_Control(UCHAR d)
{
    UG2864_Write_Command(0x81); // Set Contrast Control for Bank 0
    UG2864_Write_Command(d);        // Default => 0x80
}



void UG2864_Set_Area_Brightness(UCHAR d)
{
    UG2864_Write_Command(0x82); // Set Brightness for Area Color Banks
    UG2864_Write_Command(d);        // Default => 0x80
}



void UG2864_Set_Pump(UCHAR d)
{
    // 0x8a (0x00) => Select Internal DC/DC Voltage Converter
    // 0x8b (0x01) => Select External VCC Supply
    UG2864_Write_Command(0x8D); // Set Master Configuration
    UG2864_Write_Command(d);        // Default => 0x8b
}



void UG2864_Set_Segment_Remap(UCHAR d)
{
    // Default => 0xA0
    // 0xA0 (0x00) => Column Address 0 Mapped to SEG0
    // 0xA1 (0x01) => Column Address 0 Mapped to SEG131
    UG2864_Write_Command(0xA0 | d); // Set Segment Re-Map
}



void UG2864_Set_Entire_Display(UCHAR d)
{
    // Default => 0xA4
    // 0xA4 (0x00) => Normal Display
    // 0xA5 (0x01) => Entire Display On
    UG2864_Write_Command(0xA4 | d); // Set Entire Display On / Off
}



void UG2864_Set_Inverse_Display(UCHAR d)
{
    // Default => 0xA6
    // 0xA6 (0x00) => Normal Display
    // 0xA7 (0x01) => Inverse Display On
    UG2864_Write_Command(0xA6 | d); // Set Inverse Display On/Off
}



void UG2864_Set_Multiplex_Ratio(UCHAR d)
{
    UG2864_Write_Command(0xA8); // Set Multiplex Ratio
    UG2864_Write_Command(d);        // Default => 0x3F (1/64 Duty)
}



void UG2864_Set_Dim_Mode(UCHAR a, UCHAR b)
{
    UG2864_Write_Command(0xAB); // Set Dim Mode Configuration
    UG2864_Write_Command(0X00); // => (Dummy Write for First Parameter)
    UG2864_Write_Command(a);        // Default => 0x80 (Contrast Control for Bank 0)
    UG2864_Write_Command(
        b);        // Default => 0x80 (Brightness for Area Color Banks)
    UG2864_Write_Command(0xAC); // Set Display On in Dim Mode
}



void UG2864_Set_Master_Config(UCHAR d)
{
    // 0x8a (0x00) => Select Internal DC/DC Voltage Converter
    // 0x8b (0x01) => Select External VCC Supply
    UG2864_Write_Command(0xAD);     // Set Master Configuration
    UG2864_Write_Command(0x8a | d); // Default => 0x8b
}



void UG2864_Set_Display_On_Off(UCHAR d)
{
    // Default => 0xAE
    // 0xAE (0x00) => Display Off
    // 0xAF (0x01) => Display On
    UG2864_Write_Command(0xAE | d); // Set Display On/Off
}



void UG2864_Set_Start_Page(UCHAR d)
{
    // Default => 0xB0 (0x00)
    UG2864_Write_Command(0xB0 |
                         d); // Set Page Start Address for Page Addressing Mode
}



void UG2864_Set_Common_Remap(UCHAR d)
{
    // Default => 0xC0
    // 0xC0 (0x00) => Scan from COM0 to 63
    // 0xC8 (0x08) => Scan from COM63 to 0
    UG2864_Write_Command(0xC0 | d); // Set COM Output Scan Direction
}



void UG2864_Set_Display_Offset(UCHAR d)
{
    UG2864_Write_Command(0xD3); // Set Display Offset
    UG2864_Write_Command(d);        // Default => 0x00
}



void UG2864_Set_Display_Clock(UCHAR d)
{
    // D[3:0] => Display Clock Divider
    // D[7:4] => Oscillator Frequency
    UG2864_Write_Command(
        0xD5); // Set Display Clock Divide Ratio / Oscillator Frequency
    UG2864_Write_Command(d);        // Default => 0x70
}



void UG2864_Set_Area_Color(UCHAR d)
{
    UG2864_Write_Command(
        0xD8); // Set Area Color Mode On/Off & Low Power Display Mode
    UG2864_Write_Command(
        d);        // Default => 0x00 (Monochrome Mode & Normal Power Display Mode
}



void UG2864_Set_Precharge_Period(UCHAR d)
{
    // D[3:0] => Phase 1 Period in 1~15 Display Clocks
    // D[7:4] => Phase 2 Period in 1~15 Display Clocks
    UG2864_Write_Command(0xD9); // Set Pre-Charge Period
    UG2864_Write_Command(
        d);        // Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
}



void UG2864_Set_Common_Config(UCHAR d)
{
    // Alternative COM Pin Configuration
    // Disable COM Left/Right Re-Map
    UG2864_Write_Command(0xDA);     // Set COM Pins Hardware Configuration
    UG2864_Write_Command(0x02 | d); // Default => 0x12 (0x10)
}



void UG2864_Set_VCOMH(UCHAR d)
{
    UG2864_Write_Command(0xDB); // Set VCOMH Deselect Level
    UG2864_Write_Command(d);        // Default => 0x34 (0.77*VCC)
}



void UG2864_Set_Read_Modify_Write(UCHAR d)
{
    // Default => 0xE0
    // 0xE0 (0x00) => Enter Read Modify Write
    // 0xEE (0x0E) => Exit Read Modify Write
    UG2864_Write_Command(0xE0 | d); // Set Read Modify Write Mode
}



void UG2864_Set_NOP(void)
{
    UG2864_Write_Command(0xE3); // Command for No Operation
}



//Clear OLED
void UG2864_Fill_RAM(UCHAR Data)
{
    UCHAR i, j;

    for(i = 0; i < 8; i++)
    {
        UG2864_Set_Start_Page(i);
        UG2864_Set_Start_Column(0x00);
        for(j = 0; j < 128; j++)
        {
            UG2864_Write_Data(Data);
        }
    }
}




void UG2864_OLED_Init(void)
{
    UG2864_Set_Display_On_Off(0x00);        // Display Off (0x00/0x01)
    UG2864_Set_Display_Clock(0x50);         // Set Clock as 104 Frames/Sec
    UG2864_Set_Multiplex_Ratio(0x3F);   // Set to 64 MUX(0x0F~0x3F)
    UG2864_Set_Display_Offset(0x00);        // Shift Mapping RAM Counter (0x00~0x3F)
    UG2864_Set_Start_Line(
        0x00);            // Set Mapping RAM Display Start Line (0x00~0x3F)
    UG2864_Set_Pump(0x14);                  // Charge Pump Setting
    UG2864_Set_Addressing_Mode(0x00);   // Set Page Addressing Mode (0x00/0x01/0x02)
    UG2864_Set_Segment_Remap(0x01);     // Set SEG/Column Mapping (0x00/0x01)
    UG2864_Set_Common_Remap(0x00);      // Set COM/Row Scan Direction (0x00/0x08)
    UG2864_Set_Common_Config(
        0x10);         // Set Alternative Configuration (0x00/0x10)

    //Contrast decide the light intensity
    UG2864_Set_Contrast_Control(
        gBrightnessTab[gCfgCtrl.brightness]); // Set SEG Output Current
    UG2864_Set_Precharge_Period(
        0x22);                                                              // Set Pre-Charge as 13 Clocks & Discharge as 2 Clock
    UG2864_Set_VCOMH(
        0x34);                                                                                     // Set VCOM Deselect Level
    UG2864_Set_Entire_Display(
        0x00);                                                                    // Disable Entire Display On (0x00/0x01)
    UG2864_Set_Inverse_Display(
        0x00);                                                               // Disable Inverse Display On (0x00/0x01)

    UG2864_Fill_RAM(0x00);                      // Clear Screen

    UG2864_Set_Display_On_Off(0x01);    // Display On (0x00/0x01)
}



void UG2864_OLED_Init_Remap(void)
{

    UG2864_Set_Display_On_Off(0x00); // Display Off (0x00/0x01)

    UG2864_Set_Segment_Remap(0x00); // Set SEG/Column Mapping (0x00/0x01)
    UG2864_Set_Common_Remap(0x08); // Set COM/Row Scan Direction (0x00/0x08)

    UG2864_Set_Entire_Display(0x00); // Disable Entire Display On (0x00/0x01)

    UG2864_Fill_RAM(0x00); // Clear Screen
    UG2864_Set_Display_On_Off(0x01); // Display On (0x00/0x01)
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Deactivate Scrolling (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void UG2864_Deactivate_Scroll(void)
{
    UG2864_Write_Command(0x2E); // Deactivate Scrolling
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Continuous Horizontal Scrolling (Partial or Full Screen)
//
// a: Scrolling Direction
// "0x00" (Rightward)
// "0x01" (Leftward)
// b: Set Numbers of Column Scroll per Step
// c: Define Start Page Address
// d: Define End Page Address
// e: Set Time Interval between Each Scroll Step in Terms of Frame Frequency
// f: UG2864_Delay Time
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void UG2864_Horizontal_Scroll(UCHAR a, UCHAR b, UCHAR c, UCHAR d, UCHAR e,
                              UCHAR f)
{
    UG2864_Write_Command(0x26 | a); // Horizontal Scroll Setup
    UG2864_Write_Command(b);
    UG2864_Write_Command(c);
    UG2864_Write_Command(e);
    UG2864_Write_Command(d);
    UG2864_Write_Command(0x2F); // Activate Scrolling
    UG2864_Delay(f);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Continuous Vertical / Horizontal / Diagonal Scrolling (Partial or Full Screen)
//
// a: Scrolling Direction
// "0x00" (Vertical & Rightward)
// "0x01" (Vertical & Leftward)
// b: Set Numbers of Column Scroll per Step (Horizontal / Diagonal Scrolling)
// c: Define Start Row Address (Horizontal / Diagonal Scrolling)
// d: Define End Page Address (Horizontal / Diagonal Scrolling)
// e: Set Top Fixed Area (Vertical Scrolling)
// f: Set Vertical Scroll Area (Vertical Scrolling)
// g: Set Numbers of Row Scroll per Step (Vertical / Diagonal Scrolling)
// h: Set Time Interval between Each Scroll Step in Terms of Frame Frequency
// i: UG2864_Delay Time
// * e+f must be less than or equal to the Multiplex Ratio...
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void UG2864_Continuous_Scroll(UCHAR a, UCHAR b, UCHAR c, UCHAR d,
                              UCHAR e, UCHAR f, UCHAR g, UCHAR h,
                              UCHAR i)
{
    UG2864_Write_Command(0xA3); // Set Vertical Scroll Area
    UG2864_Write_Command(e); // Default => 0x00 (Top Fixed Area)
    UG2864_Write_Command(f); // Default => 0x40 (Vertical Scroll Area)
    UG2864_Write_Command(0x29 + a); // Continuous Vertical & Horizontal Scroll Setup
    UG2864_Write_Command(b);
    UG2864_Write_Command(c);
    UG2864_Write_Command(h);
    UG2864_Write_Command(d);
    UG2864_Write_Command(g);
    UG2864_Write_Command(0x2F); // Activate Scrolling
    UG2864_Delay(i);
}


// Show Pattern (Partial or Full Screen)
//
// a: Start Page
// b: End Page
// c: Start Column
// d: Total Columns
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void UG2864_Show_Pattern(UCHAR *Data_Pointer, UCHAR a, UCHAR b, UCHAR c,
                         UCHAR d)
{
    UCHAR *Src_Pointer;
    UCHAR i, j;

    Src_Pointer = Data_Pointer;

    for(i = a; i < (b + 1); i++)
    {
        UG2864_Set_Start_Page(i);
        UG2864_Set_Start_Column(c);
        for(j = 0; j < d; j++)
        {
            UG2864_Write_Data(*Src_Pointer);
            Src_Pointer++;
        }
    }
}


// Show ASCII
//
// a: Start Page
// b: End Page
// c: Start Column
// d: Total Columns
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void UG2864_Show_ASCII(UCHAR *Data_Pointer, UCHAR a, UCHAR b, UCHAR c, UCHAR d)
{
    UCHAR *Src_Pointer;
    UCHAR i, j;

    Src_Pointer = Data_Pointer;

    for(i = a; i < (b + 1); i++)
    {
        UG2864_Set_Start_Page(i);
        UG2864_Set_Start_Column(c);
        for(j = 0; j < d; j++)
        {
            UG2864_Write_Data(*Src_Pointer);
            Src_Pointer++;
        }
    }
}


/*
********************************************************************************
 * [FUNC.name  ] void UG2864_Display(... ...)                                 *
 * [FUNC.spec. ] Interface display (parameters/menu/trend), refresh frequency *
 *               is 50Hz, must guaranteed by  calling function.               *
 * [FUNC.input ]                                                              *
 *               UCHAR *pData       Display data                      *
 *               UCHAR  beginPage   Start page                        *
 *               UCHAR  stopPage    End page                          *
 *               UCHAR  beginColumn Start column                      *
 *               UCHAR  stopColumn  End column                        *
 *               UCHAR  dir     Display direction                       *
 * [FUNC.output]                                                              *
 *               void                                                         *
 * [FUNC.return]                                                              *
 *               Void                                                         *
 *----------------------------------------------------------------------------*
********************************************************************************
 */
void UG2864_Display(UCHAR *pData, UCHAR beginPage, UCHAR stopPage,
                    UCHAR beginColumn, UCHAR stopColumn, GUI_DIR_E dir)
{
    INT16 i = 0;
    INT16 j = 0;

    //Select different policy by direction
    switch (dir)
    {
        //Horizontal & vertical positive
        case GUI_DIR_HORIZON_POS:
        case GUI_DIR_VERTICAL_POS:
        {
            for (i = beginPage; i <= stopPage; i++)
            {
                UG2864_Set_Start_Page(i);
                UG2864_Set_Start_Column(beginColumn);
                for (j = 0; j < stopColumn; j++)
                {
                    UG2864_Write_Data(*pData++);
                }
            }
        }
        break;

        case GUI_DIR_HORIZON_NEG:
        case GUI_DIR_VERTICAL_NEG:
        {
            //Reverse axis
            beginPage = 7 - beginPage;
            stopPage  = 7 - stopPage;
            beginColumn = 128 - beginColumn - stopColumn;
            //Reverse display ASCII
            for (i = beginPage; i >= stopPage; i--)
            {
                UG2864_Set_Start_Page(i);
                UG2864_Set_Start_Column(beginColumn);
                j = stopColumn;
                while (0 < j--)
                {
                    UG2864_Write_Data(gInvertMap[*(pData + j)]);
                }
                pData += stopColumn;
            }
        }
        break;

        default:
            break;
    }
}

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
 *               UCHAR  dir           Display direction                 *
 * [FUNC.output]                                                              *
 *               void                                                         *
 * [FUNC.return]                                                              *
 *               Void                                                         *
********************************************************************************
 */
void UG2864_ClearRegion(UCHAR beginPage, UCHAR stopPage,
                        UCHAR beginColumn, UCHAR stopColumn, GUI_DIR_E dir)
{
    INT16 i = 0;
    INT16 j = 0;

    switch (dir)
    {
        case GUI_DIR_HORIZON_POS:
        case GUI_DIR_VERTICAL_POS:
        {
            for (i = beginPage; i <= stopPage; i++)
            {
                UG2864_Set_Start_Page(i);
                UG2864_Set_Start_Column(beginColumn);
                for (j = 0; j < stopColumn; j++)
                {
                    UG2864_Write_Data(0);
                }
            }
        }
        break;

        case GUI_DIR_HORIZON_NEG:
        case GUI_DIR_VERTICAL_NEG:
        {
            //Reverse axis
            beginPage = 7 - beginPage;
            stopPage  = 7 - stopPage;
            beginColumn = 128 - beginColumn - stopColumn;
            //Reverse display ASCII
            for (i = beginPage; i >= stopPage; i--)
            {
                UG2864_Set_Start_Page(i);
                UG2864_Set_Start_Column(beginColumn);
                j = stopColumn;
                while (0 < j--)
                {
                    UG2864_Write_Data(0);
                }
            }
        }
        break;

        default:
            break;
    }
}






