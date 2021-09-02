```c
  __       _
 / _| __ _| |
| |_ / _` | |
|  _| (_| | |
|_|  \__, |_|
        |_|
```
This is an [ANTLR](https://www.antlr.org/) powered text processing language for Linux.  
Similar in spirit to awk which uses a C-like syntax, fql uses SQL (T-SQL specifically).


### What can it do?

fql gives you the power of SQL for your delimited or fixed-width data files 
without the need to import them into a database. fql aims to be fast as well 
as memory efficient. Most queries should use relatively a low amount of memory. 
Keep in mind that memory consumption increases for things like GROUP BY, ORDER BY 
and JOIN. Queries are case insensitive so `SELECT * FROM T1 WHERE FOO = 'BAR'` is 
the same as `select * from t1 where foo = 'bar'`.

### What can it *not* do?

- windowed-functions (like RANK)
- temp tables not so temp
- TOP PERCENT
- WITH / common table expressions
- COUNT([column]) -- All counts are treated as COUNT(*)
- MERGE
- APPLY
- UNION without ALL
- PIVOT/UNPIVOT
- Implicit UPDATE/DELETE into subquery

Most recent and possibly unstable features:
- UPDATE 
- DELETE
  
### Is it fast?

Here is a naive benchmark vs other similar projects:
- [textql](https://github.com/dinedal/textql)
- [q](http://harelba.github.io/q)
- [csvsql](https://csvkit.readthedocs.io/en/latest/scripts/csvsql.html)
- [sqlite](https://www.sqlite.org/index.html): Just for comparison, let's import the tables into sqlite ahead of time.
- base benchmark with shell tools


Benchmarks are performed on 2 tables of generic random data of 
2 000 000 records (not including header).  The gentsv.sh script 
can be used to build these files:

```sh
:) ./gentsv.sh 2000000 > t2.temp
:) wc -l t2.temp
2000001 t2.temp
:) head -10 t2.temp
foo     bar     baz
f493263f        5b      7791
c79cfff4        e0      19075
3157f48c        8e      83146
7f34ca2e        82      19950
a464be18        97      95934
bb193135        6c      38038
c6c04cf4        57      185937
a36ab33b        5b      45490
a05c7214        b1      143027
```

**RESULTS**

1. JOIN t1 to t2 on foo

program|time
---|---
fql|3.568s
shell tools|6.133s
sqlite3|20.275s
q|53.986s
csvsql|2m50.405s
textql|N/A #

2. `COUNT(*)` GROUP BY bar

program|time
---|---
shell tools|0.635s
fql|1.113s
sqlite3|1.138s
textql|15.582s
q|17.319s
csvsql|1m57.014s


3. SELECT with LIKE '%aa[0-9]aa%'

program|time
---|---
sqlite3|0.206s *
shell tools|0.417s
fql|0.762s
q|13.737s *
textql|14.154s
csvsql|1m56.471s *


4. ORDER BY

program|time
---|---
sqlite3|2.105s
shell tools|3.125s *
fql|6.753s
textql|22.980s
q|49.119s
csvsql|2m6.498s

\* See benchmark.sh for additional notes

### Installation
**Requirements**
- [libcsv](https://github.com/jasonKercher/libcsv): for reading and writing correct csv files based on [RFC 4180](https://www.ietf.org/rfc/rfc4180.txt)
- [antlr4 C++ runtime library](https://github.com/antlr/antlr4/tree/master/runtime/Cpp): This is available though pacman (`pacman -S antlr4-runtime`) if you have an Arch Linux based distribution.  I struggled getting this installed on Ubuntu and just wound up compiling it.
- [libpcre2](https://www.pcre.org/): for LIKE statement implementation
- [libcheck](https://github.com/libcheck/check): This is only for `make check`.

Once antlr4 runtime is installed, we want to use the find_antlr_cpath.sh script to set ANTLR4_CPATH for the configure script:
```sh
ANTLR4_CPATH=$(./find_antlr_cpath.sh) ./configure
make
make check   # optional
make install
```

Installed program: fql

Installed header: fql.h

Installed library: libfql.so

### Library

Here is a quick example of how to use the library API:

```c
#include <stdlib.h>
#include "fql.h"

int main(int argc, char** argv)
{
	struct fql_handle* handle = fql_new();
	int ret = fql_make_plans(handle,
	                         "select t1.foo "
	                         "from t1 "
	                         "join t2 "
	                         "    on t1.foo = t2.foo");

	if (ret == FQL_FAIL) {
		return EXIT_FAILURE;
	}

	// In case we don't know. We know
	// it is 1 based on the query...
	int field_count = fql_field_count(handle);

	// fql_step will initialize this for us
	struct fql_field* fields = NULL;

	// fql_step returns:
	// 0 at EOF
	// FQL_FAIL on runtime error
	while ((ret = fql_step(handle, &fields)) == 1) {
		// Since we are pulling from a file, we
		// can assume this is of type FQL_STRING
		if (fields[0].type == FQL_STRING) {
			const char* foo = fields[0].data.s;
			// Do something with foo
		}
	}
	if (ret == FQL_FAIL) {
		return EXIT_FAILURE;
	}

	fql_free(handle);
}
```




