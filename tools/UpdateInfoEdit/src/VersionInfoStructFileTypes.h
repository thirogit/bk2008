#ifndef __VERSIONINFOSTRUCTFILETYPES_H__
#define __VERSIONINFOSTRUCTFILETYPES_H__

#define MAX_ARCHIVE_NAME 80
#define MAX_DESCRIPTION 100
#define MIN_VER_NO 1000
#define MAX_FILECHANGED_PATH MAX_PATH

#define VERSION_INFO_HEADER_ID "BKVI01"

typedef struct tagDAYDATE
{
	UINT year;
	UINT month;
	UINT day;
} DAYDATE;

typedef struct tagVERSION_INFO
{
	UINT size;
	UINT version_number;	
	char archive_name[MAX_ARCHIVE_NAME];
	char description[MAX_DESCRIPTION];
	DAYDATE version_date;
	UINT files_changed_count;
	char files_changed[1][MAX_FILECHANGED_PATH];
} VERSION_INFO;

typedef struct tagVERSION_INFO_HEADER
{
	char id[6];
	UINT size;
	UINT version_info_count;	
	UINT max_version_no;
} VERSION_INFO_HEADER;


#endif //__VERSIONINFOSTRUCTFILETYPES_H__