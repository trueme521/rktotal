#include <string.h>
//#include "Sinodes.h"
#include "des.h"


class DES{
public:
	// Encrypt/decrypt the data in "data", according to the "key".
	// Caller is responsible for confirming the buffer size of "data"
	// points to is 8*"blocks" bytes.
	// The data encrypted/decrypted is stored in data.
	// The return code is 1:success, other:failed.

	int encrypt ( const char key[8], unsigned char* data, int blocks = 1 );
	int decrypt ( const char key[8], unsigned char* data, int blocks = 1 );

	// Encrypt/decrypt any size data,according to a special method.
	// Before calling yencrypt, copy data to a new buffer with size
	// calculated by extend.

	int yencrypt ( const char key[8], unsigned char* data, int size );
	int ydecrypt ( const char key[8], unsigned char* in, int blocks, int* size = 0 );
	int extend ( int size ) { return (size/8+1)*8; };

private:
	void des(unsigned char* in, unsigned char* out, int blocks);
	void des_block(unsigned char* in, unsigned char* out);

private:
	unsigned long KnL[32];
	enum Mode { ENCRYPT, DECRYPT };
	void deskey(const char key[8], Mode md);
	void usekey(unsigned long *);
	void cookey(unsigned long *);

private:
	void scrunch(unsigned char *, unsigned long *);
	void unscrun(unsigned long *, unsigned char *);
	void desfunc(unsigned long *, unsigned long *);

private:
	static unsigned char Df_Key[24];
	static unsigned short bytebit[8];
	static unsigned long bigbyte[24];
	static unsigned char pc1[56];
	static unsigned char totrot[16];
	static unsigned char pc2[48];
	static unsigned long SP1[64];
	static unsigned long SP2[64];
	static unsigned long SP3[64];
	static unsigned long SP4[64];
	static unsigned long SP5[64];
	static unsigned long SP6[64];
	static unsigned long SP7[64];
	static unsigned long SP8[64];
};



int DES::encrypt ( const char key[8], unsigned char* data, int blocks )
{
  if ((!data)||(blocks<1))
      return 0;
  deskey ( key, ENCRYPT );
  des ( data, data, blocks);
  return 1;
};

int DES::decrypt ( const char key[8], unsigned char* data, int blocks )
{
  if ((!data)||(blocks<1))
      return 0;
  deskey ( key, DECRYPT );
  des ( data, data, blocks);
  return 1;
};

int DES::yencrypt ( const char key[8], unsigned char* data, int size )
{
  if ((!data)||(size<1))
    return 0;

  // The last char of data is bitwise complemented and filled the rest
  // buffer.If size is 16, it will extend to 24,and 17 still 24.
  char lastChar = *(data+size-1);
  int blocks = size/8+1;
  memset(data+size, ~lastChar, blocks*8-size);
  deskey( key, ENCRYPT );
  return encrypt ( (const char *)data, data, blocks);
};

int DES::ydecrypt ( const char key[8], unsigned char* data, int blocks, int* size )
{
  if ( (!data) || (blocks<1) )
    return 0;

  deskey ( key, DECRYPT );
  if ( !decrypt ( (const char *)data, data, blocks) )
    return 0;
  if ( size != 0 )
  {
    int pos = blocks*8-1;
    char endChar = data[pos];
    while ((pos>0)&&(data[pos]==endChar))
        pos--;
    if ( data[pos] != ~endChar )
      return 0;
    *size = pos+1;
  }
  return 1;
};

// -----------------------------------------------------------------------
// des
//     Encrpts/Decrypts(according to the key currently loaded int the
//     internal key register) SOME blocks of eight bytes at address 'in'
//     into the block at address 'out'. They can be the same.
//
//     "in"
//     "out"
//     "block" Number of blocks.
// -----------------------------------------------------------------------
void DES::des ( unsigned char* in, unsigned char* out, int blocks )
{
for (int i = 0; i < blocks; i++,in+=8,out+=8)
    des_block(in,out);
};

