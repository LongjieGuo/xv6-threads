#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"

char*
strcpy(char *s, const char *t)
{
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
    ;
  return os;
}

int
strcmp(const char *p, const char *q)
{
  while(*p && *p == *q)
    p++, q++;
  return (uchar)*p - (uchar)*q;
}

uint
strlen(const char *s)
{
  int n;

  for(n = 0; s[n]; n++)
    ;
  return n;
}

void*
memset(void *dst, int c, uint n)
{
  stosb(dst, c, n);
  return dst;
}

char*
strchr(const char *s, char c)
{
  for(; *s; s++)
    if(*s == c)
      return (char*)s;
  return 0;
}

char*
gets(char *buf, int max)
{
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
    cc = read(0, &c, 1);
    if(cc < 1)
      break;
    buf[i++] = c;
    if(c == '\n' || c == '\r')
      break;
  }
  buf[i] = '\0';
  return buf;
}

int
stat(const char *n, struct stat *st)
{
  int fd;
  int r;

  fd = open(n, O_RDONLY);
  if(fd < 0)
    return -1;
  r = fstat(fd, st);
  close(fd);
  return r;
}

int
atoi(const char *s)
{
  int n;

  n = 0;
  while('0' <= *s && *s <= '9')
    n = n*10 + *s++ - '0';
  return n;
}

void*
memmove(void *vdst, const void *vsrc, int n)
{
  char *dst;
  const char *src;

  dst = vdst;
  src = vsrc;
  while(n-- > 0)
    *dst++ = *src++;
  return vdst;
}


void lock_acquire(lock_t *m){
  while(xchg(&m->flag, 1) == 1);
}

void lock_release(lock_t *m){
	xchg(&m->flag, 0);
}

int lock_init(lock_t *m)
{
  m->flag = 0;
  return 0;
}

int thread_create(void (*start_routine)(void *, void *), void* arg1, void* arg2)
{
  // This routine should call malloc() to create a new 
  //user stack, use clone() to create the child thread and get it running. 
  //It returns the newly created PID to the parent and 0 to the child (if successful), -1 otherwise.
  void* stack = malloc(4096); // later this will change to the users stack 
  return clone(start_routine, arg1, arg2, stack);
}

int thread_join()
{
  //An int thread_join() call should also be created,
  // which calls the underlying join() system call, frees the user stack,
  // and then returns. It returns the waited-for PID (when successful), -1 otherwise.
  void* stack_ptr;
  int ret = join(&stack_ptr);
  return ret;
}