/* SPDX-License-Identifier: MIT */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
/* CPU model of public context helper DMA scheduling, not an ISA emulator. */
static int simulate(uint32_t words,uint32_t block,int fixed){
 uint32_t left=words,offset=0;unsigned bursts=0;
 while(left){
  uint32_t burst=left;if(left>block)burst=block==1024?0:block;
  uint32_t transferred=burst==0?1024:burst;
  if(transferred>left||offset>words||transferred>words-offset)return 0;
  offset+=block;left-=fixed?(left<block?left:block):block;
  if(++bursts>words/16+1)return 0;
 }
 return bursts==(words+block-1)/block;
}
int main(void){
 assert(!simulate(16,256,0));assert(!simulate(16,1024,0));
 unsigned cases=0;
 const unsigned blocks[]={16,256,1024};
 for(unsigned b=0;b<3;b++)for(unsigned words=0;words<=8192;words+=16){assert(simulate(words,blocks[b],1));cases++;}
 printf("PASS %u legal16-dword allocation cases; oldpartial-block underflow reproduced\n",cases);
}
