#ifndef CXXTRACING_H
#define CXXTRACING_H

#include "cxxtracing_common.h"
#include <memory>




	CXX_TRACING_EXPORT std::shared_ptr<void> CXX_TRACING_API make_activity(const char* name);
extern "C"{
	CXX_TRACING_EXPORT void CXX_TRACING_API init();
	CXX_TRACING_EXPORT void CXX_TRACING_API shutdown();

	CXX_TRACING_EXPORT void CXX_TRACING_API begin_activity(const char *);
	CXX_TRACING_EXPORT void CXX_TRACING_API end_activity(const char *);
	CXX_TRACING_EXPORT void CXX_TRACING_API add_mark(const char *);
	CXX_TRACING_EXPORT void CXX_TRACING_API set_process_name(const char *);
	CXX_TRACING_EXPORT void CXX_TRACING_API set_thread_name(const char *);
	CXX_TRACING_EXPORT void CXX_TRACING_API set_category_name(const char *);
}



#ifdef CXXTRACING_ENABLED
#define CXXPROF_INIT()
#define CXXPROF_PROCESS_ALIAS(ProcessName);
#define CXXPROF_THREAD_ALIAS(ThreadName);
#define CXXTRACING_ACTIVITY(ActivityName);
#define CXXTRACING_MARK(MarkName)
#define CXXPROF_SHUTDOWN()
#define CXXPROF_RESUME()
#define CXXPROF_PAUSE()
#else // !CXXTRACING_ENABLED
#define CXXPROF_INIT() 
//#define CXXPROF_PROCESS_ALIAS(ProcessName) cxxtracing::Monitor::instance().setProcessName(ProcessName);
//#define CXXPROF_THREAD_ALIAS(ThreadName) cxxtracing::Monitor::instance().setCurrentThreadName(std::this_thread::get_id(), ThreadName);
//#define CXXTRACING_ACTIVITY(ActivityName) auto __ActivityName##_LINE_ make_activity(ActivityName);
//#define CXXTRACING_MARK(MarkName)
//#define CXXPROF_SHUTDOWN()
//#define CXXPROF_RESUME()
//#define CXXPROF_PAUSE()
#endif // CXXTRACING_ENABLED

#endif  //CXXTRACING_H
