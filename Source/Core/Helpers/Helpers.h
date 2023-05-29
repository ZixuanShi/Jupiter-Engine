#pragma once

// If the operation has failed to match the expectation, log an error message and return false
// - operator: Could be a variable, or function call that returns some value.
// - expectation: The expected value of the operation
// - failedMessage: The message to log if failed to match expectation
#define RETURN_IF_FAILED(operation, expectation, failedMessage, ...)\
	if(operation != expectation)\
	{\
		JPT_ERROR(failedMessage, __VA_ARGS__);\
		return false;\
	}
