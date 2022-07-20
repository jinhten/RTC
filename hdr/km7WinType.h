#ifndef __km7WinType_H__
#define __km7WinType_H__

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <climits>

//아래의 RGB, GetRValue, GetGValue, GetBValue는 WinGDI.h에 선언된 내용
#ifndef RGB
#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#endif

#ifndef GetRValue                           //GetRValue가 선언되어 있지 않다면
#define GetRValue(rgb)      ( (BYTE)(rgb) ) //GetRValue를 내가 선언
#endif
#ifndef GetGValue
#define GetGValue(rgb)      ( (BYTE)(((WORD)(rgb))>>8) )
#endif
#ifndef GetBValue
#define GetBValue(rgb)      ( (BYTE)((rgb)>>16) )
#endif

#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L
#define BI_JPEG       4L
#define BI_PNG        5L

//아래의 DWORD, WORD, BYTE, COLORREF는 WinDef.h에 선언된 내용
typedef int                 errno_t;
typedef unsigned long       DWORD;
typedef DWORD               COLORREF;
typedef unsigned short      WORD;
typedef unsigned char       BYTE;
typedef long long           LONG;

typedef bool                BOOL;
typedef float               FLOAT;
typedef FLOAT*              PFLOAT;
typedef BOOL*               PBOOL;
typedef BOOL*               LPBOOL;
typedef BYTE*               PBYTE;
typedef BYTE*               LPBYTE;
typedef int*                PINT;
typedef int*                LPINT;
typedef WORD*               PWORD;
typedef WORD*               LPWORD;
typedef long*               LPLONG;
typedef DWORD*              PDWORD;
typedef DWORD*              LPDWORD;
typedef void*               LPVOID;
typedef const void*         LPCVOID;

typedef char*               LPSTR;
typedef const char*         LPCSTR;
typedef wchar_t*            LPWSTR;
typedef const wchar_t*      LPCWSTR;

typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int*       PUINT;

typedef in_addr             IN_ADDR;
typedef sockaddr*           LPSOCKADDR;


typedef struct tagBITMAPFILEHEADER {
    WORD    bfType;
    DWORD   bfSize;
    WORD    bfReserved1;
    WORD    bfReserved2;
    DWORD   bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER{
    DWORD  biSize;
    LONG   biWidth;
    LONG   biHeight;
    WORD   biPlanes;
    WORD   biBitCount;
    DWORD  biCompression;
    DWORD  biSizeImage;
    LONG   biXPelsPerMeter;
    LONG   biYPelsPerMeter;
    DWORD  biClrUsed;
    DWORD  biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

typedef struct tagRGBQUAD {
    BYTE    rgbBlue;
    BYTE    rgbGreen;
    BYTE    rgbRed; 
    BYTE    rgbReserved; 
} RGBQUAD;

typedef struct tagBITMAPINFO {
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD          bmiColors[1];
} BITMAPINFO, *LPBITMAPINFO, *PBITMAPINFO;

typedef LONG LSTATUS;
typedef void *HKEY;
typedef void *HANDLE;
typedef long long ULONG_PTR;

struct _wfinddata64_t
{
    unsigned   attrib;
    LONG       time_create;    // -1 for FAT file systems
    LONG       time_access;    // -1 for FAT file systems
    LONG       time_write;
    int64      size;
    wchar_t    name[260];
};

#define HKEY_CLASSES_ROOT                   (( HKEY ) (ULONG_PTR)((LONG)0x80000000) )
#define HKEY_CURRENT_USER                   (( HKEY ) (ULONG_PTR)((LONG)0x80000001) )
#define HKEY_LOCAL_MACHINE                  (( HKEY ) (ULONG_PTR)((LONG)0x80000002) )
#define HKEY_USERS                          (( HKEY ) (ULONG_PTR)((LONG)0x80000003) )
#define HKEY_PERFORMANCE_DATA               (( HKEY ) (ULONG_PTR)((LONG)0x80000004) )
#define HKEY_PERFORMANCE_TEXT               (( HKEY ) (ULONG_PTR)((LONG)0x80000050) )
#define HKEY_PERFORMANCE_NLSTEXT            (( HKEY ) (ULONG_PTR)((LONG)0x80000060) )

#define REG_OPTION_RESERVED         (0x00000000L)   // Parameter is reserved

#define REG_OPTION_NON_VOLATILE     (0x00000000L)   // Key is preserved
                                                    // when system is rebooted

#define REG_OPTION_VOLATILE         (0x00000001L)   // Key is not preserved
                                                    // when system is rebooted

#define REG_OPTION_CREATE_LINK      (0x00000002L)   // Created key is a
                                                    // symbolic link

#define REG_OPTION_BACKUP_RESTORE   (0x00000004L)   // open for backup or restore
                                                    // special access rules
                                                    // privilege required

#define REG_OPTION_OPEN_LINK        (0x00000008L)   // Open symbolic link

#define REG_OPTION_DONT_VIRTUALIZE  (0x00000010L)   // Disable Open/Read/Write
                                                    // virtualization for this
                                                    // open and the resulting
                                                    // handle.

#define KEY_QUERY_VALUE         (0x0001)
#define KEY_SET_VALUE           (0x0002)
#define KEY_CREATE_SUB_KEY      (0x0004)
#define KEY_ENUMERATE_SUB_KEYS  (0x0008)
#define KEY_NOTIFY              (0x0010)
#define KEY_CREATE_LINK         (0x0020)
#define KEY_WOW64_32KEY         (0x0200)
#define KEY_WOW64_64KEY         (0x0100)
#define KEY_WOW64_RES           (0x0300)

#define KEY_READ                ((STANDARD_RIGHTS_READ       |\
                                  KEY_QUERY_VALUE            |\
                                  KEY_ENUMERATE_SUB_KEYS     |\
                                  KEY_NOTIFY)                 \
                                  &                           \
                                 (~SYNCHRONIZE))


