/* soapcalcProxy.h
   Generated by gSOAP 2.8.0 from calc.h
   Copyright(C) 2000-2010, Robert van Engelen, Genivia Inc. All Rights Reserved.
   The generated code is released under one of the following licenses:
   GPL, the gSOAP public license, or Genivia's license for commercial use.
*/

#ifndef soapcalcProxy_H
#define soapcalcProxy_H
#include "soapH.h"

class SOAP_CMAC calcProxy : public soap
{ public:
	/// Endpoint URL of service 'calcProxy' (change as needed)
	const char *soap_endpoint;
	/// Constructor
	calcProxy();
	/// Constructor with copy of another engine state
	calcProxy(const struct soap&);
	/// Constructor with engine input+output mode control
	calcProxy(soap_mode iomode);
	/// Constructor with engine input and output mode control
	calcProxy(soap_mode imode, soap_mode omode);
	/// Destructor frees deserialized data
	virtual	~calcProxy();
	/// Initializer used by constructors
	virtual	void calcProxy_init(soap_mode imode, soap_mode omode);
	/// Delete all deserialized data (uses soap_destroy and soap_end)
	virtual	void destroy();
	/// Disables and removes SOAP Header from message
	virtual	void soap_noheader();
	/// Get SOAP Fault structure (NULL when absent)
	virtual	const SOAP_ENV__Fault *soap_fault();
	/// Get SOAP Fault string (NULL when absent)
	virtual	const char *soap_fault_string();
	/// Get SOAP Fault detail as string (NULL when absent)
	virtual	const char *soap_fault_detail();
	/// Force close connection (normally automatic, except for send_X ops)
	virtual	int soap_close_socket();
	/// Print fault
	virtual	void soap_print_fault(FILE*);
#ifndef WITH_LEAN
	/// Print fault to stream
	virtual	void soap_stream_fault(std::ostream&);
	/// Put fault into buffer
	virtual	char *soap_sprint_fault(char *buf, size_t len);
#endif

	/// Web service operation 'add' (returns error code or SOAP_OK)
	virtual	int add(double a, double b, double *result);

	/// Web service operation 'sub' (returns error code or SOAP_OK)
	virtual	int sub(double a, double b, double *result);

	/// Web service operation 'mul' (returns error code or SOAP_OK)
	virtual	int mul(double a, double b, double *result);

	/// Web service operation 'div' (returns error code or SOAP_OK)
	virtual	int div(double a, double b, double *result);

	/// Web service operation 'pow' (returns error code or SOAP_OK)
	virtual	int pow(double a, double b, double *result);
};
#endif
