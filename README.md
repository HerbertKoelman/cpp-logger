### What it does

[cpp-logger](https://github.com/HerbertKoelman/cpp-logger) is a set of very simple classes that handles logging. Yes ! I know, yet one more. And you're right.

The library was first implemented for AIX IBM's Unix, on which existing libraries did not compile. So I had to write my own thing. 

Today, this work is mainly used to train C++ beginners. That's why, I keep maintaining cpp-logger.

To use this library:

    $ mkdir build
    $ cd build
    $ cmake .. && make all
    ...
    $ make install
    ...

Install moves files into your system's default location for headers and libraries (often /usr/local/include and /usr/local/lib). You can relocate installation by setting CMAKE_INSTALL_PREFIX 
cmake property.

You can run the provide unit tests with this command `make all tests`. Google test will only display if the tests did run fine or not. If you need more you can find it in 
`<build-dir>/tests/Testing/Temporary/`. Probably you'll be interested in opening this one : `LastTest.log`.
     
     $ tail LastTest.log
     ...
     Running main() from /Users/herbert/Documents/c++/cpp-logger/cmake-build-release/googletest-src/googletest/src/gtest_main.cc
     [==========] Running 4 tests from 1 test case.
     [----------] Global test environment set-up.
     [----------] 4 tests from exceptions
     [ RUN      ] exceptions.logger_exception
     [       OK ] exceptions.logger_exception (0 ms)
     [ RUN      ] exceptions.logger_exception_message
     [       OK ] exceptions.logger_exception_message (0 ms)
     [ RUN      ] exceptions.sink_exception
     [       OK ] exceptions.sink_exception (0 ms)
     [ RUN      ] exceptions.facility_exception
     [       OK ] exceptions.facility_exception (0 ms)
     [----------] 4 tests from exceptions (0 ms total)
     
     [----------] Global test environment tear-down
     [==========] 4 tests from 1 test case ran. (0 ms total)
     [  PASSED  ] 4 tests.
     <end of output>
     Test time =   0.00 sec
     ----------------------------------------------------------
     Test Passed.
     "sink_tests" end time: Apr 22 13:39 CEST
     "sink_tests" time elapsed: 00:00:00
     ----------------------------------------------------------
     
     End testing: Apr 22 13:39 CEST

The module come with some performance tests. These tests can be run manualy by issuing a command such as the following:
     
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

Doxygen documentation can be generated with this target.

    make doxygen

> Doxygen can be downloaded [here](http://www.stack.nl/~dimitri/doxygen/index.html).

The package comes with some unit testing that are built and can be run through the target **test**. The tests are depending on [GoogleTest 1.8.1](https://github.com/google/googletest), the package 
is automatically downloaded using the CMake script `cmake/GTestExtConfig.cmake`. This means you need to have access to the Internet. So if that's not an option, you can disable testing by 
setting the option `BUILD_TESTS` to **no** like this:

    mkdir build
    cd build
    cmake -DBUILD_TESTS=no .. && make all
    make install

Here after a list of build targets usefull to mention:
- test : run all registered units programs.
- package: create an archive (tar.gz)
- doxygen: create doxygen html documentation (in <build-dir>/html)

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
  - Compiler: 8.3.1 20190223 (Red Hat 8.3.1-2)
  - OS version: 4.20.16-100.fc28.x86_64
  - Performance: 
    - CPU: Intel(R) Celeron(R) 2957U @ 1.40GHz
    - Memory: 8GB
    - Command: time ./logger_performance_tests > /dev/null
    - Result: 0m0,249s (user: 0m0,172s,sys:0m0,056s)
  
> Coming soon, Ubuntu 16 workstation.  
  
### How to use it

Sample code can be found in the `tests` directory. It shows how this stuff can be used.

When using this package into your own work, you'll need to give access to the header files and one of these the 
libraries provided. The package contains :
- `include`: all needed headers are isolated into the `logger` subdirectory.
- `lib`: contains a library for static linking and one for dynamic loading (shared object, dynalib, ...).

**More specific exemple will come soon...**

### Usefull links

#### project links

* [Project's home](https://redmine.urbix-software.fr/projects/urbix-cpp-logger)
* [GitHub home](https://github.com/HerbertKoelman/cpp-logger)
* [Project's doxygen]()

### misc

* **Author**: herbert koelman(herbert.koelman@me.com)
