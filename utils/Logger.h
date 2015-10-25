#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <vector>
#include <map>

namespace utils {
  const enum LOG_LEVEL {
    LOG_LVL_DEBUG = 0,
    LOG_LVL_WARN,
    LOG_LVL_INFO,
    LOG_LVL_ERROR,
    LOG_LVL_FATAL,
    LOG_LVL_MAX
  };

  using logfilevec = std::vector<std::pair<LOG_LEVEL, std::string>>;
  using logfilemap = std::map<LOG_LEVEL, std::vector<std::string>>;

  const logfilevec logFiles = {
    { LOG_LVL_DEBUG, "../logs/Ewe.log" }
  };

  class LoggerConfig {
  public:
    logfilevec files;
    LOG_LEVEL logLevel;
    bool showDate;

    LoggerConfig();
  };

  class Logger {
  private:
    LOG_LEVEL _logLevel;
    logfilemap _files;
    bool _showDate;

    std::string _className;

    bool _accept(LOG_LEVEL lvl);
    std::string _date();

    void print(std::string& message, LOG_LEVEL lvl, std::string& lvl_s, std::ostream& stream);
  public:
    Logger(std::string className, LoggerConfig config = LoggerConfig());
    ~Logger();

    void debug(std::string message);
    void warn(std::string message);
    void info(std::string message);
    void error(std::string message);
    void fatal(std::string message);
  };

}

#endif // LOGGER_H_
