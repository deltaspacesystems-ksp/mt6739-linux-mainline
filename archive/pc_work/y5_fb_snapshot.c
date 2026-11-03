#include <fcntl.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
int main(void){
 const unsigned width=720,height=1440,stride=2880;const uint64_t phys=0x775e8e38ULL;
 unsigned offset=(unsigned)(phys&4095);size_t len=(offset+(size_t)stride*height+4095)&~4095ULL;
 int fd=open("/dev/mem",O_RDONLY|O_SYNC);if(fd<0)return 2;
 const volatile unsigned char*map=mmap(NULL,len,PROT_READ,MAP_SHARED,fd,(off_t)(phys&~4095ULL));if(map==MAP_FAILED){close(fd);return 2;}
 unsigned n=0,black=0,blue=0,white=0;uint64_t hash=1469598103934665603ULL;
 for(unsigned sample=0;sample<4050;sample++){ unsigned y=(sample*761U+11)%height,x=(sample*313U+7)%width;
  uint32_t p=*(const volatile uint32_t*)(map+offset+(size_t)y*stride+x*4);unsigned r=(p>>16)&255,g=(p>>8)&255,b=p&255;
  n++;black+=(r==0&&g==0&&b==0);blue+=(r<64&&g>=16&&g<90&&b>=24&&b<120);white+=(r>160&&g>160&&b>160);hash=(hash^p)*1099511628211ULL;
 }
 printf("FB samples=%u black=%u dark_blue=%u white=%u hash=0x%016" PRIx64 "\n",n,black,blue,white,hash);
 munmap((void*)map,len);close(fd);return 0;
}
