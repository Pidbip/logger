/*
 * logger_sink.h
 *
 *  Created on: Nov 26, 2017
 *      Author: pidbip
 */

#ifndef COMMON_LOGGER_SINK_H_
#define COMMON_LOGGER_SINK_H_


#ifdef __linux__
#include <syslog.h>
#endif

#include <fstream>
#include <string>
#include <thread>
#include <mutex>


namespace logger {

#ifdef __linux__
enum prior{
	EMERG	=	LOG_EMERG,		/* system is unusable */
	ALERT	=	LOG_ALERT,		/* action must be taken immediately */
	CRIT	=	LOG_CRIT,		/* critical conditions */
	ERR		=	LOG_ERR,		/* error conditions */
	WARNING	=	LOG_WARNING,	/* warning conditions */
	NOTICE	=	LOG_NOTICE,		/* normal but significant condition */
	INFO	=	LOG_INFO,		/* informational */
	DEBUG	=   LOG_DEBUG		/* debug-level messages */
};
#elif _WIN32
enum prior{
	EMERG = 0,
	ALERT,
	CRIT,
	ERR,
	WARNING,
	NOTICE,
	INFO,
	DEBUG
};
#endif


std::string get_time_date();


class logger_sink_base{
public:
	logger_sink_base(const char *ident, prior prioryty_max = DEBUG):
		m_ident(ident),
		m_prioryty_max(prioryty_max)
	{
	}

	virtual ~logger_sink_base(){
	}

	void set_priority_max(prior prioryty){
		m_prioryty_max = prioryty;
	}

	virtual void sink(std::ostringstream&, prior) = 0;

protected:
	std::mutex m_mutex;
	const char *m_ident;
	prior m_prioryty_max;
};

#ifdef __linux__
class logger_sink_posix final: public logger_sink_base{
public:
	logger_sink_posix(const char *ident, int option = LOG_PID, int facility = LOG_USER);

	virtual ~logger_sink_posix();

	void sink(std::ostringstream& stream, prior priority = INFO) override;
};
#endif

class logger_sink_consol final: public logger_sink_base{
public:
	logger_sink_consol(const char *ident);
	virtual ~logger_sink_consol();

	void sink(std::ostringstream& stream, prior priority = INFO) override;
};

class logger_sink_file final: public logger_sink_base{
public:
	logger_sink_file(const char *ident, const std::string &filename);

	virtual ~logger_sink_file();

	void sink(std::ostringstream& stream, prior priority = INFO) override;

private:
	const std::string m_filename;
	std::ofstream m_log_file;
};

} /* namespace logger */

#endif /* COMMON_LOGGER_SINK_H_ */
