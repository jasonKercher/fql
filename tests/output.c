#include "output.h"
#include "fql.h"
#include <stdio.h>

int _testdiff(const char* s)
{
	char filename0[256];
	char filename1[256];
	sprintf(filename0, "control/%s", s);
	sprintf(filename1, "results/%s", s);

	FILE* file0 = fopen(filename0, "r");
	if (file0 == NULL) {
		perror(filename0);
		return 1;
	}
	FILE* file1 = fopen(filename1, "r");
	if (file1 == NULL) {
		perror(filename1);
		return 1;
	}

	int i0 = 0;
	int i1 = 0;
	int ret = 0;

	while (i0 != EOF) {
		i0 = getc(file0);
		i1 = getc(file1);
		ck_assert_int_eq(i0, i1);
	}

	fclose(file0);
	fclose(file1);

	return ret;
}

void output_const(struct fqlhandle* fql)
{
	const char* query = "\n\
		declare @i int = 0                                          \n\
		while @i < 2                                                \n\
		BEGIN                                                       \n\
		    select 1 into [results/const1]                          \n\
		    select 1.1 into [results/const2]                        \n\
		    select 'x''' into [results/const3]                      \n\
		    select 3 / 2 into [results/const4]                      \n\
		    select 1.1 + 1 into [results/const5]                    \n\
		    select 1 * 2.0 / (3 + 4.0) into [results/const6]        \n\
		    select 123 * -2.0 / (-3 + 4.1) into [results/const7]    \n\
		    select '13' / 5 into [results/const8]                   \n\
		    select '1' / 3.0 into [results/const9]                  \n\
		    select '4' + '5' into [results/const10]                 \n\
		    select '1' + '9' + 5 into [results/const11]             \n\
		    select -9223372036854775806 + -2 into [results/const12] \n\
		    select -1 into [results/const13]                        \n\
		    select +1.1 into [results/const14]                      \n\
		    select -(1 * 2) into [results/const15]                  \n\
		    select ~1 into [results/const16]                        \n\
		    select left('testing fql', 4) into [results/const17]    \n\
		    select left('αβΩ', 1) into [results/const18]            \n\
		    select right('testing fql', 4) into [results/const19]   \n\
		    select right('αβΩ', 1) into [results/const20]           \n\
		    set @i += 1                                             \n\
		END";
	ck_assert_int_eq(fql_exec(fql, query), FQL_GOOD);

	unsigned i = 1;
	for (; i <= 20; ++i) {
		char filename[64];
		sprintf(filename, "const%u", i);
		ck_assert_int_eq(_testdiff(filename), 0);
	}
}

void output_read(struct fqlhandle* fql)
{
	const char* query = "\n\
		declare @i int = 0                                               \n\
		while @i < 2                                                     \n\
		BEGIN                                                            \n\
		    select foo, bar, baz into [results/read1] from t1            \n\
		    select foo into [results/read2] from [../parent]             \n\
		    select foo into [results/read3] from [sub/sub]               \n\
		    select * into [results/read4] from t1                        \n\
		    select 5 into [results/read5] from t1                        \n\
		    select foo + bar, baz / 10, baz into [results/read6] from t1 \n\
		    select -(baz+0) into [results/read7] from t1                 \n\
		                                                                 \n\
		    select foo, left(right(foo, 5), 2)                           \n\
		    into [results/read8]                                         \n\
		    from t1                                                      \n\
		    set @i += 1                                                  \n\
		END";
	ck_assert_int_eq(fql_exec(fql, query), FQL_GOOD);

	unsigned i = 1;
	for (; i <= 8; ++i) {
		char filename[64];
		sprintf(filename, "read%u", i);
		ck_assert_int_eq(_testdiff(filename), 0);
	}
}

