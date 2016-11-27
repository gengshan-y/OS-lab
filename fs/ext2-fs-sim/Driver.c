#include "Driver.h"
#include <stdio.h>
#include <string.h>

void hexDump (char *desc, void *addr, int len) {
    int i;
    unsigned char buff[17];
    unsigned char *pc = (unsigned char*)addr;

    // Output description if given.
    if (desc != NULL)
        printf ("%s:\n", desc);

    if (len == 0) {
        printf("  ZERO LENGTH\n");
        return;
    }
    if (len < 0) {
        printf("  NEGATIVE LENGTH: %i\n",len);
        return;
    }

    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0)
                printf ("  %s\n", buff);

            // Output the offset.
            printf ("  %04x ", i);
        }

        // Now the hex code for the specific character.
        printf (" %02x", pc[i]);

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];
        buff[(i % 16) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        printf ("   ");
        i++;
    }

    // And print the final ASCII bit.
    printf ("  %s\n", buff);
}


int DevFormat() {
  int it, itt;
  for (it = 0; it < SECTORS; it++) {
    for (itt = 0; itt < BYTES_PER_SECTOR; itt++) {
      myDisk.sector[it].content[itt] = 0;
    }
  }
  printf("disk with %d sectors, %d bytes per sector initialized\n", SECTORS, BYTES_PER_SECTOR);
  return 1;
}

int DevWrite(int BlockNumber, char *Data) {
  printf("writing dev to block %d\n", BlockNumber);
  hexDump("hexdump", Data, sizeof(Data));
  if (BlockNumber >= SECTORS) {
    printf("error, BlockNumber >= SECTORS\n");
    return 0;
  }

  memcpy(myDisk.sector[BlockNumber].content, Data, BYTES_PER_SECTOR);
  return 1;
}

int DevRead (int BlockNumber, char *Data) {
  if (BlockNumber >= SECTORS) {
    printf("error, BlockNumber >= SECTORS\n");
    return 0;
  }  

  memcpy(Data, myDisk.sector[BlockNumber].content, BYTES_PER_SECTOR);
  printf("reading dev from block %d\n", BlockNumber);
  hexDump("hexdump", Data, sizeof(Data));
  return 1;
}
