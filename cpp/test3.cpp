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
			ITRow *row;
			while ((row = (ITRow*)set->Fetch()) != nullptr) {
				std::cout << row->Printable() << std::endl;
				ITValue *col1 = row->Column(0); // row->Column("customer_code")
				ITValue *col2 = row->Column(1); // row->Column("customer_name")
				ITValue *col3 = row->Column(2); // row->Column("company_name")

				std::cout << col1->Printable() << " : " << col2->Printable() << " : " << col3->Printable() << std::endl;

				col1->Release();
				col2->Release();
				col3->Release();
				row->Release();
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