void output_logic(struct fqlhandle* fql)
{
	const char* query = "\n\
		declare @i int = 0                                                         \n\
		while @i < 2                                                               \n\
		BEGIN                                                                      \n\
		    select 1 into [results/logic1] where '01' = 1                          \n\
		    select 1 into [results/logic2] where -1 = 1                            \n\
		    select 1 into [results/logic3] where 50.5 = '050.5'                    \n\
		    select 1 into [results/logic4] where 50.4 = 50.5                       \n\
		    select 1 into [results/logic5] where 'hi' = 'Hi   '                    \n\
		    select 1 into [results/logic6] where 'hi' = ' hi'                      \n\
		    select 1 into [results/logic7] where 1 != 0                            \n\
		    select 1 into [results/logic8] where 9223372036854775807 <>            \n\
		    9223372036854775807                                                    \n\
		    select 1 into [results/logic9] where -.0001 != -.00011                 \n\
		    select 1 into [results/logic10] where 33.333 <> 33.333                 \n\
		    select 1 into [results/logic11] where 'abcde f' != 'abcde'             \n\
		    select 1 into [results/logic12] where '' <> ''                         \n\
		    select 1 into [results/logic13] where 5 > -6                           \n\
		    select 1 into [results/logic14] where -9223372036854775808 >           \n\
		    9223372036854775807                                                    \n\
		    select 1 into [results/logic15] where 1.1 > 1                          \n\
		    select 1 into [results/logic16] where -1.1 > 1                         \n\
		    select 1 into [results/logic17] where 'Bad' > 'abc'                    \n\
		    select 1 into [results/logic18] where 'A' > '{'                        \n\
		    select 1 into [results/logic19] where 5 >= 5                           \n\
		    select 1 into [results/logic20] where 5 >= '4'                         \n\
		    select 1 into [results/logic21] where -1 >= 1                          \n\
		    select 1 into [results/logic22] where -22.1 >= -31                     \n\
		    select 1 into [results/logic23] where 22.1 >= 22.1                     \n\
		    select 1 into [results/logic24] where -22.10001 >= -22.1               \n\
		    select 1 into [results/logic25] where 'abc' >= '[c]   '                \n\
		    select 1 into [results/logic26] where 'abc' >= 'abc   '                \n\
		    select 1 into [results/logic27] where 'abc' >= 'abc d'                 \n\
		    select 1 into [results/logic28] where -1 < 1                           \n\
		    select 1 into [results/logic29] where '1' < -1                         \n\
		    select 1 into [results/logic30] where 5.992 < 5.993                    \n\
		    select 1 into [results/logic31] where -11 < -12.1                      \n\
		    select 1 into [results/logic32] where 'shnt' < 'TNHS'                  \n\
		    select 1 into [results/logic33] where 'shnt' < 'SHNT'                  \n\
		    select 1 into [results/logic34] where 1 <= 1                           \n\
		    select 1 into [results/logic35] where -12341 <= 1                      \n\
		    select 1 into [results/logic36] where 0 <= -1                          \n\
		    select 1 into [results/logic37] where -888.1234 <= 888                 \n\
		    select 1 into [results/logic38] where 888.0 <= 888                     \n\
		    select 1 into [results/logic39] where 888.1234 <= 888                  \n\
		    select 1 into [results/logic40] where 'hello world!' <= 'hello world!' \n\
		    select 1 into [results/logic41] where 'hello world' <= 'hello world!'  \n\
		    select 1 into [results/logic42] where 'hello_world!' <= 'hello world!' \n\
		    select 1 into [results/logic43] where 'sh.nt' like '%.%'               \n\
		    select 1 into [results/logic44] where 'sh.nt' not like '%.%'           \n\
		    select 1 into [results/logic45] where 's' in ('qcn', 's')              \n\
		    select 1 into [results/logic46] where 1 in (1, 78)                     \n\
		    select 1 into [results/logic47] where '' in (' n', '565')              \n\
		    select 1 into [results/logic48] where '' in (' n', '      ')           \n\
		    select 1 into [results/logic49] where left('hello', 2) = 'he  '        \n\
		    select 1 into [results/logic50] where 1000 + 233 <                     \n\
		    right('hi1234', 4)                                                     \n\
		    select 1 into [results/logic51] where 100 * -1 > 0 / 1000              \n\
		    select 1 into [results/logic52] where 1=1 and 1=1                      \n\
		    select 1 into [results/logic53] where 1=1 or 1=0                       \n\
		    select 1 into [results/logic54] where 1=1 and 1=0                      \n\
		    select 1 into [results/logic55] where 1=0 or 1=1 and 1=0               \n\
		    select 1 into [results/logic56] where 1=1 or 1=1 and 1=0               \n\
		    select 1 into [results/logic57] where (1=1 or 1=1) and 1=0             \n\
		    select 1 into [results/logic58] where 1=0 and 1=1 or 1=1               \n\
		    select 1 into [results/logic59] where (1=1 and 1=0) or (1=1 and        \n\
		    1=0)                                                                   \n\
		    select 1 into [results/logic60] where (1=1 and 1=0) or (1=1 and        \n\
		    1=1)                                                                   \n\
		    set @i += 1                                                            \n\
		END";

	ck_assert_int_eq(fql_exec(fql, query), FQL_GOOD);

	unsigned i = 1;
	for (; i <= 60; ++i) {
		char filename[64];
		sprintf(filename, "logic%u", i);
		ck_assert_int_eq(_testdiff(filename), 0);
	}
}

