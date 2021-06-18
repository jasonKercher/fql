```
  __       _
 / _| __ _| |
| |_ / _` | |
|  _| (_| | |
|_|  \__, |_|
        |_|
```
This is an [ANTLR](https://www.antlr.org/) powered text processing language for Linux.  Similar in spirit to awk which uses a C-like syntax, fql uses SQL (T-SQL specifically).  From `fql --help`:

```
Usage fql -[bCdhHOpv] [-sS delim] [--strict] [file.sql]

This is a text processing program using SQL (Transact-SQL specifically).

If no file argument is provided, queries are read from stdin. Files are
referenced as tables in the query via a priority list. For example, given
the following query: 'SELECT * FROM t1', we match files on these rules:

    RULE                   EXAMPLE MATCHES
    exact match            t1
    exact ignoring case    T1,t1
    ignore extension       t1.txt,t1.csv
    ignore extension/case  t1.txt,T1.csv

To match t1.txt exactly (or match any filename with dots in them), use
'select * from [t1.txt]'. Without the [], the parser is going to assume
t1 is the schema for a table txt. Do the same for a filename with
white space as the parser will treat this as '[TABLE] [ALIAS]'.

Optional arguments:
 -b, --char-as-byte   scalar functions assume single-byte encoding
 -C, --cartesian      use cartesian join algorithm (less memory use)
 -d, --dry-run        validate and build a plan, but do not execute
 -h, --no-header      for default schema, do not print a header
 -O, --override       allow processing of unsupported language features
 -p, --print          print the processing plan
 -s, --in-delim arg   for default schema, specify an input seperator
 -S, --out-delim arg  for default schema, speficy seperator for SELECT
 -v, --verbose        print additional information to stderr
 --strict             see strict rules below...

Strict mode will only allow exact matches to files to be used as tables.
It will also throw errors if you try to select foo from a file with 2:
foo,bar,foo
a,b,c
```
### Installation
Bear with me because I'm new to autotools.  There is a configure script, but it is not checking everything it should.

**Requirements**
 - [libcsv](https://github.com/jasonKercher/libcsv): for reading and writing correct csv files based on [RFC 4180](https://www.ietf.org/rfc/rfc4180.txt)
 - [antlr4 C++ runtime library](https://github.com/antlr/antlr4/tree/master/runtime/Cpp): This is available though pacman (`pacman -S antlr4-runtime`) if you have an Arch Linux based distribution.  I struggled getting this installed on Ubuntu and just wound up compiling it.
 - [libpcre2](https://www.pcre.org/): for LIKE statement implementation
 - [libcheck](https://github.com/libcheck/check): This is only for `make check`.

Once antlr4 runtime is installed, you need determine the location of the antlr4-runtime headers since my shit autotools skills won't do it for you.  The parent directory must be set to `ANTLR4_CPATH`.  For me, this is `/usr/include`
```
ANTLR4_CPATH=/usr/include ./configure
make
make check #optional
make install
```

Installed program: fql

Installed header: fql.h

Installed library: libfql.so

For information on how to use the library API see the lib/include/fql.h.