#define KEY_WRITE               ((STANDARD_RIGHTS_WRITE      |\
                                  KEY_SET_VALUE              |\
                                  KEY_CREATE_SUB_KEY)         \
                                  &                           \
                                 (~SYNCHRONIZE))

#define KEY_EXECUTE             ((KEY_READ)                   \
                                  &                           \
                                 (~SYNCHRONIZE))

#define KEY_ALL_ACCESS          ((STANDARD_RIGHTS_ALL        |\
                                  KEY_QUERY_VALUE            |\
                                  KEY_SET_VALUE              |\
                                  KEY_CREATE_SUB_KEY         |\
                                  KEY_ENUMERATE_SUB_KEYS     |\
                                  KEY_NOTIFY                 |\
                                  KEY_CREATE_LINK)            \
                                  &                           \
                                 (~SYNCHRONIZE))

#define STANDARD_RIGHTS_ALL              (0x001F0000L)

#define SPECIFIC_RIGHTS_ALL              (0x0000FFFFL)

#define DELETE                           (0x00010000L)
#define READ_CONTROL                     (0x00020000L)
#define WRITE_DAC                        (0x00040000L)
#define WRITE_OWNER                      (0x00080000L)
#define SYNCHRONIZE                      (0x00100000L)

#define ERROR_SUCCESS                    0L

#define REG_NONE                    ( 0ul ) // No value type
#define REG_SZ                      ( 1ul ) // Unicode nul terminated string
#define REG_EXPAND_SZ               ( 2ul ) // Unicode nul terminated string
                                            // (with environment variable references)
#define REG_BINARY                  ( 3ul ) // Free form binary
#define REG_DWORD                   ( 4ul ) // 32-bit number
#define REG_DWORD_LITTLE_ENDIAN     ( 4ul ) // 32-bit number (same as REG_DWORD)
#define REG_DWORD_BIG_ENDIAN        ( 5ul ) // 32-bit number
#define REG_LINK                    ( 6ul ) // Symbolic Link (unicode)
#define REG_MULTI_SZ                ( 7ul ) // Multiple Unicode strings
#define REG_RESOURCE_LIST           ( 8ul ) // Resource list in the resource map
#define REG_FULL_RESOURCE_DESCRIPTOR ( 9ul ) // Resource list in the hardware description
#define REG_RESOURCE_REQUIREMENTS_LIST ( 10ul )
#define REG_QWORD                   ( 11ul ) // 64-bit number
#define REG_QWORD_LITTLE_ENDIAN     ( 11ul ) // 64-bit number (same as REG_QWORD)

#define INFINITE            0xFFFFFFFF  // Infinite timeout

#define _A_NORMAL 0x00 // Normal file - No read/write restrictions
#define _A_RDONLY 0x01 // Read only file
#define _A_HIDDEN 0x02 // Hidden file
#define _A_SYSTEM 0x04 // System file
#define _A_SUBDIR 0x10 // Subdirectory
#define _A_ARCH   0x20 // Archive file

#define SD_RECEIVE 0
#define SD_SEND 1
#define SD_BOTH 2