void output_scalar(struct fqlhandle* fql)
{
	const char* query = "\n\
		declare @i int = 0                                                \n\
		while @i < 2                                                      \n\
		BEGIN                                                             \n\
		    select len('shnt') into [results/scalar1]                     \n\
		    select len('shnt   ') into [results/scalar2]                  \n\
		    select len('♜♞ ') into [results/scalar3]                      \n\
		    select len(100) into [results/scalar4]                        \n\
		    select len('♜♞ ') into [results/scalar5]                      \n\
		    select datalength('shnt') into [results/scalar6]              \n\
		    select datalength('shnt   ') into [results/scalar7]           \n\
		    select datalength('♜♞ ') into [results/scalar8]               \n\
		    select datalength(100) into [results/scalar9]                 \n\
		    select datalength(100.0) into [results/scalar10]              \n\
		    select left('shnt', 3) into [results/scalar11]                \n\
		    select left('shnt  ', 12) into [results/scalar12]             \n\
		    select left('♗♕♔♗♘', 3) into [results/scalar13]               \n\
		    select left('♗♕♔♗♘', 3) into [results/scalar14]               \n\
		    select right('shnt ', 3) into [results/scalar15]              \n\
		    select right('shnt  ', 12) into [results/scalar16]            \n\
		    select right('♗♕♔♗♘', 3) into [results/scalar17]              \n\
		    select right('♗♕♔♗♘', 3) into [results/scalar18]              \n\
		    select substring('shnt ', 3, 1) into [results/scalar19]       \n\
		    select substring('shnt  ', 12, 20) into [results/scalar20]    \n\
		    select substring('shnt  ', 3, 20) into [results/scalar21]     \n\
		    select substring('shnt', -1, 3) into [results/scalar22]       \n\
		    select substring('♗♕♔♗♘', 3, 2) into [results/scalar23]       \n\
		    select substring('♗♕♔♗♘', 4, 3) into [results/scalar24]       \n\
		    select replace('shnt', 'nt', 'xx') into [results/scalar25]    \n\
		    select replace('shnt', 'xx', 'nt') into [results/scalar26]    \n\
		    select replace('Haters gonna hate', 'hate', 'love')           \n\
		    into [results/scalar27]                                       \n\
		    select replace('Hello World', 'world', 'Entire Planet!')      \n\
		    into [results/scalar28]                                       \n\
		    select replace('Hello Entire Planet!', 'planet', 'Room')      \n\
		    into [results/scalar29]                                       \n\
		    select cast(1234 as bit) into [results/scalar30]              \n\
		    select cast('0' as bit) into [results/scalar31]               \n\
		    select cast(-1 as bit) into [results/scalar32]                \n\
		    select cast(55 as int) into [results/scalar33]                \n\
		    select cast('1234' as int) into [results/scalar34]            \n\
		    select cast('-000001234' as int) into [results/scalar35]      \n\
		    select cast('0x7F' as int) into [results/scalar36]            \n\
		    select cast(5.0001 as int) into [results/scalar37]            \n\
		    select cast(55.0 as float) into [results/scalar38]            \n\
		    select cast('1234' as float) into [results/scalar39]          \n\
		    select cast('-000001234' as float) into [results/scalar40]    \n\
		    select cast('6.12366' as float) into [results/scalar41]       \n\
		    select cast(55 as varchar) into [results/scalar42]            \n\
		    select cast('shnt' as varchar(3)) into [results/scalar43]     \n\
		    select cast(55 as char) into [results/scalar44]               \n\
		    select cast('shnt' as char(3)) into [results/scalar45]        \n\
		    select cast('shnt' as char(10)) into [results/scalar46]       \n\
		    set @i += 1                                                   \n\
		END";

	ck_assert_int_eq(fql_exec(fql, query), FQL_GOOD);

	unsigned i = 1;
	for (; i <= 46; ++i) {
		char filename[64];
		sprintf(filename, "scalar%u", i);
		ck_assert_int_eq(_testdiff(filename), 0);
	}
}

