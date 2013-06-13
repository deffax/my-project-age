#pragma once

const unsigned char LOGFLAG_WRITE_TO_LOG_FILE = 1 << 0;
const unsigned char LOGFLAG_WRITE_TO_DEBUGGER = 1 << 1;

namespace Logger
{
	class ErrorMessenger
	{
		bool m_enabled;
	public:
		ErrorMessenger();
		void Show(const std::string& errorMessage, bool isFatal, const char* funcName,
			const char* sourceFile, unsigned int lineNum);
	};

	void Init(const char* loggingConfigFile);
	void Destroy();

	void Log(const std::string& tag, const std::string& message, 
		const char* funcName, const char* sourceFile, unsigned int lineNum);
	void SetDisplayFlags(const std::string& tag, unsigned char flags);
};

//---------------------------------------------------------------------------------------------------------------------
// Debug macros
//---------------------------------------------------------------------------------------------------------------------

// Fatal Errors are fatal and are always presented to the user.
#define GCC_FATAL(str) \
	do \
	{ \
		static Logger::ErrorMessenger* pErrorMessenger = GCC_NEW Logger::ErrorMessenger; \
		std::string s((str)); \
		pErrorMessenger->Show(s, true, __FUNCTION__, __FILE__, __LINE__); \
	} \
	while (0)\

#if 1 //ndef NDEBUG


#define GCC_ERROR(str) \
	do \
	{ \
		static Logger::ErrorMessenger* pErrorMessenger = GCC_NEW Logger::ErrorMessenger; \
		std::string s((str)); \
		pErrorMessenger->Show(s, false, __FUNCTION__, __FILE__, __LINE__); \
	} \
	while (0)\


#define GCC_WARNING(str) \
	do \
	{ \
		std::string s((str)); \
		Logger::Log("WARNING", s, __FUNCTION__, __FILE__, __LINE__); \
	}\
	while (0)\


#define GCC_INFO(str) \
	do \
	{ \
		std::string s((str)); \
		Logger::Log("INFO", s, NULL, NULL, 0); \
	} \
	while (0) \


#define GCC_LOG(tag, str) \
	do \
	{ \
		std::string s((str)); \
		Logger::Log(tag, s, NULL, NULL, 0); \
	} \
	while (0) \

// This macro replaces GCC_ASSERT().
#define GCC_ASSERT(expr) \
	do \
	{ \
		if (!(expr)) \
		{ \
			static Logger::ErrorMessenger* pErrorMessenger = GCC_NEW Logger::ErrorMessenger; \
			pErrorMessenger->Show(#expr, false, __FUNCTION__, __FILE__, __LINE__); \
		} \
	} \
	while (0) \

#else  // NDEBUG is defined


#define GCC_ERROR(str) do { (void)sizeof(str); } while(0) 
#define GCC_WARNING(str) do { (void)sizeof(str); } while(0) 
#define GCC_INFO(str) do { (void)sizeof(str); } while(0) 
#define GCC_LOG(tag, str) do { (void)sizeof(tag); (void)sizeof(str); } while(0) 
#define GCC_ASSERT(expr) do { (void)sizeof(expr); } while(0) 

#endif  // !defined NDEBUG
