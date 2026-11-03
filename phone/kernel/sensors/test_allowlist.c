#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <linux/i2c.h>
#include <stdio.h>
static bool als_control;
static bool allowed(const struct i2c_msg *m, int n)
{
 unsigned int r, len;
 /* Exact stock MC3xxx identity registers only; no probing other addresses. */
 if(n==2 && m[0].addr==0x4c && m[1].addr==0x4c &&
    !(m[0].flags & ~I2C_M_DMA_SAFE) &&
    (m[1].flags & ~I2C_M_DMA_SAFE)==I2C_M_RD &&
    m[0].len==1 && m[1].len==1 && m[0].buf && m[1].buf &&
    (m[0].buf[0]==0x3b || m[0].buf[0]==0x18))return true;
 if (als_control && n == 1 && m[0].addr == 0x53 &&
     !(m[0].flags & ~I2C_M_DMA_SAFE) && m[0].len == 2 && m[0].buf &&
     m[0].buf[0] == 0 && (m[0].buf[1] == 0 || m[0].buf[1] == 2))
  return true;
 if (n != 2 || m[0].addr != 0x53 || m[1].addr != 0x53 ||
     (m[0].flags & ~I2C_M_DMA_SAFE) ||
     (m[1].flags & ~I2C_M_DMA_SAFE) != I2C_M_RD || m[0].len != 1 ||
     !m[0].buf || !m[1].buf)
  return false;
 r = m[0].buf[0]; len = m[1].len;
 return (r <= 6 && len == 1) ||
        (r == 8 && len == 2) || ((r == 10 || r == 13) && len == 3);
}

int main(void) {
 uint8_t reg=6,out[4];struct i2c_msg m[2]={{.addr=0x53,.len=1,.buf=&reg},{.addr=0x53,.flags=I2C_M_RD,.len=1,.buf=out}};
 assert(allowed(m,2));m[0].flags=I2C_M_DMA_SAFE;m[1].flags=I2C_M_RD|I2C_M_DMA_SAFE;assert(allowed(m,2));m[0].flags=0;m[1].flags=I2C_M_RD;
 for(unsigned r=0;r<256;r++) for(unsigned len=0;len<5;len++) {
  reg=r;m[1].len=len;
  bool expected=(r<=6&&len==1)||(r==8&&len==2)||((r==10||r==13)&&len==3);
  assert(allowed(m,2)==expected);
 }
 reg=6;m[1].len=1;assert(!allowed(m,0));assert(!allowed(m,1));
 m[0].addr=0x54;assert(!allowed(m,2));m[0].addr=0x53;
 m[1].addr=0x54;assert(!allowed(m,2));m[1].addr=0x53;
 m[0].flags=I2C_M_RD;assert(!allowed(m,2));m[0].flags=0;
 m[1].flags=0;assert(!allowed(m,2));m[1].flags=I2C_M_RD|I2C_M_TEN;assert(!allowed(m,2));m[1].flags=I2C_M_RD;
 m[0].len=2;assert(!allowed(m,2));m[0].len=1;
 m[0].buf=0;assert(!allowed(m,2));m[0].buf=&reg;m[1].buf=0;assert(!allowed(m,2));
 m[0]=(struct i2c_msg){.addr=0x4c,.len=1,.buf=&reg};m[1]=(struct i2c_msg){.addr=0x4c,.flags=I2C_M_RD,.len=1,.buf=out};
 for(unsigned r=0;r<256;r++){reg=r;assert(allowed(m,2)==(r==0x3b||r==0x18));}
 reg=0x3b;m[1].len=2;assert(!allowed(m,2));m[1].len=1;m[1].flags=0;assert(!allowed(m,2));
 uint8_t w[2]={0,2};m[0]=(struct i2c_msg){.addr=0x53,.len=2,.buf=w};
 assert(!allowed(m,1));als_control=true;
 for(unsigned r=0;r<256;r++)for(unsigned v=0;v<256;v++) {w[0]=r;w[1]=v;assert(allowed(m,1)==(r==0&&(v==0||v==2)));}
 puts("PASS exact production allowlist: 1280 register/length pairs plus malformed flags/address/count/buffer cases");
}
