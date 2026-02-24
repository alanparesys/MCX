#pragma once

#ifdef _WIN32
    #ifdef MCX_STATIC
        #define MCX_API
    #elif defined(MCX_CORE_EXPORTS)
        #define MCX_API __declspec(dllexport)
    #else
        #define MCX_API __declspec(dllimport)
    #endif
#else
    #define MCX_API __attribute__((visibility("default")))
#endif
