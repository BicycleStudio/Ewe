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
using std::vector;
using std::string;
using std::pair;
using std::for_each;

utils::Logger::Logger(std::string className, LoggerConfig config) : _className(className) {
  for (auto p : config.files) this->_files[p.first].push_back(p.second);  
  this->_logLevel = config.logLevel;
  this->_showDate = config.showDate;
}

utils::Logger::~Logger() { }


bool utils::Logger::_accept(LEVEL lvl) {
  return static_cast<int>(this->_logLevel) <= static_cast<int>(lvl);
}

string utils::Logger::_date() {
  if (this->_showDate) {
    auto _ = system_clock::to_time_t(system_clock::now());
    const size_t size = 100;
    char buf[size];
    ctime_s(buf, size, &_);
    return string(buf);
  }
  else return string();
}

void utils::Logger::debug(std::string message) {
  static vector<string> files = this->_files[LEVEL::DEBUG];
  if (_accept(LEVEL::DEBUG)) {
    clog << _date() << " [DEBUG] " << this->_className << " " << message << endl;
  }
}

void utils::Logger::info(std::string message)  {
  static vector<string> files = this->_files[LEVEL::INFO];
  if (_accept(LEVEL::INFO)) {
    cout << _date() << " [INFO] " << this->_className << " " << message << endl;
  }
}

void utils::Logger::warn(std::string message)  {
  static vector<string> files = this->_files[LEVEL::WARN];
  if (_accept(LEVEL::WARN)) {
    clog << _date() << " [WARN] " << this->_className << " " << message << endl;
  }
}

void utils::Logger::error(std::string message) {
  static vector<string> files = this->_files[LEVEL::ERROR];
  if (_accept(LEVEL::ERROR)) {
    cerr << _date() << " [ERROR] " << this->_className << " " << message << endl;
  }
}

void utils::Logger::fatal(std::string message) {
  static vector<string> files = this->_files[LEVEL::FATAL];
  if (_accept(LEVEL::FATAL)) {
    cerr << _date() << " [FATAL] " << this->_className << " " << message << endl;
  }
}
