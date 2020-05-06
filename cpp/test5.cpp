#include <it.h>

#include <iostream>

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
		ITRow *row; 
		while ((row = query.NextRow()) != nullptr) {
			std::cout << row->Printable() << std::endl;
			row->Release();                                                           
		}
	}
	catch(std::exception& e) {
		std::cout << e.what() << std::endl;
	}

    conn.Close();
    return 0;
}

