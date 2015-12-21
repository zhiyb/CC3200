/*
 * Author: Yubo Zhi (normanzyb@gmail.com)
 */

#ifndef MACROS_H
#define MACROS_H

#include <stdint.h>

#define CONCAT(a,b)	a ## b
#define CONCAT_E(a,b)	CONCAT(a, b)

#define STRINGIFY(x)	#x
#define TOSTRING(x)	STRINGIFY(x)

#endif