// -----------------------------------------------------------------------
// des_block
//     Encrpts/Decrypts(according to the key currently loaded int the
//     internal key register) one block of eight bytes at address 'in'
//     into the block at address 'out'. They can be the same.
//
//     "in"
//     "out"
// -----------------------------------------------------------------------
void DES::des_block(unsigned char *in, unsigned char *out)
{
unsigned long work[2];

scrunch(in, work);
desfunc(work, KnL);
unscrun(work, out);
};

// ----------------------------------------------------------------------
// deskey
//     Sets the internal key register (KnR) according to the hexadecimal
//     key contained in the 8 bytes of hexkey, according to the DES,
//     for encryption or decrytion according to MODE
//
//     "key" is the 64 bits key.
//     "md" means encryption or decryption.
// ----------------------------------------------------------------------
void DES::deskey(const char key[8], Mode md)
{
register int ii, j, l, m, n;
unsigned char pc1m[56], pcr[56];
unsigned long kn[32];

for (j = 0; j < 56; j++) {
  l = pc1[j];
  m = l & 07;
  pc1m[j] = (key[l >> 3] & bytebit[m]) ? 1:0;
}
for (ii = 0; ii < 16; ii++) {
  if (md == DECRYPT) m = (15 - ii) << 1;
    else m = ii << 1;
    n = m + 1;
    kn[m] = kn[n] = 0L;
    for (j = 0; j < 28; j++) {
    l = j + totrot[ii];
    if (l < 28) pcr[j] = pc1m[l];
      else pcr[j] = pc1m[l - 28];
    }
    for (j = 28; j < 56; j++) {
    l = j + totrot[ii];
    if (l < 56) pcr[j] = pc1m[l];
      else pcr[j] = pc1m[l - 28];
    }
    for (j = 0; j < 24; j++) {
    if (pcr[ pc2[j] ]) kn[m] |= bigbyte[j];
    if (pcr[ pc2[j+24] ]) kn[n] |= bigbyte[j];
    }
}
cookey(kn);
return;
};

// ----------------------------------------------------------------------
// cookey
//     Only called by deskey.
// -----------------------------------------------------------------------
void DES::cookey(register unsigned long *raw1)
{
register unsigned long *cook, *raw0;
unsigned long dough[32];
register int i;

cook = dough;
for (i = 0; i < 16; i++, raw1++) {
  raw0 = raw1++;
  *cook = (*raw0 & 0x00fc0000L) << 6;
  *cook |= (*raw0 & 0x00000fc0L) << 10;
  *cook |= (*raw1 & 0x00fc0000L) >> 10;
  *cook++ |= (*raw1 & 0x00000fc0L) >> 6;
  *cook = (*raw0 & 0x0003f000L) << 12;
  *cook |= (*raw0 & 0x0000003fL) << 16;
  *cook |= (*raw1 & 0x0003f000L) >> 4;
  *cook++ |= (*raw1 & 0x0000003fL);
}
usekey(dough);
return;
};

// ----------------------------------------------------------------------
// usekey
//     Only called by cookey.
//     Loads the interal key register with the data in cookedkey.
// -----------------------------------------------------------------------
void DES::usekey(register unsigned long *from)
{
register unsigned long *to, *endp;

to = KnL, endp = &KnL[32];
while (to < endp) *to++ = *from++;
return;
};

void DES::scrunch(register unsigned char *outof, register unsigned long *into )
{
*into = (*outof++ & 0xffL) << 24;
*into |= (*outof++ & 0xffL) << 16;
*into |= (*outof++ & 0xffL) << 8;
*into++ |= (*outof++ & 0xffL);
*into = (*outof++ & 0xffL) << 24;
*into |= (*outof++ & 0xffL) << 16;
*into |= (*outof++ & 0xffL) << 8;
*into |= (*outof & 0xffL);
return;
};

void DES::unscrun(register unsigned long *outof, register unsigned char *into)
{
*into++ = (unsigned char)((*outof >> 24) & 0xffL);
*into++ = (unsigned char)((*outof >> 16) & 0xffL);
*into++ = (unsigned char)((*outof >> 8) & 0xffL);
*into++ = (unsigned char)(*outof++ & 0xffL);
*into++ = (unsigned char)((*outof >> 24) & 0xffL);
*into++ = (unsigned char)((*outof >> 16) & 0xffL);
*into++ = (unsigned char)((*outof >> 8) & 0xffL);
*into = (unsigned char)(*outof & 0xffL);
return;
};

