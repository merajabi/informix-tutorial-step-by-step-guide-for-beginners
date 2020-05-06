# Informix tutorial, step by step guide for beginners
## summary
This tutorial is for ```IBM Informix Developer Edition```, It will present guides and sample code for connceting to Informix server from C++ clients.
before starting this guide you need working Informix databse server, and you should have client sdk installed on your machine.
to learn how to run docker image of informix developer edition and install client SDK look at the root of this repository.

>[Informix tutorial, step by step guide for beginners](https://github.com/informix/informix-dockerhub-readme)

```IBM Informix Developer Edition```  is free database software for application development and prototyping.

## how to compile C++ programs with Informix Client SDK
In this section I will use direct command line for simplicity, you can use any build system later for your projects.
I assume you have exported these variables before in your ```.profile``` if not, its time to do that

```sh
export INFORMIXSERVER=informix
export INFORMIXDIR=/opt/ibm/informix
```

next we will define these variables.

* IFX_DIFINED: macro needed for ```Informix Client SDK```
* IFX_INCPATH: path to ```Informix Client SDK``` includes
* IFX_LIBPATH: path to ```Informix Client SDK``` libraries
* IFX_LIBS: required libraries from ```Informix Client SDK```
* LIBS: other required system libraries

```sh
IFX_DIFINED="-DLINUX -DIT_HAS_DISTINCT_LONG_DOUBLE -DIT_COMPILER_HAS_LONG_LONG -DIT_DLLIB -DMITRACE_OFF"
IFX_INCPATH="-I$INFORMIXDIR/incl -I$INFORMIXDIR/incl/c++ -I$INFORMIXDIR/incl/dmi -I$INFORMIXDIR/incl/esql"
IFX_LIBPATH="-L$INFORMIXDIR/lib -L$INFORMIXDIR/lib/c++ -L$INFORMIXDIR/lib/dmi -L$INFORMIXDIR/lib/esql"
IFX_LIBS="-lifc++  -lifdmi   -lifsql -lifasf -lifgen -lifos -lifgls -lifglx $INFORMIXDIR/lib/esql/checkapi.o"
LIBS="-lcrypt -lnsl -lm -ldl"
```

Ok, now we are ready to compile our program, assuming we have one file with name ```test1.cpp``` we should issue following command to compile and link our program.

```sh
g++ $IFX_DIFINED -fPIC -fsigned-char $IFX_INCPATH -c test1.cpp
g++ -o test1.out test1.o $IFX_LIBPATH $IFX_LIBS $LIBS
```
Its done, now we could run our program, but before that we might set another environmet variable so linux can find linked libraries in runtime.

```sh
export LD_LIBRARY_PATH="$INFORMIXDIR/lib:$INFORMIXDIR/lib/esql:$INFORMIXDIR/lib/c++:$INFORMIXDIR/lib/dmi:$INFORMIXDIR/lib/cli"
./test1.out
```
the shell script in this folder will do all this for you, plus running the ```valgrind```, call this script with the name of the cpp file as follow.

```sh
./build.sh test1
```
the out put will be somthing like this, at the begining you will see valgrind Output Summary and at the end after your Program Output.

```sh
***	valgrind Output Summary:
---------------------------------------------------
==29708== HEAP SUMMARY:
==29708==     in use at exit: 121,153 bytes in 1,060 blocks
==29708==   total heap usage: 2,074 allocs, 1,014 frees, 546,600 bytes allocated
==29708== 
==29708== Searching for pointers to 1,060 not-freed blocks
==29708== Checked 618,152 bytes
==29708== 
==29708== LEAK SUMMARY:
==29708==    definitely lost: 0 bytes in 0 blocks
==29708==    indirectly lost: 0 bytes in 0 blocks
==29708==      possibly lost: 0 bytes in 0 blocks
==29708==    still reachable: 121,153 bytes in 1,060 blocks
==29708==         suppressed: 0 bytes in 0 blocks
==29708== Reachable blocks (those to which a pointer was found) are not shown.
==29708== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==29708== 
==29708== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
==29708== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

***	Program Output:
---------------------------------------------------
102	Carole Sadler                  	Sports Spot         
```

## connecting to database
ok now that we have the build script let's write our first program.
for all programs in this tutorial to run correctly, you should have created the ```sales_demo``` database and have inserted at least two record on it. 
if you have not done this before, you may read :

>[Informix tutorial, step by step guide for beginners](https://github.com/informix/informix-dockerhub-readme)

or issue following command.

```sql

dbaccess - - <<EOT!
create database sales_demo;
create table customer (
 customer_code integer,
 customer_name char(31),
 company_name char(20)
);
insert into customer values (102, "Carole Sadler", "Sports Spot");
insert into customer values (103, "John Doe", "John Company");
EOT!

```

The class ITDBInfo prepares Connection String and has a constructor like this ```ITDBInfo(Database,Server,User,Password)``` you can also set these parameters individually.

I guess the rest of program is self explanetory, but remmember in all Informix SDK they have used ITString as a string objects, but it has a constructor which accept c_string (char*).

```cpp
#include <it.h>			// Informix Client SDK header
#include <iostream>

int main(int argc, char *argv[])
{
    // Prepare Connection String
    ITDBInfo db("sales_demo","informix","informix","in4mix"); 

    // Prepare Connection Object
    ITConnection conn(db);

    // Open Connection
	conn.Open();

    // Print Operation state
	std::cout << ((conn.IsOpen())?"Success":"Fail") << std::endl;

    // Close Connection
    conn.Close();
    return 0;
}
```

## simple select queries
In this section we will test some simple method for query our database.

In the first example we will select on row from table with ```ExecOneRow``` method, I have commented the program so it may represent what it do.

Before we continue Remember that Informix SDK APIs use old school C++, whenever a method return a pointer you should make sure to call ```Release()``` method to tell the internal reference counter that you have done with the memory, and it can free that space.

```cpp
#include <it.h>
#include <iostream>

int main(int argc, char *argv[])
{
    // Prepare Connection String
    ITDBInfo db("sales_demo","informix","informix","in4mix"); 

    // Prepare Connection Object
    ITConnection conn(db);

    // Open Connection
	conn.Open();
	if ( !conn.IsOpen() ) {
		std::cout << "Couldn't open connection" << std::endl;
		return 1;
	}

	try {
		// Create a query object
		ITQuery query(conn);

		// select one row from contents of the table
		ITRow *row = query.ExecOneRow("select * from customer limit 1;"); 
		if( !row ) {
			throw std::runtime_error("Database Query failed!");
		}

		// Pront the compelete table row to output
		std::cout << row->Printable() << std::endl;

		// Release the row pointer
		row->Release();                                                           
	}
	catch(std::exception& e) {
		std::cout << e.what() << std::endl;
	}

    // Close Connection
    conn.Close();
    return 0;
}
```

now run the progrom to see the output


```sh
./build.sh test1

***	valgrind Output Summary:
---------------------------------------------------
==8059== HEAP SUMMARY:
==8059==     in use at exit: 121,153 bytes in 1,060 blocks
==8059==   total heap usage: 2,074 allocs, 1,014 frees, 546,600 bytes allocated
==8059== 
==8059== Searching for pointers to 1,060 not-freed blocks
==8059== Checked 618,152 bytes
==8059== 
==8059== LEAK SUMMARY:
==8059==    definitely lost: 0 bytes in 0 blocks
==8059==    indirectly lost: 0 bytes in 0 blocks
==8059==      possibly lost: 0 bytes in 0 blocks
==8059==    still reachable: 121,153 bytes in 1,060 blocks
==8059==         suppressed: 0 bytes in 0 blocks
==8059== Reachable blocks (those to which a pointer was found) are not shown.
==8059== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==8059== 
==8059== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
==8059== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

***	Program Output:
---------------------------------------------------
102	Carole Sadler                  	Sports Spot
```

next we will select multiple line from out table, for this perpuse we may use ```ExecToSet``` method as follow.

ITValue class can hold any type of value, a row or even a single column data, no matter what is the type of that column is. in this case it holds a row and ```Printable()``` method will output all the row data at once.

on each call to ```Fetch()``` it will return a pointer to the next row in the set, and you must make sure to Release all these rows and finally the set itself.

```cpp
// Create a query object
ITQuery query(conn);

// select the contents of the table
ITSet *set = query.ExecToSet("select * from customer;"); 
if( !set ) {
	throw std::runtime_error("Query failed!");
}

// show the contents of the table
ITValue *v;
while ((v = set->Fetch()) != nullptr) {
	std::cout << v->Printable() << std::endl;
	v->Release();
}                                                           
set->Release();
```

ok let's see how we can access the columns indivdually.

one way for this is to cast the ITValue to ITRow in the above example and then accessing each columns, you can access retrived columns by index or column name.

you can see the output of ```Column()``` method is ITValue* again, and this time ```Printable()``` will output the value of that column only. just do not forget to release all these pointers.

```cpp
// show the contents of the table
ITValue *val;
while ((val = set->Fetch()) != nullptr) {
	ITRow *row = (ITRow*)val;
	ITValue *col1 = row->Column(0); // row->Column("customer_code")
	ITValue *col2 = row->Column(1); // row->Column("customer_name")
	ITValue *col3 = row->Column(2); // row->Column("company_name")

	std::cout << col1->Printable() << " : " << col2->Printable() << " : " << col3->Printable() << std::endl;

	col1->Release();
	col2->Release();
	col3->Release();

	val->Release();
}
```

There is also another method to query multiple lines from the database, and by the form of implementation I can guess for large table its more efficient too, but I am not sure yet, I am still learning.

now ```ExecForIteration()``` method call do not return any thing but a boolian, later each call to ```NextRow()``` will return a row of results.
and as previous we can extract column by index or name.

```cpp
// Create a query object
ITQuery query(conn);

// select the contents of the table
if( !query.ExecForIteration("select * from customer;") ) {
	throw std::runtime_error("Query failed!");
}

// show the contents of the table
ITRow *row; 
while ((row = query.NextRow()) != nullptr) {
	std::cout << row->Printable() << std::endl;
	row->Release();                                                           
}
```

Ok what about other sql commands, create, insert, update, ...commands? 
there is another method in ```ITQuery``` class named ```ExecForStatus``` which you may use.
for example look at following lines of codes, which drops the customer table and create and populate it again.

```cpp
ITQuery query(conn);
if(	!q.ExecForStatus("drop table customer;") ){
	std::cout << "cannot drop the table" << std::endl;
}
if( !q.ExecForStatus("create table customer (customer_code integer, customer_name char(31), company_name char(20) );") ){
	throw std::runtime_error("cannot not create the table!");
}
if( !q.ExecForStatus("insert into customer values (102, "Carole Sadler", "Sports Spot");") ){
	throw std::runtime_error("cannot insert into the table");
}
if( !q.ExecForStatus("insert into customer values (103, "John Doe", "John Company");") ) {
	throw std::runtime_error("cannot insert into the table");
}
```

so far so good in the next sections we will learn about transactions and prepared statments, but for now I present a simple solution to guard pointer returned from Informix APIs.
Remmmber that we should call release for each pointer that API returns to us. if you are using C++11 or later thats easy.

assume we have folloing lines of code, if exception happens in middle block of code, the parts denoted by ```...```, the memory will be lost

```cpp
do {
	ITRow *row = query.NextRow();
	if(!row) break;
	{
		std::cout << row->Printable() << std::endl;
		...
	}
	row->Release();
}while(true);
```

so I will guard it by using ```std::unique_ptr``` and define a custom deleter lambda to call the Release() function as follow.
this way there is no need to call ```row->Release()``` explicitly, it will be called as soon as the variable goes out of scope.

```cpp
do {
	std::unique_ptr<ITRow, std::function<void(ITRow*)> > row(query.NextRow(), [&](ITRow*){if(row!=nullptr){ row->Release();}} );
	if(!row) break;
	{
		std::cout << row->Printable() << std::endl;
		...
	}
}while(true);
```

