## cpp-logger [![Build Status](https://travis-ci.com/HerbertKoelman/cpp-logger.svg?branch=master)](https://travis-ci.com/HerbertKoelman/cpp-logger) [![codecov](https://codecov.io/gh/HerbertKoelman/cpp-logger/branch/master/graph/badge.svg)](https://codecov.io/gh/HerbertKoelman/cpp-logger)

### What it does

[cpp-logger](https://github.com/HerbertKoelman/cpp-logger) is a set of very simple classes that handles logging. Yes ! I know, 
one more. And you're right.

The library was first implemented for IBM's Unix: AIX. Existing libraries did not compile on AIX, so I had to write my own thing.

> **FYI** It's still used to log messages by programs that are in production :-)

Today, this library is mainly used to train C++ beginners. That's why, I keep maintaining cpp-logger.

#### Build

To use this library:

    $ mkdir build
    $ cd build
    $ cmake .. && make all
    ...
    $ make install
    ...

> **WARN** your compiler need to support the language standard [C++11](https://en.cppreference.com/w/cpp/compiler_support) or higher.

Install moves files into your system's default location for headers and libraries (often `/usr/local/include` and `/usr/local/lib`). You can relocate installation by setting the cmake property `CMAKE_INSTALL_PREFIX`.

Here after a list of build targets usefull to mention:
- test : run all registered units programs.
- package: create an archive (tar.gz)
- doxygen: create doxygen html documentation (in <build-dir>/html)

#### Testing

Unit tests are provided in `./tests`. You can build and run them with this command `make all tests`. CTest provides full diagnostic in `<build-dir>/tests/Testing/Temporary/`. We use [GoogleTest 1.8.1](https://github.com/google/googletest). 
The test framework is automatically downloaded and setup by the provided cmake package `cmake/GTestExtConfig.cmake`.

> **FYI** unit testing can be switched of with the cmake option BUILD_TESTS (`cmake -DBUILD_TESTS=false .. ` turns things off)

#### Documentation

Doxygen documentation can be generated with this target.

    make doxygen

> Doxygen can be downloaded [here](http://www.doxygen.nl).

### Performance

You can check performance with `./logger_performance_tests`:
     
     $ time ./logger_performance_tests --gtest_filter=logger_performance.stdout_sink
     ...
     <6>1 2019-04-22T13:46:55.974395+02:00 user Mac-mini-de-herbert.local performance.46854.142923200 - [M ECID="0123456780123456780123456780123456780123456780123456780123456789"][L SUBSYS=stdout] 
         Ran performance tests on version: 2.0.0. Wrote 10000 log entries in 157 ms
     --------
     called 10000 time logger->info(...) in 157 milliseconds.
     [       OK ] logger_performance.stdout_sink (159 ms)
     [----------] 1 test from logger_performance (159 ms total)
     
     [----------] Global test environment tear-down
     [==========] 1 test from 1 test case ran. (159 ms total)
     [  PASSED  ] 1 test.
     
     real	0m0.213s
     user	0m0.047s
     sys	0m0.026s

On my Mac Mini, the performance tests shows that I can write 10000 log entries in 157 ms (around 62000 entries/s).

The library has been tested on:
- Mac OS X
  - Compiler : AppleClang 10.0.1.10010046
  - OS version: 10.14.4 (Darwin 18.5.0)
  - Performance: 
      - CPU: Intel Core i7
      - Memory: 8GB
      - Command: time ./logger_performance_tests > /dev/null
      - Result: 0m0,272s (user: 0m0,128s,sys:0m0,031s)
- Fedora Linux
  - Compiler: GCC 8.3.1 20190223 (Red Hat 8.3.1-2)
  - OS version: 4.20.16-100.fc28.x86_64
  - Performance: 
    - CPU: Intel(R) Celeron(R) 2957U @ 1.40GHz
    - Memory: 8GB
    - Command: time ./logger_performance_tests > /dev/null
    - Result: 0m0,249s (user: 0m0,172s,sys:0m0,056s)
- Ubuntu Linux
  - Compiler: GCC 5.4.0-6 20160609 (Ubuntu Xenial)
  - OS version: 4.15.0-48-generic #51~16.04.1-Ubuntu
  - Performance: 
    - CPU: Intel(R) Core(TM) i5-7300U CPU @ 2.60GHz
    - Memory: 5GB
    - Command: time ./logger_performance_tests > /dev/null
    - Result: 0m0.846s (user: 0m0.313s, sys: 0m0.074s)

### How it's done

The module is made of two distinct parts. On one side, we address the writing of log messages somewhere (`logger::logger`) and 
on the other side, we provide a set of classes and interfaces that does the actual writting of messages (`logger::sink`). Finally, 
a way to reuse and share logger instances is provided through factory methods (`logger::registry`).

Out of the box, the library comes with four `logger::sink` implementations:
- `logger::file_sink`: write messages into a file. The following sinks are extending this class
  - `logger::stdout_sink`: send/write messages to the standard output stream (`stdout`)
  - `logger::stderr_sink`: send/write messages to the standard error stream (`stderr`)
- `logger::syslog_sink`: send messages to the `syslog` facility, which is in charge of doing whatever must be done with the messages sent by your application.

Factory methods/functions are in charge of creating and setting up `logger::logger` instances:
```cpp
logger::logger_ptr logger = logger::get<logger::stdout_sink>("consumer-thread");

logger->info("consumer ready to handle incomming messages (status: %s)", "initialized");
```

The logger factory functions are:
- `template logger::get<>(name, args...)` 
- `logger::get(name)`

These factories know how to create a new instance and register them for reuse (using `logger::regisrty::add`). The factory
automatically sets:
- the program name property of the sink
- the current logging level.

> **WARN** it is possible to create loggers without using the factories, but using factories is our the preferred way.

The following functions are affecting all the registered loggers.
- `logger::set_level()`: set the current logging level of all regsitered loggers.
- `logger::set_ecid()`: set the execution correlation ID of registered loggers.

```cpp
logger::set_level(logger::log_level::alert); // from now on, all loggers will only display alert messages or above.

log->info("Hello, world..."); // This it not displayed as log level was set to alert and above.
```
 
### How to use it

#### Create your own logger sink

The library is devided into two parts:
1. One that is ment for the library users, people that just want log things.
2. Another that is in charge of dump messages somewhere.

The first part is adressed through the `logger::logger`interface and the factory methods. The second is addressed by 
extending the abstract class `logger::sink`.

Let's say you need to write messages using QNX's system logger facility. On QNX, logging is done by calling the [`slogf(...)`](http://www.qnx.com/developers/docs/6.3.0SP3/neutrino/lib_ref/s/slogf.html)
function. This function is expecting the following parameters:
1. *opcode*: A combination of a major and minor code. Create the opcode using the `_SLOG_SETCODE(major, minor)` macro that's defined in <sys/slog.h>. The major and minor codes are defined in <sys/slogcodes.h>.
2. *severity*: The severity of the log message; see "Severity levels," below.
3. *fmt*: A standard printf() string followed by printf() arguments.

You can extend `cpp-logger` by writing a sink that would look like this:
```cpp
class slog_sink: public logger::sink {
public:

    explicit slog_sink(int opcode): logger::sink{"slog","app", logger::log_level::info}, _opcode{opcode}{
        // intentional
    }

    void write(logger::log_level level, const char *fmt, ...) override {

        logger::log_level target_level =  this->level(); // level's accessor is threadsafe

        if (target_level >= level) {

            // fill a buffer with the user message
            va_list args1;
            va_list args2;

            va_start(args1, fmt); // initialize va_list
            va_copy(args2, args1); // save a copy args1 -> args2

            // this call only returns the actual number of bytes needed to store the message.
            // It doesn't count the needed end-of-string
            size_t buffer_size = vsnprintf(NULL, 0, fmt, args1) + 1; // plus 1 character to store \0
            va_end(args1); // don't need this one anymore

            // add one more character to handle \n (see code below)
            char buffer[buffer_size + 1];
            memset(buffer, 0, buffer_size + 1);

            // fill buffer with message ...
            vsnprintf(buffer, buffer_size, fmt, args2);
            size_t len = strlen(buffer);
            va_end(args2);

            // add a new line if not already there
            if (buffer[len - 1] != '\n') {
                buffer[len] = '\n'; // override ending \0 (that's why we provisioned for one more character above)
            }

            std::string ecid = this->ecid(); // get current value once.
            std::string pattern = "[L SUBSYS=" + name() + "] %s %s";

            slogf(_opcode,
                  level,
                  _pattern.c_str(),
                  ecid.empty() ? "" : ecid.c_str(),
                  buffer);
        }

    }

private:
    int         _opcode;
};
```

Now, the sink can be used like any other `logger::sink`:
```cpp
logger::logger_ptr logger = logger::get<slog_sink>("slog_test", _SLOG_SETCODE(1, 0));
logger->info( "Tada, you're done");
```

#### Add logging to your program

Sample code can be found in the `tests` directory. It shows how this stuff can be used.

When using this package into your own work, you'll need to give access to the header files and one of these the 
libraries provided. The package contains :
- `include`: all needed headers are isolated into the `logger` subdirectory.
- `lib`: contains a library for static linking and one for dynamic loading (shared object, dynalib, ...).

**More specific exemple will come soon...**

### Misc

#### Using GCOV and LCOV

You can generate coverage infos by using the GCOV option. When passed to cmake, it will build the code (and the unit tests) using the `--coverage`flag. It will also add the gcov library to 
the test programs setup.

```
$ cmake -DGCOV=yes ..
...
$ make all test
```

These commands will build and generate gcov related informations. These files are suffixed with `gcda`and `gcno` and will be used by the lcov command to report.

```
$ lcov --directory CMakeFiles/cpp-logger-static.dir/src/ --capture --output-file coverage.info
Capturing coverage data from CMakeFiles/cpp-logger-static.dir/src/
Found gcov version: 8.3.1
Scanning CMakeFiles/cpp-logger-static.dir/src/ for .gcda files ...
Found 10 data files in CMakeFiles/cpp-logger-static.dir/src/
Processing src/file_sink.cpp.gcda
Processing src/exceptions.cpp.gcda
Processing src/syslog_sink.cpp.gcda
Processing src/stdout_sink.cpp.gcda
Processing src/stderr_sink.cpp.gcda
Processing src/sink.cpp.gcda
Processing src/registry.cpp.gcda
Processing src/logger.cpp.gcda
Processing src/facilities.cpp.gcda
Processing src/cpp-logger.cpp.gcda
Finished .info-file creation
```

The command `lcov --summary coverage.info` displays the coverage information. As you can see, this include the system header files. To limit the report to your code, you can remove unwanted coverage information by using the command `lcov` like this:
```
$ lcov --remove coverage.info '/usr/*' --output-file coverage.info
Reading tracefile coverage.info
Removing /usr/include/c++/8/atomic
...
Removing /usr/include/c++/8/tuple
Removing /usr/include/c++/8/utility
Removing /usr/include/c++/8/x86_64-redhat-linux/bits/gthr-default.h
Deleted 30 files
Writing data to coverage.info
Summary coverage rate:
  lines......: 75.6% (214 of 283 lines)
  functions..: 76.6% (49 of 64 functions)
  branches...: no data found
```

The above command removed everything that comes from the `/usr/*`directory. The final report is obtained with `lcov --summary coverage.info` or `lcov --list coverage.info`:
```
Reading tracefile coverage.info
                                     |Lines       |Functions  |Branches    
Filename                             |Rate     Num|Rate    Num|Rate     Num
===========================================================================
[/shared/home/herbert/c++/cpp-logger/]
include/logger/exceptions.hpp        | 0.0%      6| 0.0%     2|    -      0
include/logger/facilities.hpp        |33.3%      6|33.3%     3|    -      0
include/logger/registry.hpp          | 5.9%     17|33.3%     3|    -      0
include/logger/sinks.hpp             | 100%      3| 100%     3|    -      0
src/cpp-logger.cpp                   | 100%      2| 100%     1|    -      0
src/exceptions.cpp                   | 0.0%      6| 0.0%     3|    -      0
src/facilities.cpp                   | 100%      3| 100%     1|    -      0
src/file_sink.cpp                    |95.1%     61|80.0%     5|    -      0
src/logger.cpp                       | 100%     27| 100%     9|    -      0
src/registry.cpp                     |83.0%     47|78.6%    14|    -      0
src/sink.cpp                         |53.1%     49|80.0%    10|    -      0
src/stderr_sink.cpp                  | 100%      6| 100%     2|    -      0
src/stdout_sink.cpp                  | 100%      6| 100%     2|    -      0
src/syslog_sink.cpp                  |93.2%     44| 100%     6|    -      0
===========================================================================
                               Total:|75.6%    283|76.6%    64|    -      0
```

> **WARN** to use gcov and lcov, you will need to install the related packages (on Fedora `sudo dnf install lcov`)

To get an html report, you can use `genhtml -o coverage-report coverage.info`. This will generate HMLT in the directory specified with the `-o`command line option.

#### Manually create a logger

A logger instance can be created this way:

```cpp
std::string name { "consumer-thread"};

// create a logger instance and pass it a sink instance that handles the actual writting of your messages.
logger::logger_ptr logger{
    new logger::logger{
            name,
            new logger::stdout_sink(name, "APP", logger::log_level::info) // logger::logger is in charge of deleting the sink instance
    }
};

logger->info("consumer ready to handle incomming messages (status: %s)", "initialized");
```

#### project links

* [Project's home](https://redmine.urbix-software.fr/projects/urbix-cpp-logger)
* [GitHub home](https://github.com/HerbertKoelman/cpp-logger)

### misc

* **Author**: herbert koelman(herbert.koelman@me.com)
