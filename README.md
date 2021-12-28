# HTTPparser
A C library to build parse tree for HTTP protocol.

## Setup instructions
**Install dependencies :**
 * [Cutlery](https://github.com/RohanVDvivedi/Cutlery)

**Download source code :**
 * `git clone https://github.com/RohanVDvivedi/HTTPparser.git`

**Build from source :**
 * `cd HTTPparser`
 * `make clean all`

**Install from the build :**
 * `sudo make install`
 * ***Once you have installed from source, you may discard the build by*** `make clean`

## Using The library
 * add `-lhttpparser -lcutlery` linker flag, while compiling your application
 * do not forget to include appropriate public api headers as and when needed. this includes
   * `#include<http_parser.h>`
   * `#include<http_serializer.h>`

## Instructions for uninstalling library

**Uninstall :**
 * `cd HTTPparser`
 * `sudo make uninstall`
