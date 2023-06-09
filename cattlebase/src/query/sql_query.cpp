/***********************************************************************
 sql_query.cpp - Implements the SQLQuery class.

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

#define MYSQLPP_NOT_HEADER
//#include "platform.h"

#include "sql_query.h"

#include "exceptions.h"
#include "mysql_func.h"

using namespace std;



SQLQuery::SQLQuery(const SQLQuery& q) :
stringstream(const_cast<SQLQuery&>(q).str()),	// yes, the cast is evil -- got a better idea?
Success(q.Success),
errmsg(q.errmsg),
def(q.def)
{
}

void SQLQuery::reset()
{
	seekg(0);
	seekp(0);
	clear();
	stringstream::str("");

	parsed.clear();
	def.clear();
}

char* SQLQuery::preview_char()
{
	*this << std::ends;
	size_t length = rdbuf()->str().size();
	char *s = new char[length + 1];
	get(s, length, '\0');
	seekg(0, std::ios::beg);
	seekp(-1, std::ios::cur);
	return s;
}

static SQLString* pprepare(char option, SQLString& S,
		bool replace = true)
{
	SQLString *ss;
	if (S.processed) return &S;

	char optchr = option;

	if(optchr == 'n') //put NULL if string length is 0
	{
		if(!S.length())
		{
			ss = new SQLStringNull();
			if (replace) 
			{
				S = *ss;
				S.processed = true;
				return &S;
			}
			else return ss;
		}
		else optchr = 'q';
	}  
		
	TRACE("S = %s\n",S.c_str());
		
	if (optchr == 'r' || (optchr == 'q' && S.is_string)) 
	{
		char *s = new char[S.size() * 2 + 1];
		escape_string(s, S.c_str(),
				static_cast<unsigned long>(S.size()));
		ss = new SQLString("'");
		*ss += s;
		*ss += "'";
		delete[] s;

		if (replace) 
		{
			S = *ss;
			S.processed = true;
			return &S;
		}
		else return ss;
		
	}
	else if (optchr == 'R' || (optchr == 'Q' && S.is_string)) 
	{
		SQLString *ss = new SQLString("'" + S + "'");

		if (replace) 
		{
			S = *ss;
			S.processed = true;
			return &S;
		}
		else return ss;
		
	}
	else 
	{
		if (replace) S.processed = true;
		return &S;
	}
}

	/*
	if (option == 'r' || (option == 'q' && S.is_string)) 
	{
		char *s = new char[S.size() * 2 + 1];
		escape_string(s, S.c_str(),
				static_cast<unsigned long>(S.size()));
		SQLString *ss = new SQLString("'");
		*ss += s;
		*ss += "'";
		delete[] s;

		if (replace) 
		{
			S = *ss;
			S.processed = true;
			return &S;
		}
		else return ss;
		
	}
	else if (option == 'R' || (option == 'Q' && S.is_string)) 
	{
		SQLString *ss = new SQLString("'" + S + "'");

		if (replace) 
		{
			S = *ss;
			S.processed = true;
			return &S;
		}
		else return ss;
		
	}
	else 
	{
		if (replace) S.processed = true;
		return &S;
	}*/

void SQLQuery::proc(SQLQueryParms& p)
{
	seekg(0, std::ios::beg);
	seekp(0, std::ios::beg);
	char num;
	SQLString *ss;
	SQLQueryParms *c;

	for (std::vector<SQLParseElement>::iterator i = parsed.begin();
			i != parsed.end(); ++i) {
		*this << i->before;
		num = i->num;
		if (num != -1) {
			if (num < static_cast<int>(p.size()))
				c = &p;
			else if (num < static_cast<int>(def.size()))
				c = &def;
			else {
				*this << " ERROR";
				throw
					SQLQueryNEParms
					("Not enough parameters to fill the template.");
			}
			ss = pprepare(i->option, (*c)[num], c->bound());
			*this << *ss;
			if (ss != &(*c)[num]) {
				delete ss;
			}
		}
	}
}

std::string SQLQuery::str(SQLQueryParms& p)
{
	if (!parsed.empty()) {
		proc(p);
	}

	*this << std::ends;

	return stringstream::str();
}

std::string SQLQuery::str(SQLQueryParms& p, query_reset r)
{
	std::string tmp = str(p);
	if (r == RESET_QUERY) {
		reset();
	}
	return tmp;
}

SQLQueryParms SQLQueryParms::operator +(const SQLQueryParms & other) const
{
	if (other.size() <= size()) {
		return *this;
	}
	SQLQueryParms New = *this;
	size_t i;
	for (i = size(); i < other.size(); i++) {
		New.push_back(other[i]);
	}

	return New;
}

void SQLQuery::parse()
{
	std::string str = "";
	char num[4];
	long int n;
	char option;
	std::string name;
	char *s, *s0;
	s0 = s = preview_char();
	while (*s) {
		if (*s == '%') {
			s++;
			if (*s == '%') {
				str += *s++;
			}
			else if (*s >= '0' && *s <= '9') {
				num[0] = *s;
				s++;
				if (*s >= '0' && *s <= '9') {
					num[1] = *s;
					num[2] = 0;
					s++;
					if (*s >= '0' && *s <= '9') {
						num[2] = *s;
						num[3] = 0;
						s++;
					}
					else {
						num[2] = 0;
					}
				}
				else {
					num[1] = 0;
				}

				n = strtol(num, 0, 10);
				option = ' ';

				if (*s == 'n' || *s == 'q' || *s == 'Q' || *s == 'r' || *s == 'R') {
					option = *s++;
				}

				if (*s == ':') {
					s++;
					for ( /* */ ; (*s >= 'A' && *s <= 'Z') ||
						 *s == '_' || (*s >= 'a' && *s <= 'z'); s++) {
						name += *s;
					}

					if (*s == ':') {
						s++;
					}

					if (n >= static_cast<long int>(parsed_names.size())) {
						parsed_names.insert(parsed_names.end(),
								static_cast<vector<string>::size_type>(
										n + 1) - parsed_names.size(),
								string());
					}
					parsed_names[n] = name;
					parsed_nums[name] = n;
				}

				parsed.push_back(SQLParseElement(str, option, char(n)));
				str = "";
				name = "";
			}
			else {
				str += '%';
			}
		}
		else {
			str += *s++;
		}
	}

	parsed.push_back(SQLParseElement(str, ' ', -1));
	delete[] s0;
}


