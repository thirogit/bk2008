/***********************************************************************
 sql_string.cpp - Implements the SQLString template.

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
#include "stdafx.h"
#include "sql_string.h"

#include <sstream>

using namespace std;



SQLString::SQLString(const SQLString &copy) :
is_string(copy.is_string),
dont_escape(copy.dont_escape),
processed(copy.processed),
zerolen2null(copy.zerolen2null),
string(copy)
{
	
}

SQLString::SQLString() :
is_string(false),
dont_escape(false),
processed(false),
zerolen2null(false)
{
}

SQLString::SQLString(const string& str,bool zerolen) :
string(str),
is_string(true),
dont_escape(false),
processed(false),
zerolen2null(zerolen)
{
}

SQLString::SQLString(const char* str,bool zerolen) : 
string(str),
is_string(true),
dont_escape(false),
processed(false),
zerolen2null(zerolen)
{
}

SQLString::SQLString(char i) :
is_string(false),
dont_escape(false),
processed(false)
{
	ostringstream outs;
	outs << static_cast<short int>(i);
	assign(outs.str());
}

SQLString::SQLString(unsigned char i) :
is_string(false),
dont_escape(false),
processed(false)
{
	ostringstream outs;
	outs << static_cast<unsigned short int>(i);
	assign(outs.str());
}

SQLString::SQLString(short int i) :
is_string(false),
dont_escape(false),
processed(false)
{
	ostringstream outs;
	outs << i;
	assign(outs.str());
}

SQLString::SQLString(unsigned short int i) :
is_string(false),
dont_escape(false),
processed(false)
{
	ostringstream outs;
	outs << i;
	assign(outs.str());
}

SQLString::SQLString(int i) :
is_string(false),
dont_escape(false),
processed(false)
{
	ostringstream outs;
	outs << i;
	assign(outs.str());
}

SQLString::SQLString(unsigned int i) :
is_string(false),
dont_escape(false),
processed(false)
{
	ostringstream outs;
	outs << i;
	assign(outs.str());
}

SQLString::SQLString(longlong i) :
is_string(false),
dont_escape(false),
processed(false)
{
	ostringstream outs;
	outs << i;
	assign(outs.str());
}

SQLString::SQLString(ulonglong i) :
is_string(false),
dont_escape(false),
processed(false) 
{
	ostringstream outs;
	outs << i;
	assign(outs.str());
}

SQLString::SQLString(float f) :
is_string(false),
dont_escape(false),
processed(false)
{
	AssignDouble((double)f);
}

SQLString::SQLString(double f) :
is_string(false),
dont_escape(false),
processed(false)
{
	AssignDouble(f);
}

void SQLString::AssignDouble(double d)
{
	ostringstream outs;
	outs.precision(STREAM_FLOAT_PREC);
	outs << d;
	assign(outs.str());
}

SQLString::SQLString(const NullInt &nullint) :
is_string(false),
dont_escape(false),
processed(false)
{
	ostringstream outs;
	if(nullint.IsNull())
	 outs << "NULL";
	else outs << nullint.ToInt();
	assign(outs.str());


}


SQLString::SQLString(const NullDouble &nulldouble) : 
is_string(false),
dont_escape(false),
processed(false)
{
	ostringstream outs;
	if(nulldouble.IsNull())
	{
		outs << "NULL";
		assign(outs.str());
	}
	else
	{
		AssignDouble(nulldouble.GetDouble());
	}
}

SQLString::SQLString(const CowSex &cowsex) :
is_string(false),
dont_escape(false),
processed(false)
{
	ostringstream outs;
	outs << (int)cowsex.getCOWSEX();
	assign(outs.str());

}

SQLString::SQLString(const CDateTime &datetime,bool bShortDate/* = false*/) :
dont_escape(false),
processed(false)
{
	ostringstream outs;
	if(!datetime.IsNull())
	{	
		is_string = true;
		if(bShortDate)
			outs << datetime.GetDateInFmt(CDateTime::YYYYdotMMdotDD);
		else
			outs << datetime.GetDateInFmt(CDateTime::YYYYdotMMdotDD_HHcolonMMcolonSS);
		
	}
	else
	{
		is_string = false;
		outs << "NULL";
	}
	assign(outs.str());	

}


SQLString& SQLString::operator =(const char* str)
{
	std::string::operator =(str);
	return *this;
}

	
SQLString& SQLString::operator =(const std::string& str)
{
	std::string::operator =(str);
	return *this;
}

std::string SQLString::quotedstr(void)
{
	if(!length() && zerolen2null)
	{	
		return (std::string)"NULL";
	}
	if (is_string) 
	{
		if (!dont_escape) 
		{
			char* s = new char[size() * 2 + 1];
			escape_string(s, c_str(), size());
			std::string ret = s;	
			delete []s;
			return '\'' + ret + '\'';
		}			
	
	}
	return  *this;
}
