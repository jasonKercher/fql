```
  __       _
 / _| __ _| |
| |_ / _` | |
|  _| (_| | |
|_|  \__, |_|
        |_|
```
This is an [ANTLR](https://www.antlr.org/) powered text processing language for Linux.  Similar in spirit to awk which uses a C-like syntax, fql uses SQL (T-SQL specifically).


### What can it do?

fql gives you the power of SQL for your delimited or fixed-width data files without the need to import them into a database.  fql aims to be fast as well as memory efficient.  Most queries should use relatively a low amount of memory. Keep in mind that memory consumption increases for things like GROUP BY, ORDER BY and JOIN. Queries are case insensitive so `SELECT * FROM T1 WHERE FOO = 'BAR'` is the same as `select * from t1 where foo = 'bar'`. Currently, fql can only do SELECT statements though it should be able to handle any amount of complexity with a handful of exceptions.

### What can it *not* do?

There is not yet any concept of NULL.  Because of this LEFT, RIGHT and FULL JOIN are not available.  However, there is a workaround for single key joins.  This will not (currently) work:

```sql
select t1.*
from t1
left join t2
	on t1.foo = t2.foo
where t2.foo is NULL
```

For now, just use an IN statement with a subquery:

```sql
select *
from t1
where foo not in (select foo from t2)
```

There is an interface for scalar and aggregate functions, but many functions are not yet implemented. Speaking of not implemented functions, I never implemented cast. Until then, you can always implicitly cast:

```sql
select number_field + 0 from [file.txt] -- int
select number_field + 0.0 from [file.txt] -- float
select left(not_number_field, 9999999) from [file.txt] -- lol string
```

Other missing features:
 - windowed-functions
 - temp tables not so temp
 - TOP PERCENT
 - WITH
 - UNION
 - COUNT(DISTINCT [col])
 - CASE statements

### Is it fast?

Here is a naive benchmark vs other similar projects:
- [textql](https://github.com/dinedal/textql)
- [q](http://harelba.github.io/q)
- [csvsql](https://csvkit.readthedocs.io/en/latest/scripts/csvsql.html)
- [sqlite](https://www.sqlite.org/index.html): Just for comparison, let's import the tables into sqlite ahead of time.
- base benchmark with shell tools


Benchmarks are performed on 2 tables of generic random data of 2 000 000 records (not including header).  The gencsv.sh script can be used to build these files: `./gencsv.sh 2000001 > t1.temp` and `./gencsv.sh 2000001`:

```sh
:) wc -l t2.temp
2000000 t2.temp
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
fql|4.230s
shell tools|6.133s
sqlite3|20.275s
q|53.986s
csvsql|2m50.405s
textql|N/A #

2. `COUNT(*)` GROUP BY bar

program|time
---|---
shell tools|0.635s
sqlite3|1.138s
fql|1.503s
textql|15.582s
q|17.319s
csvsql|1m57.014s


3. SELECT with LIKE '%aa[0-9]aa%'

program|time
---|---
sqlite3|0.206s *
shell tools|0.417s
fql|1.185s
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




