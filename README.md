# HTTPparser
A C library to parse HTTP 1.0 and 1.1 protocol over a stream (ConnMan stream). It contains a (ConnMan based) stream parsing and serializing api.

This library in its current form does not support HTTP 2 or 3. This might change with further development.

## Setup instructions
**Install dependencies :**
 * [Cutlery](https://github.com/RohanVDvivedi/Cutlery)
 * [ConnMan](https://github.com/RohanVDvivedi/ConnMan)

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
   * `#include<http_body_stream.h>`

## Instructions for uninstalling library

**Uninstall :**
 * `cd HTTPparser`
 * `sudo make uninstall`
