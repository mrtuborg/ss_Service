/**
 * @file
 * @author Vladimir A. Nosenko (nosenko@ieee.org)
 * @date   December, 2010
 * @brief  Global environment.
 * @details
 *      Copyright (c) 2010 Vladimir A.Nosenko.
 *
 *      The license and distribution terms for this file may be
 *      found in the file LICENSE in this distribution
 *
 *
 */

#include <netinet/in.h>
#include <queue>
#include "../../rcsLib/ortsTypes/ortsTypes.h"
#include "buffer/ssBuffer.h"
#include "../../udp_port/udp_port.h"
#include "../../rcsLib/rcsCmd/rcsCmd.h"
#include "srvAppLayer/FunctionNode/param_desc.h"
#include "srvAppLayer/FunctionNode/FunctionNode.h"
#include "srvAppLayer/srvAppLayer.h"


int verbose_level=0; ///< Debug detail level printing.


bool listen_mode=false;          ///< needless variable.@todo Need to be deleted.
bool awaitingPattern_mode=false; ///< needless variable.@todo Need to be deleted.
bool pattern_found=false;        ///< needless variable.@todo Need to be deleted.
char patternFile[255]={0};       ///< needless variable.@todo Need to be deleted.
char reactionFile[255]={0};      ///< needless variable.@todo Need to be deleted.
char dataFile[255]={0};          ///< needless variable.@todo Need to be deleted.
char if_name[255]={0};           ///< needless variable.@todo Need to be deleted.


bool AppTerminated=false; ///< Programm termination signal.
                          ///  All processes need to finish own job.
char eq_ip_addr[255]={0}; ///< Equipment IP address.


WORD wUdp=0;               ///< Server udp port number for communicate with client.
WORD eq_udp_listen_port=0; ///< Server udp port number for listen an equipment.
WORD eq_udp_sending_port=0;///< Server udp port number for sending into equipment.

in_addr equipAddr;         ///< Storage for in_addr of equipment
