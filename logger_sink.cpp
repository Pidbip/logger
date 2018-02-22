/*
 * logger_sink.cpp
 *
 *  Created on: Nov 26, 2017
 *      Author: pidbip
 */

#define _CRT_SECURE_NO_WARNINGS 1


#include <sstream>
#include <iostream>
#include <chrono>  // chrono::system_clock
#include <ctime>   // localtime
#include <iomanip> // put_time
#include <stdexcept>

#include "logger_sink.h"


namespace logger {

/*
 *
 *
 *
 */

#ifdef __linux__
logger_sink_posix::logger_sink_posix(const char *ident, int option, int facility):
	logger_sink_base(ident)
{
	openlog(m_ident, option, facility);
}

logger_sink_posix::~logger_sink_posix(){
	closelog();
}

void logger_sink_posix::sink(std::ostringstream& stream, prior priority) {
	if(priority > m_prioryty_max) return;
	std::unique_lock<std::mutex> lock(m_mutex);
	syslog(priority, "%s", stream.str().c_str());
}
#endif

/*
 *
 *
 *
 *
 */


logger_sink_consol::logger_sink_consol(const char *ident):
	logger_sink_base(ident)
{
}

logger_sink_consol::~logger_sink_consol(){
}

void logger_sink_consol::sink(std::ostringstream& stream, prior priority) {
	if(priority > m_prioryty_max) return;
	std::unique_lock<std::mutex> lock(m_mutex);

	std::cerr << get_time_date();
	std::cerr << " [" << m_ident << "] " <<  stream.str() << std::endl << std::flush;
}


/*
 *
 *
 *
 */


logger_sink_file::logger_sink_file(const char *ident, const std::string &filename):
	logger_sink_base(ident),
	m_filename(filename)
{
	m_log_file.open(filename);
}

logger_sink_file::~logger_sink_file(){
	if(m_log_file.is_open())
		m_log_file.close();
}

void logger_sink_file::sink(std::ostringstream& stream, prior priority){
	if(priority > m_prioryty_max) return;
	std::unique_lock<std::mutex> lock(m_mutex);

	m_log_file << get_time_date();
	m_log_file << " [" << m_ident << "] " <<  stream.str() << std::endl << std::flush;
}


/*
 *
 *
 *
 */
std::string get_time_date(){
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
}



} /* namespace logger */
