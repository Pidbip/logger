/*
 * Log.cpp
 *
 *  Created on: Oct 30, 2017
 *      Author: pidbip
 */

#include "Log.h"

namespace logger {

	logger_sink_base *g_log_sink = nullptr;

	const std::string priorityNames[] = {
		"EMERG", "ALERT", "CRIT", "ERR", "WARNING", "NOTICE", "INFO", "DEBUG"
	};

	Log::Log(prior priority) :
			m_priority(priority)
	{
		m_stream << "<" << priorityNames[m_priority].c_str() << "> - ";
	}

	Log::~Log(){
		g_log_sink->sink(m_stream, m_priority);
	}

	std::ostringstream& Log::stream(){
		return m_stream;
	}

	#ifdef __linux__
	void InitSystemLog(const char *ident, int option, int facility, prior prior_max){
		g_log_sink = new logger_sink_posix(ident, option, facility);
		g_log_sink->set_priority_max(prior_max);
	}
	#endif

	void InitLocalLog(const char *ident, const std::string &filename, prior prior_max){
		g_log_sink = new logger_sink_file(ident, filename);
		g_log_sink->set_priority_max(prior_max);
	}

	void InitConsolLog(const char *ident, prior prior_max){
		g_log_sink = new logger_sink_consol(ident);
		g_log_sink->set_priority_max(prior_max);
	}


}
