#pragma once

namespace gvk
{
	// Define LOGGING_ON_SEPARATE_THREAD to have all the logging being transmitted and performed by a separate thread
	#if !defined(NO_SEPARATE_LOGGING_THREAD)
	//#define LOGGING_ON_SEPARATE_THREAD
	#endif

	// Define PRINT_STACKTRACE to have the stack trace printed for errors
	#if !defined(DONT_PRINT_STACKTRACE)
	#define PRINT_STACKTRACE
	#endif

	// LOG-LEVELS:
	// 0 ... nothing (except debug messages in DEBUG-mode)
	// 1 ... error messages only
	// 2 ... errors and warnings
	// 3 ... errors, warnings and infos
	// 4 ... errors, warnings, infos, and verbose
	// 5 ... errors, warnings, infos, verbose, and mega-verbose
	#if !defined(LOG_LEVEL)
	#define LOG_LEVEL 3
	#endif

	enum struct log_type
	{
		error,
		warning,
		info,
		verbose,
		debug,
		debug_verbose,
		system
	};

	enum struct log_importance
	{
		normal,
		important
	};

	struct log_pack
	{
		std::string mMessage;
		log_type mLogType;
		log_importance mLogImportance;
		std::string mStacktrace;
	};
	
	extern void set_console_output_color(gvk::log_type level, gvk::log_importance importance);
	extern void set_console_output_color_for_stacktrace(gvk::log_type level, gvk::log_importance importance);
	extern void reset_console_output_color();
	extern void dispatch_log(gvk::log_pack pToBeLogged);
	
	#if LOG_LEVEL > 0
	#define LOG_ERROR(msg)		gvk::dispatch_log(gvk::log_pack{ fmt::format("{}{}{}\n", "ERR:  ", msg, fmt::format(" | file[{}] line[{}]", avk::extract_file_name(std::string(__FILE__)), __LINE__)), gvk::log_type::error, gvk::log_importance::normal })
	#define LOG_ERROR_EM(msg)	gvk::dispatch_log(gvk::log_pack{ fmt::format("{}{}{}\n", "ERR:  ", msg, fmt::format(" | file[{}] line[{}]", avk::extract_file_name(std::string(__FILE__)), __LINE__)), gvk::log_type::error, gvk::log_importance::important})
	#define LOG_ERROR__(msg)	gvk::dispatch_log(gvk::log_pack{ fmt::format("{}{}\n", "ERR:  ", msg), gvk::log_type::error, gvk::log_importance::normal })
	#define LOG_ERROR_EM__(msg)	gvk::dispatch_log(gvk::log_pack{ fmt::format("{}{}\n", "ERR:  ", msg), gvk::log_type::error, gvk::log_importance::important })
	#else
	#define LOG_ERROR(msg)
	#define LOG_ERROR_EM(msg)
	#define LOG_ERROR__(msg)
	#define LOG_ERROR_EM__(msg)
	#endif

	#if LOG_LEVEL > 1
	#define LOG_WARNING(msg)		gvk::dispatch_log(gvk::log_pack{ fmt::format("{}{}{}\n", "WARN: ", msg, fmt::format(" | file[{}] line[{}]", avk::extract_file_name(std::string(__FILE__)), __LINE__)), gvk::log_type::warning, gvk::log_importance::normal })
	#define LOG_WARNING_EM(msg)		gvk::dispatch_log(gvk::log_pack{ fmt::format("{}{}{}\n", "WARN: ", msg, fmt::format(" | file[{}] line[{}]", avk::extract_file_name(std::string(__FILE__)), __LINE__)), gvk::log_type::warning, gvk::log_importance::important })
	#define LOG_WARNING__(msg)		gvk::dispatch_log(gvk::log_pack{ fmt::format("{}{}\n", "WARN: ", msg), gvk::log_type::warning, gvk::log_importance::normal })
	#define LOG_WARNING_EM__(msg)	gvk::dispatch_log(gvk::log_pack{ fmt::format("{}{}\n", "WARN: ", msg), gvk::log_type::warning, gvk::log_importance::important })
	#else 
	#define LOG_WARNING(msg)
	#define LOG_WARNING_EM(msg)
	#define LOG_WARNING__(msg)
	#define LOG_WARNING_EM__(msg)
	#endif

