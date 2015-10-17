#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <vector>
#include <map>
#include <memory>

namespace utils {
  const enum LEVEL {
    DEBUG = 0,
    WARN,
    INFO,
    ERROR,
    FATAL
  };

  using logfilemap = std::vector<std::pair<LEVEL, std::string>>;
  using _logfilemap = std::map<LEVEL, std::vector<std::string>>;

  const logfilemap logFiles = {
    { LEVEL::DEBUG, "../logs/Ewe.log" }
  };

  class LoggerConfig {
  public:
    logfilemap files;
    LEVEL logLevel;
    bool showDate;
  };

  class DefaultLoggerConfig : public LoggerConfig {
  public:
    DefaultLoggerConfig() {
      files = logFiles;
      logLevel = LEVEL::WARN;
      showDate = true;
    }
  };

  class Logger {
  private:
    LEVEL _logLevel;
    _logfilemap _files;
    bool _showDate;

    std::string _className;

    bool _accept(LEVEL lvl);
    std::string _date();
  public:
    Logger(std::string className, LoggerConfig config = DefaultLoggerConfig());
    ~Logger();

    void debug(std::string message);
    void warn(std::string message);
    void info(std::string message);
    void error(std::string message);
    void fatal(std::string message);
  };

}

#endif // LOGGER_H_
