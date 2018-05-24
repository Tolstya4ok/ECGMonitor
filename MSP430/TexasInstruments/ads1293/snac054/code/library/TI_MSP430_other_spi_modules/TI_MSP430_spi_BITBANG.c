//******************************************************************************
//  Description:  This file contains functions that allow the MSP430 device to
//  access the SPI interface of the ADS1293.  There are multiple
//  instances of each function; the one to be compiled is selected by the
//  system variable TI_ADS1293_SER_INTF, defined in "TI_ADS1293_hardware_board.h".
//
//  MSP430/ADS1293 Interface Code Library v1.0
// 
//
//   Vishy Natarajan
//   Texas Instruments Inc.
//   April 2013
//   Built with IAR Embedded Workbench Version:  5.5x
//******************************************************************************
// Change Log:
//******************************************************************************
// Version:  1.00
// Comments: Initial Release Version
//******************************************************************************
/*  Copyright 2011-2012 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user who
  downloaded the software, his/her employer (which must be your employer) and
  Texas Instruments Incorporated (the "License"). You may not use this Software
  unless you agree to abide by the terms of the License. The License limits your
  use, and you acknowledge, that the Software may not be modified, copied or
  distributed unless embedded on a Texas Instruments microcontroller which is 
  integrated into your product. Other than for the foregoing purpose, you may 
  not use, reproduce, copy, prepare derivative works of, modify, distribute, 
  perform, display or sell this Software and/or its documentation for any 
  purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED �AS IS� WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL TEXAS
  INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL
  EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT
  LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL
  DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS,
  TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT
  LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
*******************************************************************************/

#include <stdint.h>
#include "TI_ADS1293.h"
#include "TI_MSP430.h"
#include "TI_MSP430_hardware_board.h"
#include "TI_MSP430_spi.h"

//******************************************************************************
// If Bit Bang is used
//******************************************************************************
#if TI_ADS1293_SER_INTF == TI_ADS1293_SER_INTF_BITBANG

uint8_t TI_ADS1293_SPI_bitbang_inout(uint8_t);

//------------------------------------------------------------------------------
//  void TI_ADS1293_SPISetup(void)
//
//  DESCRIPTION:
//  Configures the assigned interface to function as a SPI port and
//  initializes it.
//------------------------------------------------------------------------------
void TI_ADS1293_SPISetup(void)
{
  TI_ADS1293_CSn_PxOUT |= TI_ADS1293_CSn_PIN;
  TI_ADS1293_CSn_PxDIR |= TI_ADS1293_CSn_PIN;                                  // /CS disable
  
  // Disable special function on bitbang pins
  TI_ADS1293_SPI_BITBANG_SIMO_PxSEL &= ~TI_ADS1293_SPI_BITBANG_SIMO;
  TI_ADS1293_SPI_BITBANG_SOMI_PxSEL &= ~TI_ADS1293_SPI_BITBANG_SOMI;
  TI_ADS1293_SPI_BITBANG_UCLK_PxSEL &= ~TI_ADS1293_SPI_BITBANG_UCLK;
  
  // Config bitbang pins: SIMO low and UCLK high
  TI_ADS1293_SPI_BITBANG_PxOUT &= ~TI_ADS1293_SPI_BITBANG_SIMO;
  TI_ADS1293_SPI_BITBANG_PxOUT |= TI_ADS1293_SPI_BITBANG_UCLK; 
  
  // Config bitbang pins direction: SIMO, UCLK as OUT & SOMI as IN
  TI_ADS1293_SPI_BITBANG_PxDIR |= TI_ADS1293_SPI_BITBANG_SIMO | TI_ADS1293_SPI_BITBANG_UCLK;
  TI_ADS1293_SPI_BITBANG_PxDIR &= ~TI_ADS1293_SPI_BITBANG_SOMI;
   
}
//------------------------------------------------------------------------------
// Write 8-bit value using selected bit-bang pins & read out 8-bits
//------------------------------------------------------------------------------
uint8_t TI_ADS1293_SPI_bitbang_inout(uint8_t value)
{
  uint8_t x=0,shift=0;
  uint8_t y;

  // Determine how many bit positions SOMI is from least-significant bit
  x = TI_ADS1293_SPI_BITBANG_SOMI;
  while(x>1)
  {
    shift++;
    x = x >> 1;
  };

  x = 0;
  for(y=8;y>0;y--)
  {
    if(value & 0x80)                                                           // If bit is high...
      TI_ADS1293_SPI_BITBANG_PxOUT |= TI_ADS1293_SPI_BITBANG_SIMO;             // Set SIMO high...
    else
      TI_ADS1293_SPI_BITBANG_PxOUT &= ~TI_ADS1293_SPI_BITBANG_SIMO;            // Set SIMO low...
    value = value << 1; 
    
    TI_ADS1293_SPI_BITBANG_PxOUT &= ~TI_ADS1293_SPI_BITBANG_UCLK;              // Set clock low

    TI_ADS1293_SPI_BITBANG_PxOUT |= TI_ADS1293_SPI_BITBANG_UCLK;               // Set clock high 

    x = x << 1;                                                                // Make room for next bit
    x = x | ((TI_ADS1293_SPI_BITBANG_PxIN & TI_ADS1293_SPI_BITBANG_SOMI) >> shift);    
                                                                               // Store next bit
  }
  TI_ADS1293_SPI_BITBANG_PxOUT |= TI_ADS1293_SPI_BITBANG_UCLK;                 // Set clock high   
  return(x);
} 
//------------------------------------------------------------------------------
//  void TI_ADS1293_SPIWriteReg(uint8_t addr, uint8_t value)
//
//  DESCRIPTION:
//  Writes "value" to a single configuration register at address "addr". 
//------------------------------------------------------------------------------
void TI_ADS1293_SPIWriteReg(uint8_t addr, uint8_t value)
{
  uint8_t inst;
 

  TI_ADS1293_CSn_PxOUT &= ~TI_ADS1293_CSn_PIN;                                 // /CS enable 
  
  inst = ADS1293_WRITE_BIT & addr;                                             // register address
  TI_ADS1293_SPI_bitbang_inout(inst);                                          // send address  
  TI_ADS1293_SPI_bitbang_inout(value);                                         // Send data value, ignore return
 
  TI_ADS1293_CSn_PxOUT |= TI_ADS1293_CSn_PIN;                                  // /CS disable
 
}

