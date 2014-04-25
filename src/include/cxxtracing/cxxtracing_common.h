#ifndef CXX_TRACING_COMMON_H
#define CXX_TRACING_COMMON_H


#ifndef CXX_TRACING_EXPORTS
#define CXX_TRACING_EXPORT _declspec(dllimport)
#else
#define CXX_TRACING_EXPORT _declspec(dllexport)
#endif

#define CXX_TRACING_API

#endif //CXX_TRACING_COMMON_H