void DES::desfunc(register unsigned long *block,register unsigned long *keys)
{
register unsigned long fval, work, right, leftt;
register int round;

leftt = block[0];
right = block[1];
work = ((leftt >> 4) ^ right) & 0x0f0f0f0fL;
right ^= work;
leftt ^= (work << 4);
work = ((leftt >> 16) ^ right) & 0x0000ffffL;
right ^= work;
leftt ^= (work << 16);
work = ((right >> 2) ^ leftt) & 0x33333333L;
leftt ^= work;
right ^= (work << 2);
work = ((right >> 8) ^ leftt) & 0x00ff00ffL;
leftt ^= work;
right ^= (work << 8);
right = ((right << 1) | ((right >> 31) & 1L)) & 0xffffffffL;
work = (leftt ^ right) & 0xaaaaaaaaL;
leftt ^= work;
right ^= work;
leftt = ((leftt << 1) | ((leftt >> 31) & 1L)) & 0xffffffffL;

for (round = 0; round < 8; round++) {
    work = (right << 28) | (right >> 4);
    work ^= *keys++;
    fval = SP7[work       & 0x3fL];
    fval |= SP5[(work >> 8) & 0x3fL];
    fval |= SP3[(work >> 16) & 0x3fL];
    fval |= SP1[(work >> 24) & 0x3fL];
    work = right ^ *keys++;
    fval |= SP8[work       & 0x3fL];
    fval |= SP6[(work >> 8) & 0x3fL];
    fval |= SP4[(work >> 16) & 0x3fL];
    fval |= SP2[(work >> 24) & 0x3fL];
    leftt ^= fval;
    work = (leftt << 28) | (leftt >> 4);
    work ^= *keys++;
    fval = SP7[work       & 0x3fL];
    fval |= SP5[(work >> 8) & 0x3fL];
    fval |= SP3[(work >> 16) & 0x3fL];
    fval |= SP1[(work >> 24) & 0x3fL];
    work = leftt ^ *keys++;
    fval |= SP8[work       & 0x3fL];
    fval |= SP6[(work >> 8) & 0x3fL];
    fval |= SP4[(work >> 16) & 0x3fL];
  fval |= SP2[(work >> 24) & 0x3fL];
  right ^= fval;
}
right = (right << 31) | (right >> 1);
work = (leftt ^ right) & 0xaaaaaaaaL;
leftt ^= work;
right ^= work;
leftt = (leftt << 31) | ( leftt >> 1);
work = ((leftt >> 8) ^ right) & 0x00ff00ffL;
right ^= work;
leftt ^= (work << 8);
work = ((leftt >> 2) ^ right) & 0x33333333L;
right ^= work;
leftt ^= (work << 2);
work = ((right >> 16) ^ leftt) & 0x0000ffffL;
leftt ^= work;
right ^= (work << 16);
work = ((right >> 4) ^ leftt) & 0x0f0f0f0fL;
leftt ^= work;
right ^= (work << 4);
*block++ = right;
*block = leftt;
return;
};
// -----------------------------------------------------------------------
// Initial of static data members. These data will be used by all the
// instances of class,and can  not be changed.
// -----------------------------------------------------------------------
unsigned char DES::Df_Key[24] = {
    0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
    0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10,
    0x89, 0xab, 0xcd, 0xef, 0x01, 0x23, 0x45, 0x67 };

unsigned short DES::bytebit[8] = {
    0200, 0100, 040, 020, 010, 04, 02, 01 };

unsigned long DES::bigbyte[24] = {
    0x800000L, 0x400000L, 0x200000L, 0x100000L,
    0x80000L, 0x40000L, 0x20000L, 0x10000L,
    0x8000L,   0x4000L,   0x2000L,   0x1000L,
    0x800L,   0x400L,   0x200L,   0x100L,
    0x80L,   0x40L,   0x20L,   0x10L,
    0x8L,     0x4L,     0x2L,     0x1L     };

