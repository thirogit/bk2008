// GenerateEmpry.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GenerateEmpry.h"
#include "PassProfilePassProfiles.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		PassProfilePassProfiles profiles;
		PassProfilePassProfileArray profilesArary;
		PassProfilePassProfile profile;
		PassProfilePassField field;

		PassProfileBIRTHINFO bIRTHINFO_;
		PassProfileEAN eAN_;
		PassProfileEANBARCODE eANBARCODE_;
		PassProfileFARMNOBARCODE fARMNOBARCODE_;
		PassProfileFIRSTOWNER fIRSTOWNER_;
		PassProfileMOTHEREAN mOTHEREAN_;
		PassProfilePASSDATE pASSDATE_;
		PassProfilePASSNO pASSNO_;
		PassProfilePassField passField_;
		PassProfileSEX sEX_;
		PassProfileSTOCK sTOCK_;

		field.SetH(0);
		field.SetW(0);
		field.SetX(0);
		field.SetY(0);
		field.SetBarcode(false);
		bIRTHINFO_.SetPassField(field);
		eAN_.SetPassField(field);
		eANBARCODE_.SetPassField(field);
		fARMNOBARCODE_.SetPassField(field);
		fIRSTOWNER_.SetPassField(field);
		mOTHEREAN_.SetPassField(field);
		pASSDATE_.SetPassField(field);
		pASSNO_.SetPassField(field);
		
		sEX_.SetPassField(field);
		sTOCK_.SetPassField(field);

		profile.SetBIRTHINFO(bIRTHINFO_);
		profile.SetEAN(eAN_);
		profile.SetEANBARCODE(eANBARCODE_);
		profile.SetFARMNOBARCODE(fARMNOBARCODE_);
		profile.SetFIRSTOWNER(fIRSTOWNER_);
		profile.SetMOTHEREAN(mOTHEREAN_);
		profile.SetPASSDATE(pASSDATE_);
		profile.SetPASSNO(pASSNO_);
		profile.SetSEX(sEX_);
		profile.SetSTOCK(sTOCK_);

		cmString profileName = _T("PROFILE1");
		profile.SetPassProfileName(profileName);

		profilesArary.Insert(0,profile);
		profiles.SetPassProfileArray(profilesArary);

		cmString outputFile = _T("passprofiles.xml");

		profiles.SaveToXmlFile(outputFile);



	}

	return nRetCode;
}
