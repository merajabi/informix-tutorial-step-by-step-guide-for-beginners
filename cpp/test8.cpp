#include <it.h>
#include <iostream>
#include <memory>
#include <functional>

#define ITReleaseGuard(TYPE,param,Func) std::unique_ptr<TYPE, std::function<void(TYPE*)> > param(Func, [&](TYPE*){if(param!=nullptr){ param->Release();}} )
										//std::unique_ptr<ITRow, std::function<void(ITRow*)> > row(query.NextRow(), [&](ITRow*){if(param!=nullptr){ row->Release();}} );

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
			ITReleaseGuard(ITRow,row,query.NextRow());
			if(!row) break;

			ITReleaseGuard(ITValue,col0,row->Column(0));
			ITReleaseGuard(ITValue,col1,row->Column(1));
			ITReleaseGuard(ITValue,col2,row->Column(2));

			std::cout << col0->Printable() << " : " << col1->Printable() << " : " << col2->Printable() << std::endl;
		}while(true);
	}
	catch(std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	conn.Close();
    return 0;
}

