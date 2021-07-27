
#include <cxxabi.h>

#include <hardware/sync.h>

//------------------------------------------------------------------------------

namespace __cxxabiv1 {

//------------------------------------------------------------------------------

// ripped from gcc-arm-none-eabi-10-2020-q4-major
struct __cxa_eh_globals {
	__cxa_exception *caughtExceptions;
	unsigned int uncaughtExceptions;
#ifdef __ARM_EABI_UNWINDER__
	__cxa_exception* propagatingExceptions;
#endif
};

//------------------------------------------------------------------------------

static __cxa_eh_globals eh_globals_core0;
static __cxa_eh_globals eh_globals_core1;

static auto get_globals() noexcept
{
	return get_core_num() == 0 ? &eh_globals_core0 : &eh_globals_core1;
}

//------------------------------------------------------------------------------

extern "C" __cxa_eh_globals* __wrap___cxa_get_globals_fast() _GLIBCXX_NOTHROW
{
	return get_globals();
}

extern "C" __cxa_eh_globals* __wrap___cxa_get_globals() _GLIBCXX_NOTHROW
{
	return get_globals();
}

//------------------------------------------------------------------------------

}

//------------------------------------------------------------------------------
