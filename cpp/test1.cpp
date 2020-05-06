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