#define FILE_SHARE_READ                 0x00000001  
#define FILE_SHARE_WRITE                0x00000002  
#define FILE_SHARE_DELETE               0x00000004  
#define FILE_ATTRIBUTE_READONLY             0x00000001  
#define FILE_ATTRIBUTE_HIDDEN               0x00000002  
#define FILE_ATTRIBUTE_SYSTEM               0x00000004  
#define FILE_ATTRIBUTE_DIRECTORY            0x00000010  
#define FILE_ATTRIBUTE_ARCHIVE              0x00000020  
#define FILE_ATTRIBUTE_DEVICE               0x00000040  
#define FILE_ATTRIBUTE_NORMAL               0x00000080  
#define FILE_ATTRIBUTE_TEMPORARY            0x00000100  
#define FILE_ATTRIBUTE_SPARSE_FILE          0x00000200  
#define FILE_ATTRIBUTE_REPARSE_POINT        0x00000400  
#define FILE_ATTRIBUTE_COMPRESSED           0x00000800  
#define FILE_ATTRIBUTE_OFFLINE              0x00001000  
#define FILE_ATTRIBUTE_NOT_CONTENT_INDEXED  0x00002000  
#define FILE_ATTRIBUTE_ENCRYPTED            0x00004000  
#define FILE_ATTRIBUTE_INTEGRITY_STREAM     0x00008000  
#define FILE_ATTRIBUTE_VIRTUAL              0x00010000  
#define FILE_ATTRIBUTE_NO_SCRUB_DATA        0x00020000  
#define FILE_ATTRIBUTE_EA                   0x00040000  
#define FILE_ATTRIBUTE_PINNED               0x00080000  
#define FILE_ATTRIBUTE_UNPINNED             0x00100000  
#define FILE_ATTRIBUTE_RECALL_ON_OPEN       0x00040000  
#define FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS 0x00400000 
#define TREE_CONNECT_ATTRIBUTE_PRIVACY      0x00004000  
#define TREE_CONNECT_ATTRIBUTE_INTEGRITY    0x00008000  
#define TREE_CONNECT_ATTRIBUTE_GLOBAL       0x00000004  
#define TREE_CONNECT_ATTRIBUTE_PINNED       0x00000002  
#define FILE_ATTRIBUTE_STRICTLY_SEQUENTIAL  0x20000000  
#define FILE_NOTIFY_CHANGE_FILE_NAME    0x00000001   
#define FILE_NOTIFY_CHANGE_DIR_NAME     0x00000002   
#define FILE_NOTIFY_CHANGE_ATTRIBUTES   0x00000004   
#define FILE_NOTIFY_CHANGE_SIZE         0x00000008   
#define FILE_NOTIFY_CHANGE_LAST_WRITE   0x00000010   
#define FILE_NOTIFY_CHANGE_LAST_ACCESS  0x00000020   
#define FILE_NOTIFY_CHANGE_CREATION     0x00000040   
#define FILE_NOTIFY_CHANGE_SECURITY     0x00000100   
#define FILE_ACTION_ADDED                   0x00000001   
#define FILE_ACTION_REMOVED                 0x00000002   
#define FILE_ACTION_MODIFIED                0x00000003   
#define FILE_ACTION_RENAMED_OLD_NAME        0x00000004   
#define FILE_ACTION_RENAMED_NEW_NAME        0x00000005   
#define MAILSLOT_NO_MESSAGE             ((DWORD)-1) 
#define MAILSLOT_WAIT_FOREVER           ((DWORD)-1) 
#define FILE_CASE_SENSITIVE_SEARCH          0x00000001  
#define FILE_CASE_PRESERVED_NAMES           0x00000002  
#define FILE_UNICODE_ON_DISK                0x00000004  
#define FILE_PERSISTENT_ACLS                0x00000008  
#define FILE_FILE_COMPRESSION               0x00000010  
#define FILE_VOLUME_QUOTAS                  0x00000020  
#define FILE_SUPPORTS_SPARSE_FILES          0x00000040  
#define FILE_SUPPORTS_REPARSE_POINTS        0x00000080  
#define FILE_SUPPORTS_REMOTE_STORAGE        0x00000100  
#define FILE_RETURNS_CLEANUP_RESULT_INFO    0x00000200  
#define FILE_SUPPORTS_POSIX_UNLINK_RENAME   0x00000400  

typedef struct _LIST_ENTRY {
   struct _LIST_ENTRY *Flink;
   struct _LIST_ENTRY *Blink;
} LIST_ENTRY, *PLIST_ENTRY;

typedef struct _RTL_CRITICAL_SECTION_DEBUG {
    WORD   Type;
    WORD   CreatorBackTraceIndex;
    struct _RTL_CRITICAL_SECTION *CriticalSection;
    LIST_ENTRY ProcessLocksList;
    DWORD EntryCount;
    DWORD ContentionCount;
    DWORD Flags;
    WORD   CreatorBackTraceIndexHigh;
    WORD   SpareWORD  ;
} RTL_CRITICAL_SECTION_DEBUG, *PRTL_CRITICAL_SECTION_DEBUG, RTL_RESOURCE_DEBUG, *PRTL_RESOURCE_DEBUG;

typedef struct _RTL_CRITICAL_SECTION {
    PRTL_CRITICAL_SECTION_DEBUG DebugInfo;

    //
    //  The following three fields control entering and exiting the critical
    //  section for the resource
    //

    LONG LockCount;
    LONG RecursionCount;
    HANDLE OwningThread;        // from the thread's ClientId->UniqueThread
    HANDLE LockSemaphore;
    ULONG_PTR SpinCount;        // force size on 64-bit systems when packed
} RTL_CRITICAL_SECTION, *PRTL_CRITICAL_SECTION;

typedef RTL_CRITICAL_SECTION CRITICAL_SECTION;

#endif // __km7WinType_H__
