#ifndef __IOCTL_32H
#define __IOCTL_32H

#define ioc(x,y)       ((x<<8)|y)

#define FIONREAD        ioc('f', 127)
#define FIONBIO         ioc('f', 126)
#define FIOASYNC        ioc('f', 125)
#define FIOTCPCKSUM     ioc('f', 128)
#define FIONSTATUS      ioc('f', 120)
#define FIONURG         ioc('f', 121)

#define SIOCSHIWAT      ioc('s',  0)
#define SIOCGHIWAT      ioc('s',  1)
#define SIOCSLOWAT      ioc('s',  2)
#define SIOCGLOWAT      ioc('s',  3)
#define SIOCATMARK      ioc('s',  7)
#define SIOCSPGRP       ioc('s',  8)
#define SIOCGPGRP       ioc('s',  9)
#define SIOCSHOSTID     ioc('s', 10)

#define SIOCADDRT       ioc('r', 10)
#define SIOCDELRT       ioc('r', 11)
#define SIOMETRIC1RT    ioc('r', 12)
#define SIOMETRIC2RT    ioc('r', 13)
#define SIOMETRIC3RT    ioc('r', 14)
#define SIOMETRIC4RT    ioc('r', 15)

#define SIOCREGADDNET   ioc('r', 12)
#define SIOCREGDELNET   ioc('r', 13)
#define SIOCREGROUTES   ioc('r', 14)
#define SIOCFLUSHROUTES ioc('r', 15)

#define SIOCSIFADDR     ioc('i', 12)
#define SIOCGIFADDR     ioc('i',13)
#define SIOCSIFDSTADDR  ioc('i', 14)
#define SIOCGIFDSTADDR  ioc('i',15)
#define SIOCSIFFLAGS    ioc('i', 16)
#define SIOCGIFFLAGS    ioc('i',17)
#define SIOCGIFBRDADDR  ioc('i',18)
#define SIOCSIFBRDADDR  ioc('i',19)
#define SIOCGIFCONF     ioc('i',20)
#define SIOCGIFNETMASK  ioc('i',21)
#define SIOCSIFNETMASK  ioc('i',22)
#define SIOCGIFMETRIC   ioc('i',23)
#define SIOCSIFMETRIC   ioc('i',24)
#define SIOCSIFSETSIG   ioc('i', 25)
#define SIOCSIFCLRSIG   ioc('i', 26)
#define SIOCSIFBRD      ioc('i', 27) /* SINGLE-rt bcst. using old # for bkw cmpt */
#define SIOCSIFALLRTB   ioc('i', 63) /* added to configure all-route broadcst */

#define SIOCGIFLOAD     ioc('i', 27)
#define SIOCSIFFILTERSRC ioc('i', 28)
#define SIOCGIFFILTERSRC ioc('i',29)

#define SIOCSARP        ioc('i', 30)
#define SIOCGARP        ioc('i', 31)
#define SIOCDARP        ioc('i', 32)
#define SIOCSIFSNMPSIG  ioc('i', 33)
#define SIOCSIFSNMPCLR  ioc('i', 34)
#define SIOCSIFSNMPCRC  ioc('i', 35)
#define SIOCSIFPRIORITY ioc('i', 36)
#define SIOCGIFPRIORITY ioc('i', 37)
#define SIOCSIFFILTERDST ioc('i', 38)
#define SIOCGIFFILTERDST ioc('i',39)
#define SIOCSIF802_3     ioc('i',40)
#define SIOCSIFNO802_3   ioc('i',41)
#define SIOCSIFNOREDIR   ioc('i',42)
#define SIOCSIFYESREDIR  ioc('i',43)

#define SIOCSIFMTU      ioc('i',45)
#define SIOCSIFFDDI     ioc('i',46)
#define SIOCSIFNOFDDI   ioc('i',47)
#define SIOCSRDBRD      ioc('i',48)
#define SIOCSARP_TR     ioc('i',49)
#define SIOCGARP_TR     ioc('i',50)

/* multicast ioctls */
#define SIOCADDMULTI    ioc('i', 51)    /* add m'cast addr */
#define SIOCDELMULTI    ioc('i', 52)    /* del m'cast addr */
#define SIOCMULTISBC    ioc('i', 61)   /* use broadcast to send IP multicast*/
#define SIOCMULTISFA    ioc('i', 62)   /* use functional addr to send IP multicast*/


#ifdef SLBOOTP
#define SIOCGUNIT       ioc('i', 70)  /* Used to retreive unit number on */
                                      /* serial interface */
#endif

#define SIOCSIFSPIPE     ioc('i',71)   /* used to set pipe size on interface */
                                    /* this is used as tcp send buffer size */
#define SIOCSIFRPIPE     ioc('i',72)   /* used to set pipe size on interface */
                                    /* this is used as tcp recv buffer size */
#define SIOCSIFTCPSEG   ioc('i',73)  /* set the TCP segment size on interface*/
#define SIOCSIFUSE576   ioc('i',74)  /* enable/disable the automatic change of mss to 576 */
                                     /* if going through a router */
#define SIOCGIFVALID    ioc('i',75)   /* to check if the interface is Valid or not */
                                      /* sk June 14 1995 */
#define SIOCGIFBOUND    ioc('i',76)  /* ioctl to return bound/shld bind ifs */
/* Interface Tracing Support */
#define SIOCGIFEFLAGS   ioc('i',150)
#define SIOCSIFEFLAGS   ioc('i',151)
#define SIOCGIFTRACE    ioc('i',152)
#define SIOCSIFTRACE    ioc('i',153)

#ifdef SLSTATS
#define SIOCSSTAT      ioc('i',154)
#define SIOCGSTAT      ioc('i',155)
#endif

/* NETSTAT stuff */
#define SIOSTATMBUF     ioc('n',40)
#define SIOSTATTCP      ioc('n',41)
#define SIOSTATUDP      ioc('n',42)
#define SIOSTATIP       ioc('n',43)
#define SIOSTATSO       ioc('n',44)
#define SIOSTATRT       ioc('n',45)
#define SIOFLUSHRT      ioc('n',46)
#define SIOSTATICMP     ioc('n',47)
#define SIOSTATIF       ioc('n',48)
#define SIOSTATAT       ioc('n',49)
#define SIOSTATARP      ioc('n',50)
#define SIOSTATIF42     ioc('n',51)
#endif
