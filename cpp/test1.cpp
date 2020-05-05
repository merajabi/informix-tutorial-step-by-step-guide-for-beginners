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
		if ( conn.Error() ) {
		    throw std::runtime_error("Couldn't open connection");
		}

		{
			// Create a query object
			ITQuery query(conn);

			// select the contents of the table
			ITRow *row = query.ExecOneRow("select * from customer limit 1;"); 
			if( !row ) {
				throw std::runtime_error("Query failed!");
			}
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

