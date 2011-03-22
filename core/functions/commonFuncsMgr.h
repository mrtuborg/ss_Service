/**
 * @file
 * @author Vladimir A. Nosenko (nosenko@ieee.org)
 * @date   December, 2010
 * @brief  Class commonFuncsMgr interface header
 * @details
 *      Copyright (c) 2010 Vladimir A.Nosenko.
 *
 *      The license and distribution terms for this file may be
 *      found in the file LICENSE in this distribution
 *
 *
 */

/**
 * @class commonFuncsMgr
 * @brief common functions manager implementation (set of function independs on target)
 */

/// @todo add to commonFuncsMgr class as static method
extern void* equipListenPolling(void*);

class commonFuncsMgr
{
    SrvAppLayer* appLayer;
    
    public:
	commonFuncsMgr(SrvAppLayer *appl);
	~commonFuncsMgr();
	
	errType startCommonFuncs();
	errType stopCommonFuncs();
};
