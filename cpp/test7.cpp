#include <it.h>
#include <iostream>
#include <memory>
#include <functional>

void ITRelease(ITEssential* itObject){
	if(itObject) {
		itObject->Release();
	}
}

int main(int argc, char *argv[])
{
    // Prepare Connection String
    ITDBInfo db("sales_demo","informix","informix","in4mix"); 

    // Prepare Connection Object
    ITConnection conn(db);

	try {
		conn.Open();
		if ( !conn.IsOpen() ) {
		    throw std::runtime_error("Couldn't open connection");
		}

		// Create a query object
		ITQuery query(conn);

		// select the contents of the table

		if( !query.ExecForIteration("select * from customer;") ) {
			throw std::runtime_error("Query failed!");
		}

		// show the contents of the table
		do {
			ITRow *rowP = query.NextRow();
			if(!rowP) break;
			std::unique_ptr<ITRow, std::function<void(ITEssential*)> > row(rowP, std::bind(&::ITRelease, rowP) );

			ITValue *col1P = row->Column(0);
			std::unique_ptr<ITValue, std::function<void(ITEssential*)> > col1(col1P, std::bind(&::ITRelease, col1P) );

			ITValue *col2P = row->Column(1);
			std::unique_ptr<ITValue, std::function<void(ITEssential*)> > col2(col2P, std::bind(&::ITRelease, col2P) );

			ITValue *col3P = row->Column(2);
			std::unique_ptr<ITValue, std::function<void(ITEssential*)> > col3(col3P, std::bind(&::ITRelease, col3P) );

			std::cout << col1->Printable() << " : " << col2->Printable() << " : " << col3->Printable() << std::endl;
		}while(true);
	}
	catch(std::exception& e) {
		std::cout << e.what() << std::endl;
	}

    conn.Close();
    return 0;
}

