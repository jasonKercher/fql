#!/bin/bash

# gen functions are all basically this:
# https://stackoverflow.com/questions/29253591/generate-large-csv-with-random-content-in-bash

gen_narrow ()
{
  local count=$1
  ((++count)) # header
  hexdump -v -e '5/1 "%02x""\n"' /dev/urandom |
  awk -v OFS='\t' '
    NR == 1 { print "foo", "bar", "baz" }
    { print substr($0, 1, 8), substr($0, 9, 2), int(NR * 32768 * rand()) }' 
  head -n "$count"
}

fql_count=0
run_fql ()
{
  ((++fql_count))
  printf 'test %s: fql <<< "%s" ...' $fql_count "$1"
  if time fql <<< "$1" > fql${fql_count}.temp; then
    echo success
  else
    echo fail
  fi
}

textql_count=0
run_textql ()
{
  local args=("$@")
  shift

  ((++textql_count))
  printf 'test %s: textql -output-dlm tab -dlm tab -header -sql "%s" %s...' $textql_count "${args[0]}" "$*"
  if time textql -output-dlm tab -dlm tab -header -sql "${args[@]}" > textql${textql_count}.temp; then
    echo success
  else
    echo fail
  fi
}

q_count=0
run_q ()
{
  local args=("$@")
  shift

  ((++q_count))
  printf 'test %s: q -Ht "%s" %s' $q_count "${args[0]}" "$*"
  if time q -Ht "${args[@]}" > q${q_count}.temp; then
    echo success
  else
    echo fail
  fi
}

csvsql_count=0
run_csvsql ()
{
  local args=("$@")
  shift

  ((++csvsql_count))
  printf 'test %s: csvsql -t --query "%s" %s...' $csvsql_count "${args[0]}" "$*"
  if time csvsql -t --query "${args[@]}" > csvsql${csvsql_count}.temp; then
    echo success
  else
    echo fail
  fi
}

sqlite3_count=0
run_sqlite3 ()
{
  ((++sqlite3_count))
  printf 'test %s: sqlite3 "%s"...' $sqlite3_count "$*"
  if time sqlite3 "${args[@]}" > sqlite3${sqlite3_count}.temp; then
    echo success
  else
    echo fail
  fi
}


gen_narrow 2000000 > t1.temp
gen_narrow 2000000 > t2.temp


# fql
run_fql "select * from t1 join t2 on t1.foo = t2.foo"      # 1
run_fql "select bar, count(*) from t1 group by bar"        # 2
run_fql "select foo from t1 where foo like '%aa[0-9]aa%'"  # 3
run_fql "select * from t1 order by foo, baz desc"          # 4

# textql
# could not get this to work.  It kept trying to treat the data as numeric...
run_textql "select * from t1 join t2 on t1.foo = t2.foo" t1.temp t2.temp  # 1
run_textql "select bar,count(*) from t1 group by bar" t1.temp             # 2
run_textql "select foo from t1 where foo like '%aa[0-9]aa%'" t1.temp      # 3
run_textql "select * from t1 order by foo, baz desc" t1.temp              # 4

# q
run_q "select * from t1.temp t1 join ./t2.temp t2 on t1.foo = t2.foo" # 1
run_q "select bar,count(*) from t1.temp group by bar"                 # 2
# Couldn't figure out how to do [0-9]...
run_q "select foo from t1.temp where foo like '%aa_aa%'"              # 3
run_q "select * from t1.temp order by foo, baz desc"                  # 4

# csvsql
run_csvsql "select * from t1 join t2 on t1.foo = t2.foo" t1.temp t2.temp  # 1
run_csvsql "select bar,count(*) from t1 group by bar" t1.temp             # 2
# Couldn't figure out how to do [0-9]...
run_csvsql "select foo from t1 where foo like '%aa_aa%'" t1.temp          # 3
run_csvsql "select * from t1 order by foo, baz desc" t1.temp              # 4

# import for sqlite tests
sqlite3 MYDB "
create table t1 (foo text, bar text, baz text);
.mode tabs
.import t1.temp t1

create table t2 (foo text, bar text, baz text);
.mode tabs
.import t2.temp t2
"

# sqlite
run_sqlite3 MYDB "select * from t1 join t2 on t1.foo = t2.foo"    # 1
run_sqlite3 MYDB "select bar,count(*) from t1 group by bar"       # 2
# Couldn't figure out how to do [0-9]...
run_sqlite3 MYDB "select foo from t1 where foo like '%aa_aa%'"    # 3
run_sqlite3 MYDB "select * from t1 order by foo, baz desc"        # 4

rm MYDB


# shell tools
# 1
echo "test 1: join -o 1.1 1.2 1.3 2.1 2.2 2.3 <(sort t1.temp) <(sort t2.temp)"
time join -o 1.1 1.2 1.3 2.1 2.2 2.3 <(sort t1.temp) <(sort t2.temp) > shell1.temp

#2
echo "test 2: awk 'NR>1{arr[$2]++}END{for (a in arr) print a, arr[a]}' t1.temp"
time gawk 'NR>1{arr[$2]++}END{for (a in arr) print a, arr[a]}' t1.temp > shell2.temp

# 3
echo "test 3: awk '{if ($1 ~ /.*aa[0-9]aa.*/) {print $1}}' t1.temp"
time gawk '{if ($1 ~ /.*aa[0-9]aa.*/) {print $1}}' t1.temp > shell3.temp

# 4 couldn't figure out how to sort the second key descending
echo "test 4: sort -k1 -k2 t1.temp"
time sort -k1 -k2 t1.temp > shell4.temp


