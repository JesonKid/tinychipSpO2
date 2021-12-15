/**************************************************
 * Copyright 2004-2012 IAR Systems. All rights reserved.
 *
 * $Revision: 6236 $
 **************************************************/
#include "flash_loader.h"
#include <intrinsics.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <yfuns.h>
#include "uart.h"

//#define DEBUG_MESS


#ifdef DEBUG_MESS
static __no_init char message[80];
void cSpyMessageBox(char* msg)
{
    int log_fd;
    log_fd = __open("$MESSAGE_BOX$", _LLIO_CREAT | _LLIO_TRUNC | _LLIO_WRONLY);
    if (log_fd == -1){
         exit(1);
    }
    __write(log_fd, (unsigned char *)msg, strlen(msg));
    __close(log_fd);
}

//------------------------------------------------------------------------------
// C-SPY will display a message box window with the text given by the msg parameter.
// Text may be split on multiple lines by embedding newlines (\n) in the message string.
// The flash loader execution will halt until the message box OK button is pressed.
// The flash loader will then terminate and notify C-SPY that the flash download failed.
//------------------------------------------------------------------------------

void cSpyFatalError(char* msg)
{
    cSpyMessageBox(msg);
    exit(1);
}
void cSpyMessageLog(char* msg)
{
    int messageLogFile = -1;
    messageLogFile = __open("$DEBUG_LOG$", _LLIO_CREAT | _LLIO_TRUNC | _LLIO_WRONLY);
    if (messageLogFile == -1) {
        cSpyFatalError("Flash loader failed");
    }
     __write(messageLogFile, (unsigned char *)msg, strlen(msg));
     __close(messageLogFile);
} 
#endif // DEBUG_MESS


#if USE_ARGC_ARGV
uint32_t FlashInit(void *base_of_flash, uint32_t image_size,
                   uint32_t link_address, uint32_t flags,
                   int argc, char const *argv[])
#else
uint32_t FlashInit(void *base_of_flash, uint32_t image_size,
                   uint32_t link_address, uint32_t flags)
#endif
{
  return RESULT_OK;
}

uint32_t FlashWrite(void *block_start,
                    uint32_t offset_into_block,
                    uint32_t count,
                    char const *buffer)
{ 
  char *to = (char*)block_start + offset_into_block;
  while (count--)
  {
    *to++ =*buffer++;
    while((M0P_FLASH->MCR_f.program_busy_0 == 1) || (M0P_FLASH->MCR_f.program_busy_1 == 1));
  }
  return RESULT_OK; 
}

uint32_t FlashErase(void *block_start,
                    uint32_t block_size)
{    
    M0P_FLASH->ECONF_f.nvr_select = 0x0;
    M0P_FLASH->ECONF_f.block_select = 0x0;
    M0P_FLASH->ECONF_f.sector_index = (uint32_t)((uint32_t)block_start - 0x10000000)/block_size;
    M0P_FLASH->ECOMM_f.sector_erase = 1;
    while((M0P_FLASH->MCR_f.erase_busy_0 == 1) || (M0P_FLASH->MCR_f.erase_busy_1 == 1));
  return RESULT_OK;
}

OPTIONAL_SIGNOFF
uint32_t FlashSignoff(void)
{
  return RESULT_OK;
}