unsigned char DES::pc1[56] = {
    56, 48, 40, 32, 24, 16, 8, 0, 57, 49, 41, 33, 25, 17,
    9, 1, 58, 50, 42, 34, 26, 18, 10, 2, 59, 51, 43, 35,
    62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37, 29, 21,
    13, 5, 60, 52, 44, 36, 28, 20, 12, 4, 27, 19, 11, 3   };

unsigned char DES::totrot[16] = {
    1, 2, 4, 6, 8, 10, 12, 14, 15, 17, 19, 21, 23, 25, 27, 28 };

unsigned char DES::pc2[48] = {
    13, 16, 10, 23, 0, 4,     2, 27, 14, 5, 20, 9,
    22, 18, 11, 3, 25, 7,   15, 6, 26, 19, 12, 1,
    40, 51, 30, 36, 46, 54,   29, 39, 50, 44, 32, 47,
    43, 48, 38, 55, 33, 52,   45, 41, 49, 35, 28, 31   };

unsigned long DES::SP1[64] = {
    0x01010400L, 0x00000000L, 0x00010000L, 0x01010404L,
    0x01010004L, 0x00010404L, 0x00000004L, 0x00010000L,
    0x00000400L, 0x01010400L, 0x01010404L, 0x00000400L,
    0x01000404L, 0x01010004L, 0x01000000L, 0x00000004L,
    0x00000404L, 0x01000400L, 0x01000400L, 0x00010400L,
    0x00010400L, 0x01010000L, 0x01010000L, 0x01000404L,
    0x00010004L, 0x01000004L, 0x01000004L, 0x00010004L,
    0x00000000L, 0x00000404L, 0x00010404L, 0x01000000L,
    0x00010000L, 0x01010404L, 0x00000004L, 0x01010000L,
    0x01010400L, 0x01000000L, 0x01000000L, 0x00000400L,
    0x01010004L, 0x00010000L, 0x00010400L, 0x01000004L,
    0x00000400L, 0x00000004L, 0x01000404L, 0x00010404L,
    0x01010404L, 0x00010004L, 0x01010000L, 0x01000404L,
    0x01000004L, 0x00000404L, 0x00010404L, 0x01010400L,
    0x00000404L, 0x01000400L, 0x01000400L, 0x00000000L,
    0x00010004L, 0x00010400L, 0x00000000L, 0x01010004L };

unsigned long DES::SP2[64] = {
    0x80108020L, 0x80008000L, 0x00008000L, 0x00108020L,
    0x00100000L, 0x00000020L, 0x80100020L, 0x80008020L,
    0x80000020L, 0x80108020L, 0x80108000L, 0x80000000L,
    0x80008000L, 0x00100000L, 0x00000020L, 0x80100020L,
    0x00108000L, 0x00100020L, 0x80008020L, 0x00000000L,
    0x80000000L, 0x00008000L, 0x00108020L, 0x80100000L,
    0x00100020L, 0x80000020L, 0x00000000L, 0x00108000L,
    0x00008020L, 0x80108000L, 0x80100000L, 0x00008020L,
    0x00000000L, 0x00108020L, 0x80100020L, 0x00100000L,
    0x80008020L, 0x80100000L, 0x80108000L, 0x00008000L,
    0x80100000L, 0x80008000L, 0x00000020L, 0x80108020L,
    0x00108020L, 0x00000020L, 0x00008000L, 0x80000000L,
    0x00008020L, 0x80108000L, 0x00100000L, 0x80000020L,
    0x00100020L, 0x80008020L, 0x80000020L, 0x00100020L,
    0x00108000L, 0x00000000L, 0x80008000L, 0x00008020L,
    0x80000000L, 0x80100020L, 0x80108020L, 0x00108000L };

