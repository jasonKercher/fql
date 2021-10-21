```none
  __                _
 / _|     __ _     | |
| |_     / _` |    | |
|  _|ile| (_| |uery| |anguage
|_|      \__, |    |_|
            |_|     
```
This is an [ANTLR](https://www.antlr.org/) powered text processing language for Linux.
Similar in spirit to awk which uses a C-like syntax, fql uses SQL (T-SQL specifically).


### What can it do?

fql gives you the power of SQL for your delimited or fixed-width data files
without the need to import them into a database. fql aims to be fast as well
as memory efficient. Most queries should use a relatively low amount of memory.
Keep in mind that memory consumption increases for things like GROUP BY, ORDER BY
and JOIN. Queries are case insensitive so `SELECT * FROM T1 WHERE FOO = 'BAR'` is
the same as `select * from t1 where foo = 'bar'`.


<video src="https://raw.githubusercontent.com/jasonKercher/fql/master/fql.mp4" width="100%"></video>


### What can it *not* do?

- windowed-functions (like RANK)
- RIGHT and FULL JOIN. INNER and LEFT JOIN work.
- TOP PERCENT
- WITH / common table expressions
- COUNT([column]) -- All counts are treated as COUNT(*\*)
- MERGE
- APPLY
- UNION without ALL
- PIVOT/UNPIVOT
- Implicit UPDATE/DELETE into subqueries
- UPDATE/DELETE TOP (...)

Most recent and possibly unstable feature(s):
- fql --thread (-t)


### Is it fast?

Here is a naive benchmark vs other similar projects:
- [textql](https://github.com/dinedal/textql)
- [q](http://harelba.github.io/q)
- [csvsql](https://csvkit.readthedocs.io/en/latest/scripts/csvsql.html)
- [sqlite](https://www.sqlite.org/index.html): Just for comparison, let's import the tables into sqlite ahead of time.
- base benchmark with shell tools


Benchmarks are performed on 2 tables of generic random data of
2 000 000 records (not including header).  The gentsv.sh script
can be used to build these files.

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
fql -t|2.095s
fql|2.620s
shell tools|6.435s
sqlite3|20.560s
q|53.742s
csvsql|2m50.405s
textql|N/A #
csvq|N/A #

2. COUNT(*) GROUP BY bar

program|time
---|---
shell tools|0.630s
fql -t|0.842s
fql|0.980s
sqlite3|1.160s
csvq|4.213s
q|15.405s
textql|17.690s
csvsql|1m57.014s


3. SELECT with LIKE '%aa[0-9]aa%'

program|time
---|---
sqlite3|0.211s *
shell tools|0.470s
fql -t|0.738s
fql|0.745s
csvq|4.089s
q|13.737s *
textql|14.154s
csvsql|1m56.471s *


4. ORDER BY

program|time
---|---
sqlite3|2.105s
shell tools|3.125s *
fql -t|5.654s
fql|5.779s
csvq|11.926s
textql|22.980s
q|42.524s
csvsql|2m6.498s

\* See benchmark.sh for additional notes

### Installation

**Arch Linux**
If you are using an Arch Linux based distribution, fql can be retrieved from the [AUR](https://aur.archlinux.org/packages/fql/).

**Compiling from source**

*Requirements*
- [libcsv](https://github.com/jasonKercher/libcsv): for reading and writing correct csv files based
on [RFC 4180](https://www.ietf.org/rfc/rfc4180.txt)
- [antlr4 C++ runtime library](https://github.com/antlr/antlr4/tree/master/runtime/Cpp): This is available though
pacman (`pacman -S antlr4-runtime`) if you have an Arch Linux based distribution.  I struggled getting this
installed on Ubuntu and just wound up compiling it.
- [libpcre2](https://www.pcre.org/): for LIKE statement implementation
- [libcheck](https://github.com/libcheck/check): This is only for `make check`.

Once antlr4 runtime is installed, we need to tell the configure script where to find the antlr header files.
These headers are installed at `${PREFIX}/include/antlr4-runtime` where `$PREFIX` is the install prefix for
the antlr4-runtime. So we need to set ANTLR4_CPATH (`ANTLR4_CPATH=${PREFIX}/include`) Rather than searching
for it manually, you can use the find_antlr_cpath.sh script to find it for you:
```sh
ANTLR4_CPATH=$(./find_antlr_cpath.sh) ./configure
# Or if you know what it is...
ANTLR4_CPATH=/usr/lib ./configure
make
make check   # optional
make install
```

Installed program: fql

Installed header: fql.h

Installed library: libfql.so

### Library

The `fql` program was designed to work within a shell environment, however, it can also be utilized
as a library. In fact, the `fql` program is really just a thin wrapper for `fql_exec` library
function. Here is a quick example of how to use the library API:

```c
#include <stdlib.h>
#include "fql.h"

int main(int argc, char** argv)
{
	struct fql_handle* handle = fql_new();
	int ret = fql_make_plans(handle,
	                         "select t2.*           "
	                         "from t1               "
	                         "left join t2          "
	                         "    on t1.foo = t2.foo");

	if (ret == FQL_FAIL) {
		fql_free(handle);
		return EXIT_FAILURE;
	}

	// In case we don't know how many fields will return...
	int field_count = fql_field_count(handle);

	// fql_step will initialize this for us
	struct fql_field* fields = NULL;

	// fql_step returns...
	//     FQL_FAIL: runtime error
	//     0:        complete
	//     1:        still running
	while ((ret = fql_step(handle, &fields)) == 1) {

		// Based on the query (it's a LEFT JOIN), there
		// is a chance this field could return NULL. We
		// can check for this with the is_null flag...
		if (fields[0].is_null) {
			// do something about it...
			continue;
		}

		// Since we are using the default schema,
		// we can assume this is of type FQL_STRING.
		// If we had to check though...
		if (fields[0].type == FQL_STRING) {
			const char* foo = fields[0].data.s;
			// foo now contains the first value of
			// the first field returned by the query
		}
	}

	fql_free(handle);
	return ret;
}
```
