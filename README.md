# HTTPparser
A C library to parse HTTP 1.0 and 1.1 protocol over a stream (ConnMan stream). It contains a (ConnMan based) stream parsing and serializing api.

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
 * add `-lhttpparser -lconnman -lcutlery -lssl -lz` linker flag, while compiling your application
 * do not forget to include appropriate public api headers as and when needed. this includes
   * `#include<httpparser/http_parser.h>`
   * `#include<httpparser/http_serializer.h>`
   * `#include<httpparser/http_body_stream.h>`
   * `#include<httpparser/init_content_encoding_streams.h>`
   * `#include<httpparser/file_handling_util.h>`

## Instructions for uninstalling library

**Uninstall :**
 * `cd HTTPparser`
 * `sudo make uninstall`
