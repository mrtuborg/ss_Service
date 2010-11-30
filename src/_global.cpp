#include <netinet/in.h>
#include <queue>
#include "../../rcsLib/ortsTypes/ortsTypes.h"
#include "buffer/ssBuffer.h"
#include "../../udp_port/udp_port.h"
#include "../../rcsLib/rcsCmd/rcsCmd.h"
#include "ICAppLayer/FunctionNode/param_desc.h"
#include "ICAppLayer/FunctionNode/FunctionNode.h"
#include "ICAppLayer/ICAppLayer.h"
int verbose_level=0;
bool listen_mode=false;
bool AppTerminated=false;
bool awaitingPattern_mode=false;
bool pattern_found=false;

char eq_ip_addr[255]={0};
char dataFile[255]={0};
char if_name[255]={0};
char patternFile[255]={0};
char reactionFile[255]={0};

WORD wUdp=0;
WORD eq_udp_listen_port=0;
WORD eq_udp_sending_port=0;
in_addr equipAddr;