void output_switch(struct fqlhandle* fql)
{
	const char* query = "\n\
		declare @i int = 0              \n\
		while @i < 2                    \n\
		BEGIN                           \n\
		    select case 0               \n\
		        when 1 then 2           \n\
		        when 0 then 3           \n\
		        end                     \n\
		    into [results/switch1]      \n\
		                                \n\
		    select case                 \n\
		        when 0=1 then 2         \n\
		        when 0=0 then 3         \n\
		        end                     \n\
		    into [results/switch2]      \n\
		                                \n\
		    select 1                    \n\
		    into [results/switch3]      \n\
		    where 2 = case              \n\
		            when 1=1 then 1     \n\
		            else 0 end          \n\
		                                \n\
		    select 1                    \n\
		    into [results/switch4]      \n\
		    where 2 = case              \n\
		            when 1=1 then 2     \n\
		            else -1 end         \n\
		                                \n\
		    select case                 \n\
		        when 1 = 2 then 0       \n\
		        when 2 = 3 then 1       \n\
		        else 8 end              \n\
		    into [results/switch5]      \n\
		                                \n\
		    select case                 \n\
		        when 'x' = 'y' then 1   \n\
		        else '2' end            \n\
		    into [results/switch6]      \n\
		                                \n\
		    select case                 \n\
		        when 'x' = 'x' then '2' \n\
		        else 1 end              \n\
		    into [results/switch7]      \n\
		                                \n\
		    select case                 \n\
		        when 1=1 then '2'       \n\
		        else '3' end            \n\
		    into [results/switch8]      \n\
		                                \n\
		    select case 'a'             \n\
		        when 'b' then '5'       \n\
		        when 'x' then 2         \n\
		        when 'a' then '1' end   \n\
		    into [results/switch9]      \n\
		    set @i += 1                 \n\
		END";

	ck_assert_int_eq(fql_exec(fql, query), FQL_GOOD);

	unsigned i = 1;
	for (; i <= 9; ++i) {
		char filename[64];
		sprintf(filename, "switch%u", i);
		ck_assert_int_eq(_testdiff(filename), 0);
	}
}

