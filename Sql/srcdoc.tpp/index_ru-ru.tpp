topic "U++ SQL Basics";
[ $$0,0#00000000000000000000000000000000:Default]
[b83;*4 $$1,0#07864147445237544204411237157677:title]
[b83;_R+117 $$2,0#50327538467692101372348457405326:heading1]
[b50;a25;*R2 $$3,3#10189570498413014020290115411120:heading2]
[l25;b25;a25;R2 $$4,4#24747716437513346533020414014942:text2]
[{_}%EN-US 
[s1; U`+`+ SQL&]
[s2; U`+`+ SQL Basics&]
[s3; Database Model&]
[s4; The database model is described by the user in schema (`".sch`") 
files. The user models the database and U`+`+ SQL packages handle 
the creation/modification of the database.&]
[s3; Schema description files (.sch) and sql script files (.sql)&]
[s4; Each database should be described in a schema description file, 
which is a file with extension `".sch`" and . describes what 
tables you want to have in the database. U`+`+ SQL packages will 
take the schema, create `".sql`" script files from it (containing 
DDL statements like `'create table`'...) and execute them against 
the database to realize your database table structure (you do 
not need to write sql statements, U`+`+ will generate them for 
you.)&]
[s4; To add this file to your Upp package, make sure your package 
is selected in the package list (located at the top`-left of 
DinrusIDE), right`-click on the white`-space in the file list (underneath 
the package list at bottom`-left), and select `"Insert package 
directory file(s)`". Then give the new name of your file (eg. 
`"MyDatabase.sch`") and click `"Open`". The new file will show 
up in your file list. &]
[s3; SQL Create, Read, Update, Delete (CRUD)&]
[s4; The four basic functions of database storage are all supported, 
with the ability to have U`+`+ generate the sql statements for 
you. The SQL code written using U`+`+ acts as a supplement for 
writing sql queries themselves. The code is much more descriptive 
and easier to read than SQL. In fact, sql scripts and query strings 
are generated by the U`+`+ SQL classes, and you can view the 
generated queries with a simple method call.&]
[s3; Schema Update/Upgrade&]
[s4; The SQL packages also allow for database updates without losing 
your stored data. The main purpose is to allow incremental development 
of the model. This functionality is limited though, as it is 
implemented using `"ALTER`" sql commands and does not support 
removing columns or changing datatypes. Those functions have 
to be done manually. &]
[s0; ]