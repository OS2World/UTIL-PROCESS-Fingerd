/************************************************************************
 *                                                                      *
 *  FINGERD.C  - OS/2 Finger Server
 *                                                                      *
 *  Copyright (C) 1996 by International Business Machines Corporation.  *
 *                                All Rights Reserved.                  *
 *                                                                      *
 *                                - Robert A. Rose 01/31/97             *
 *                                                                      *
 ************************************************************************/
#define INCL_DOSPROCESS
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define IBUFSIZE 8192
#define OBUFSIZE 32768
#define MAXSRVTHRD 20
#define STACK_SIZE 32768

struct sockaddr_in server;         /* server address information      */
int s;                             /* socket for accepting connections*/
char host[256];
char killpassword[32];             /* password for kill               */
int kdigit=0;
char shutpassword[32];             /* shutdown password               */
int sdigit=0;
int badhacks=0;
int ourshut=0;

ULONG nthrds=4L;

int f_help=1,
    f_logging=1,
    f_dlls=1,
    f_sems=1,
    f_share=1,
    f_aproc=1,
    f_sproc=1,
    f_kill=0,
    f_shutdown=0,
    f_finger=1;


char *
yesorno(int flg)
{
     return(flg ? "Yes" : "No");
}

void
prfvers(char *buf)
{
    buf+=sprintf(buf,"Finger for OS/2 version 1.0\n");
    buf+=sprintf(buf,"Written by Robert A. Rose (robert_rose@vnet.ibm.com)\n");
    buf+=sprintf(buf,"Compiled at %s %s\n",__DATE__,__TIME__);
    buf+=sprintf(buf,"Current Settings:\n");
    buf+=sprintf(buf,"Threads = %2.2d                  Help=%s\n",nthrds,yesorno(f_help));
    buf+=sprintf(buf,"KillPass=%3.3s                  DLLs=%s\n",yesorno(kdigit),yesorno(f_dlls));
    buf+=sprintf(buf,"ShutPass=%3.3s                  Sems=%s\n",yesorno(sdigit),yesorno(f_sems));
    buf+=sprintf(buf,"                             Share=%s\n",yesorno(f_share));
    buf+=sprintf(buf,"Logging=%3.3s                AllProc=%s\n",yesorno(f_logging),yesorno(f_aproc));
    buf+=sprintf(buf,"                          SpecProc=%s\n",yesorno(f_sproc));
    buf+=sprintf(buf,"                              KILL=%s\n",yesorno(f_kill));
    buf+=sprintf(buf,"                          SHUTDOWN=%s\n",yesorno(f_shutdown));
    buf+=sprintf(buf,"                           .finger=%s\n",yesorno(f_finger));
}

void
prfhelp(char *buf)
{
     buf+=sprintf(buf,"Finger for OS/2 Help (version 1.0)\n");
     if (f_help)     buf+=sprintf(buf,"    finger help@<host>                   - this help text\n");
     if (f_aproc)    buf+=sprintf(buf,"    finger procs@<host>                  - list of all processes running\n");
     if (f_dlls)     buf+=sprintf(buf,"    finger dlls@<host>                   - DLLs\n");
     if (f_sems)     buf+=sprintf(buf,"    finger sems@<host>                   - Semaphores\n");
     if (f_share)    buf+=sprintf(buf,"    finger share@<host>                  - Shared memory\n");
     if (f_sproc)    buf+=sprintf(buf,"    finger proc_<pid>@<host>             - detailed process information\n");
     if (f_kill)     buf+=sprintf(buf,"    finger kill_<password>_<pid>@<host>  - kill process\n");
     if (f_shutdown) buf+=sprintf(buf,"    finger shutdown_<password>@<host>    - shutdown system\n");
     if (f_finger)   buf+=sprintf(buf,"    finger <host>@<host>                 - conventional finger info\n");
}

int
file2buf(char *fspec,char *buf)
{
     FILE *fp;
     int c;

     fp=fopen(fspec,"rb");
     if (fp != NULL) {
          c=fread(buf,1,OBUFSIZE-1,fp);
          buf[OBUFSIZE-1]='\0';
     }
     fclose(fp);
     return(c);
}

void
cmd2buf(char *fspec,char *cmd,char *buf)
{
     char fcmd[256];

     sprintf(fcmd,"%s >%s",cmd,fspec);
     system(fcmd);
     file2buf(fspec,buf);
}

void
addhelp(char *buf,int tid)
{
     char b2[256];

     sprintf(b2,"\nfinger help@%s for full parameters. (%x)\n",host,tid);
     strcat(buf,b2);
}

