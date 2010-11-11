#pragma pack (1)
#include <pthread.h> 

typedef struct type_StateVector {
    union execState{
	struct {
	    BYTE func_exec_result: 2;
	    BYTE func_exec_id: 6;
	} fields;
	BYTE bVal;
    } execState;
    
    union servState{
	struct {
	    BYTE     auto_mode: 1;
	    BYTE   manual_mode: 1;
	    BYTE shutdown_mode: 1;
	    BYTE  transit_mode: 1;
	
	    BYTE overflow_link_sec: 1;
	    BYTE          link_sec: 3;
	}fields;
	BYTE bVal;
    } servState;
    
    BYTE funcState1;
    BYTE funcState2;
} type_StateVector;

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
    
    errType Decode_message(BYTE* dataBlock, cmd *ss_cmd);
    errType execMessage(cmd* ss_cmd);
    errType prepare_FuncResult(cmd* in_cmd, cmd* out_cmd);
    errType sendResult(sockaddr_in* sin, cmd* ss_cmd);
    
    type_StateVector ServiceState;
    
    public:
    
    ssBuffer* ICAppLayer_recvBuffer;
    ssBuffer* ICAppLayer_sendBuffer;
    
	 ICAppLayer(WORD portNum);
	~ICAppLayer();
	
	errType CreateNewFunction(FunctionNode* func);
	errType DeleteFunction(BYTE id);
	
	errType encodeBlock(cmd*, BYTE**);

	errType StartListening();
	errType StopListening();
	
	errType equip_reading_event();
	errType equip_read_data(BYTE*, size_t *);
	
	errType ProcessMessages();
	
	WORD getListenerPortNum();
	BYTE terminated(); // 1 - only exit; 2 - exit with reboot
	void terminate(BYTE mode=1);
	type_StateVector getStateVector();
};

extern ICAppLayer* app;
