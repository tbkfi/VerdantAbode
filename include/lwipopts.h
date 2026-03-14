// https://github.com/raspberrypi/pico-examples/tree/master/pico_w/wifi/http_client
// https://www.nongnu.org/lwip/2_0_x/sockets_8h.html
//
// note: Honestly, I have no idea how many of these are neccessary.
//       Some are more obvious than others, and some were copied
//       from the internet with a prayer.
// 
#ifndef _LWIPOPTS_H
#define _LWIPOPTS_H


#define LWIP_TIMEVAL_PRIVATE        0
#define LWIP_COMPAT_SOCKETS         1
#define LWIP_POSIX_SOCKETS_IO_NAMES 1

#define NO_SYS                      0 // Bare or Actual OS?
#define LWIP_SOCKET                 1
#define LWIP_NETCONN                1
#define SYS_LIGHTWEIGHT_PROT        1

#define MEM_ALIGNMENT               4
#define MEM_SIZE                    (16 * 1024)
#define MEMP_NUM_PBUF               10
#define MEMP_NUM_TCP_PCB            8

#define TCPIP_THREAD_STACKSIZE      2048
#define TCPIP_THREAD_PRIO           2
#define DEFAULT_THREAD_STACKSIZE    2048

#define LWIP_CHKSUM_ALGORITHM       3
#define LWIP_CHECKSUM_ON_COPY       1

#define TCPIP_MBOX_SIZE             8
#define DEFAULT_TCP_RECVMBOX_SIZE   8
#define DEFAULT_UDP_RECVMBOX_SIZE   8
#define DEFAULT_RAW_RECVMBOX_SIZE   8
#define DEFAULT_ACCEPTMBOX_SIZE     8

#define LWIP_IPV4                   1 // IPv4
#define LWIP_DHCP                   1 // DHCP
#define LWIP_ARP                    1 // ARP
#define LWIP_ETHERNET               1

#define LWIP_DNS                    1 // DNS
#define LWIP_HTTPC                  1 // HTTP Client

#endif
