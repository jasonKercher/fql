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
	const char* query = "\
		declare @i int = 0                                          \
		while @i < 2                                                \
		BEGIN                                                       \
		    select 1 into [results/const1]                          \
	            select 1.1 into [results/const2]                        \
	            select 'x''' into [results/const3]                      \
	            select 3 / 2 into [results/const4]                      \
	            select 1.1 + 1 into [results/const5]                    \
	            select 1 * 2.0 / (3 + 4.0) into [results/const6]        \
	            select 123 * -2.0 / (-3 + 4.1) into [results/const7]    \
	            select '13' / 5 into [results/const8]                   \
	            select '1' / 3.0 into [results/const9]                  \
	            select '4' + '5' into [results/const10]                 \
	            select '1' + '9' + 5 into [results/const11]             \
	            select -9223372036854775806 + -2 into [results/const12] \
	            select -1 into [results/const13]                        \
	            select +1.1 into [results/const14]                      \
	            select -(1 * 2) into [results/const15]                  \
	            select ~1 into [results/const16]                        \
	            select left('testing fql', 4) into [results/const17]    \
	            select left('αβΩ', 1) into [results/const18]            \
	            select right('testing fql', 4) into [results/const19]   \
	            select right('αβΩ', 1) into [results/const20]           \
		    set @i += 1                                             \
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
	const char* query = "\
		declare @i int = 0                                               \
		while @i < 2                                                     \
		BEGIN                                                            \
	            select foo, bar, baz into [results/read1] from t1            \
	            select foo into [results/read2] from [../parent]             \
	            select foo into [results/read3] from [sub/sub]               \
	            select * into [results/read4] from t1                        \
	            select 5 into [results/read5] from t1                        \
	            select foo + bar, baz / 10, baz into [results/read6] from t1 \
	            select -(baz+0) into [results/read7] from t1                 \
	                                                                         \
	            select foo, left(right(foo, 5), 2)                           \
	            into [results/read8]                                         \
	            from t1                                                      \
		    set @i += 1                                                  \
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
	const char* query = "\
		declare @i int = 0                                                         \
		while @i < 2                                                               \
		BEGIN                                                                      \
	            select 1 into [results/logic1] where '01' = 1                          \
	            select 1 into [results/logic2] where -1 = 1                            \
	            select 1 into [results/logic3] where 50.5 = '050.5'                    \
	            select 1 into [results/logic4] where 50.4 = 50.5                       \
	            select 1 into [results/logic5] where 'hi' = 'Hi   '                    \
	            select 1 into [results/logic6] where 'hi' = ' hi'                      \
	            select 1 into [results/logic7] where 1 != 0                            \
	            select 1 into [results/logic8] where 9223372036854775807 <>            \
	            9223372036854775807                                                    \
	            select 1 into [results/logic9] where -.0001 != -.00011                 \
	            select 1 into [results/logic10] where 33.333 <> 33.333                 \
	            select 1 into [results/logic11] where 'abcde f' != 'abcde'             \
	            select 1 into [results/logic12] where '' <> ''                         \
	            select 1 into [results/logic13] where 5 > -6                           \
	            select 1 into [results/logic14] where -9223372036854775808 >           \
	            9223372036854775807                                                    \
	            select 1 into [results/logic15] where 1.1 > 1                          \
	            select 1 into [results/logic16] where -1.1 > 1                         \
	            select 1 into [results/logic17] where 'Bad' > 'abc'                    \
	            select 1 into [results/logic18] where 'A' > '{'                        \
	            select 1 into [results/logic19] where 5 >= 5                           \
	            select 1 into [results/logic20] where 5 >= '4'                         \
	            select 1 into [results/logic21] where -1 >= 1                          \
	            select 1 into [results/logic22] where -22.1 >= -31                     \
	            select 1 into [results/logic23] where 22.1 >= 22.1                     \
	            select 1 into [results/logic24] where -22.10001 >= -22.1               \
	            select 1 into [results/logic25] where 'abc' >= '[c]   '                \
	            select 1 into [results/logic26] where 'abc' >= 'abc   '                \
	            select 1 into [results/logic27] where 'abc' >= 'abc d'                 \
	            select 1 into [results/logic28] where -1 < 1                           \
	            select 1 into [results/logic29] where '1' < -1                         \
	            select 1 into [results/logic30] where 5.992 < 5.993                    \
	            select 1 into [results/logic31] where -11 < -12.1                      \
	            select 1 into [results/logic32] where 'shnt' < 'TNHS'                  \
	            select 1 into [results/logic33] where 'shnt' < 'SHNT'                  \
	            select 1 into [results/logic34] where 1 <= 1                           \
	            select 1 into [results/logic35] where -12341 <= 1                      \
	            select 1 into [results/logic36] where 0 <= -1                          \
	            select 1 into [results/logic37] where -888.1234 <= 888                 \
	            select 1 into [results/logic38] where 888.0 <= 888                     \
	            select 1 into [results/logic39] where 888.1234 <= 888                  \
	            select 1 into [results/logic40] where 'hello world!' <= 'hello world!' \
	            select 1 into [results/logic41] where 'hello world' <= 'hello world!'  \
	            select 1 into [results/logic42] where 'hello_world!' <= 'hello world!' \
	            select 1 into [results/logic43] where 'sh.nt' like '%.%'               \
	            select 1 into [results/logic44] where 'sh.nt' not like '%.%'           \
	            select 1 into [results/logic45] where 's' in ('qcn', 's')              \
	            select 1 into [results/logic46] where 1 in (1, 78)                     \
	            select 1 into [results/logic47] where '' in (' n', '565')              \
	            select 1 into [results/logic48] where '' in (' n', '      ')           \
	            select 1 into [results/logic49] where left('hello', 2) = 'he  '        \
	            select 1 into [results/logic50] where 1000 + 233 <                     \
	            right('hi1234', 4)                                                     \
	            select 1 into [results/logic51] where 100 * -1 > 0 / 1000              \
	            select 1 into [results/logic52] where 1=1 and 1=1                      \
	            select 1 into [results/logic53] where 1=1 or 1=0                       \
	            select 1 into [results/logic54] where 1=1 and 1=0                      \
	            select 1 into [results/logic55] where 1=0 or 1=1 and 1=0               \
	            select 1 into [results/logic56] where 1=1 or 1=1 and 1=0               \
	            select 1 into [results/logic57] where (1=1 or 1=1) and 1=0             \
	            select 1 into [results/logic58] where 1=0 and 1=1 or 1=1               \
	            select 1 into [results/logic59] where (1=1 and 1=0) or (1=1 and        \
	            1=0)                                                                   \
	            select 1 into [results/logic60] where (1=1 and 1=0) or (1=1 and        \
	            1=1)                                                                   \
		    set @i += 1                                                            \
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
	const char* query = "\
		declare @i int = 0                                                \
		while @i < 2                                                      \
		BEGIN                                                             \
	            select len('shnt') into [results/scalar1]                     \
	            select len('shnt   ') into [results/scalar2]                  \
	            select len('♜♞ ') into [results/scalar3]                      \
	            select len(100) into [results/scalar4]                        \
	            select len('♜♞ ') into [results/scalar5]                      \
	            select datalength('shnt') into [results/scalar6]              \
	            select datalength('shnt   ') into [results/scalar7]           \
	            select datalength('♜♞ ') into [results/scalar8]               \
	            select datalength(100) into [results/scalar9]                 \
	            select datalength(100.0) into [results/scalar10]              \
	            select left('shnt', 3) into [results/scalar11]                \
	            select left('shnt  ', 12) into [results/scalar12]             \
	            select left('♗♕♔♗♘', 3) into [results/scalar13]               \
	            select left('♗♕♔♗♘', 3) into [results/scalar14]               \
	            select right('shnt ', 3) into [results/scalar15]              \
	            select right('shnt  ', 12) into [results/scalar16]            \
	            select right('♗♕♔♗♘', 3) into [results/scalar17]              \
	            select right('♗♕♔♗♘', 3) into [results/scalar18]              \
	            select substring('shnt ', 3, 1) into [results/scalar19]       \
	            select substring('shnt  ', 12, 20) into [results/scalar20]    \
	            select substring('shnt  ', 3, 20) into [results/scalar21]     \
	            select substring('shnt', -1, 3) into [results/scalar22]       \
	            select substring('♗♕♔♗♘', 3, 2) into [results/scalar23]       \
	            select substring('♗♕♔♗♘', 4, 3) into [results/scalar24]       \
	            select replace('shnt', 'nt', 'xx') into [results/scalar25]    \
	            select replace('shnt', 'xx', 'nt') into [results/scalar26]    \
	            select replace('Haters gonna hate', 'hate', 'love')           \
	            into [results/scalar27]                                       \
	            select replace('Hello World', 'world', 'Entire Planet!')      \
	            into [results/scalar28]                                       \
	            select replace('Hello Entire Planet!', 'planet', 'Room')      \
	            into [results/scalar29]                                       \
	            select cast(1234 as bit) into [results/scalar30]              \
	            select cast('0' as bit) into [results/scalar31]               \
	            select cast(-1 as bit) into [results/scalar32]                \
	            select cast(55 as int) into [results/scalar33]                \
	            select cast('1234' as int) into [results/scalar34]            \
	            select cast('-000001234' as int) into [results/scalar35]      \
	            select cast('0x7F' as int) into [results/scalar36]            \
	            select cast(5.0001 as int) into [results/scalar37]            \
	            select cast(55.0 as float) into [results/scalar38]            \
	            select cast('1234' as float) into [results/scalar39]          \
	            select cast('-000001234' as float) into [results/scalar40]    \
	            select cast('6.12366' as float) into [results/scalar41]       \
	            select cast(55 as varchar) into [results/scalar42]            \
	            select cast('shnt' as varchar(3)) into [results/scalar43]     \
	            select cast(55 as char) into [results/scalar44]               \
	            select cast('shnt' as char(3)) into [results/scalar45]        \
	            select cast('shnt' as char(10)) into [results/scalar46]       \
		    set @i += 1                                                   \
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
	const char* query = "\
		declare @i int = 0              \
		while @i < 2                    \
		BEGIN                           \
	            select case 0               \
	                when 1 then 2           \
	                when 0 then 3           \
	                end                     \
	            into [results/switch1]      \
	                                        \
	            select case                 \
	                when 0=1 then 2         \
	                when 0=0 then 3         \
	                end                     \
	            into [results/switch2]      \
	                                        \
	            select 1                    \
	            into [results/switch3]      \
	            where 2 = case              \
	                    when 1=1 then 1     \
	                    else 0 end          \
	                                        \
	            select 1                    \
	            into [results/switch4]      \
	            where 2 = case              \
	                    when 1=1 then 2     \
	                    else -1 end         \
	                                        \
	            select case                 \
	                when 1 = 2 then 0       \
	                when 2 = 3 then 1       \
	                else 8 end              \
	            into [results/switch5]      \
	                                        \
	            select case                 \
	                when 'x' = 'y' then 1   \
	                else '2' end            \
	            into [results/switch6]      \
	                                        \
	            select case                 \
	                when 'x' = 'x' then '2' \
	                else 1 end              \
	            into [results/switch7]      \
	                                        \
	            select case                 \
	                when 1=1 then '2'       \
	                else '3' end            \
	            into [results/switch8]      \
	                                        \
	            select case 'a'             \
	                when 'b' then '5'       \
	                when 'x' then 2         \
	                when 'a' then '1' end   \
	            into [results/switch9]      \
		    set @i += 1                 \
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
	const char* query = "\
		declare @i int = 0                                                 \
		while @i < 2                                                       \
		BEGIN                                                              \
			select NULL into [results/null1]                           \
			select 1 + NULL into [results/null2]                       \
			select NULL + 1 into [results/null3]                       \
			select left('00000000' + cast(5 * (1+NULL) as varchar), 8) \
			into [results/null4]                                       \
			select isnull(1 + NULL, NULL) into [results/null5]         \
			select 1 into [results/null6] where NULL - 4 is NULL       \
			select 1 into [results/null7] where NULL - 4 is not NULL   \
			select 1 into [results/null8] where 6 - 4 is not NULL      \
			select 1 into [results/null9] where 6 - 4 is NULL          \
			set @i += 1                                                \
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
	const char* query = "\
		declare @i int = 0                \
		while @i < 2                      \
		BEGIN                             \
		    select *                      \
		    into [results/join1]          \
		    from t1                       \
		    join t2                       \
		        on t1.foo = t2.foo        \
		                                  \
		    select *                      \
		    into [results/join2]          \
		    from t2                       \
		    join t1                       \
		        on t1.foo = t2.foo        \
                                                  \
		    select t1.bar, t2.bar         \
		    into [results/join3]          \
		    from t1                       \
		    join t2                       \
		        on right(t1.foo, 2)       \
			 = right(t2.foo, 2)       \
                                                  \
                    select t1.bar, t2.bar, x.bar  \
		    into [results/join4]          \
                    from t1                       \
                    join t2                       \
                        on t1.baz = t2.baz        \
                    join t2 X                     \
                        on left(t1.foo, 2)        \
			 = right(x.foo, 2)        \
                                                  \
		    set @i += 1               \
		END";

	ck_assert_int_eq(fql_exec(fql, query), FQL_GOOD);

	fql_set_force_cartesian(fql, true);

	const char* cartesian_query = "\
		declare @i int = 0         \
		while @i < 2               \
		BEGIN                      \
	      	    select t1.Foo          \
		    into [results/join5]   \
		    from t1                \
		    join t2                \
		        on t1.foo = t2.foo \
                                           \
		    set @i += 1            \
		END";

	ck_assert_int_eq(fql_exec(fql, cartesian_query), FQL_GOOD);

	unsigned i = 1;
	for (; i <= 5; ++i) {
		char filename[64];
		sprintf(filename, "join%u", i);
		ck_assert_int_eq(_testdiff(filename), 0);
	}
}