	#if LOG_LEVEL > 2
	#define LOG_INFO(msg)		gvk::dispatch_log(gvk::log_pack{ fmt::format("{}{}{}\n", "INFO: ", msg, fmt::format(" | file[{}] line[{}]", avk::extract_file_name(std::string(__FILE__)), __LINE__)), gvk::log_type::info, gvk::log_importance::normal })
	#define LOG_INFO_EM(msg)	gvk::dispatch_log(gvk::log_pack{ fmt::format("{}{}{}\n", "INFO: ", msg, fmt::format(" | file[{}] line[{}]", avk::extract_file_name(std::string(__FILE__)), __LINE__)), gvk::log_type::info, gvk::log_importance::important })
	#define LOG_INFO__(msg)		gvk::dispatch_log(gvk::log_pack{ fmt::format("{}{}\n", "INFO: ", msg), gvk::log_type::info, gvk::log_importance::normal })
	#define LOG_INFO_EM__(msg)	gvk::dispatch_log(gvk::log_pack{ fmt::format("{}{}\n", "INFO: ", msg), gvk::log_type::info, gvk::log_importance::important })
	#else
	#define LOG_INFO(msg)
	#define LOG_INFO_EM(msg)
	#define LOG_INFO__(msg)
	#define LOG_INFO_EM__(msg)
	#endif

	#if LOG_LEVEL > 3
	#define LOG_VERBOSE(msg)		gvk::dispatch_log(gvk::log_pack{ fmt::format("{}{}{}\n", "VRBS: ", msg, fmt::format(" | file[{}] line[{}]", avk::extract_file_name(std::string(__FILE__)), __LINE__)), gvk::log_type::verbose, gvk::log_importance::normal })
	#define LOG_VERBOSE_EM(msg)		gvk::dispatch_log(gvk::log_pack{ fmt::format("{}{}{}\n", "VRBS: ", msg, fmt::format(" | file[{}] line[{}]", avk::extract_file_name(std::string(__FILE__)), __LINE__)), gvk::log_type::verbose, gvk::log_importance::important })
	#define LOG_VERBOSE__(msg)		gvk::dispatch_log(gvk::log_pack{ fmt::format("{}{}\n", "VRBS: ", msg), gvk::log_type::verbose, gvk::log_importance::normal })
	#define LOG_VERBOSE_EM__(msg)	gvk::dispatch_log(gvk::log_pack{ fmt::format("{}{}\n", "VRBS: ", msg), gvk::log_type::verbose, gvk::log_importance::important })
	#else 
	#define LOG_VERBOSE(msg)
	#define LOG_VERBOSE_EM(msg)
	#define LOG_VERBOSE__(msg)
	#define LOG_VERBOSE_EM__(msg)
	#endif

	#if LOG_LEVEL > 4
	#define LOG_MEGA_VERBOSE(msg)		gvk::dispatch_log(gvk::log_pack{ fmt::format("{}{}{}\n", "MVRBS:", msg, fmt::format(" | file[{}] line[{}]", avk::extract_file_name(std::string(__FILE__)), __LINE__)), gvk::log_type::verbose, gvk::log_importance::normal })
	#define LOG_MEGA_VERBOSE_EM(msg)	gvk::dispatch_log(gvk::log_pack{ fmt::format("{}{}{}\n", "MVRBS:", msg, fmt::format(" | file[{}] line[{}]", avk::extract_file_name(std::string(__FILE__)), __LINE__)), gvk::log_type::verbose, gvk::log_importance::important })
	#define LOG_MEGA_VERBOSE__(msg)		gvk::dispatch_log(gvk::log_pack{ fmt::format("{}{}\n", "MVRBS:", msg), gvk::log_type::verbose, gvk::log_importance::normal })
	#define LOG_MEGA_VERBOSE_EM__(msg)	gvk::dispatch_log(gvk::log_pack{ fmt::format("{}{}\n", "MVRBS:", msg), gvk::log_type::verbose, gvk::log_importance::important })
	#else 
	#define LOG_MEGA_VERBOSE(msg)
	#define LOG_MEGA_VERBOSE_EM(msg)
	#define LOG_MEGA_VERBOSE__(msg)
	#define LOG_MEGA_VERBOSE_EM__(msg)
	#endif