unsigned long DES::SP3[64] = {
    0x00000208L, 0x08020200L, 0x00000000L, 0x08020008L,
    0x08000200L, 0x00000000L, 0x00020208L, 0x08000200L,
    0x00020008L, 0x08000008L, 0x08000008L, 0x00020000L,
    0x08020208L, 0x00020008L, 0x08020000L, 0x00000208L,
    0x08000000L, 0x00000008L, 0x08020200L, 0x00000200L,
    0x00020200L, 0x08020000L, 0x08020008L, 0x00020208L,
    0x08000208L, 0x00020200L, 0x00020000L, 0x08000208L,
    0x00000008L, 0x08020208L, 0x00000200L, 0x08000000L,
    0x08020200L, 0x08000000L, 0x00020008L, 0x00000208L,
    0x00020000L, 0x08020200L, 0x08000200L, 0x00000000L,
    0x00000200L, 0x00020008L, 0x08020208L, 0x08000200L,
    0x08000008L, 0x00000200L, 0x00000000L, 0x08020008L,
    0x08000208L, 0x00020000L, 0x08000000L, 0x08020208L,
    0x00000008L, 0x00020208L, 0x00020200L, 0x08000008L,
    0x08020000L, 0x08000208L, 0x00000208L, 0x08020000L,
    0x00020208L, 0x00000008L, 0x08020008L, 0x00020200L };

unsigned long DES::SP4[64] = {
    0x00802001L, 0x00002081L, 0x00002081L, 0x00000080L,
    0x00802080L, 0x00800081L, 0x00800001L, 0x00002001L,
    0x00000000L, 0x00802000L, 0x00802000L, 0x00802081L,
    0x00000081L, 0x00000000L, 0x00800080L, 0x00800001L,
    0x00000001L, 0x00002000L, 0x00800000L, 0x00802001L,
    0x00000080L, 0x00800000L, 0x00002001L, 0x00002080L,
    0x00800081L, 0x00000001L, 0x00002080L, 0x00800080L,
    0x00002000L, 0x00802080L, 0x00802081L, 0x00000081L,
    0x00800080L, 0x00800001L, 0x00802000L, 0x00802081L,
    0x00000081L, 0x00000000L, 0x00000000L, 0x00802000L,
    0x00002080L, 0x00800080L, 0x00800081L, 0x00000001L,
    0x00802001L, 0x00002081L, 0x00002081L, 0x00000080L,
    0x00802081L, 0x00000081L, 0x00000001L, 0x00002000L,
    0x00800001L, 0x00002001L, 0x00802080L, 0x00800081L,
    0x00002001L, 0x00002080L, 0x00800000L, 0x00802001L,
    0x00000080L, 0x00800000L, 0x00002000L, 0x00802080L };

unsigned long DES::SP5[64] = {
    0x00000100L, 0x02080100L, 0x02080000L, 0x42000100L,
    0x00080000L, 0x00000100L, 0x40000000L, 0x02080000L,
    0x40080100L, 0x00080000L, 0x02000100L, 0x40080100L,
    0x42000100L, 0x42080000L, 0x00080100L, 0x40000000L,
    0x02000000L, 0x40080000L, 0x40080000L, 0x00000000L,
    0x40000100L, 0x42080100L, 0x42080100L, 0x02000100L,
    0x42080000L, 0x40000100L, 0x00000000L, 0x42000000L,
    0x02080100L, 0x02000000L, 0x42000000L, 0x00080100L,
    0x00080000L, 0x42000100L, 0x00000100L, 0x02000000L,
    0x40000000L, 0x02080000L, 0x42000100L, 0x40080100L,
    0x02000100L, 0x40000000L, 0x42080000L, 0x02080100L,
    0x40080100L, 0x00000100L, 0x02000000L, 0x42080000L,
    0x42080100L, 0x00080100L, 0x42000000L, 0x42080100L,
    0x02080000L, 0x00000000L, 0x40080000L, 0x42000000L,
    0x00080100L, 0x02000100L, 0x40000100L, 0x00080000L,
    0x00000000L, 0x40080000L, 0x02080100L, 0x40000100L };