void __far
w_finger(ULONG argv)
{
     unsigned long tid=argv;
     struct sockaddr_in client;         /* client address information      */
     struct hostent *remhst;            /* remote host information         */
     int ns;                            /* socket connected to client      */
     int namelen;                       /* length of client name           */
     char *ibuf,*obuf;
     char tfile[20];
     int rv;
int bleft;
char *wlk;

     sprintf(tfile,"TMP%04.4d.$$$",tid);     //temporary filespec

     ibuf=malloc(IBUFSIZE);               //buffer for I/O
     obuf=malloc(OBUFSIZE);               //buffer for I/O

     while (1) {
          memset(ibuf,0,IBUFSIZE);
          memset(obuf,0,OBUFSIZE);
          /*
           * Listen for connections. Specify the backlog as 1.
           */
          if (listen(s,1) != 0) {
               printf("Thread %d - Error on listen()\n",tid);
               free(ibuf);
               free(obuf);
               _endthread();
          }
          /*
           * Accept a connection.
           */
          namelen=sizeof(client);
          if ((ns=accept(s,(struct sockaddr *)&client,&namelen)) == -1) {
               printf("Thread %d - Error on accept()\n",tid);
               free(ibuf);
               free(obuf);
               _endthread();
          }

          /*
           * Receive the message on the newly connected socket.
           */
          if ((rv=recv(ns,ibuf,IBUFSIZE,0)) == -1) {
               printf("Thread %d - Error on recv()\n",tid);
               free(ibuf);
               free(obuf);
               _endthread();
          }
          if (rv < IBUFSIZE) ibuf[rv]='\0';  else ibuf[IBUFSIZE-1]='\0';

          remhst=gethostbyaddr(&client.sin_addr.s_addr,4,AF_INET);
          if (f_logging) {
              FILE *fp;

               printf("#%2.2d From: %s (%s)\n",tid,remhst->h_name,inet_ntoa(client.sin_addr));
               printf("   >%s\n",ibuf);
               fp=fopen("FINGER.LOG","at");
               fprintf(fp,"#%2.2d From: %s (%s)\n",tid,remhst->h_name,inet_ntoa(client.sin_addr));
               fprintf(fp,"   >%s\n",ibuf);
               fclose(fp);
          }
          if (f_help && strnicmp("help",ibuf,4) == 0) {
               prfhelp(obuf);
          }
          if (f_help && strnicmp("version",ibuf,4) == 0) {
               prfvers(obuf);
          }
          else if (f_dlls && strnicmp("dlls",ibuf,4) == 0) {
               cmd2buf(tfile,"pstat /L",obuf);
          }
          else if (f_sems && strnicmp("sems",ibuf,4) == 0) {
               cmd2buf(tfile,"pstat /S",obuf);
          }
          else if (f_share && strnicmp("share",ibuf,4) == 0) {
               cmd2buf(tfile,"pstat /M",obuf);
          }
          else if (f_aproc && strnicmp("procs",ibuf,5) == 0) {
               cmd2buf(tfile,"pstat /C",obuf);
          }
          else if (f_finger && strlen(ibuf) <= 2) {
               file2buf(".finger",obuf);
          }
          else if (f_sproc && strnicmp("proc_",ibuf,5) == 0) {
               char buf2[80];
               int pid;

               sscanf(ibuf+5,"%x",&pid);
               sprintf(buf2,"pstat /P:%x",pid);
               cmd2buf(tfile,buf2,obuf);
          }
          else if (f_kill && strnicmp("kill_",ibuf,5) == 0) {
               if (strnicmp(killpassword,ibuf+5,kdigit) == 0) {
                    int pid,rc;
                    char *wlk=obuf;

                    sscanf(ibuf+5+kdigit+1,"%x",&pid);
                    wlk+=sprintf(wlk,"Killing Process %04.4x...\n",pid);
                    rc=DosKillProcess(1,pid);
                    if (rc) {
                         wlk+=sprintf(wlk,"Uh-oh, return code %d (decimal) from DosKillProcess!\n",rc);
                    }
                    else {
                         wlk+=sprintf(wlk,"Direct hit tail-gunner Beavis!\n");
                    }
               }
               else {
                    char *wlk=obuf;
                    wlk+=sprintf(wlk,"*********************************\n");
                    wlk+=sprintf(wlk,"*   INVALID PROCKILL ATTEMPT!   *\n");
                    wlk+=sprintf(wlk,"*                               *\n");
                    wlk+=sprintf(wlk,"* This attempt has been logged! *\n");
                    wlk+=sprintf(wlk,"*                               *\n");
                    wlk+=sprintf(wlk,"* Origin: %16.16s      *\n",inet_ntoa(client.sin_addr));
                    wlk+=sprintf(wlk,"*         %21.21s *\n",remhst->h_name);
                    wlk+=sprintf(wlk,"*********************************\n");
                    badhacks++;
               }
          }
          else if (!f_kill && strnicmp("kill_",ibuf,5) == 0) {
               sprintf(obuf,"Feature not enabled.\n");
          }
          else if (f_shutdown && strnicmp("shutdown_",ibuf,9) == 0) {
               if (strnicmp(shutpassword,ibuf+9,sdigit) == 0) {
                    int rc;
                    char *wlk=obuf;

                    if (ourshut) {
                         wlk+=sprintf(wlk,"Forcing shutdown...\n");
                         rc=DosShutdown(0L);
                         if (rc) {
                              wlk+=sprintf(wlk,"Dude, return code %04.4x from DosShutdown!\n",rc);
                         }
                         else {
                              wlk+=sprintf(wlk,"Good night!\n");
                         }
                    }
                    else {
                         wlk+=sprintf(wlk,"Flushing buffers and cache...\n");
                         rc=DosShutdown(1L);
                         ourshut=1;
                         if (rc) {
                              wlk+=sprintf(wlk,"Uh-oh, return code %04.4x from DosShutdown!\n",rc);
                         }
                         else {
                              wlk+=sprintf(wlk,"Uh, Beavis, are you sure?\n");
                         }
                    }
               }
               else {
                    char *wlk=obuf;
                    wlk+=sprintf(wlk,"*********************************\n");
                    wlk+=sprintf(wlk,"*   INVALID SHUTDOWN ATTEMPT!   *\n");
                    wlk+=sprintf(wlk,"*                               *\n");
                    wlk+=sprintf(wlk,"* This attempt has been logged! *\n");
                    wlk+=sprintf(wlk,"*                               *\n");
                    wlk+=sprintf(wlk,"* Origin: %16.16s      *\n",inet_ntoa(client.sin_addr));
                    wlk+=sprintf(wlk,"*         %21.21s *\n",remhst->h_name);
                    wlk+=sprintf(wlk,"*********************************\n");
                    badhacks++;
               }
          }
          else if (!f_shutdown && strnicmp("shutdown_",ibuf,9) == 0) {
               sprintf(obuf,"Feature not enabled.\n");
          }
          else if (f_finger && strnicmp(host,ibuf,strlen(host)) == 0) {
               file2buf(".finger",obuf);
          }
          if (f_help) addhelp(obuf,tid);
          /*
           * Send the message back to the client.
           */
          bleft=strlen(obuf);
          wlk=obuf;
          while (bleft) {
             if (bleft > 2048) {
                if (send(ns,wlk,2048,0) < 0) {
                     printf("Thread %d - Error on send()\n",tid);
                     free(ibuf);
                     free(obuf);
                     _endthread();
                }
                bleft-=2048;
                wlk+=2048;
             }
             else {
                if (send(ns,wlk,bleft,0) < 0) {
                     printf("Thread %d - Error on send()\n",tid);
                     free(ibuf);
                     free(obuf);
                     _endthread();
                }
                bleft=0;
             }
          }
          soclose(ns);
     }
     free(ibuf);
     free(obuf);
     _endthread();
}

