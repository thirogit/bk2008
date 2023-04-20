///////////////////////////////////////////////////////////////////////////////
//
//		ITracer
//		-------
//		Abstract interface for logging data
//
////Holger Kloos, 2003/////////////////////////////////////////////////////////


#ifndef __TRACER_H__
#define __TRACER_H__

interface ITracer
{
	virtual void MakeTrace(const char* sTrace, ...) = 0;
};

#endif