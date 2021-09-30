#include "output.h"
#include <stdio.h>

int _testdiff(const char* s, const char* resultdir)
{
	char filename0[256];
	char filename1[256];
	sprintf(filename0, "control/%s", s);
	sprintf(filename1, "%s/%s", resultdir, s);

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

void output_const(struct fqlhandle* fql, const char* resultdir)
{
	char query[4096];
	sprintf(query,
	        "select 1 into [%s/const1]                          "
	        "select 1.1 into [%s/const2]                        "
	        "select 'x''' into [%s/const3]                      "
	        "select 3 / 2 into [%s/const4]                      "
	        "select 1.1 + 1 into [%s/const5]                    "
	        "select 1 * 2.0 / (3 + 4.0) into [%s/const6]        "
	        "select 123 * -2.0 / (-3 + 4.1) into [%s/const7]    "
	        "select '13' / 5 into [%s/const8]                   "
	        "select '1' / 3.0 into [%s/const9]                  "
	        "select '4' + '5' into [%s/const10]                 "
	        "select '1' + '9' + 5 into [%s/const11]             "
	        "select -9223372036854775806 + -2 into [%s/const12] "
	        "select -1 into [%s/const13]                        "
	        "select +1.1 into [%s/const14]                      "
	        "select -(1 * 2) into [%s/const15]                  "
	        "select ~1 into [%s/const16]                        "
	        "select left('testing fql', 4) into [%s/const17]    "
	        "select left('αβΩ', 1) into [%s/const18]            "
	        "select right('testing fql', 4) into [%s/const19]   "
	        "select right('αβΩ', 1) into [%s/const20]           ",
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir);
	ck_assert_int_eq(fql_exec(fql, query), FQL_GOOD);

	unsigned i = 1;
	for (; i <= 20; ++i) {
		char filename[64];
		sprintf(filename, "const%u", i);
		ck_assert_int_eq(_testdiff(filename, resultdir), 0);
	}
}

void output_read(struct fqlhandle* fql, const char* resultdir)
{
	char query[4096];
	sprintf(query,
	        "select foo, bar, baz into [%s/read1] from t1            "
	        "select foo into [%s/read2] from [../parent]             "
	        "select foo into [%s/read3] from [sub/sub]               "
	        "select * into [%s/read4] from t1                        "
	        "select 5 into [%s/read5] from t1                        "
	        "select foo + bar, baz / 10, baz into [%s/read6] from t1 "
	        "select -(baz+0) into [%s/read7] from t1                 "
	        "                                                        "
	        "select foo, left(right(foo, 5), 2)                      "
	        "into [%s/read8]                                         "
	        "from t1                                                 ",
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir);
	ck_assert_int_eq(fql_exec(fql, query), FQL_GOOD);

	unsigned i = 1;
	for (; i <= 8; ++i) {
		char filename[64];
		sprintf(filename, "read%u", i);
		ck_assert_int_eq(_testdiff(filename, resultdir), 0);
	}
}

void output_logic(struct fqlhandle* fql, const char* resultdir)
{
	char query[4096];
	sprintf(query,
	        "select 1 into [%s/logic1] where '01' = 1"
	        "select 1 into [%s/logic2] where -1 = 1"
	        "select 1 into [%s/logic3] where 50.5 = '050.5'"
	        "select 1 into [%s/logic4] where 50.4 = 50.5"
	        "select 1 into [%s/logic5] where 'hi' = 'Hi   '"
	        "select 1 into [%s/logic6] where 'hi' = ' hi'"
	        "select 1 into [%s/logic7] where 1 != 0"
	        "select 1 into [%s/logic8] where 9223372036854775807 <> "
	        "9223372036854775807"
	        "select 1 into [%s/logic9] where -.0001 != -.00011"
	        "select 1 into [%s/logic10] where 33.333 <> 33.333"
	        "select 1 into [%s/logic11] where 'abcde f' != 'abcde'"
	        "select 1 into [%s/logic12] where '' <> ''"
	        "select 1 into [%s/logic13] where 5 > -6"
	        "select 1 into [%s/logic14] where -9223372036854775808 > "
	        "9223372036854775807"
	        "select 1 into [%s/logic15] where 1.1 > 1"
	        "select 1 into [%s/logic16] where -1.1 > 1"
	        "select 1 into [%s/logic17] where 'Bad' > 'abc'"
	        "select 1 into [%s/logic18] where 'A' > '{'" /* THIS BREAKS FROM T-SQL */
	        "select 1 into [%s/logic19] where 5 >= 5"
	        "select 1 into [%s/logic20] where 5 >= '4'"
	        "select 1 into [%s/logic21] where -1 >= 1"
	        "select 1 into [%s/logic22] where -22.1 >= -31"
	        "select 1 into [%s/logic23] where 22.1 >= 22.1"
	        "select 1 into [%s/logic24] where -22.10001 >= -22.1"
	        "select 1 into [%s/logic25] where 'abc' >= '[c]   '"
	        "select 1 into [%s/logic26] where 'abc' >= 'abc   '"
	        "select 1 into [%s/logic27] where 'abc' >= 'abc d'"
	        "select 1 into [%s/logic28] where -1 < 1"
	        "select 1 into [%s/logic29] where '1' < -1"
	        "select 1 into [%s/logic30] where 5.992 < 5.993"
	        "select 1 into [%s/logic31] where -11 < -12.1"
	        "select 1 into [%s/logic32] where 'shnt' < 'TNHS'"
	        "select 1 into [%s/logic33] where 'shnt' < 'SHNT'"
	        "select 1 into [%s/logic34] where 1 <= 1"
	        "select 1 into [%s/logic35] where -12341 <= 1"
	        "select 1 into [%s/logic36] where 0 <= -1"
	        "select 1 into [%s/logic37] where -888.1234 <= 888"
	        "select 1 into [%s/logic38] where 888.0 <= 888"
	        "select 1 into [%s/logic39] where 888.1234 <= 888"
	        "select 1 into [%s/logic40] where 'hello world!' <= 'hello "
	        "world!'"
	        "select 1 into [%s/logic41] where 'hello world' <= 'hello "
	        "world!'"
	        "select 1 into [%s/logic42] where 'hello_world!' <= 'hello "
	        "world!'"
	        "select 1 into [%s/logic43] where 'sh.nt' like '%.%'"
	        "select 1 into [%s/logic44] where 'sh.nt' not like '%.%'"
	        "select 1 into [%s/logic45] where 's' in ('qcn', 's')"
	        "select 1 into [%s/logic46] where 1 in (1, 78)"
	        "select 1 into [%s/logic47] where '' in (' n', '565')"
	        "select 1 into [%s/logic48] where '' in (' n', '      ')"
	        "select 1 into [%s/logic49] where left('hello', 2) = 'he    "
	        "'"
	        "select 1 into [%s/logic50] where 1000 + 233 < "
	        "right('hi1234', 4)"
	        "select 1 into [%s/logic51] where 100 * -1 > 0 / 1000"
	        "select 1 into [%s/logic52] where 1=1 and 1=1"
	        "select 1 into [%s/logic53] where 1=1 or 1=0"
	        "select 1 into [%s/logic54] where 1=1 and 1=0"
	        "select 1 into [%s/logic55] where 1=0 or 1=1 and 1=0"
	        "select 1 into [%s/logic56] where 1=1 or 1=1 and 1=0"
	        "select 1 into [%s/logic57] where (1=1 or 1=1) and 1=0"
	        "select 1 into [%s/logic58] where 1=0 and 1=1 or 1=1"
	        "select 1 into [%s/logic59] where (1=1 and 1=0) or (1=1 and "
	        "1=0)"
	        "select 1 into [%s/logic60] where (1=1 and 1=0) or (1=1 and "
	        "1=1)",
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir);

	ck_assert_int_eq(fql_exec(fql, query), FQL_GOOD);

	unsigned i = 1;
	for (; i <= 60; ++i) {
		char filename[64];
		sprintf(filename, "logic%u", i);
		ck_assert_int_eq(_testdiff(filename, resultdir), 0);
	}
}

void output_scalar(struct fqlhandle* fql, const char* resultdir)
{
	char query[4096];
	sprintf(query,
	        "select len('shnt') into [%s/scalar1]                  "
	        "select len('shnt   ') into [%s/scalar2]               "
	        "select len('♜♞ ') into [%s/scalar3]                   "
	        "select len(100) into [%s/scalar4]                     "
	        "select len('♜♞ ') into [%s/scalar5]                   "
	        "select datalength('shnt') into [%s/scalar6]           "
	        "select datalength('shnt   ') into [%s/scalar7]        "
	        "select datalength('♜♞ ') into [%s/scalar8]            "
	        "select datalength(100) into [%s/scalar9]              "
	        "select datalength(100.0) into [%s/scalar10]           "
	        "select left('shnt', 3) into [%s/scalar11]             "
	        "select left('shnt  ', 12) into [%s/scalar12]          "
	        "select left('♗♕♔♗♘', 3) into [%s/scalar13]            "
	        "select left('♗♕♔♗♘', 3) into [%s/scalar14]            "
	        "select right('shnt ', 3) into [%s/scalar15]           "
	        "select right('shnt  ', 12) into [%s/scalar16]         "
	        "select right('♗♕♔♗♘', 3) into [%s/scalar17]           "
	        "select right('♗♕♔♗♘', 3) into [%s/scalar18]           "
	        "select substring('shnt ', 3, 1) into [%s/scalar19]    "
	        "select substring('shnt  ', 12, 20) into [%s/scalar20] "
	        "select substring('shnt  ', 3, 20) into [%s/scalar21]  "
	        "select substring('shnt', -1, 3) into [%s/scalar22]    "
	        "select substring('♗♕♔♗♘', 3, 2) into [%s/scalar23]    "
	        "select substring('♗♕♔♗♘', 4, 3) into [%s/scalar24]    "
	        "select replace('shnt', 'nt', 'xx') into [%s/scalar25] "
	        "select replace('shnt', 'xx', 'nt') into [%s/scalar26] "
	        "select replace('Haters gonna hate', 'hate', 'love')        "
	        "into [%s/scalar27]                                    "
	        "select replace('Hello World', 'world', 'Entire Planet!')   "
	        "into [%s/scalar28]                                    "
	        "select replace('Hello Entire Planet!', 'planet', 'Room')   "
	        "into [%s/scalar29]                                    "
	        "select cast(1234 as bit) into [%s/scalar30]           "
	        "select cast('0' as bit) into [%s/scalar31]            "
	        "select cast(-1 as bit) into [%s/scalar32]             "
	        "select cast(55 as int) into [%s/scalar33]             "
	        "select cast('1234' as int) into [%s/scalar34]         "
	        "select cast('-000001234' as int) into [%s/scalar35]   "
	        "select cast('0x7F' as int) into [%s/scalar36]         "
	        "select cast(5.0001 as int) into [%s/scalar37]         "
	        "select cast(55.0 as float) into [%s/scalar38]         "
	        "select cast('1234' as float) into [%s/scalar39]       "
	        "select cast('-000001234' as float) into [%s/scalar40] "
	        "select cast('6.12366' as float) into [%s/scalar41]    "
	        "select cast(55 as varchar) into [%s/scalar42]         "
	        "select cast('shnt' as varchar(3)) into [%s/scalar43]  "
	        "select cast(55 as char) into [%s/scalar44]            "
	        "select cast('shnt' as char(3)) into [%s/scalar45]     "
	        "select cast('shnt' as char(10)) into [%s/scalar46]    ",
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir);

	ck_assert_int_eq(fql_exec(fql, query), FQL_GOOD);

	unsigned i = 1;
	for (; i <= 46; ++i) {
		char filename[64];
		sprintf(filename, "scalar%u", i);
		ck_assert_int_eq(_testdiff(filename, resultdir), 0);
	}
}

void output_switch(struct fqlhandle* fql, const char* resultdir)
{
	char query[4096];
	sprintf(query,
	        "select case 0               "
	        "    when 1 then 2           "
	        "    when 0 then 3           "
	        "    end                     "
	        "into [%s/switch1]      "
	        "                            "
	        "select case                 "
	        "    when 0=1 then 2         "
	        "    when 0=0 then 3         "
	        "    end                     "
	        "into [%s/switch2]      "
	        "                            "
	        "select 1                    "
	        "into [%s/switch3]      "
	        "where 2 = case              "
	        "        when 1=1 then 1     "
	        "        else 0 end          "
	        "                            "
	        "select 1                    "
	        "into [%s/switch4]      "
	        "where 2 = case              "
	        "        when 1=1 then 2     "
	        "        else -1 end         "
	        "                            "
	        "select case                 "
	        "    when 1 = 2 then 0       "
	        "    when 2 = 3 then 1       "
	        "    else 8 end              "
	        "into [%s/switch5]      "
	        "                            "
	        "select case                 "
	        "    when 'x' = 'y' then 1   "
	        "    else '2' end            "
	        "into [%s/switch6]      "
	        "                            "
	        "select case                 "
	        "    when 'x' = 'x' then '2' "
	        "    else 1 end              "
	        "into [%s/switch7]      "
	        "                            "
	        "select case                 "
	        "    when 1=1 then '2'       "
	        "    else '3' end            "
	        "into [%s/switch8]      "
	        "                            "
	        "select case 'a'             "
	        "    when 'b' then '5'       "
	        "    when 'x' then 2         "
	        "    when 'a' then '1' end   "
	        "into [%s/switch9]      ",
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir,
	        resultdir);

	ck_assert_int_eq(fql_exec(fql, query), FQL_GOOD);

	unsigned i = 1;
	for (; i <= 9; ++i) {
		char filename[64];
		sprintf(filename, "switch%u", i);
		ck_assert_int_eq(_testdiff(filename, resultdir), 0);
	}
}
