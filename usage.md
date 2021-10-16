Just showing off features here... For more details and options, try the `--help` option.

fql is a command line program that targets Linux and is not cross-platform. For the following, let's assume a bash shell.
fql allows you to treat your delimited or fixed-length files as tables without having to import them into a database.

### *Basic* command line usage

By default, queries are read from stdin or from a file argument if one is provided. It can also be sent as an argument
to the `-c (--command)` option.
```sh
fql <<< "select 'hello world!'"
fql hello.sql
```

Personally, I prefer a here-doc especially for bigger queries.  It just looks cleaner, while still giving you access
to shell variables (if that is an option).
```sh
fql <<FQL
select id
from [${filename}]
group by ID
having count(*) > 1
FQL
```

Alternatively, if this is not a safe option, variables can be declared on the command line with `-D (--declare)`.
```sh
fql -D "mytable=${filename}" <<FQL
select id
from @mytable
group by ID
having count(*) > 1
FQL
```
Note: If a variable is used as a table, it must be defined on the command line or via the API (`fql_import_variable`).

More standard style variables can be declared as well.
```sh
fql -D "user_name=${user_name}" <<FQL
select id
from userlist
where user_name = @user_name
FQL
```


If stdin is not being used to read a query, that frees it up to be used as another table.  fql has a handful of internal
features that are named with 2 leading underscores.  One of them is `__stdin`.

```sh
fql --command "select * from __stdin where state = 'MO' order by city" < addresslist.txt
```

Likewise, that means fql can be used in a pipeline.  For this example, let's assume a sql file that references `__stdin`.
Something like...

```sql
/* nameupdate.sql */

update N
set first_name = 'Robert'
from [names.txt] n
join __stdin s
    on s.ID = N.ID
where s.fullname like '[rb]ob%'
```
Now, we can do this:
```sh
cat namelist.txt | fql nameupdate.sql
```

In the spirit of the caseless nature of SQL, tables names are also caselessly matched to files. And... In the spirit of not
having to use brackets around each table name, the extension of the file can be ommited as well. So these 2 queries are
equivalent (if we assume no ambiguity in file names):


```sql
select * from [names.csv]
/* is the same as... */
SELECT * fRoM NAMES
```

Assuming `select * from t1`, here is the priority list for matching files:
```
RULE                   EXAMPLE MATCHES
exact match            t1
exact ignoring case    T1,t1
ignore extension       t1.txt,t1.csv
ignore extension/case  t1.txt,T1.csv
```

If the brackets are missing from a table name with an extension, the parser will
see that as a `SCHEMA.TABLE` combination, and will probably not find the file you
were looking for.

### Schema

The schema can be defined to tell fql what kind of file we are working with.  This is better explained by example...

The default layout that fql expects is a delimited file with headers on the first row.  fql can also be set to work
Let's say the math teacher was nice enough to give us the grades in the default layout. Imagine your standard csv file:

**math_grades.csv**
```none
id,name,grade
1,Joe Shepard,55
2,John Davidson,88
3,"Dwayne ""The Rock"" Johnson",99
```

Now, for comparison, let's the art teacher gave us the class grades in tab delimited files with no header. AND, to
make matters worse, she put spaces in the file name...
A pre-installed schema that should install with fql called "noheader" can handle this for us.

**art grades.txt**
```none
1       Joe Shepard     61
2       John Davidson   77
3       "Dwayne ""The Rock"" Johnson"   66
```
I can no longer address the 3rd field as `[grade]` like before, but the noheader schema allows
us to address it with with the internal name `__<field number>`.  Example...

```sh
fql <<< "select * from noheader.[art grades] where __3 > 60"
```
Note that we need brackets around the table name here because of the space.  Otherwise, the parser
will see this as a `TABLE ALIAS` combination.

If you want to keep your query a little cleaner, you can define noheader as the default schema...
```sh
fql --schema=noheader <<< "select * from [art grades.txt] where __3 > 60"
```

And one more step further... two files need not have the same layout in order to interact.  A pipe delimited
file with headers will interface with a comma delimited file without headers or even a fixed-length file seemlessly.
If we use our example layouts above, we can join those 2 grade files with ease...
```sh
fql <<FQL
select A.__1 ID, m.name [Student Name]
into [passing.txt]
from math_grades M
join noheader.[art grades] A
    on A.__1 = m.ID
where (cast(a.__3 as int) + m.grade) / 2.0 > 60
order by 1 desc
FQL
```

When using a join, the output will be in the same format as the left side of the join. So the
output of the previous query would be comma delimited with a header:
```none
ID,Student Name
3,"Dwayne ""The Rock"" Johnson"
2,John Davidson
```

### Types

Note in the WHERE clause in the above example. We had to cast one of the addition inputs to an INT.  By default, 
fields from files are a VARCHAR (string) type.  And if we add 2 strings, we will concatenate.  Casting one of 
them to an INT causes an implicit cast on the second addition input.

fql only has 3 types. These are STRING, INT, and FLOAT.  Casting to string requires casting to either VARCHAR, TEXT
or CHAR.  The only difference is casting to CHAR will add trailing spaces.

The query...
```sql
select cast('bob' as char(10)), 'xx'
```
should yield:
```none
bob       ,xx
```

Scalar string functions assume data to be UTF-8 by default. So the expression `right('♗♕♔♗♘', 3)` should give
us `♔♗♘`.  This can be toggled with the `--char-as-byte (-b)` option.  Given this option, the previous query
only gives us `♘` since these chess piece characters are all 3 bytes each in UTF-8.

### Examples




### Re-inventing the wheel

fql is not a new idea.  There are multiple similar projects (csvsql, csvq, q, textql) that are loaded with
more bells and whistles, however, they do not scale well with the size of the input. fql aims closer in speed
to an actual relational-database system.  At the bare minimum, I would *always* expect a single query to out-perform
a relational-database if we factor in the time required to import the data into that database's native format.
