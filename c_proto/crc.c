/* crc.c  01/04/2015  D.J.Whale
 *
 * Implements ITU-V.41 FCS algorithm.
 */

#include <stdio.h>

typedef unsigned char BYTE;
typedef unsigned short WORD;

WORD UpdateCrc(BYTE ch, WORD *pwCrc)
{
  ch = (ch^(BYTE)((*pwCrc) & 0x00FF));
  ch = (ch^(ch<<4));
  *pwCrc = (*pwCrc >> 8) 
         ^ ((WORD)ch << 8) 
         ^ ((WORD)ch<<3) 
         ^ ((WORD)ch>>4);
  return *pwCrc;
}

WORD ComputeCrc(BYTE *data, unsigned int len, BYTE *crc0, BYTE *crc1)
{
  BYTE bBlock;
  WORD wCrc;

  wCrc = 0x6363;

  do
  {
    bBlock = *data++;
    UpdateCrc(bBlock, &wCrc);
  }
  while (--len);

  *crc0 = (BYTE) (wCrc & 0xFF);
  *crc1 = (BYTE) ((wCrc >> 8) & 0xFF);
  return wCrc;
}


void test(BYTE* buf, unsigned int len)
{
  int i;
  BYTE crc0, crc1;

  printf("buf:");

  for(i=0; i<len; i++)
  {
    printf("%02X ",buf[i]);
  }

  ComputeCrc(buf, len, &crc0, &crc1);

  printf("\nCRC:%02X %02X\n", (unsigned int)crc0, (unsigned int)crc1);
}


/* Test vectors from: st.com AN4433 */

/* Select NFC T4 application */
BYTE buf1[] = {0x02,0x00,0xA4,0x04,0x00,0x07,0xD2,0x76,0x00,0x00,0x85,0x01,0x01,0x00};

/* Select CC file */
BYTE buf2[] = {0x03,0x00,0xA4,0x00,0x0C,0x02,0xE1,0x03};

/* Read CC file length */
BYTE buf3[] = {0x02,0x00,0xB0,0x00,0x00,0x02};

/* Read CC file */
BYTE buf4[] = {0x03,0x00,0xB0,0x00,0x00,0x0F};

/* Select NDEF file */
BYTE buf5[] = {0x02,0x00,0xA4,0x00,0x0C,0x02,0x00,0x01};

/* Read NDEF message length */
BYTE buf6[] = {0x03,0x00,0xB0,0x00,0x00,0x02};
//NOTE: appnote says 0x6B 0x7D, but returns 0x40, 0x79

/* Read NDEF message */
BYTE buf7[] = {0x02,0x00,0xB0,0x00,0x02,0x14};
//NOTE: appnote does not give expected CRC due to printing error

/* Deselect */
BYTE buf8[] = {0xC2};


int main(void)
{
  test(buf1, sizeof(buf1));
  test(buf2, sizeof(buf2));
  test(buf3, sizeof(buf3));
  test(buf4, sizeof(buf4));
  test(buf5, sizeof(buf5));
  test(buf6, sizeof(buf6));
  test(buf7, sizeof(buf7));
  test(buf8, sizeof(buf8));

  return 0;
}