	#if defined(_DEBUG)
	#define LOG_DEBUG(msg)		gvk::dispatch_log(gvk::log_pack{ fmt::format("{}{}{}\n", "DBG:  ", msg, fmt::format(" | file[{}] line[{}]", avk::extract_file_name(std::string(__FILE__)), __LINE__)), gvk::log_type::debug, gvk::log_importance::normal })
	#define LOG_DEBUG_EM(msg)	gvk::dispatch_log(gvk::log_pack{ fmt::format("{}{}{}\n", "DBG:  ", msg, fmt::format(" | file[{}] line[{}]", avk::extract_file_name(std::string(__FILE__)), __LINE__)), gvk::log_type::debug, gvk::log_importance::important })
	#define LOG_DEBUG__(msg)	gvk::dispatch_log(gvk::log_pack{ fmt::format("{}{}\n", "DBG:  ", msg), gvk::log_type::debug, gvk::log_importance::normal })
	#define LOG_DEBUG_EM__(msg)	gvk::dispatch_log(gvk::log_pack{ fmt::format("{}{}\n", "DBG:  ", msg), gvk::log_type::debug, gvk::log_importance::important })
	#else
	#define LOG_DEBUG(msg)
	#define LOG_DEBUG_EM(msg)
	#define LOG_DEBUG__(msg)
	#define LOG_DEBUG_EM__(msg)	
	#endif

	#if defined(_DEBUG) && LOG_LEVEL > 3
	#define LOG_DEBUG_VERBOSE(msg)		gvk::dispatch_log(gvk::log_pack{ fmt::format("{}{}{}\n", "DBG-V:", msg, fmt::format(" | file[{}] line[{}]", avk::extract_file_name(std::string(__FILE__)), __LINE__)), gvk::log_type::debug_verbose, gvk::log_importance::normal })
	#define LOG_DEBUG_VERBOSE_EM(msg)	gvk::dispatch_log(gvk::log_pack{ fmt::format("{}{}{}\n", "DBG-V:", msg, fmt::format(" | file[{}] line[{}]", avk::extract_file_name(std::string(__FILE__)), __LINE__)), gvk::log_type::debug_verbose, gvk::log_importance::important })
	#define LOG_DEBUG_VERBOSE__(msg)	gvk::dispatch_log(gvk::log_pack{ fmt::format("{}{}\n", "DBG-V:", msg), gvk::log_type::debug_verbose, gvk::log_importance::normal })
	#define LOG_DEBUG_VERBOSE_EM__(msg)	gvk::dispatch_log(gvk::log_pack{ fmt::format("{}{}\n", "DBG-V:", msg), gvk::log_type::debug_verbose, gvk::log_importance::important })
	#else
	#define LOG_DEBUG_VERBOSE(msg)
	#define LOG_DEBUG_VERBOSE_EM(msg)   
	#define LOG_DEBUG_VERBOSE__(msg)
	#define LOG_DEBUG_VERBOSE_EM__(msg)
	#endif

	#if defined(_DEBUG) && LOG_LEVEL > 4
	#define LOG_DEBUG_MEGA_VERBOSE(msg)		gvk::dispatch_log(gvk::log_pack{ fmt::format("{}{}{}\n", "DBG-MV:", msg, fmt::format(" | file[{}] line[{}]", avk::extract_file_name(std::string(__FILE__)), __LINE__)), gvk::log_type::debug_verbose, gvk::log_importance::normal })
	#define LOG_DEBUG_MEGA_VERBOSE_EM(msg)	gvk::dispatch_log(gvk::log_pack{ fmt::format("{}{}{}\n", "DBG-MV:", msg, fmt::format(" | file[{}] line[{}]", avk::extract_file_name(std::string(__FILE__)), __LINE__)), gvk::log_type::debug_verbose, gvk::log_importance::important })
	#define LOG_DEBUG_MEGA_VERBOSE__(msg)	gvk::dispatch_log(gvk::log_pack{ fmt::format("{}{}\n", "DBG-MV:", msg), gvk::log_type::debug_verbose, gvk::log_importance::normal })
	#define LOG_DEBUG_MEGA_VERBOSE_EM__(msg)	gvk::dispatch_log(gvk::log_pack{ fmt::format("{}{}\n", "DBG-MV:", msg), gvk::log_type::debug_verbose, gvk::log_importance::important })
	#else
	#define LOG_DEBUG_MEGA_VERBOSE(msg)
	#define LOG_DEBUG_MEGA_VERBOSE_EM(msg)   
	#define LOG_DEBUG_MEGA_VERBOSE__(msg)
	#define LOG_DEBUG_MEGA_VERBOSE_EM__(msg)
	#endif

	std::string to_string(const glm::mat4&);
	std::string to_string(const glm::mat3&);
	std::string to_string_compact(const glm::mat4&);
	std::string to_string_compact(const glm::mat3&);

	std::string to_string(const glm::vec2&);
	std::string to_string(const glm::vec3&);
	std::string to_string(const glm::vec4&);

	std::string get_current_callstack();
}