void output_null(struct fqlhandle* fql)
{
	const char* query = "\n\
		declare @i int = 0                                             \n\
		while @i < 2                                                   \n\
		BEGIN                                                          \n\
		    select NULL into [results/null1]                           \n\
		    select 1 + NULL into [results/null2]                       \n\
		    select NULL + 1 into [results/null3]                       \n\
		    select left('00000000' + cast(5 * (1+NULL) as varchar), 8) \n\
		    into [results/null4]                                       \n\
		    select isnull(1 + NULL, NULL) into [results/null5]         \n\
		    select 1 into [results/null6] where NULL - 4 is NULL       \n\
		    select 1 into [results/null7] where NULL - 4 is not NULL   \n\
		    select 1 into [results/null8] where 6 - 4 is not NULL      \n\
		    select 1 into [results/null9] where 6 - 4 is NULL          \n\
		    set @i += 1                                                \n\
		END";

	ck_assert_int_eq(fql_exec(fql, query), FQL_GOOD);

	unsigned i = 1;
	for (; i <= 9; ++i) {
		char filename[64];
		sprintf(filename, "null%u", i);
		ck_assert_int_eq(_testdiff(filename), 0);
	}
}

void output_join(struct fqlhandle* fql)
{
	const char* query = "\n\
		declare @i int = 0                \n\
		while @i < 2                      \n\
		BEGIN                             \n\
		    select *                      \n\
		    into [results/join1]          \n\
		    from t1                       \n\
		    join t2                       \n\
		        on t1.foo = t2.foo        \n\
		                                  \n\
		    select *                      \n\
		    into [results/join2]          \n\
		    from t2                       \n\
		    join t1                       \n\
		        on t1.foo = t2.foo        \n\
		                                  \n\
		    select t1.bar, t2.bar         \n\
		    into [results/join3]          \n\
		    from t1                       \n\
		    join t2                       \n\
		        on right(t1.foo, 2)       \n\
		         = right(t2.foo, 2)       \n\
		                                  \n\
		    select t1.bar, t2.bar, x.bar  \n\
		    into [results/join4]          \n\
		    from t1                       \n\
		    join t2                       \n\
		        on t1.baz = t2.baz        \n\
		    join t2 X                     \n\
		        on left(t1.foo, 2)        \n\
		         = right(x.foo, 2)        \n\
		                                  \n\
		    set @i += 1                   \n\
		END";

	ck_assert_int_eq(fql_exec(fql, query), FQL_GOOD);

	fql_set_force_cartesian(fql, true);

	const char* cartesian_query = "\n\
		declare @i int = 0         \n\
		while @i < 2               \n\
		BEGIN                      \n\
		    select t1.Foo          \n\
		    into [results/join5]   \n\
		    from t1                \n\
		    join t2                \n\
		        on t1.foo = t2.foo \n\
		                           \n\
		    set @i += 1            \n\
		END";

	ck_assert_int_eq(fql_exec(fql, cartesian_query), FQL_GOOD);

	unsigned i = 1;
	for (; i <= 5; ++i) {
		char filename[64];
		sprintf(filename, "join%u", i);
		ck_assert_int_eq(_testdiff(filename), 0);
	}
}

