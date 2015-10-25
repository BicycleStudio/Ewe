#include "Logger.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <ctime>

using std::chrono::system_clock;
using std::time_t;
using std::ctime;
using std::clog;
using std::cerr;
using std::cout;
using std::endl;
using std::ofstream;
using std::ios_base;
using std::vector;
using std::string;
using std::pair;
using std::for_each;

utils::LoggerConfig::LoggerConfig() {
  files = logFiles;
  logLevel = LOG_LVL_WARN;
  showDate = true;
}

utils::Logger::Logger(std::string className, LoggerConfig config) : _className(className) {
  for (auto p : config.files) {
    auto lvl = p.first;
    for (int i = static_cast<int>(lvl); i < static_cast<int>(LOG_LVL_MAX); i++)
      this->_files[static_cast<LOG_LEVEL>(i)].push_back(p.second);
  }
  this->_logLevel = config.logLevel;
  this->_showDate = config.showDate;
}

utils::Logger::~Logger() { }


bool utils::Logger::_accept(LOG_LEVEL lvl) {
  return static_cast<int>(this->_logLevel) <= static_cast<int>(lvl);
}

string utils::Logger::_date() {
  if (this->_showDate) {
    auto _ = system_clock::to_time_t(system_clock::now());
    const size_t size = 100;
    char buf[size];
    ctime_s(buf, size, &_);
    auto res = string(buf);
    res = res.substr(0, res.size() - 2);
    return res;
  }
  else return string();
}

void utils::Logger::print(std::string& message, LOG_LEVEL lvl, std::string& lvl_s, std::ostream& stream) {
  static vector<string> files = this->_files[lvl];
  if (_accept(lvl)) {
    auto _message = _date() + " [" + lvl_s + "] " + this->_className + " :: " + message;
    stream << _message << endl;
    for (auto file = files.begin(); file != files.end(); file ++) {
      ofstream of(*file, ios_base::app);
      of << _message << endl;
      of.close();
    }
  }
}

void utils::Logger::debug(std::string message) {
  this->print(message, LOG_LVL_DEBUG, string("DEBUG"), clog);
}

void utils::Logger::info(std::string message)  {
  this->print(message, LOG_LVL_INFO, string("INFO"), cout);
}

void utils::Logger::warn(std::string message)  {
  this->print(message, LOG_LVL_WARN, string("WARN"), clog);
}

void utils::Logger::error(std::string message) {
  this->print(message, LOG_LVL_ERROR, string("ERROR"), cerr);
}

void utils::Logger::fatal(std::string message) {
  this->print(message, LOG_LVL_FATAL, string("FATAL"), cerr);
}
