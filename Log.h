/*
 * Log.h
 *
 *  Created on: Oct 30, 2017
 *      Author: pidbip
 */

#ifndef LOG_H_
#define LOG_H_

#include <string>
#include <sstream>

#include "logger_sink.h"


#define LOG(level) \
		 logger::Log(logger::level).stream() << __FILE__ ":" << __LINE__ << ":" << __FUNCTION__ << ": "

#define LOG_BUFFER(b,l) \
		std::string(b, l);


namespace logger{

class Log {
public:
	Log(prior priority);

	~Log();

	std::ostringstream& stream();

private:
	prior m_priority;
	std::ostringstream m_stream;
};


#ifdef __linux__
void InitSystemLog(const char *ident, int option = LOG_PID, int facility = LOG_USER, prior prior_max = DEBUG);
#endif

void InitLocalLog(const char *ident, const std::string &filename, prior prior_max = DEBUG);

void InitConsolLog(const char *ident, prior prior_max = DEBUG);

}




#endif /* LOG_H_ */
