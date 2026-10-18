#pragma once

#include <iostream>
#pragma once

#if TRINACRIA_DEBUG

#define STRINGIFY_MACRO(x) STRINGIFY_MACRO2(x)
#define STRINGIFY_MACRO2(x) #x

// Start
#define TRCN_DEPEND_START(debugname) const std::string HIDDEN_DEBUG_NAME_TN = (debugname)

// With msgs & exceptions
#define TRCN_ASSERTEXP_MSG(condition, msg) if(!(condition)) throw std::runtime_error((msg))
#define TRCN_DEPEND_ASSERTEXP_MSG(condition, msg) if(!(condition)) throw std::runtime_error(HIDDEN_DEBUG_NAME_TN + ' ' + (msg))
// With msgs
#define TRCN_ASSERT_MSG(condition, msg) if(!(condition)) std::cerr << (msg) << '\n'
#define TRCN_DEPEND_ASSERT_MSG(condition, msg) if(!(condition)) std::cerr << HIDDEN_DEBUG_NAME_TN << ' ' << (msg) << '\n'

// Without msgs, with exceptions
#define TRCN_ASSERTEXP(condition) if(!(condition))\
	throw std::runtime_error("The assertion " #condition " has failed in file " __FILE__ ", line :" STRINGIFY_MACRO(__LINE__) )

#define TRCN_DEPEND_ASSERTEXP(condition)\
	if(!(condition))\
		throw std::runtime_error("The assertion " #condition " in function " +\
			HIDDEN_DEBUG_NAME_TN + " has failed in file " __FILE__ ", line :" STRINGIFY_MACRO(__LINE__) )

// Withot msgs
#define TRCN_ASSERT(condition) if(!(condition))\
	std::cerr << "The assertion " #condition " has failed in file " __FILE__ ", line :" STRINGIFY_MACRO(__LINE__)

#define TRCN_DEPEND_ASSERT(condition)\
	if(!(condition)) std::cerr << "The assertion " #condition " in function " <<\
		HIDDEN_DEBUG_NAME_TN << " has failed in file " __FILE__ ", line :" STRINGIFY_MACRO(__LINE__)

// With msg, with return
#define TRCN_RETURN_ASSERT_MSG(condition, msg, returnval)\
	if(!(condition)) {\
		std::cerr << (msg) << '\n';\
		return (returnval);\
	}

#define TRCN_DEPEND_RETURN_ASSERT_MSG(condition, msg, returnval)\
	if(!(condition)){\
		std::cerr << HIDDEN_DEBUG_NAME_TN << ' ' << (msg) << '\n';\
		return (returnval);\
	}

// Without msg, with return
#define TRCN_RETURN_ASSERT(condition, returnval)\
	if(!(condition)) {\
		std::cerr << "The assertion " #condition " has failed in file " __FILE__ ", line :" STRINGIFY_MACRO(__LINE__);\
		return (returnval);\
	}

#define TRCN_DEPEND_RETURN_ASSERT(condition, returnval)\
	if(!(condition)){\
		std::cerr << "The assertion " #condition " in function " <<\
		HIDDEN_DEBUG_NAME_TN << " has failed in file " __FILE__ ", line :" STRINGIFY_MACRO(__LINE__);\
		return (returnval);\
	}

// With msg, with return void
#define TRCN_RETURN_ASSERT_MSG_VOID(condition, msg)\
	if(!(condition)) {\
		std::cerr << (msg) << '\n';\
		return;\
	}

#define TRCN_DEPEND_RETURN_ASSERT_MSG_VOID(condition, msg)\
	if(!(condition)){\
		std::cerr << HIDDEN_DEBUG_NAME_TN << ' ' << (msg) << '\n';\
		return;\
	}

// Without msg, with return void
#define TRCN_RETURN_ASSERT_VOID(condition)\
	if(!(condition)) {\
		std::cerr << "The assertion " #condition " has failed in file " __FILE__ ", line :" STRINGIFY_MACRO(__LINE__);\
		return;\
	}

#define TRCN_DEPEND_RETURN_ASSERT_VOID(condition)\
	if(!(condition)){\
		std::cerr << "The assertion " #condition " in function " <<\
		HIDDEN_DEBUG_NAME_TN << " has failed in file " __FILE__ ", line :" STRINGIFY_MACRO(__LINE__);\
		return;\
	}

#else

#define TRCN_DEPEND_START(debugname)
// With msgs & exceptions
#define TRCN_ASSERTEXP_MSG(condition, msg)
#define TRCN_DEPEND_ASSERTEXP_MSG(condition, msg)
// With msgs
#define TRCN_ASSERT_MSG(condition, msg)
#define TRCN_DEPEND_ASSERT_MSG(condition, msg)
// Without msgs, with exceptions
#define TRCN_ASSERTEXP(condition)
#define TRCN_DEPEND_ASSERTEXP(condition)
// Withot msgs
#define TRCN_ASSERT(condition)
#define TRCN_DEPEND_ASSERT(condition)

// With msg, with return
#define TRCN_RETURN_ASSERT_MSG(condition, msg, returnval)
#define TRCN_DEPEND_RETURN_ASSERT_MSG(condition, msg, returnval)

// Without msg, with return
#define TRCN_RETURN_ASSERT(condition, returnval)
#define TRCN_DEPEND_RETURN_ASSERT(condition, returnval)
// With msg, with return void
#define TRCN_RETURN_ASSERT_MSG_VOID(condition, msg)
#define TRCN_DEPEND_RETURN_ASSERT_MSG_VOID(condition, msg)

// Without msg, with return void
#define TRCN_RETURN_ASSERT_VOID(condition)
#define TRCN_DEPEND_RETURN_ASSERT_VOID(condition)

#endif