#ifndef _TIMEVAL
#define _TIMEVAL

struct timeval {
        unsigned long  tv_sec;
        unsigned long  tv_usec;
};

struct timezone {
  unsigned short tmz;
};

struct itimerval {
        struct timeval it_interval;
        struct timeval it_value;
};

int _System gettimeofday(struct timeval * ,struct timezone * );
int _System settimeofday(struct timeval *,struct timezone * );

#endif  /* _TIMEVAL */
