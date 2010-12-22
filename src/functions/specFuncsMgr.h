/**
 * @file
 * @author Vladimir A. Nosenko (nosenko@ieee.org)
 * @date   December, 2010
 * @brief  Class specFuncsMgr interface header
 * @details
 *      Copyright (c) 2010 Vladimir A.Nosenko.
 *
 *      The license and distribution terms for this file may be
 *      found in the file LICENSE in this distribution
 *
 *
 */

/**
 * @class specFuncsMgr
 * @brief special functions set manager.
 * @details implementation in /funcs/0/0_SpecFuncs.cpp ... /funcs/9/9_CommonFuncs.cpp (set of function depends on target)
 */
class specFuncsMgr
{
    srvAppLayer* appLayer;
    
    public:
     specFuncsMgr(srvAppLayer *appl); ///< Links special functions set with srvAppLayer
    ~specFuncsMgr();

     errType startSpecFuncs();    ///< Declare special functions set
     errType stopSpecFuncs();
};
