#include <it.h>

#include <iostream>
#include <string>

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
		}

	}
	catch(std::exception& e) {
		std::cout << e.what() << std::endl;
	}

    conn.Close();
    return 0;
}

