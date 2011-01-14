/**
 * @file
 * @author Vladimir A. Nosenko (nosenko@ieee.org)
 * @date   December, 2010
 * @brief  Global environment interface header.
 * @details
 *      Copyright (c) 2010 Vladimir A.Nosenko.
 *
 *      The license and distribution terms for this file may be
 *      found in the file LICENSE in this distribution
 *
 *
 */

extern bool AppTerminated;
extern bool awaitingPattern_mode;
extern bool pattern_found;
extern int verbose_level;
extern bool listen_mode;
extern char eq_ip_addr[255];
extern char dataFile[255];
extern char if_name[255];
extern char patternFile[255];
extern char reactionFile[255];

extern WORD eq_udp_listen_port;
extern WORD eq_udp_sending_port;
extern in_addr equipAddr;
extern WORD wUdp;

//#define CLIENT_IP "127.0.0.1"
//extern srvAppLayer *app;
