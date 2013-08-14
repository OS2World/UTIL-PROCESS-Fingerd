#ifndef __NERRNO_32H
#define __NERRNO_32H

/*
 * The re-defination of error constants are necessary to avoid conflict with
 * standard IBM C Set/2 V1.0 error constants.
 *
 * All OS/2 SOCKET API error constants are biased by SOCBASEERR from the "normal"
 *
 */

#define SOCBASEERR              10000

/*
 * OS/2 SOCKET API definitions of regular Microsoft C 6.0 error constants
 */

#define SOCEPERM                (SOCBASEERR+1)             /* Not owner */
#define SOCESRCH                (SOCBASEERR+3)             /* No such process */
#define SOCEINTR                (SOCBASEERR+4)             /* Interrupted system call */
#define SOCENXIO                (SOCBASEERR+6)             /* No such device or address */
#define SOCEBADF                (SOCBASEERR+9)             /* Bad file number */
#define SOCEACCES               (SOCBASEERR+13)            /* Permission denied */
#define SOCEFAULT               (SOCBASEERR+14)            /* Bad address */
#define SOCEINVAL               (SOCBASEERR+22)            /* Invalid argument */
#define SOCEMFILE               (SOCBASEERR+24)            /* Too many open files */
#define SOCEPIPE                (SOCBASEERR+32)            /* Broken pipe */

#define SOCEOS2ERR              (SOCBASEERR+100)            /* OS/2 Error */

/*
 * OS/2 SOCKET API definitions of regular BSD error constants
 */

#define SOCEWOULDBLOCK          (SOCBASEERR+35)            /* Operation would block */
#define SOCEINPROGRESS          (SOCBASEERR+36)            /* Operation now in progress */
#define SOCEALREADY             (SOCBASEERR+37)            /* Operation already in progress */
#define SOCENOTSOCK             (SOCBASEERR+38)            /* Socket operation on non-socket */
#define SOCEDESTADDRREQ         (SOCBASEERR+39)            /* Destination address required */
#define SOCEMSGSIZE             (SOCBASEERR+40)            /* Message too long */
#define SOCEPROTOTYPE           (SOCBASEERR+41)            /* Protocol wrong type for socket */
#define SOCENOPROTOOPT          (SOCBASEERR+42)            /* Protocol not available */
#define SOCEPROTONOSUPPORT      (SOCBASEERR+43)            /* Protocol not supported */
#define SOCESOCKTNOSUPPORT      (SOCBASEERR+44)            /* Socket type not supported */
#define SOCEOPNOTSUPP           (SOCBASEERR+45)            /* Operation not supported on socket */
#define SOCEPFNOSUPPORT         (SOCBASEERR+46)            /* Protocol family not supported */
#define SOCEAFNOSUPPORT         (SOCBASEERR+47)            /* Address family not supported by protocol family */
#define SOCEADDRINUSE           (SOCBASEERR+48)            /* Address already in use */
#define SOCEADDRNOTAVAIL        (SOCBASEERR+49)            /* Can't assign requested address */
#define SOCENETDOWN             (SOCBASEERR+50)            /* Network is down */
#define SOCENETUNREACH          (SOCBASEERR+51)            /* Network is unreachable */
#define SOCENETRESET            (SOCBASEERR+52)            /* Network dropped connection on reset */
#define SOCECONNABORTED         (SOCBASEERR+53)            /* Software caused connection abort */
#define SOCECONNRESET           (SOCBASEERR+54)            /* Connection reset by peer */
#define SOCENOBUFS              (SOCBASEERR+55)            /* No buffer space available */
#define SOCEISCONN              (SOCBASEERR+56)            /* Socket is already connected */
#define SOCENOTCONN             (SOCBASEERR+57)            /* Socket is not connected */
#define SOCESHUTDOWN            (SOCBASEERR+58)            /* Can't send after socket shutdown */
#define SOCETOOMANYREFS         (SOCBASEERR+59)            /* Too many references: can't splice */
#define SOCETIMEDOUT            (SOCBASEERR+60)            /* Connection timed out */
#define SOCECONNREFUSED         (SOCBASEERR+61)            /* Connection refused */
#define SOCELOOP                (SOCBASEERR+62)            /* Too many levels of symbolic links */
#define SOCENAMETOOLONG         (SOCBASEERR+63)            /* File name too long */
#define SOCEHOSTDOWN            (SOCBASEERR+64)            /* Host is down */
#define SOCEHOSTUNREACH         (SOCBASEERR+65)            /* No route to host */
#define SOCENOTEMPTY            (SOCBASEERR+66)            /* Directory not empty */

/*
 * OS/2 SOCKET API errors redefined as regular BSD error constants
 */

#define EWOULDBLOCK             SOCEWOULDBLOCK
#define EINPROGRESS             SOCEINPROGRESS
#define EALREADY                SOCEALREADY
#define ENOTSOCK                SOCENOTSOCK
#define EDESTADDRREQ            SOCEDESTADDRREQ
#define EMSGSIZE                SOCEMSGSIZE
#define EPROTOTYPE              SOCEPROTOTYPE
#define ENOPROTOOPT             SOCENOPROTOOPT
#define EPROTONOSUPPORT         SOCEPROTONOSUPPORT
#define ESOCKTNOSUPPORT         SOCESOCKTNOSUPPORT
#define EOPNOTSUPP              SOCEOPNOTSUPP
#define EPFNOSUPPORT            SOCEPFNOSUPPORT
#define EAFNOSUPPORT            SOCEAFNOSUPPORT
#define EADDRINUSE              SOCEADDRINUSE
#define EADDRNOTAVAIL           SOCEADDRNOTAVAIL
#define ENETDOWN                SOCENETDOWN
#define ENETUNREACH             SOCENETUNREACH
#define ENETRESET               SOCENETRESET
#define ECONNABORTED            SOCECONNABORTED
#define ECONNRESET              SOCECONNRESET
#define ENOBUFS                 SOCENOBUFS
#define EISCONN                 SOCEISCONN
#define ENOTCONN                SOCENOTCONN
#define ESHUTDOWN               SOCESHUTDOWN
#define ETOOMANYREFS            SOCETOOMANYREFS
#define ETIMEDOUT               SOCETIMEDOUT
#define ECONNREFUSED            SOCECONNREFUSED
#define ELOOP                   SOCELOOP
#ifndef ENAMETOOLONG                                            //RAR
#define ENAMETOOLONG            SOCENAMETOOLONG
#endif                                                          //RAR
#define EHOSTDOWN               SOCEHOSTDOWN
#define EHOSTUNREACH            SOCEHOSTUNREACH
#ifndef ENOTEMPTY                                               //RAR
#define ENOTEMPTY               SOCENOTEMPTY
#endif                                                          //RAR

#endif  /* __NERRNO_32H */