//------------------------------------------------------------------------------
//  uint8_t TI_ADS1293_SPIReadReg(uint8_t addr)
//
//  DESCRIPTION:
//  Reads a single configuration register at address "addr" and returns the
//  value read. 
//------------------------------------------------------------------------------
uint8_t TI_ADS1293_SPIReadReg(uint8_t addr)
{
  uint8_t x, inst;
  
  TI_ADS1293_CSn_PxOUT &= ~TI_ADS1293_CSn_PIN;                                 // /CS enable
 
  inst = ADS1293_READ_BIT | addr;                                              // register address
  
  TI_ADS1293_SPI_bitbang_inout(inst);                                          // Send address, ignore return  
  
  x = TI_ADS1293_SPI_bitbang_inout(0);                                         // Dummy write to read data
  
  TI_ADS1293_CSn_PxOUT |= TI_ADS1293_CSn_PIN;                                  // /CS disable

  return x;
}

//------------------------------------------------------------------------------
//  void TI_ADS1293_SPIAutoIncWriteReg(uint8_t addr, uint8_t *buffer, uint8_t count)
//
//  DESCRIPTION:
//  Writes values to multiple configuration registers, the first register being
//  at address "addr".  First data byte is at "buffer", and both addr and
//  buffer are incremented sequentially (within the ADS1293 and MSP430
//  respectively) until "count" writes have been performed. 
//------------------------------------------------------------------------------
void TI_ADS1293_SPIAutoIncWriteReg(uint8_t addr, uint8_t *buffer, uint8_t count)
{
  uint8_t inst, i;

  TI_ADS1293_CSn_PxOUT &= ~TI_ADS1293_CSn_PIN;                                 // /CS enable
  
  inst = ADS1293_WRITE_BIT & addr;                                             // register address
  TI_ADS1293_SPI_bitbang_inout(inst);                                          // Send address, ignore return  
  
  for(i= 0; i < count; i++)
  {
    TI_ADS1293_SPI_bitbang_inout(*(buffer+i));                                 // Send address, ignore return   
  }
  
   TI_ADS1293_CSn_PxOUT |= TI_ADS1293_CSn_PIN;                                 // /CS disable
  
}

//------------------------------------------------------------------------------
//  void TI_ADS1293_SPIAutoIncReadReg(uint8_t addr, unit8_t *buffer, uint8_t count)
//
//  DESCRIPTION:
//  Reads multiple configuration registers, the first register being at address
//  "addr".  Values read are deposited sequentially starting at address
//  "buffer", until "count" registers have been read.
//------------------------------------------------------------------------------
void TI_ADS1293_SPIAutoIncReadReg(uint8_t addr, uint8_t *buffer, uint8_t count)
{
  uint8_t i, inst;
  
  TI_ADS1293_CSn_PxOUT &= ~TI_ADS1293_CSn_PIN;                                 // /CS enable
 
  inst = ADS1293_READ_BIT | addr;                                              // register address
  
  TI_ADS1293_SPI_bitbang_inout(inst);                                          // Send address, ignore return  
  
  for(i=0; i < count; i++)
  {
    *(buffer+i) = TI_ADS1293_SPI_bitbang_inout(0);                             // Send address, ignore return  
  }
  TI_ADS1293_CSn_PxOUT |= TI_ADS1293_CSn_PIN;                                  // /CS disable

}

//------------------------------------------------------------------------------
//  void TI_ADS1293_SPIStreamReadReg(uint8_t *buffer, uint8_t count)
//
//  DESCRIPTION:
//  Special read function for reading status, pace and ecg data registers of selected
//  channels. Channels to be read must be selected in CH_CNFG before calling this function. 
//  Data Loop Register read is extended "count+1" times where "count" is number of source bytes 
//  enabled in CH_CNFG. Data read are deposited sequentially starting at address "buffer" 
//  until "count" bytes have been read.
//------------------------------------------------------------------------------
void TI_ADS1293_SPIStreamReadReg(uint8_t *buffer, uint8_t count)
{
  uint8_t i, inst;
  
  TI_ADS1293_CSn_PxOUT &= ~TI_ADS1293_CSn_PIN;                                 // /CS enable
 
  inst = ADS1293_READ_BIT | TI_ADS1293_DATA_LOOP_REG;                          // read from data loop register
  
  TI_ADS1293_SPI_bitbang_inout(inst);                                          // Send address, ignore return  

  for(i=0; i<count; i++)
  {
    *(buffer+i) = TI_ADS1293_SPI_bitbang_inout(0);                             // Send address, ignore return  
  }
    
  TI_ADS1293_CSn_PxOUT |= TI_ADS1293_CSn_PIN;                                  // /CS disable  
    
}
//------------------------------------------------------------------------------
#endif