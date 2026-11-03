#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/uio.h>
#include <elf.h>
#include <errno.h>
struct regs { unsigned long long x[31], sp, pc, pstate; };
static void rdstr(int pid, unsigned long a, char *out, int n){
  out[0]=0; struct iovec l={out,n-1}, r={(void*)a,n-1};
  long k=process_vm_readv(pid,&l,1,&r,1,0); if(k<0){strcpy(out,"?");return;} out[k]=0;
}
int main(int argc,char**argv){
  int pid=fork();
  if(!pid){ ptrace(PTRACE_TRACEME,0,0,0); if(chroot("/aroot")||chdir("/")){perror("chroot");_exit(1);} execv(argv[1],argv+1); perror("exec"); _exit(1);}
  int st; waitpid(pid,&st,0);
  ptrace(PTRACE_SETOPTIONS,pid,0,PTRACE_O_TRACESYSGOOD|PTRACE_O_TRACECLONE|PTRACE_O_TRACEFORK);
  int in=0; char buf[256]; 
  for(;;){
    ptrace(PTRACE_SYSCALL,pid,0,0);
    int w=waitpid(-1,&st,__WALL); if(w<0)break;
    if(WIFEXITED(st)&&w==pid)break;
    if(!WIFSTOPPED(st)){continue;}
    if(WSTOPSIG(st)==(SIGTRAP|0x80)){
      struct regs r; struct iovec io={&r,sizeof r};
      ptrace(PTRACE_GETREGSET,w,(void*)NT_PRSTATUS,&io);
      int c32 = io.iov_len == 72; unsigned *q=(unsigned*)&r;
      long nr = c32 ? q[7] : r.x[8];
      if (c32) {
        static int e2[65536];
        if(!e2[w&0xffff]){ e2[w&0xffff]=1;
          fprintf(stderr,"[%d] c%ld(%x,%x,%x",w,nr,q[0],q[1],q[2]);
          int pi = (nr==322||nr==334)?1: (nr==5||nr==33||nr==195||nr==106||nr==196||nr==38||nr==10||nr==85)?0:-1;
          if(pi>=0){ rdstr(w,q[pi],buf,sizeof buf); fprintf(stderr," \"%s\"",buf);} fprintf(stderr,")");
        } else { e2[w&0xffff]=0; fprintf(stderr," = %d\n",(int)q[0]); }
        continue;
      }
      static int ent[65536]; 
      if(!ent[w&0xffff]){ ent[w&0xffff]=1;
        if(nr==56||nr==48||nr==78||nr==35){ int ai= nr==56?1:(nr==78?1:(nr==35?0:1)); if(nr==48)ai=1; rdstr(w,r.x[ai],buf,sizeof buf); fprintf(stderr,"[%d] sc%ld(\"%s\"",w,nr,buf);} else if(nr==29){fprintf(stderr,"[%d] ioctl(fd=%lld,cmd=0x%llx",w,r.x[0],r.x[1]);} else fprintf(stderr,"[%d] sc%ld(",w,nr);
        fprintf(stderr,")");
      } else { ent[w&0xffff]=0; fprintf(stderr," = %lld\n",(long long)r.x[0]); }
    } 
    else if(WSTOPSIG(st)!=SIGTRAP && WSTOPSIG(st)!=SIGSTOP){ ptrace(PTRACE_SYSCALL,w,0,WSTOPSIG(st)); }
  }
  return 0;
}
