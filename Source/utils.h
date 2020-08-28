#pragma once
#include <stdlib.h>

#define FUCK_SUCCESS 1

// ALL ERR < 0 so you can use < 0 to detect error
#define FUCK_ERR -1
#define FUCK_ERR__CANNOT_FIND_UNIT -2
#define FUCK_ERR__UNKNOWN_UNIT_TYPE -3
#define FUCK_ERR__NULL_UNIT -4
#define FUCK_ERR__NULL_AI_MODULE -5

#define SAFE_FREE(X) {if((X)) free((X)); (X) = NULL;}
#define SAFE_DELETE(X) {if((X)) delete (X); (X) = NULL;}