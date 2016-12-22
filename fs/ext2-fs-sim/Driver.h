/***
 * Filename:        Driver.h
 * Date:            Dec 21 2016
 * Last Edited by:  Gengshan Yang
 * Description:     Disk Simulation
 ***/

#define BYTES_PER_SECTOR	64
#define	SECTORS				32 * 1024

typedef struct {
  char content[BYTES_PER_SECTOR];
}simSector;

typedef struct {
  simSector sector[SECTORS];
}simDisk;

int DevFormat();					// formats the device

int DevWrite (						// writes one block to a specified sector
			   int BlockNumber,		// Logical block number
			   char *Data			// Data to be written
			   );

int DevRead (						// reads one block from a specified sector
			  int BlockNumber,		// Logical block number 
			  char *Data			// Data received
			  );

simDisk myDisk;