unsigned long DES::SP6[64] = {
    0x20000010L, 0x20400000L, 0x00004000L, 0x20404010L,
    0x20400000L, 0x00000010L, 0x20404010L, 0x00400000L,
    0x20004000L, 0x00404010L, 0x00400000L, 0x20000010L,
    0x00400010L, 0x20004000L, 0x20000000L, 0x00004010L,
    0x00000000L, 0x00400010L, 0x20004010L, 0x00004000L,
    0x00404000L, 0x20004010L, 0x00000010L, 0x20400010L,
    0x20400010L, 0x00000000L, 0x00404010L, 0x20404000L,
    0x00004010L, 0x00404000L, 0x20404000L, 0x20000000L,
    0x20004000L, 0x00000010L, 0x20400010L, 0x00404000L,
    0x20404010L, 0x00400000L, 0x00004010L, 0x20000010L,
    0x00400000L, 0x20004000L, 0x20000000L, 0x00004010L,
    0x20000010L, 0x20404010L, 0x00404000L, 0x20400000L,
    0x00404010L, 0x20404000L, 0x00000000L, 0x20400010L,
    0x00000010L, 0x00004000L, 0x20400000L, 0x00404010L,
    0x00004000L, 0x00400010L, 0x20004010L, 0x00000000L,
    0x20404000L, 0x20000000L, 0x00400010L, 0x20004010L };

unsigned long DES::SP7[64] = {
    0x00200000L, 0x04200002L, 0x04000802L, 0x00000000L,
    0x00000800L, 0x04000802L, 0x00200802L, 0x04200800L,
    0x04200802L, 0x00200000L, 0x00000000L, 0x04000002L,
    0x00000002L, 0x04000000L, 0x04200002L, 0x00000802L,
    0x04000800L, 0x00200802L, 0x00200002L, 0x04000800L,
    0x04000002L, 0x04200000L, 0x04200800L, 0x00200002L,
    0x04200000L, 0x00000800L, 0x00000802L, 0x04200802L,
    0x00200800L, 0x00000002L, 0x04000000L, 0x00200800L,
    0x04000000L, 0x00200800L, 0x00200000L, 0x04000802L,
    0x04000802L, 0x04200002L, 0x04200002L, 0x00000002L,
    0x00200002L, 0x04000000L, 0x04000800L, 0x00200000L,
    0x04200800L, 0x00000802L, 0x00200802L, 0x04200800L,
    0x00000802L, 0x04000002L, 0x04200802L, 0x04200000L,
    0x00200800L, 0x00000000L, 0x00000002L, 0x04200802L,
    0x00000000L, 0x00200802L, 0x04200000L, 0x00000800L,
    0x04000002L, 0x04000800L, 0x00000800L, 0x00200002L };

unsigned long DES::SP8[64] = {
    0x10001040L, 0x00001000L, 0x00040000L, 0x10041040L,
    0x10000000L, 0x10001040L, 0x00000040L, 0x10000000L,
    0x00040040L, 0x10040000L, 0x10041040L, 0x00041000L,
    0x10041000L, 0x00041040L, 0x00001000L, 0x00000040L,
    0x10040000L, 0x10000040L, 0x10001000L, 0x00001040L,
    0x00041000L, 0x00040040L, 0x10040040L, 0x10041000L,
    0x00001040L, 0x00000000L, 0x00000000L, 0x10040040L,
    0x10000040L, 0x10001000L, 0x00041040L, 0x00040000L,
    0x00041040L, 0x00040000L, 0x10041000L, 0x00001000L,
    0x00000040L, 0x10040040L, 0x00001000L, 0x00041040L,
    0x10001000L, 0x00000040L, 0x10000040L, 0x10040000L,
    0x10040040L, 0x10000000L, 0x00040000L, 0x10001040L,
    0x00000000L, 0x10041040L, 0x00040040L, 0x10000040L,
    0x10040000L, 0x10001000L, 0x10001040L, 0x00000000L,
    0x10041040L, 0x00041000L, 0x00041000L, 0x00001040L,
    0x00001040L, 0x00040040L, 0x10000000L, 0x10041000L };



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CDes::CDes()
{
	m_encryptBuf = NULL;
	m_decryptBuf = NULL;
	m_eBufLen = 0;
	m_dBufLen = 0;
}