void output_subquery(struct fqlhandle* fql)
{
	const char* query = "\n\
		declare @i int = 0                                            \n\
		while @i < 2                                                  \n\
		BEGIN                                                         \n\
		    select (select 7) into [results/sub1]                     \n\
		    select (select (select (select 7))) into [results/sub2]   \n\
		    select '6' + (select 7) into [results/sub3]               \n\
		    select '6' + (select '7') into [results/sub4]             \n\
		    select (select bar from t1 where foo = '44b72d44')        \n\
		    into [results/sub5]                                       \n\
		                                                              \n\
		    select 7 into [results/sub6] where 1 = (select 1)         \n\
		    select 13 into [results/sub7]                             \n\
		    where 71 = (select bar from t1 where foo = '44b72d44')    \n\
		                                                              \n\
		    select '67'                                               \n\
		    into [results/sub8]                                       \n\
		    where '67' = '6' + (                                      \n\
		        select '7'                                            \n\
		        from t1                                               \n\
		        where foo = '3138b3f8'                                \n\
		    )                                                         \n\
		                                                              \n\
		    select (                                                  \n\
		        select * from T5 where foo = '0cda1b43'               \n\
		    ) into [results/sub9]                                     \n\
		                                                              \n\
		    select baz into [results/sub10]                           \n\
		    from t1 where foo in (select '282a4957')                  \n\
		                                                              \n\
		    select foo into [results/sub11]                           \n\
		    from t1 where foo in (select foo from t1)                 \n\
		                                                              \n\
		    select * into [results/sub12] from (select 7)x            \n\
		    select '6' + t into [results/sub13] from (select 7 t)x    \n\
		    select '6' + t into [results/sub14] from (select '7' t)x  \n\
		                                                              \n\
		    select * into [results/sub15]                             \n\
		    from (select bar from t1 where foo = 'c1519b0d')x         \n\
		    select shnt, bar into [results/sub16]                     \n\
		    from (select left(foo, 1) shnt, * from t1)x               \n\
		                                                              \n\
		    select shnt into [results/sub17] from (                   \n\
		        select bar shnt from (                                \n\
		            select * from t1 where right(baz, 2) = 82         \n\
		        ) x                                                   \n\
		    ) x                                                       \n\
		                                                              \n\
		    set @i += 1                                               \n\
		END";

	ck_assert_int_eq(fql_exec(fql, query), FQL_GOOD);

	unsigned i = 1;
	for (; i <= 17; ++i) {
		char filename[64];
		sprintf(filename, "sub%u", i);
		ck_assert_int_eq(_testdiff(filename), 0);
	}
}

void output_group(struct fqlhandle* fql)
{
	const char* query = "\n\
		declare @i int = 0                                         \n\
		while @i < 2                                               \n\
		BEGIN                                                      \n\
		    select 1 into [results/group1] group by 2              \n\
		    select count(*) into [results/group2] group by 3       \n\
		    select count(*) into [results/group3]                  \n\
		    select distinct bar into [results/group4] from t3      \n\
		    select bar into [results/group5] from t3 group by bar  \n\
		                                                           \n\
		    select len(bar) + '1' into [results/group6]            \n\
		    from t3 group by bar                                   \n\
		                                                           \n\
		    select baz + 2 into [results/group7]                   \n\
		    from t3 group by baz + (4*3)/6                         \n\
		                                                           \n\
		    select count(*) into [results/group8] from t3          \n\
		                                                           \n\
		    select bar, count(*) into [results/group9]             \n\
		    from t3 group by bar                                   \n\
		                                                           \n\
		    set @i += 1                                            \n\
		END";

	ck_assert_int_eq(fql_exec(fql, query), FQL_GOOD);

	unsigned i = 1;
	for (; i <= 9; ++i) {
		char filename[64];
		sprintf(filename, "group%u", i);
		ck_assert_int_eq(_testdiff(filename), 0);
	}
}

void output_having(struct fqlhandle* fql)
{
	const char* query = "\n\
		declare @i int = 0                                         \n\
		while @i < 2                                               \n\
		BEGIN                                                      \n\
		    select 1 into [results/having1] group by 2 having 1=1  \n\
		    select 1 into [results/having2] group by 3 having 1=0  \n\
		    select count(*) into [results/having3]                 \n\
		    group by 1 having 1=0                                  \n\
		                                                           \n\
		    select bar into [results/having4]                      \n\
		    from t3 group by bar having count(*) > 1               \n\
		                                                           \n\
		    select bar into [results/having5]                      \n\
		    from t3 group by bar having max(foo) > '9'             \n\
		    set @i += 1                                            \n\
		END";

	ck_assert_int_eq(fql_exec(fql, query), FQL_GOOD);

	unsigned i = 1;
	for (; i <= 5; ++i) {
		char filename[64];
		sprintf(filename, "having%u", i);
		ck_assert_int_eq(_testdiff(filename), 0);
	}
}

