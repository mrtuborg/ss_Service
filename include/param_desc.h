/**
 * @file
 * @author Vladimir A. Nosenko (nosenko@ieee.org)
 * @date   December, 2010
 * @brief  Class param_desc interface header.
 * @details
 *      Copyright (c) 2010 Vladimir A.Nosenko.
 *
 *      The license and distribution terms for this file may be
 *      found in the file LICENSE in this distribution
 *
 *
 */

/*****************************************************************************//**
 * @class param_desc
 * @brief parameter description
 * @details parameter description uses in function node parameter declaration
 *********************************************************************************/
class param_desc
{
    void *param;        ///< pointer to parameter value
    char* param_name;   ///< parameter string name
    WORD _length;       ///< length in bytes of storing parameter value
    OrtsType _type;
    
    bool _isVector;     ///< storage value is a vector (one-dimensional array) of simplified values types
    
    public:
        param_desc(OrtsType type, WORD len);
        param_desc(OrtsType type);
       ~param_desc();
       
        errType resize(WORD new_size);
       
       void* value();
       WORD length();
       OrtsType type();
       
       bool isVector();
       
       errType setParam(void* param_val);
       errType setName(const char* name);
       errType printParam();
       
	void dbgPrint();
	
} __attribute__ ((packed));
                  
