#pragma once

// If the condition has occured, log an error message and return false
// - condition: The expression of the operation
// - failedMessage: The message to log if expectedExpression failed
#define RETURN_FALSE_IF_LOG(condition, message, ...)\
	if((condition))\
	{\
		JPT_ERROR(message, __VA_ARGS__);\
		return false;\
	}

#define RETURN_FALSE_IF(condition)\
	if((condition))\
	{\
		return false;\
	}

#define RETURN_VALUE_IF_LOG(condition, returnValue, message, ...)\
	if ((condition))\
	{\
		JPT_LOG(message, __VA_ARGS__);\
		return returnValue;\
	}

#define RETURN_VALUE_IF(condition, returnValue)\
	if ((condition))\
	{\
		return returnValue;\
	}

#define RETURN_IF_LOG(condition, returnValue, message, ...)\
	if ((condition))\
	{\
		JPT_LOG(message, __VA_ARGS__);\
		return;\
	}

#define RETURN_IF(condition, returnValue)\
	if ((condition))\
	{\
		return;\
	}

// Helper for clean and delete.
// - pPointer: Assumed to have Clean() function implemented
#define CLEAN_THEN_DELETE(pPointer)\
	pPointer->Clean();\
	JPT_DELETE(pPointer);
	