void output_order(struct fqlhandle* fql)
{
	const char* query = "\n\
		declare @i int = 0                              \n\
		while @i < 2                                    \n\
		BEGIN                                           \n\
		    select 1 into [results/order1] order by foo \n\
		    select 2 into [results/order2] order by 1   \n\
		                                                \n\
		    select distinct bar into [results/order3]   \n\
		    from t3 order by bar                        \n\
		                                                \n\
		    select foo into [results/order4]            \n\
		    from t3 order by bar, foo                   \n\
		                                                \n\
		    select foo into [results/order5]            \n\
		    from t3 order by bar desc, foo              \n\
		                                                \n\
		    select baz + 2 into [results/order6]        \n\
		    from t3 order by baz + 0                    \n\
		                                                \n\
		    select count(*) into [results/order7]       \n\
		    from t3 group by bar order by count(*)      \n\
		                                                \n\
		    select count(*) into [results/order8]       \n\
		    from t3 group by bar order by 1             \n\
		                                                \n\
		    set @i += 1                                 \n\
		END";

	ck_assert_int_eq(fql_exec(fql, query), FQL_GOOD);

	unsigned i = 1;
	for (; i <= 8; ++i) {
		char filename[64];
		sprintf(filename, "order%u", i);
		ck_assert_int_eq(_testdiff(filename), 0);
	}
}

void output_top(struct fqlhandle* fql)
{
	const char* query = "\n\
		declare @i int = 0                       \n\
		while @i < 2                             \n\
		BEGIN                                    \n\
		    select top 0 1 into [results/top1]   \n\
		    select top 2 5 into [results/top2]   \n\
		                                         \n\
		    select top 4 foo                     \n\
		    into [results/top3] from t1          \n\
		                                         \n\
		    select top 2 foo into [results/top4] \n\
		    from t1 order by bar desc            \n\
		                                         \n\
		    select (select top 1 foo from t1)    \n\
		    into [results/top5]                  \n\
		                                         \n\
		    select top ((5*2)/4) baz             \n\
		    into [results/top6] from t1          \n\
		                                         \n\
		    set @i += 1                          \n\
		END";

	ck_assert_int_eq(fql_exec(fql, query), FQL_GOOD);

	unsigned i = 1;
	for (; i <= 6; ++i) {
		char filename[64];
		sprintf(filename, "top%u", i);
		ck_assert_int_eq(_testdiff(filename), 0);
	}
}

void output_union(struct fqlhandle* fql)
{
	const char* query = "\n\
		declare @i int = 0                                 \n\
		while @i < 2                                       \n\
		BEGIN                                              \n\
		    select 1 into [results/union1]                 \n\
		    union all select 2                             \n\
		                                                   \n\
		    select foo into [results/union2]               \n\
		    from t1 union all select '1'                   \n\
		                                                   \n\
		    select '1' into [results/union3]               \n\
		    union all select foo from t1                   \n\
		                                                   \n\
		    select distinct bar into [results/union4]      \n\
		    from t1 union all select bar from t3           \n\
		                                                   \n\
		    select bar into [results/union5] from t1       \n\
		    union all select distinct bar from t3          \n\
		                                                   \n\
		    select top 2 foo into [results/union6] from t1 \n\
		    union all select top 3 foo from t3             \n\
		                                                   \n\
		    set @i += 1                                    \n\
		END";

	ck_assert_int_eq(fql_exec(fql, query), FQL_GOOD);

	unsigned i = 1;
	for (; i <= 6; ++i) {
		char filename[64];
		sprintf(filename, "union%u", i);
		ck_assert_int_eq(_testdiff(filename), 0);
	}
}

