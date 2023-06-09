/// \file sql_string.h
/// \brief Declares an \c std::string derivative that adds some things
/// needed within the library.
///
/// This class adds some flags needed by other parts of MySQL++, and it
/// adds conversion functions from any primitive type.  This helps in
/// inserting these primitive types into the database, because we need
/// everything in string form to build SQL queries.

/***********************************************************************
 Copyright (c) 1998 by Kevin Atkinson, (c) 1999, 2000 and 2001 by
 MySQL AB, and (c) 2004, 2005 by Educational Technology Resources, Inc.
 Others may also hold copyrights on code in this file.  See the CREDITS
 file in the top directory of the distribution for details.

 This file is part of MySQL++.

 MySQL++ is free software; you can redistribute it and/or modify it
 under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 MySQL++ is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with MySQL++; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
 USA
***********************************************************************/

#ifndef MYSQLPP_SQL_STRING_H
#define MYSQLPP_SQL_STRING_H

#include "defs.h"
#include "mysql_func.h"
#include "..\classes\types\NullInt.h"
#include "..\classes\types\CowSex.h"
#include "..\classes\types\Money.h"
#include "..\classes\types\DateTime.h"
#include "..\classes\types\VATRate.h"
#include "..\classes\types\GeoCoordinate.h"
#include "..\classes\types\Time.h"

#include <stdio.h>
#include <string>

#define STREAM_FLOAT_PREC 15


/// \brief A specialized \c std::string that will convert from any
/// valid MySQL type.

class SQLString : public std::string 
{
public:
	bool zerolen2null;
	/// \brief If true, the object's string data is a copy of another
	/// string.  Otherwise, it's the string form of an integral type.
	bool is_string;

	/// \brief If true, the string data doesn't need to be SQL-escaped
	/// when building a query.
	bool dont_escape;

	/// \brief If true, one of the MySQL++ manipulators has processed
	/// the string data.
	///
	/// "Processing" is escaping special SQL characters, and/or adding
	/// quotes.  See the documentation for manip.h for details.
	///
	/// This flag is used by the template query mechanism, to prevent a
	/// string from being re-escaped or re-quoted each time that query
	/// is reused.
	bool processed;

	/// \brief Default constructor; empty string
	SQLString();

	//copy constructor
	SQLString::SQLString(const SQLString &copy);

	/// \brief Create object as a copy of a C++ string
	SQLString(const std::string& str,bool zerolen = false);

	/// \brief Create object as a copy of a C string
	SQLString(const char* str,bool zerolen = false);

	/// \brief Create object as the string form of a \c char value
	SQLString(char i);

    	/// \brief Create object as the string form of an \c unsigned
	/// \c char value
	SQLString(unsigned char i);

	/// \brief Create object as the string form of a \c short \c int
	/// value
	SQLString(short int i);

	/// \brief Create object as the string form of an \c unsigned
	/// \c short \c int value
	SQLString(unsigned short int i);

	/// \brief Create object as the string form of an \c int value
	SQLString(int i);

	/// \brief Create object as the string form of an \c unsigned
	/// \c int value
	SQLString(unsigned int i);

	/// \brief Create object as the string form of a \c longlong
	/// value
	SQLString(longlong i);

	/// \brief Create object as the string form of an \c unsigned
	/// \c longlong value
	SQLString(ulonglong i);

	/// \brief Create object as the string form of a \c float
	/// value
	SQLString(float i);

	/// \brief Create object as the string form of a \c double
	/// value
	SQLString(double i);

	//creates object as string from NullInt object
	SQLString(const NullInt &nullint);

	SQLString(const CowSex &cowsex);

	SQLString(const NullDouble& nulldouble);

	SQLString(const CDateTime &datetime,bool bShortDate = false);


	/// \brief Copy a C string into this object
	SQLString& operator =(const char* str);
	

	/// \brief Copy a C++ \c string into this object
	SQLString& operator =(const std::string& str);
	

	std::string SQLString::quotedstr(void);
	
private:
	void AssignDouble(double d);
	

};

class SQLStringNull : public SQLString
{
public:
	SQLStringNull(): SQLString("NULL")
	{
		is_string = false;
	}
};



#endif