CDes::~CDes()
{
	if (m_encryptBuf != NULL)
	{
		delete[] m_encryptBuf;
		m_encryptBuf = NULL;
		m_eBufLen = 0;
	}
	if (m_decryptBuf != NULL)
	{
		delete[] m_decryptBuf;
		m_decryptBuf = NULL;
		m_dBufLen = 0;
	}
}

/**
*DES加密调用	encrypt

*参数说明
unsigned char key[8]		--DES使用的密钥，8个字节长
char* data					--加密前的明文指针，明文可以以字符串方式存储，也可以是某些不可见的字节数组
int dataLen					--指示加密前的明文长度，单位为字节，即使明文为正常可见字符串，该字段也必须给出真实值
PADDING_MODE pad			--填充模式，目前只支持PKCS5方式
CIPHER_MODE cip				--分组处理方式，目前只支持ECB和CBC两种模式
unsigned char IV[8]			--初始向量，8个字节长，用于CBC模式，非CBC模式随便进行内容填充即可

*返回值说明
返回加密后的BASE64字符串，该字符串经由加密后的密文字节数组经BASE64运算后得到。为正常字符串，以'\0'结尾
当程序内部算法出现异常时，该返回值可能为空
特别要注意。返回的指针指向内部分配的缓冲区，请不要在外部进行释放，而且，请将内容复制到外部的数据空间内。
			原因是类对象释放时将释放该缓冲区，即使类对象不释放，当下次调用加密函数时，该缓冲区内容也会被新的内容覆盖。
*/

char* CDes::Encrypt ( const char key[8], char* data, int& dataLen, PADDING_MODE pad, CIPHER_MODE cip, const unsigned char IV[8])
{
	//不使用外部内存，内部重新分配存储空间
    char* datavv = data;
	int padLength = (dataLen/8+1)*8;			//计算填充后的总长度
	if (allocEncode(padLength+1) <= 0)				//分配内存空间
	{
		return NULL;
	}
    data = m_encryptBuf;		
    memcpy(data,datavv,dataLen);				//复制明文内容
	//填充
	if (pad==PKCS5)
	{
		paddingWithPKCS5(data, dataLen);
	}

    dataLen = padLength;

	//填充数据结尾，便于计算MD5时确定数据长度
	data[padLength]=0;
	//DES加密
    DES des ;
	int i;
	//得到初始向量
	unsigned char* cipher_v = (unsigned char*)IV;
	for (i = 0; i < padLength/8; i++)
	{
		if (cip == CBC)
		{
			//cbc模式对明文与向量进行异或
			for (int j = 0; j < 8; j++)
			{
				data[i*8+j] = ((unsigned char)data[i*8+j])^cipher_v[j];
			}
		}
		//分组加密
	    des.encrypt(key,(unsigned char *)(data+i*8),1);
		if (cip == CBC)
		{
			//cbc模式更新向量
			cipher_v = (unsigned char*)data+i*8;
		}
	}

    return data;
};




