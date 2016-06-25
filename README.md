### What it does

A set of very simple classes that handles message logging.

To use this library:

    configure
    make
    make install

Install moves files into your system's default localtion for headers and libraries (often /usr/local/include and /usr/local/lib). Use this command to change install target directory:

    configure --prefix=/usr/local

Doxygen [documentation]() can be generated with this command. I hope this help make things easier to use and understand.

    make doxygen

> Doxygen can be downloaded [here](http://www.stack.nl/~dimitri/doxygen/index.html).

The `make` target `pkg` will produce au tar.gz that can be distributed.

### How to use it

Sample code can be found in the `tests` directory. To use it, run the following commands:

  cd tests
  ./configure
  ./make

### Dependencies

* [cpp-pthread](https://github.com/HerbertKoelman/cpp-pthread)

### Usefull links

#### Memory management on AIX

Memory management on AIX is quite sophisticated, memory managementcan be fine tuned very precisely. Consider using these compiler/linker options when using pthreads:
* -bmaxdata:0xN0000000 this option activates the large memory model, N is a number in the range of [1-8].
* -bmaxmem=-1 this option tells the compiler to use as much memory it needs to optimize your code.

Thread stack size:
* 32bits programs allocate 96KB per thread on the program's heap.
* 64bits programs allocate 192KB per thread on the program's heap.

On many Linux implementations and on Mac OS X the stack size is defaulted to 8MB. You may consider setting this as a default.

More detailed information can be found in this [RedBook](http://www.redbooks.ibm.com/redbooks/pdfs/sg245674.pdf) (chapter 8).

#### project links

* [project's home]()
* [project's doxygen]()

### misc

* author herbert koelman(herbert.koelman@pmu.fr)
