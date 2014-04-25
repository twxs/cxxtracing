#ifndef CXXTRACING_H
#define CXXTRACING_H

#include "cxxtracing_common.h"
#include <memory>




CXX_TRACING_EXPORT std::shared_ptr<void> CXX_TRACING_API cxxtracing_make_activity(const char* name, const char * category = NULL);
extern "C"{
	CXX_TRACING_EXPORT void CXX_TRACING_API cxxtracing_init(const char* file);
	CXX_TRACING_EXPORT void CXX_TRACING_API cxxtracing_shutdown();

	CXX_TRACING_EXPORT void CXX_TRACING_API cxxtracing_begin_activity(const char *, const char * category = NULL);
	CXX_TRACING_EXPORT void CXX_TRACING_API cxxtracing_end_activity(const char *, const char * category = NULL);
	CXX_TRACING_EXPORT void CXX_TRACING_API cxxtracing_add_mark(const char *, const char * category = NULL);
	CXX_TRACING_EXPORT void CXX_TRACING_API cxxtracing_set_process_name(const char * name);

	// set name of the current running thread
	CXX_TRACING_EXPORT void CXX_TRACING_API cxxtracing_set_thread_name(const char * name);
}



#define CXXTRACING_HELPER_COMBINE1(X,Y)  X##Y
#define CXXTRACING_HELPER_COMBINE(X,Y)  CXXTRACING_HELPER_COMBINE1(X, Y)

#ifdef CXXTRACING_ENABLED
#define HELPER(NAME)  NAME ## _LINE_ 
#define CXXTRACING_INIT(FileName) cxxtracing_init(FileName);
#define CXXTRACING_PROCESS_NAME(Name) cxxtracing_set_process_name(Name)
#define CXXTRACING_THREAD_NAME(Name) cxxtracing_set_process_name(Name)
#define CXXTRACING_ACTIVITY(ActivityName) auto CXXTRACING_HELPER_COMBINE(junk, __LINE__) = cxxtracing_make_activity(ActivityName);
#define CXXTRACING_MARK(MarkName) cxxtracing_add_mark(MarkName);
#define CXXTRACING_ACTIVITY2(Category, ActivityName)  CXXTRACING_HELPER_COMBINE(junk, __LINE__) = cxxtracing_make_activity(ActivityName, Category);
#define CXXTRACING_MARK2(Category, MarkName) cxxtracing_add_mark(MarkName, Category);
#define CXXTRACING_SHUTDOWN() cxxtracing_shutdown();
#define CXXTRACING_RESUME()
#define CXXTRACING_PAUSE()
#else // !CXXTRACING_ENABLED

#define CXXTRACING_INIT(FileName)
#define CXXTRACING_PROCESS_NAME(Name) 
#define CXXTRACING_THREAD_NAME(Name) 
#define CXXTRACING_ACTIVITY(ActivityName)
#define CXXTRACING_MARK(MarkName) 
#define CXXTRACING_ACTIVITY2(Category, ActivityName)  
#define CXXTRACING_MARK2(Category, MarkName) 
#define CXXTRACING_SHUTDOWN() 
#define CXXTRACING_RESUME()
#define CXXTRACING_PAUSE()

#endif // CXXTRACING_ENABLED

#endif  //CXXTRACING_H
