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
			ITValue *col1 = row->Column("customer_code"); //row->Column(0);
			ITValue *col2 = row->Column("customer_name"); //row->Column(1);
			ITValue *col3 = row->Column("company_name"); // row->Column(2);

			std::cout << col1->Printable() << " : " << col2->Printable() << " : " << col3->Printable() << std::endl;

			col1->Release();
			col2->Release();
			col3->Release();

			row->Release();                                                           
		}
	}
	catch(std::exception& e) {
		std::cout << e.what() << std::endl;
	}

    conn.Close();
    return 0;
}

