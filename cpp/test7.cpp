#include <it.h>
#include <iostream>
#include <memory>
#include <functional>

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

			if( !query.ExecForIteration("select * from customer;") ) {
				throw std::runtime_error("Query failed!");
			}

			// show the contents of the table
			ITRow *rowP; 
			while ((rowP = query.NextRow()) != nullptr) {

				//std::function<void(ITRow*)> f = std::bind(&ITRow::Release, row);
				std::unique_ptr<ITRow, std::function<void(ITRow*)> > row(rowP, std::bind(&ITRow::Release, rowP) );

				//auto f = [&](ITSet*){setP->Release();};
				//std::unique_ptr<ITSet, decltype(f) > set(setP, f );

				std::cout << row->Printable() << std::endl;

				ITValue *col1P = row->Column(0);
				std::unique_ptr<ITValue, std::function<void(ITValue*)> > col1(col1P, std::bind(&ITValue::Release, col1P) );

				ITValue *col2P = row->Column(1);
				std::unique_ptr<ITValue, std::function<void(ITValue*)> > col2(col2P, std::bind(&ITValue::Release, col2P) );

				ITValue *col3P = row->Column(2);
				std::unique_ptr<ITValue, std::function<void(ITValue*)> > col3(col3P, std::bind(&ITValue::Release, col3P) );

				std::cout << col1->Printable() << " : " << col2->Printable() << " : " << col3->Printable() << std::endl;

				//col1->Release();
				//col2->Release();
				//col3->Release();

				//row->Release();                                                           
			}
		}

	}
	catch(std::exception& e) {
		std::cout << e.what() << std::endl;
	}

    conn.Close();
    return 0;
}

