
#ifndef Data_error___h
#define Data_error___h

#define DATA_TRY \
	try {

#define SOCI_ODBC_CATCH													\
	} catch (const soci::odbc_soci_error& e) {							\
		std::ostringstream buf;											\
		buf << "Error: " << e.what() << std::endl						\
			<< "Native Code: " << e.native_error_code() << std::endl	\
			<< "ODBC Code: " << e.odbc_error_code() << std::endl		\
			<< "ODBC Message: " << e.odbc_error_message() << std::endl;	\
		throw std::exception(buf.str().c_str());						\
    }																	


#endif