void __far
hackalert(void __far *argv)
{
     int oldf_kill,oldf_shutdown;

     oldf_kill=f_kill;
     oldf_shutdown=f_shutdown;
     f_kill=f_shutdown=0;
     printf("************ Too Many Hack Attempts ************\n");
     printf("**** Disabled Kill/Shutdown for 10 minutes! ****\n");
     DosSleep(10*60*1000);
     f_kill=oldf_kill;
     f_shutdown=oldf_shutdown;
     printf("*********** Resetting Kill/Shutdown ************\n");
     _endthread();
}

int
readflg(char *flg)
{
     if (strnicmp(flg,"Y",1) == 0) {
          return(1);
     }
     return(0);
}

int
main(int argc,char *argv[])
{
/*     char  *stacks[MAXSRVTHRD+1];
     ULONG tid[MAXSRVTHRD+1]; */
     int tid[MAXSRVTHRD+1];
     char *stacks[MAXSRVTHRD+1];
     ULONG i;
     char *hackstack;

     hackstack=malloc(STACK_SIZE);

     printf("FINGERD - OS/2 TCP/IP Finger Protocol Server\n");
     printf("Version 1.0 by Robert A. Rose (robert_rose@vnet.ibm.com)\n\n");

     for (i=1 ; i < argc ; i++) {
          if (strnicmp(argv[i],"/?",2) == 0) {
               printf("Command line options:\n");
               printf("    KILL=password           SHUTDOWN=password\n");
               printf("     LOG=[Y|n]                FINGER=[Y|n]\n");
               printf("    DLLS=[Y|n]                  SEMS=[Y|n]\n");
               printf("   SHARE=[Y|n]               ALLPROC=[Y|n]\n");
               printf("SPECPROC=[Y|n]                THREAD={1-%d}\n",MAXSRVTHRD);
               printf("    HELP=[Y|n]\n");
               return(0);
          }
          else if (strnicmp(argv[i],"KILL=",5) == 0) {
               kdigit=strlen(argv[i]+5);
               memmove(killpassword,argv[i]+5,kdigit);
               f_kill=1;
          }
          else if (strnicmp(argv[i],"SHUTDOWN=",9) == 0) {
               sdigit=strlen(argv[i]+9);
               memmove(shutpassword,argv[i]+9,sdigit);
               f_shutdown=1;
          }
          else if (strnicmp(argv[i],"LOG=",4) == 0) {
               f_logging=readflg(argv[i]+4);
          }
          else if (strnicmp(argv[i],"HELP=",4) == 0) {
               f_help=readflg(argv[i]+5);
          }
          else if (strnicmp(argv[i],"FINGER=",7) == 0) {
               f_finger=readflg(argv[i]+7);
          }
          else if (strnicmp(argv[i],"DLLS=",5) == 0) {
               f_dlls=readflg(argv[i]+5);
          }
          else if (strnicmp(argv[i],"SEMS=",5) == 0) {
               f_sems=readflg(argv[i]+5);
          }
          else if (strnicmp(argv[i],"SHARE=",6) == 0) {
               f_share=readflg(argv[i]+6);
          }
          else if (strnicmp(argv[i],"ALLPROC=",8) == 0) {
               f_aproc=readflg(argv[i]+8);
          }
          else if (strnicmp(argv[i],"SPECPROC=",9) == 0) {
               f_sproc=readflg(argv[i]+9);
          }
          else if (strnicmp(argv[i],"THREAD=",7) == 0) {
               sscanf(argv[i]+7,"%d",&nthrds);
               if (nthrds < 1L) nthrds=1L;
               if (nthrds > MAXSRVTHRD) nthrds=MAXSRVTHRD;
          }
     }

     if (kdigit == 0) {
          printf("**** Kill password NULL, turning off KILL! ****\n");
          f_kill=0;
     }
     else if (kdigit < 16) {
          printf("**** Kill password too short, turning off KILL! ****\n");
          f_kill=0;
     }
     if (sdigit == 0) {
          printf("**** Shutdown password NULL, turning off SHUTDOWN! ****\n");
          f_shutdown=0;
     }
     else if (sdigit < 16) {
          printf("**** Shutdown password too short, turning off SHUTDOWN! ****\n");
          f_shutdown=0;
     }

     printf("Threads = %2.2d                  Help=%s\n",nthrds,yesorno(f_help));
     printf("KillPass=%3.3s                  DLLs=%s\n",yesorno(kdigit),yesorno(f_dlls));
     printf("ShutPass=%3.3s                  Sems=%s\n",yesorno(sdigit),yesorno(f_sems));
     printf("                             Share=%s\n",yesorno(f_share));
     printf("Logging=%3.3s                AllProc=%s\n",yesorno(f_logging),yesorno(f_aproc));
     printf("                          SpecProc=%s\n",yesorno(f_sproc));
     printf("                              KILL=%s\n",yesorno(f_kill));
     printf("                          SHUTDOWN=%s\n",yesorno(f_shutdown));
     printf("                           .finger=%s\n",yesorno(f_finger));

     sock_init();

     gethostname(host,255);

     /*
      * Get a socket for accepting connections.
      */
     if ((s=socket(PF_INET,SOCK_STREAM,0)) < 0) {
          printf("Error on socket()\n");
          exit(5);
     }

     /*
      * Bind the socket to the server address.
      */
     server.sin_family     =AF_INET;
     server.sin_port       =htons(79);
     server.sin_addr.s_addr=INADDR_ANY;

     if (bind(s,(struct sockaddr *)&server,sizeof(server)) < 0) {
          printf("Error on bind()\n");
          exit(5);
     }

     printf("Starting %d threads...\n",nthrds);
     for (i=0 ; i < nthrds ; i++) {
          stacks[i]=malloc(STACK_SIZE);
     /*     tid[i]=_beginthread(w_finger,stacks[i],STACK_SIZE,(ULONG)i); */
          tid[i]=_beginthread(w_finger,malloc(8192),8192,(ULONG)i);
     //   printf("Thread %d - tid=%04.4x\n",i,tid[i]);
     }

     while (!kbhit()) {
          DosSleep(1000);
          if (badhacks > 6) {
               badhacks=0;
               _beginthread(hackalert,hackstack,STACK_SIZE,0);
          }
     }

     soclose(s);
     for (i=0 ; i < nthrds ; i++) {
          printf("Killing TID %04.4x...",tid[i]);
          DosKillThread(tid[i]);
          DosWaitThread((PTID)&tid[i],(ULONG)DCWW_WAIT);
          DosSleep(1000);
     }
     return(0);
}

