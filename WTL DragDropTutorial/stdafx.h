// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__1AB5E883_E5CD_4C40_82A5_E272A27176C9__INCLUDED_)
#define AFX_STDAFX_H__1AB5E883_E5CD_4C40_82A5_E272A27176C9__INCLUDED_

// Change these values to use different versions
#define WINVER          0x0500
#define _WIN32_WINNT    0x0501
#define _WIN32_IE       0x0600

#define _ATL_APARTMENT_THREADED
#define _WTL_USE_CSTRING
#define _ATL_NO_MSIMG
#define _ATL_NO_OPENGL

// ATL/WTL
#include <atlbase.h>
#include <atlapp.h>
extern CAppModule _Module;
#include <atlcom.h>
#include <atlwin.h>
#include <atlframe.h>
#include <atlcrack.h>
#include <atlmisc.h>
#include <atlctrls.h>
#include <atlctrlx.h>
#include <atldlgs.h>

// STL
#include <vector>

// CRT
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

// Win32
#include <shellapi.h>
#include <commoncontrols.h>     // IImageList definitions

// Other libraries

// If you get an error here, you need to have the CAB SDK installed in the same
// dir as the source, in a dir called "cabsdk".
#include "cabsdk/include/fci.h"
#include "cabsdk/include/fdi.h"

// Global data
extern bool g_bXPOrLater;
extern std::vector<CString> g_vecsTempFiles;
const LPCTSTR APP_SETTINGS_KEY = _T("software\\Mike's Classy Software\\WTLCabView");

// Types
struct CCompressedFileInfo
{
    CString  sFilename;     // name of the file as stored in the CAB
    CString  sFileType;     // descriptio of the file type
    DWORD    dwFileSize;    // uncompressed size of the file
    FILETIME ftDateTime;    // modified date/time of the file
    UINT     uAttribs;      // file attributes
    enum { from_prev_cab, in_this_cab, to_next_cab } location;
    CString  sOtherCabName; // name of the prev/next CAB
    bool     bExtractable;  // true => we can extract this file

    CCompressedFileInfo() :
        dwFileSize(0), uAttribs(0), location(in_this_cab), bExtractable(true)
    { }
};

struct CDraggedFileInfo
{
    // Data set at the beginning of a drag/drop:
    CString sFilename;      // name of the file as stored in the CAB
    CString sTempFilePath;  // path to the file we extract from the CAB
    int nListIdx;           // index of this item in the list ctrl

    // Data set while extracting files:
    bool bPartialFile;      // true if this file is continued in another cab
    CString sCabName;       // name of the CAB file
    bool bCabMissing;       // true if the file is partially in this cab and
                            // the CAB it's continued in isn't found, meaning
                            // the file can't be extracted

    CDraggedFileInfo ( const CString& s, int n ) :
        sFilename(s), nListIdx(n), bPartialFile(false), bCabMissing(false)
    { }
};

// Version of CComObjectStack that doesn't freak out and assert when IUnknown
// methods are called.
template <class Base>
class CComObjectStack2 : public CComObjectStack<Base>
{
public:
    CComObjectStack2() : CComObjectStack<Base>() { }

    STDMETHOD_(ULONG, AddRef)() { return 1; }
    STDMETHOD_(ULONG, Release)() { return 1; }

    STDMETHOD(QueryInterface)(REFIID iid, void ** ppvObject)
        { return _InternalQueryInterface(iid, ppvObject); }
};

// Convenience macros
#define countof(x) (sizeof(x)/sizeof((x)[0]))
#define _S(x) (CString(LPCTSTR(x)))

#if _ATL_VER < 0x0700
#undef BEGIN_MSG_MAP
#define BEGIN_MSG_MAP(x) BEGIN_MSG_MAP_EX(x)
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__1AB5E883_E5CD_4C40_82A5_E272A27176C9__INCLUDED_)
