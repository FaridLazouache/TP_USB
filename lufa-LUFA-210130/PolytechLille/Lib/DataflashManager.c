/*
             LUFA Library
     Copyright (C) Dean Camera, 2017.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2017  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
 *
 *  Functions to manage the physical Dataflash media, including reading and writing of
 *  blocks of data. These functions are called by the SCSI layer when data must be stored
 *  or retrieved to/from the physical storage media. If a different media is used (such
 *  as a SD card or EEPROM), functions similar to these will need to be generated.
 */

#define  INCLUDE_FROM_DATAFLASHMANAGER_C
#include "DataflashManager.h"
#include "../../AT45DB641E.h"

/** Initialise the Dataflash memory. */
void DataflashManager_Initialisation(void)
{
}

/** Writes blocks (OS blocks, not Dataflash pages) to the storage medium, the board Dataflash IC(s), from
 *  the pre-selected data OUT endpoint. This routine reads in OS sized blocks from the endpoint and writes
 *  them to the Dataflash in Dataflash page sized blocks.
 *
 *  \param[in] MSInterfaceInfo  Pointer to a structure containing a Mass Storage Class configuration and state
 *  \param[in] BlockAddress  Data block starting address for the write sequence
 *  \param[in] TotalBlocks   Number of blocks of data to write
 */
void DataflashManager_WriteBlocks(USB_ClassInfo_MS_Device_t* const MSInterfaceInfo,
                                  const uint32_t BlockAddress,
                                  uint16_t TotalBlocks)
{
	/* Wait until endpoint is ready before continuing */
	if (Endpoint_WaitUntilReady()) return;

	int i,j;
	for(i=0;i<TotalBlocks;i++){
		for(j=0;j<VIRTUAL_MEMORY_BLOCK_SIZE;j++){
			/* Check if the endpoint is currently empty */
			if (!(Endpoint_IsReadWriteAllowed()))
			{
				/* Clear the current endpoint bank */
				Endpoint_ClearOUT();
				/* Wait until the host has sent another packet */
				if (Endpoint_WaitUntilReady()) return;
			}
			/* Read one byte from USB host */
			uint8_t byte=Endpoint_Read_8();
			/* Store byte in memory at address BlockAddress+i*VIRTUAL_MEMORY_BLOCK_SIZE+j */
			// <-- Insert code here
			if(j < 255)// Reading CS1 - 0x84 = Buffer write for CS1
				AT45DB641E_cmd(&PORTB, 4, 0x84, &byte, 1);
			else if(j > 256 && j < 511)//Reading CS2 - 0x87 = Buffer read for CS2
				AT45DB641E_cmd(&PORTB, 4, 0x87, &byte, 1);

			if (MSInterfaceInfo->State.IsMassStoreReset) return;
		}
	}
	/* If the endpoint is empty, clear it ready for the next packet from the host */
	if (!(Endpoint_IsReadWriteAllowed())) Endpoint_ClearOUT();
}

/** Reads blocks (OS blocks, not Dataflash pages) from the storage medium, the board Dataflash IC(s), into
 *  the pre-selected data IN endpoint. This routine reads in Dataflash page sized blocks from the Dataflash
 *  and writes them in OS sized blocks to the endpoint.
 *
 *  \param[in] MSInterfaceInfo  Pointer to a structure containing a Mass Storage Class configuration and state
 *  \param[in] BlockAddress  Data block starting address for the read sequence
 *  \param[in] TotalBlocks   Number of blocks of data to read
 */
void DataflashManager_ReadBlocks(USB_ClassInfo_MS_Device_t* const MSInterfaceInfo,
                                 const uint32_t BlockAddress,
                                 uint16_t TotalBlocks)
{
	/* Wait until endpoint is ready before continuing */
	if (Endpoint_WaitUntilReady()) return;

	int i,j;
	for(i=0;i<TotalBlocks;i++){
		for(j=0;j<VIRTUAL_MEMORY_BLOCK_SIZE;j++){
			/* Check if the endpoint is currently empty */
			if (!(Endpoint_IsReadWriteAllowed()))
			{
				/* Clear the current endpoint bank */
				Endpoint_ClearIN();
				/* Wait until the host has sent another packet */
				if (Endpoint_WaitUntilReady()) return;
			}
			/* Retrieve byte in memory at address BlockAddress+i*VIRTUAL_MEMORY_BLOCK_SIZE+j */
			uint8_t byte;
			// <-- Insert code here
			if(j < 255)// Reading CS1 - 0xD4 = Buffer read for CS1
				AT45DB641E_cmd(&PORTB, 4, 0xD4, &byte, 1);
			else if(j > 256 && j < 511)//Reading CS2 - 0xD6 = Buffer read for CS2
				AT45DB641E_cmd(&PORTB, 4, 0xD6, &byte, 1);
			/* Send byte to USB host */
			Endpoint_Write_8(byte);
			if (MSInterfaceInfo->State.IsMassStoreReset) return;
		}
	}
	/* If the endpoint is full, send its contents to the host */
	if (!(Endpoint_IsReadWriteAllowed())) Endpoint_ClearIN();
}

/** Disables the Dataflash memory write protection bits on the board Dataflash ICs, if enabled. */
void DataflashManager_ResetDataflashProtections(void)
{
}

/** Performs a simple test on the attached Dataflash IC(s) to ensure that they are working.
 *
 *  \return Boolean \c true if all media chips are working, \c false otherwise
 */
bool DataflashManager_CheckDataflashOperation(void)
{
	return true;
}