/**
*DES解密调用	decrypt

*参数说明
unsigned char key[8]		--DES使用的密钥，8个字节长
char* data					--解密前的密文字符串指针，密文为BASE64形式进行存储，且必须是BSSE64形式
PADDING_MODE pad			--填充模式，目前只支持PKCS5方式
CIPHER_MODE cip				--分组处理方式，目前只支持ECB和CBC两种模式
unsigned char IV[8]			--初始向量，8个字节长，用于CBC模式，非CBC模式随便进行内容填充即可
int &resultLength			--调用后，用该值返回明文内容的长度，正常情况下，该长度与strlen(<本函数返回字符串>)相等。

*返回值说明
返回解密后的明文字符串，该字符串为正常字符串，以'\0'结尾。当程序内部算法出现异常时，该返回值可能为空，或长度为0的字符串
特别要注意。返回的指针指向内部分配的缓冲区，请不要在外部进行释放，而且，请将内容复制到外部的数据空间内。
			原因是类对象释放时将释放该缓冲区，即使类对象不释放，当下次调用加密函数时，该缓冲区内容也会被新的内容覆盖。

另外，由于DES算法的特性，要求密文在BASE64反解后，字节数为8的倍数。如果长度存在余数，本程序会自动舍弃结尾的多余信息。
*/
char* CDes::Decrypt ( const char key[8], char* data, int &resultLength, PADDING_MODE pad, CIPHER_MODE cip, const unsigned char IV[8])
{
//base64解码
    int  reaLen = resultLength;
    char* datarea=data;
	
//将字符指针转化为字符数组
	if (allocDecode(reaLen+1) <= 0)
	{
		resultLength = 0;
		return NULL;
	}
    char* datares = m_decryptBuf;//datarea = datares;    
//对齐，正常情况下，密文长度是8的倍数，如果不是，舍弃掉结尾多余的部分
	reaLen = (reaLen/8)*8;
	if (reaLen == 0)
	{
		//如果舍弃后长度为0，直接返回空字符串
		resultLength = 0;
		datares[0]=0;		
		return datares;
	}
    memcpy(datares,datarea,reaLen);
    datares[reaLen] = '\0';
//DES解密
    DES des ;
	int i;
	unsigned char* cipher_v;
	for (i = reaLen/8-1; i >= 0; i--)
	{
		if (cip == CBC)
		{
			//处理向量
			if (i > 0)
				cipher_v = (unsigned char*)datares+(i-1)*8;
			else
				cipher_v = (unsigned char*)IV;
		}
	    des.decrypt(key,(unsigned char*)(datares+i*8),1);
		if (cip == CBC)
		{
			//cbc模式对明文与向量进行异或
			for (int j = 0; j < 8; j++)
			{
				datares[i*8+j] = ((unsigned char)datares[i*8+j])^cipher_v[j];
			}
		}
	}
	resultLength = reaLen;
	//去掉填充，并计算实际明文长度，用于返回
	if (pad==PKCS5)
	{
		resultLength -= unPaddingWithPKCS5(datares, reaLen);
	}
    return datares;
};

/**
PKCS5填充函数
*/
int CDes::paddingWithPKCS5( char* pData, int dataLen)
{
	unsigned char byPadByte = 8-(dataLen % 8);
	int i;
	for (i = dataLen; i < dataLen+byPadByte; i++)
	{
		pData[i] = (char)byPadByte;
	}
	return byPadByte;
}
/**
PKCS5填充去除函数
*/
int CDes::unPaddingWithPKCS5( char* pData, int dataLen)
{
	unsigned char byPadByte = (unsigned char)pData[dataLen-1];
	int i;
	for (i = dataLen-1; i >= dataLen-byPadByte && i >=0; i--)
	{
		pData[i] = 0;
	}
	return byPadByte;
}

/**
分配加密buffer
*/
unsigned long CDes::allocEncode(unsigned long newSize)
{
	if (newSize <= m_eBufLen)
		goto AENCEND;
	char* temp = new char[newSize];
	if (temp == NULL)
	{
		delete[] m_encryptBuf;
		m_encryptBuf = NULL;
		m_eBufLen = 0;
		goto AENCEND;
	}
	if (m_encryptBuf != NULL)
	{
		memcpy(temp, m_encryptBuf, m_eBufLen);
		delete[] m_encryptBuf;
		m_encryptBuf = NULL;
		m_eBufLen = 0;
	}
	m_encryptBuf = temp;
	m_eBufLen = newSize;
AENCEND:
	return m_eBufLen;
}

/**
分配解密buffer
*/
unsigned long CDes::allocDecode(unsigned long newSize)
{
	if (newSize <= m_dBufLen)
		goto ADECEND;
	char* temp = new char[newSize];
	if (temp == NULL)
	{
		delete[] m_decryptBuf;
		m_decryptBuf = NULL;
		m_dBufLen = 0;
		goto ADECEND;
	}
	if (m_decryptBuf != NULL)
	{
		memcpy(temp, m_decryptBuf, m_dBufLen);
		delete[] m_decryptBuf;
		m_decryptBuf = NULL;
		m_dBufLen = 0;
	}
	m_decryptBuf = temp;
	m_dBufLen = newSize;
ADECEND:
	return m_dBufLen;
}