void output_difficult(struct fqlhandle* fql)
{
	const char* query = "\n\
		declare @i int = 0                                            \n\
		while @i < 2                                                  \n\
		BEGIN                                                         \n\
		    select bar, ('0x' + bar + 0) * 1                          \n\
		    into [results/difficult1] from t2                         \n\
		                                                              \n\
		    select bar                                                \n\
		       ,sum(case when baz % 2 = 0 then 1 else 0 end) even_baz \n\
		       ,sum(case when baz % 2 = 1 then 1 else 0 end) odd_baz  \n\
		       ,count(*) TOTAL                                        \n\
		    into [results/difficult2]                                 \n\
		    from t3                                                   \n\
		    group by bar                                              \n\
		    order by bar                                              \n\
		                                                              \n\
		    select top (1*3)                                          \n\
		         (select top 1 foo from t1),                          \n\
		         bar+'eex',                                           \n\
		         max(baz+0),                                          \n\
		         qty                                                  \n\
		    into [results/difficult3]                                 \n\
		    from (                                                    \n\
		         select bar shnt, count(*) qty                        \n\
		         from t3                                              \n\
		         group by bar                                         \n\
		    ) x1                                                      \n\
		    join t3 x2                                                \n\
		        on  x2.bar = x1.shnt                                  \n\
		        and 1=1                                               \n\
		    where len(bar) = right('shnt2',1)                         \n\
		    group by bar,qty                                          \n\
		    having min(baz+0) < 20000                                 \n\
		    order by bar desc, 3                                      \n\
		                                                              \n\
		    set @i += 1                                               \n\
		END";

	ck_assert_int_eq(fql_exec(fql, query), FQL_GOOD);

	unsigned i = 1;
	for (; i <= 3; ++i) {
		char filename[64];
		sprintf(filename, "difficult%u", i);
		ck_assert_int_eq(_testdiff(filename), 0);
	}
}

void output_operations(struct fqlhandle* fql)
{
	fql_set_overwrite(fql, true);

	const char* query = "\n\
		declare @i int = 0                                        \n\
		while @i < 2                                              \n\
		BEGIN                                                     \n\
		    select * into [results/operations1] from t1           \n\
		    delete [results/operations1]                          \n\
		                                                          \n\
		    select * into [results/operations2] from t1           \n\
		    delete [results/operations2]                          \n\
		    from [results/operations2] where 1=1                  \n\
		                                                          \n\
		    select * into [results/operations3] from t1           \n\
		    delete d                                              \n\
		    from [results/operations3] d                          \n\
		                                                          \n\
		    select * into [results/operations4] from t1           \n\
		    delete [results/operations4]                          \n\
		    from [results/operations4] d                          \n\
		                                                          \n\
		    select * into [results/operations5] from t1           \n\
		    delete [results/operations5]                          \n\
		    where bar = 'b0'                                      \n\
		                                                          \n\
		    select * into [results/operations6] from t1           \n\
		    delete d                                              \n\
		    from [results/operations6] d                          \n\
		    join t2 on d.foo = t2.foo                             \n\
		                                                          \n\
		    select * into [results/operations7] from t1           \n\
		    update [results/operations7] set bar = 'xx'           \n\
		                                                          \n\
		    select * into [results/operations8] from t1           \n\
		    update [results/operations8] set bar = 'xx'           \n\
		    from [results/operations8] where 1=1                  \n\
		                                                          \n\
		    select * into [results/operations9] from t1           \n\
		    update u set bar = 'xx' from [results/operations9] u  \n\
		                                                          \n\
		    select * into [results/operations10] from t1          \n\
		    update [results/operations10] set bar = 'xx'          \n\
		    from [results/operations10] u                         \n\
		                                                          \n\
		    select * into [results/operations11] from t1          \n\
		    update u set bar = 'xx' from [results/operations11] u \n\
		    where bar = 'b0'                                      \n\
		                                                          \n\
		    select * into [results/operations12] from t1          \n\
		    update u set bar = 'xx' from [results/operations12] u \n\
		    join t2 on u.foo = t2.foo                             \n\
		                                                          \n\
		    set @i += 1                                           \n\
		END";

	ck_assert_int_eq(fql_exec(fql, query), FQL_GOOD);

	unsigned i = 1;
	for (; i <= 12; ++i) {
		char filename[64];
		sprintf(filename, "operations%u", i);
		ck_assert_int_eq(_testdiff(filename), 0);
	}
}
