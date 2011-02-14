/**
 * @file
 * @author Vladimir A. Nosenko (nosenko@ieee.org)
 * @date   December, 2010
 * @brief  Class srvAppLayer interface header.
 * @details
 *      Copyright (c) 2010 Vladimir A.Nosenko.
 *
 *      The license and distribution terms for this file may be
 *      found in the file LICENSE in this distribution
 *
 *
 */

#include <pthread.h> 

/**
 * @struct serviceState
 * @brief \ref stateVector_type structural field.
 */
typedef struct serviceState {
    BYTE linked     :1;
    BYTE inprocess  :1;
    BYTE mode_manual:1;
    BYTE mode_auto  :1;
    BYTE reserved   :4;
} __attribute__((packed)) serviceState;

/**
 * @struct stateVector_type
 * @brief  Main vector of service base states.
 */
typedef struct stateVector_type {
    BYTE         reserved;
    serviceState state;
    errType      lastResult;
    BYTE         lastFuncId;
}  __attribute__ ((packed)) stateVector_type;

/**
 * @class srvAppLayer
 * @brief Application core layer implementaion.
 * @details This layer delegate network calls to service functions and return back functions results.
 */
class SrvAppLayer
{
    BYTE AppTerminated;  ///< Application termination process indicator
                         ///    - 0 - Application run normally
                         ///    - 1 - Applcation need to exit only
                         ///    - 2 - Application need exit with reboot

    functionNode *Functions[100]; ///< Service functions array. 100 functions at maximum.
                                  /// @todo array not the best data structure for this purposes.

    BYTE func_quantity;           ///< Counter that stores really declared functions quantity
    BYTE ifCount;                 ///< Counter of ethernet interfaces. No have an idea how to use it.
    
     pthread_t	listenerThread;   ///< Handle to client requests listening thread
     pthread_t	senderThread;     ///< Handle to client answers sending thread
     pthread_t	equipListenThread;///< Handle to equipmnent data listening thread
                                  /// @note programm not have equipment data sending thread

    WORD cpListenerPortNum; ///< settings: Udp port number to listen requests from network clients
    udp_port *equip_listen; ///< udp_port instance that associates with listening data from equipment

    errType decodeMessage(BYTE* dataBlock, DWORD length, rcsCmd *ss_cmd); ///< step 1. decode recieved message from client
    errType execMessage(rcsCmd* ss_cmd);                                  ///< step 2. send data to requested service function
    errType encodeFuncResult(rcsCmd* in_cmd, rcsCmd* out_cmd);            ///< step 3. encode function execution results for sending back to client
    errType sendResult(sockaddr_in* sin, rcsCmd* ss_cmd);                 ///< step 4. send function answer to client
    
    stateVector_type ServiceState; ///< Service state vector

    public:
    
    ssBuffer* clientsRequestsQueue;  ///< Queue that stores received requests from client
    ssBuffer* functionsAnswersQueue; ///< Queue that stores service functions answers to clients
    
	 SrvAppLayer(WORD portNum);
	~SrvAppLayer();
	
	errType CreateNewFunction(functionNode* func);
	errType DeleteFunction(BYTE id);
	
	errType encodeBlock(rcsCmd*, BYTE**);

	errType StartListening();
	errType StopListening();
	
	errType equip_reading_event(DWORD timeOut_sec=0, DWORD timeOut_ms=0);
	errType equip_read_data(BYTE*, size_t *);

	errType processMessages();
	errType processInMessages(sockaddr_in*, rcsCmd*);
	errType processOutMessages(sockaddr_in*, rcsCmd*, errType);
	
	WORD getListenerPortNum();
	BYTE terminated(); // 1 - only exit; 2 - exit with reboot
	void terminate(BYTE mode=1);
	stateVector_type getStateVector();

	BYTE serviceMode(); // 0 - local, 1- remote
	errType setServiceMode(BYTE mode); //0 - local, 1- remote
};

extern SrvAppLayer* app;        ///< One global instance per application
