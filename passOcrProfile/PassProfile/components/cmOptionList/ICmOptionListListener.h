// IAsOptionListListener.h
//

#if !defined(_ICMOPTIONLISTLISTENER_H_)
#define _ICMOPTIONLISTLISTENER_H_

#include "cmString.h"

class ICmOptionListListener
{
public:
	virtual void OnInfoUpdate(const cmString& caption, 
							  const cmString& description, 
							  const cmString& tooltipText,
							  UINT tooltipIcon) = 0;
};

#endif //!defined(_ICMOPTIONLISTLISTENER_H_)