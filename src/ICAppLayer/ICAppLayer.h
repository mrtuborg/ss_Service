#include <pthread.h> 
//#include "../valuesRegistry/valuesRegistry.h"
//#include "../valuesRegistry/valueTrig.h"
//#include "../valuesRegistry/trigList.h"

typedef struct serviceState {
    BYTE linked:1;
    BYTE reserved:3;
    BYTE inprocess:1;
    BYTE mode_emergency:1;
    BYTE mode_manual:1;
    BYTE mode_auto:1;
} __attribute__((packed)) serviceState;

typedef struct stateVector_type {
    BYTE reserved;
    serviceState state;
    errType lastResult;
    BYTE lastFuncId;
}  __attribute__ ((packed)) stateVector_type;

class ICAppLayer 
{
    BYTE AppTerminated;  // 1 - only exit; 2 - exit with reboot
    FunctionNode *Functions[100];
    BYTE func_quantity;
    BYTE ifCount;
    
     pthread_t	listenerThread;
     pthread_t	senderThread;
     pthread_t	equipListenThread;

    WORD cpListenerPortNum; // connect to control panel
    
    udp_port *equip_listen;//, *equip_sending;
    //buffer* equip_recvBuffer;
    //trigList triggers;
    //valuesRegistry registry;

    errType Decode_message(BYTE* dataBlock, DWORD length, rcsCmd *ss_cmd);
    errType execMessage(rcsCmd* ss_cmd);
    errType prepare_FuncResult(rcsCmd* in_cmd, rcsCmd* out_cmd);
    errType sendResult(sockaddr_in* sin, rcsCmd* ss_cmd);
    
    stateVector_type ServiceState;
    
    public:
    
    ssBuffer* ICAppLayer_recvBuffer;
    ssBuffer* ICAppLayer_sendBuffer;
    
	 ICAppLayer(WORD portNum);
	~ICAppLayer();
	
	errType CreateNewFunction(FunctionNode* func);
	errType DeleteFunction(BYTE id);
	
	errType encodeBlock(rcsCmd*, BYTE**);

	errType StartListening();
	errType StopListening();
	
	errType equip_reading_event();
	errType equip_read_data(BYTE*, size_t *);
	errType checkTriggers();
	
	errType ProcessMessages();
	
	WORD getListenerPortNum();
	BYTE terminated(); // 1 - only exit; 2 - exit with reboot
	void terminate(BYTE mode=1);
	stateVector_type getStateVector();
};

extern ICAppLayer* app;
