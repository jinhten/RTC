#ifndef __km7Define_H_INCLUDED_2018_11_12__
#define __km7Define_H_INCLUDED_2018_11_12__

/* Note ----------------------
* kmMat has been created by Choi, Kiwan
* This is version 7
* kmMat v7 is including the following
*   - km7Define.h
*   - km7Define.h -> km7Mat.h
*   - km7Define.h -> km7Mat.h -> km7Wnd.h
*   - km7Define.h -> km7Mat.h -> km7Dnn.h
*   - km7Define.h -> km7Mat.h -> kc7Mat.h
*/

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
// define macro

// define printf and assert
#if defined(_KM_ENV_NO_DEBUG) // for NDEBUG

    #define PRINTFW(A, ...) ((void)0)
    #define PRINTFA(A, ...) ((void)0)

    #define PRINTFA_I(A)    ((void)0)
    #define PRINTFA_X(A)    ((void)0)
    #define PRINTFA_F(A)    ((void)0)
    
    #define PRINTFW_I(A)    ((void)0)
    #define PRINTFW_X(A)    ((void)0)
    #define PRINTFW_F(A)    ((void)0)

    #define ASSERTA(A, B, ...)  ((void)0)
    #define ASSERTW(A, B, ...)  ((void)0)

#else    // for window

    // define for printf    
    #define PRINTFA(A, ...)  printf(A, ##__VA_ARGS__)
    #define PRINTFW(A, ...) wprintf(A, ##__VA_ARGS__)
    #define print(A, ...)    printf(A, ##__VA_ARGS__)
    #define printw(A, ...)  wprintf(A, ##__VA_ARGS__)
    
    #define PRINTFA_L(A)    {PRINTFA("* "#A" \t: %lld \n", A);}
    #define PRINTFA_I(A)    {PRINTFA("* "#A" \t: %d   \n", A);}
    #define PRINTFA_X(A)    {PRINTFA("* "#A" \t: %#llx\n", A);}
    #define PRINTFA_F(A)    {PRINTFA("* "#A" \t: %.6f \n", A);}
    #define PRINTFA_D(A)    {PRINTFA("* "#A" \t: %.14f\n", A);}

    #define print_l(A)    {PRINTFA("* "#A" \t: %lld \n", A);}
    #define print_i(A)    {PRINTFA("* "#A" \t: %d   \n", A);}
    #define print_x(A)    {PRINTFA("* "#A" \t: %#llx\n", A);}
    #define print_f(A)    {PRINTFA("* "#A" \t: %.6f \n", A);}
    #define print_d(A)    {PRINTFA("* "#A" \t: %.14f\n", A);}
    
    #define PRINTFW_L(A)    {PRINTFW(L"* "#A" \t: %lld \n", A);}
    #define PRINTFW_I(A)    {PRINTFW(L"* "#A" \t: %d   \n", A);}
    #define PRINTFW_X(A)    {PRINTFW(L"* "#A" \t: %#llx\n", A);}
    #define PRINTFW_F(A)    {PRINTFW(L"* "#A" \t: %.6f \n", A);}
    #define PRINTFW_D(A)    {PRINTFW(L"* "#A" \t: %.14f\n", A);}

    #define printw_l(A)    {PRINTFW(L"* "#A" \t: %lld \n", A);}
    #define printw_i(A)    {PRINTFW(L"* "#A" \t: %d   \n", A);}
    #define printw_x(A)    {PRINTFW(L"* "#A" \t: %#llx\n", A);}
    #define printw_f(A)    {PRINTFW(L"* "#A" \t: %.6f \n", A);}
    #define printw_d(A)    {PRINTFW(L"* "#A" \t: %.14f\n", A);}

    // define for print mat
    #define PRINTMATA(A)    {A.PrintMat( ""#A"");}
    #define PRINTMATW(A)    {A.PrintMat(L""#A"");}

    // define for assert of class
    #define ASSERTA(A, B, ...) {if(!(A)) {printf("* assertion failed [%s]: "#A"\n   ",GetKmClass());\
                                          printf(B, ##__VA_ARGS__); printf("\n");  \
                                          throw KE_ASSERTION_FAILED; }}

    #define ASSERTW(A, B, ...) {if(!(A)) {wprintf(L"* assertion failed : "#A"\n   ");\
                                          wprintf(B, ##__VA_ARGS__); wprintf(L"\n"); \
                                          throw KE_ASSERTION_FAILED; }}

    // define for assert of functions
    #define ASSERTFA(A, B, ...) {if(!(A)) {printf("* assertion failed : "#A"\n   ");\
                                          printf(B, ##__VA_ARGS__); printf("\n");  \
                                          throw KE_ASSERTION_FAILED; }}
    
    #define ASSERTFW(A, B, ...) {if(!(A)) {wprintf(L"* assertion failed : "#A"\n   ");\
                                          wprintf(B, ##__VA_ARGS__); wprintf(L"\n"); \
                                          throw KE_ASSERTION_FAILED; }}
#endif

// define type check macro
#define TID_F32      typeid(float)
#define TID_F64      typeid(double)
#define TID_I08      typeid(char)
#define TID_I16      typeid(short)
#define TID_I32      typeid(int)
#define TID_I64      typeid(int64)
#define TID_U08      typeid(uchar)
#define TID_U16      typeid(ushort)
#define TID_U32      typeid(uint)
#define TID_U64      typeid(uint64)
#define TID_C08      typeid(char)
#define TID_C16      typeid(wchar_t)

#define IS_F32(A)   (typeid(A) == typeid(float)     )
#define IS_F64(A)   (typeid(A) == typeid(double) )
#define IS_I08(A)   (typeid(A) == typeid(char)     )
#define IS_I16(A)   (typeid(A) == typeid(short)     )
#define IS_I32(A)   (typeid(A) == typeid(int)     )
#define IS_I64(A)   (typeid(A) == typeid(int64)     )
#define IS_U08(A)   (typeid(A) == typeid(uchar)     )
#define IS_U16(A)   (typeid(A) == typeid(ushort) )
#define IS_U32(A)   (typeid(A) == typeid(uint)     )
#define IS_U64(A)   (typeid(A) == typeid(uint64) )
#define IS_C08(A)   (typeid(A) == typeid(char)     )
#define IS_C16(A)   (typeid(A) == typeid(wchar_t))

#define IS_F(A)     (IS_F32(A) || IS_F64(A))
#define IS_I(A)     (IS_I08(A) || IS_I16(A) || IS_I32(A) || IS_U64(A) || IS_U08(A) || IS_U16(A) || IS_U32(A) || IS_U64(A))

// define time check macro
#ifdef _KM_ENV_NO_DEBUG

    #define KM_CHECK_TIME_START  (void())
    #define KM_CHECK_TIME_END(A) (void(A))

#else
    #define KM_CHECK_TIME_START    {LARGE_INTEGER lc1,lc2,lfreq; \
                                 QueryPerformanceFrequency(&lfreq); \
                                 QueryPerformanceCounter(&lc1);
    #define KM_CHECK_TIME_END(A) QueryPerformanceCounter(&lc2); \
                                 PRINTFA("* TIME CHECK (%s): %.3f msec\n", A, \
                                 (lc2.QuadPart-lc1.QuadPart)/(double)lfreq.QuadPart*1.e3);}
    #define KM_CHECK_TIME_END_SEC(A) QueryPerformanceCounter(&lc2); \
                                 PRINTFA("* TIME CHECK (%s): %.3f sec\n", A, \
                                 (lc2.QuadPart-lc1.QuadPart)/(double)lfreq.QuadPart);}
    #define KM_CHECK_TIME_END_MIN(A) QueryPerformanceCounter(&lc2); \
                                 PRINTFA("* TIME CHECK (%s): %.2f min\n", A, \
                                 (lc2.QuadPart-lc1.QuadPart)/(double)lfreq.QuadPart/60.);}

    #define KM_CHECK_TIME_START0  LARGE_INTEGER lc1,lc2,lfreq; \
                                  QueryPerformanceFrequency(&lfreq); \
                                  QueryPerformanceCounter(&lc1);
    #define KM_CHECK_TIME_END0(A) QueryPerformanceCounter(&lc2); \
                                  PRINTFA("* TIME CHECK (%s): %.3f msec\n", A, \
                                  (lc2.QuadPart-lc1.QuadPart)/(double)lfreq.QuadPart*1.e3);

    #define KM_CHECK_TIME(A) {LARGE_INTEGER lc1,lfreq; \
                             QueryPerformanceFrequency(&lfreq); \
                             QueryPerformanceCounter(&lc1);    \
                             PRINTFA("* TIME CHECK Current (%s) : %.0f msec\n", A, \
                             (lc1.QuadPart)/(double)lfreq.QuadPart*1.e3);}
#endif

// define usleep macro
#define KM_USLEEP(A) {LARGE_INTEGER lc1,lc2,lfreq; long long time_usec;\
                      QueryPerformanceFrequency(&lfreq); \
                      QueryPerformanceCounter(&lc1); \
                      do{ \
                      Sleep(0); \
                      QueryPerformanceCounter(&lc2); \
                      time_usec = (lc2.QuadPart-lc1.QuadPart)*1000000\
                      /lfreq.QuadPart; \
                      }while( time_usec < (A));}

#define KM_USLEEP_START {LARGE_INTEGER lc1,lc2,lfreq; long long time_usec;\
                         QueryPerformanceFrequency(&lfreq); \
                         QueryPerformanceCounter(&lc1); \

#define KM_USLEEP_WAIT(A) do{ \
                          Sleep(0); \
                          QueryPerformanceCounter(&lc2); \
                          time_usec = (lc2.QuadPart-lc1.QuadPart)*1000000\
                          /lfreq.QuadPart; \
                          }while( time_usec < (A));}

// define value check macro
#ifdef _KM_CHECK_VALUE_ALL_DISPLAY

    #define KM_CHECK_VALUE(value, min, max) \
                        KM_PRINTF("* "#value" : %.3f (%.3f, %.3f)\n",\
                        (float)value, (float)min, (float)max);

    #define KM_CHECK_MAT(mat, nx1, nx2) \
                        KM_PRINTF("* "#value" : %d, %d (%d, %d)\n",\
                        mat.NX1(), mat.NX2(), nx1, nx2);

    #define KM_CHECK_MAT3D(mat, nx1, nx2, nx3) \
                        KM_PRINTF("* "#value" : %d, %d, %d (%d, %d, %d)\n",\
                        mat.NX1(), mat.NX2(), mat.NX3(), \
                        nx1, nx2, nx3);
#else

    #define KM_CHECK_VALUE(value, min, max) \
                        if(min > (value) || (value) > max) {\
                        PRINTFA("* "#value"(%.3f) isn't correct!\n",(float)(value));\
                        throw KE_OUTOF_RANGE; }

    #define KM_CHECK_MAT1(mat, n1) \
                        if(mat.N1() != n1) {\
                        PRINTFA("* ths size of "#mat"(%d) isn't correct!\n", \
                        mat.N1());\
                        throw KE_WRONG_CONFIG; }

    #define KM_CHECK_MAT2(mat, n1, n2) \
                        if(mat.N1() != n1 || mat.N2() != n2) {\
                        PRINTFA("* ths size of "#mat"(%d, %d) isn't correct!\n", \
                        mat.N1(), mat.N2());\
                        throw KE_WRONG_CONFIG; }
    
    #define KM_CHECK_MAT3(mat, n1, n2, n3) \
                        if(mat.N1() != n1 || mat.N2() != n2 || mat.N3() != n3) {\
                        PRINTFA("* ths size of "#mat"(%d,%d,%d) isn't correct!\n",\
                        mat.N1(), mat.N2(), mat.N3());\
                        throw KE_WRONG_CONFIG; }
#endif

// define addtional dispaly macro
#define KM_PRINTF_MEM(A) {MEMORYSTATUSEX mi; mi.dwLength = sizeof(mi); GlobalMemoryStatusEx(&mi); \
                          PRINTFA("* %s: CPU mem (%d/%d MB)\n", A \
                          ,(uint) (mi.ullAvailPhys>>20), (uint) (mi.ullTotalPhys>>20));}

#define KM_PRINTF_TIME(A) { time_t time_now = time(NULL); \
                            tm time_; localtime_s(&time_, &time_now); \
                            PRINTFA("* %s: %d-%02d-%02d %02d:%02d:%02d\n", A, \
                                      time_.tm_year+1900, time_.tm_mon+1, time_.tm_mday, \
                                      time_.tm_hour,      time_.tm_min,   time_.tm_sec); }
// define va list macro
#define GETVAWCHAR(A, B) { va_list args; \
                           va_start(args, B); \
                           vswprintf(A, sizeof(A)/2, B, args); \
                           va_end  (args); }

#define GETVACHAR(A, B)  { va_list args; \
                           va_start(args, B); \
                           vsprintf(A, B, args); \
                           va_end  (args); }

// define mathematical constants
#ifndef PI
#define PI (3.14159265358979323846)
#endif

#ifndef PIH
#define PIH (3.14159265358979323846/2.)
#endif

#ifndef PIf
#define PIf (3.14159265358979323846f)
#endif

#ifndef PIHf
#define PIHf (3.14159265358979323846f/2.f)
#endif

#ifndef PIDf
#define PIDf (3.14159265358979323846f*2.f)
#endif

#define COFFSIGMA 0.212330450072005 // n/(4*sqrt(2*log(2))); for Gaussian kernel

// define numberic limits
// * Refer to limits.h and float.h
//
// examples
//  SHRT_MIN  SHRT_MAX
//  INT_MIN   INT_MAX
//  LLONG_MIN LLONG_MAX
//  FLT_MIN   FLT_MAX
//  DBL_MIN   DBL_MAX

#define SHORT_MIN (-32768)
#define SHORT_MAX ( 32767)

#define FLOAT_MIN   (-3.402823466e+38F)
#define FLOAT_MAX   ( 3.402823466e+38F)
#define FLOAT_SMALL ( 1.175494351e-38F)

#define HALF_MIN    (-6.5504e+4)
#define HALF_MAX    ( 6.5504e+4)
#define HALF_SMALL  ( 6.10e-5)

#define end32  (0x7fffffff)
#define end32u (0xffffffff)
#define end64  (0x7fffffffffffffff)
#define end64u (0xffffffffffffffff)

// define basic function macro
#define MAX(A,B)     (((A) > (B)) ? (A):(B))
#define MIN(A,B)     (((A) > (B)) ? (B):(A))

#define HYPOT(A,B)   (sqrt((A)*(A) + (B)*(B)))

#define COUNTOF(A)   (sizeof(A)/sizeof(A[0]))

#define PREPOWER2(A) ((int) powf(2.f, floor(log2f((float)A))))

#define SINC(A)      (((A)==0)? 1.f:(sin(PIf*(A)) / (PIf*(A))))
#define SINCR(A)     (((A)==0)? 1.f:(sin(     A)  / (     A)))

#define SWAP_I16(A,B) {short  temp = (A); (A) = (B); (B) = temp;}
#define SWAP_I32(A,B) {int      temp = (A); (A) = (B); (B) = temp;}
#define SWAP_F32(A,B) {float  temp = (A); (A) = (B); (B) = temp;}
#define SWAP_F64(A,B) {double temp = (A); (A) = (B); (B) = temp;}

#define SIGN(A) (((A) == 0)? 0:(((A) > 0)? 1:-1))

// define bit check
#define GETBIT(A, B)   (((A)>>(B))&1)
#define SETBIT(A, B)   ((A) = (A |  (1<<(B))))
#define CLEARBIT(A, B) ((A) = (A & ~(1<<(B))))

// define RGB
#define RGB_WHITE kmRgb(255,255,255)
#define RGB_BLACK kmRgb(0,0,0)
#define RGB_RED   kmRgb(255,0,0)
#define RGB_GREEN kmRgb(0,255,0)
#define RGB_BLUE  kmRgb(0,0,255)

// define data management
#define MAKELLONG(a, b)      ((DWORD64)(((DWORD)(((DWORD_PTR)(a)) & 0xffffffff)) | (DWORD64)((DWORD)(((DWORD_PTR)(b)) & 0xffffffff))) << 32))
#define LOLONG(l)            ((DWORD)(((DWORD_PTR)(l)) & 0xffffffff))
#define HILONG(l)            ((DWORD)((((DWORD_PTR)(l)) >> 32) & 0xffffffff))

// define data menagement for 64bit
#define MAKEUINT64(a,b)      (uint64(b)<<32 | (uint64(a)&0xffffffff))
#define LOINT32(c)           (int(c))
#define HIINT32(c)           (int((c)>>32))

// define transfer to big endian or little endian
#define ENDIAN16(A)  (((A>> 8)&0xff)|((A<< 8)&0xff00))
#define ENDIAN32(A)  (((A>>24)&0xff)|((A>> 8)&0xff00)|((A<< 8)&0xff0000)|((A<<24)&0xff000000))
#define ENDIAN64(A)  (((A>>56)&0xff)|((A>>40)&0xff00)|((A>>24)&0xff0000)|((A>> 8)&0xff000000)|\
                      ((A<< 8)&0xff00000000)|((A<<24)&0xff0000000000)|((A<< 40)&0xff000000000000)|((A<<56)&0xff00000000000000))

// num of array
#define numof(A) (sizeof(A)/sizeof(A[0]))

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
// additional type def

typedef          long long      int64;
typedef unsigned char           uchar;
typedef unsigned short          ushort;
typedef unsigned int            uint;
typedef unsigned long long      uint64;
typedef          wchar_t        wchar;
typedef unsigned long           ulong;

// file define
#define MAX_FILE_N  5000


// * Note that the following types are originally defined in CUDA.
// * But I want kmMat to work without CUDA,
// * so they will be defined if there is no CUDA.
#ifndef __VECTOR_TYPES_H__

//struct float2 {float x, y;};
//struct float3 {float x, y, z;};
//struct float4 {float x, y, z, w;};
//
//typedef struct float2 float2;
//typedef struct float3 float3;
//typedef struct float4 float4;

#endif

// * Note that float5 isn't defined in CUDA 
struct float5 { float x, y, z, w, v; };

typedef struct float5 float5;

// axis type def
struct f32xy  { float x, y; };
struct f32yz  { float y, z; };
struct f32zx  { float z, x; };

struct f32xyz  { float x, y, z; };
struct f32xyzw { float x, y, z, w; };

struct f32ri  { float r, i; };  // real, imagenary
struct f32iq  { float i, q; };  // in-phase, quadrature
struct f32ap  { float a, p; };  // amplitude, phase
struct f32ma  { float m, a; };  // magnitude, angle

struct f32xya // x, y, angle
{
    float x = 0, y = 0, a = 0;

    f32xya() {};
    f32xya(float x, float y, float a): x(x), y(y), a(a) {};
};
struct f32yza // x, y, angle
{
    float y = 0, z = 0, a = 0;

    f32yza() {};
    f32yza(float y, float z, float a): y(y), z(z), a(a) {};
};
struct f32zxa // z, x, angle
{
    float z = 0, x = 0, a = 0;

    f32zxa() {};
    f32zxa(float z, float x, float a): z(z), x(x), a(a) {};
};

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
// define enum

// type of processor
enum kmeProcType
{
    PT_CPU,          // cpu
    PT_CUDA,         // cuda
    PT_OPENCL        // openCL
};

// DOF of LK tracker
enum kmeLKDof
{
    LK_X,                // 1 dof : x
    LK_Y,                // 1 dof : y
    LK_XY,                // 2 dof : x, y
    LK_XSX,                // 2 dof : x, scale-x
    LK_YSY,                // 2 dof : y, scale-y
    LK_XYS,                // 3 dof : x, y, scale
    LK_XYR,                // 3 dof : x, y, rotation
    LK_FULL                // 6 dof
};

// byte order of RGB
enum kmeRgbOrder
{
    RGB_RGBA,        // for windows
    RGB_BGRA        // for bitmat format
};

// colormap type
enum kmeCMapType
{
    CMAP_JET, 
    CMAP_GREY 
};

// filter option - method for filtering boundary
enum kmeFiltOpt
{
    KFO_BD_ZERO,
    KFO_BD_EXPAND,
    KFO_BD_NO_CALC,
    KFO_BD_NORMALIZE    
};

// type of filter
enum kmeFiltType 
{
    KFT_LPF,
    KFT_BPF,
    KFT_BPF_SYM,
    KFT_GAUSS,          // It isn't the same with KWT_GASSWIN.
    KFT_BIAS_REMOVAL    // bias-removal filter
};

// unit of filter
enum kmeFiltUnit
{
    KFU_HZ,  // Hz,  cycles/sec
    KFU_KHZ, // kHz, cycles/msec
    KFU_MHZ, // MHz, cycles/usec
    KFU_PM,  // cycles/m
    KFU_PMM, // cycles/mm
    KFU_PUM  // cycles/um
};

// type of window
enum kmeWndType
{
    KWT_RECT      = 0,
    KWT_HANN      = 1,
    KWT_HAMMING   = 2,
    KWT_KAISER    = 3,
    KWT_TUKEYWIN  = 4,
    KWT_TUKEYHALF = 5,
    KWT_GAUSSWIN  = 6,
    KWT_FREE      = 7,
    KWT_FREEFULL  = 8
};

// type of image data
enum kmeImgType
{
    IMG_NULL,      // null image
    IMG_CMAP,
    IMG_RGB
};

// type of child window position
enum kmeCwpType
{
    CWP_WHLT,
    CWP_WHLB,
    CWP_WHRT,
    CWP_WHRB,
    CWP_HLRT,
    CWP_HLRB,
    CWP_WLTB,
    CWP_WRTB,
    CWP_LRTB,
    CWP_RAT   // x-y-w-h
};

// type of post-proc
enum kmePPType
{
    PP_DEFPROC,
    PP_FINISH 
};

// mode of file open
enum kmeFile
{
    KF_READ,
    KF_NEW,
    KF_ADD,
    KF_MODIFY,
    KF_READ_TXT,
    KF_NEW_TXT,
    KF_ADD_TXT,
    KF_MODIFY_TXT
};

////////////////////////////////////////////////
// exception function

enum kmException
{
    KE_ASSERTION_FAILED,        // assertion fail
    KE_OUTOF_RANGE,                // out of range    
    KE_WRONG_CONFIG,            // wrong configulation
    KE_NOT_ALLOCATED_MEM,        // not allocated
    KE_DIVIDE_BY_ZERO,            // divide zero
    KE_CANNOT_OPEN,                // cannot open
    KE_CANNOT_FIND,                // cannot find
    KE_THREAD_FAILED,            // thread failed

    KE_CUDA_ERROR,                // cuda error
    KE_CUFFT_ERROR,                // cufft error
    KE_NVML_ERROR,              // nvml error
    KE_HEADER_ERROR,            // header error
    KE_NET_ERROR                // network error
};

// display exception function
inline void kmPrintException(kmException e)
{
    PRINTFA("* kmException : ");
    switch(e)
    {    
    case KE_ASSERTION_FAILED  : PRINTFA("KE_ASSERTION_FAILED");  break;
    case KE_OUTOF_RANGE       : PRINTFA("KE_OUTOF_RANGE");       break;
    case KE_WRONG_CONFIG      : PRINTFA("KE_WRONG_CONFIG");      break;
    case KE_NOT_ALLOCATED_MEM : PRINTFA("KE_NOT_ALLOCATED_MEM"); break;
    case KE_DIVIDE_BY_ZERO    : PRINTFA("KE_DIVIDE_BY_ZERO");    break;
    case KE_CANNOT_OPEN       : PRINTFA("KE_CANNOT_OPEN");       break;
    case KE_CANNOT_FIND       : PRINTFA("KE_CANNOT_FIND");       break;
    case KE_THREAD_FAILED     : PRINTFA("KE_THREAD_FAILED");     break;

    case KE_CUFFT_ERROR       : PRINTFA("KE_CUFFT_ERROR");       break;
    case KE_CUDA_ERROR        : PRINTFA("KE_CUDA_ERROR");        break;
    case KE_NVML_ERROR        : PRINTFA("KE_NVML_ERROR");        break;
    case KE_HEADER_ERROR      : PRINTFA("KE_HEADER_ERROR");      break;
    case KE_NET_ERROR         : PRINTFA("KE_NET_ERROR");         break;
    }
    PRINTFA("\n");
};

/////////////////////////////////////////////////
// define coefficient

// kernel coefficient for kckUpsample... USC_uprate_order_idx
// added kaiser window to remove high freq. signal for uprate 2
#define USC_2_2_0  (-0.06913335f * 0.8573f)
#define USC_2_2_1  ( 0.57066863f * 0.9836f)

#define USC_2_3_0  ( 0.02985932f * 0.2049f)
#define USC_2_3_1  (-0.13441840f * 0.6247f)
#define USC_2_3_2  ( 0.60663183f * 0.9522f)

#define USC_3_2_0  (-0.03878840f)
#define USC_3_2_1  (-0.08795309f)
#define USC_3_2_2  ( 0.33973264f)
#define USC_3_2_3  ( 0.78803786f)

#define USC_3_3_0  ( 0.01859528f)
#define USC_3_3_1  ( 0.03476439f)
#define USC_3_3_2  (-0.09334641f)
#define USC_3_3_3  (-0.14478339f)
#define USC_3_3_4  ( 0.37935771f)
#define USC_3_3_5  ( 0.80950320f)

#define USC_4_2_0  (-0.02482570f)
#define USC_4_2_1  (-0.06913335f)
#define USC_4_2_2  (-0.08591891f)
#define USC_4_2_3  ( 0.23364580f)
#define USC_4_2_4  ( 0.57066863f)
#define USC_4_2_5  ( 0.87626966f)

#define USC_4_3_0  ( 0.01266221f)
#define USC_4_3_1  ( 0.02985932f)
#define USC_4_3_2  ( 0.03258977f)
#define USC_4_3_3  (-0.06809884f)
#define USC_4_3_4  (-0.13441840f)
#define USC_4_3_5  (-0.13188445f)
#define USC_4_3_6  ( 0.26901577f)
#define USC_4_3_7  ( 0.60663183f)
#define USC_4_3_8  ( 0.88956771f)

#define USC_5_2_0  (-0.01763232f)
#define USC_5_2_1  (-0.05104793f)
#define USC_5_2_2  (-0.08320598f)
#define USC_5_2_3  (-0.07928520f)
#define USC_5_2_4  ( 0.17568663f)
#define USC_5_2_5  ( 0.43062926f)
#define USC_5_2_6  ( 0.70590105f)
#define USC_5_2_7  ( 0.91943891f)

#define USC_5_3_0   ( 0.00937658f)
#define USC_5_3_1   ( 0.02341203f)
#define USC_5_3_2   ( 0.03403195f)
#define USC_5_3_3   ( 0.02935353f)
#define USC_5_3_4   (-0.05285680f)
#define USC_5_3_5   (-0.11204193f)
#define USC_5_3_6   (-0.14572235f)
#define USC_5_3_7   (-0.11712442f)
#define USC_5_3_8   ( 0.20646140f)
#define USC_5_3_9   ( 0.47060070f)
#define USC_5_3_10  ( 0.73386003f)
#define USC_5_3_11  ( 0.92832966f)

// kernel for Hiltert transform.. n_tap_h = 32
#define HTC_00      ( 0.63613886f)
#define HTC_01      ( 0.21076219f)
#define HTC_02      ( 0.12491079f)
#define HTC_03      ( 0.08755485f)
#define HTC_04      ( 0.06635438f)
#define HTC_05      ( 0.05248638f)
#define HTC_06      ( 0.04255465f)
#define HTC_07      ( 0.03497156f)
#define HTC_08      ( 0.02889388f)
#define HTC_09      ( 0.02383057f)
#define HTC_10      ( 0.01947457f)
#define HTC_11      ( 0.01562230f)
#define HTC_12      ( 0.01213150f)
#define HTC_13      ( 0.00889777f)
#define HTC_14      ( 0.00584042f)
#define HTC_15      ( 0.00289359f)

// kernel for Hiltert transform.. n_tap_h = 96
#define HTC96_00        ( 0.63656412f) 
#define HTC96_01        ( 0.21203962f) 
#define HTC96_02        ( 0.12704559f) 
#define HTC96_03        ( 0.09055581f) 
#define HTC96_04        ( 0.07023398f) 
#define HTC96_05        ( 0.05726109f) 
#define HTC96_06        ( 0.04824517f) 
#define HTC96_07        ( 0.04160329f) 
#define HTC96_08        ( 0.03649738f) 
#define HTC96_09        ( 0.03244224f) 
#define HTC96_10        ( 0.02913749f) 
#define HTC96_11        ( 0.02638720f) 
#define HTC96_12        ( 0.02405813f) 
#define HTC96_13        ( 0.02205649f) 
#define HTC96_14        ( 0.02031436f) 
#define HTC96_15        ( 0.01878131f) 
#define HTC96_16        ( 0.01741916f) 
#define HTC96_17        ( 0.01619840f) 
#define HTC96_18        ( 0.01509591f) 
#define HTC96_19        ( 0.01409329f) 
#define HTC96_20        ( 0.01317572f) 
#define HTC96_21        ( 0.01233111f) 
#define HTC96_22        ( 0.01154952f) 
#define HTC96_23        ( 0.01082269f) 
#define HTC96_24        ( 0.01014367f) 
#define HTC96_25        ( 0.00950660f) 
#define HTC96_26        ( 0.00890650f) 
#define HTC96_27        ( 0.00833908f) 
#define HTC96_28        ( 0.00780064f) 
#define HTC96_29        ( 0.00728796f) 
#define HTC96_30        ( 0.00679825f) 
#define HTC96_31        ( 0.00632902f) 
#define HTC96_32        ( 0.00587810f) 
#define HTC96_33        ( 0.00544353f) 
#define HTC96_34        ( 0.00502358f) 
#define HTC96_35        ( 0.00461668f) 
#define HTC96_36        ( 0.00422143f) 
#define HTC96_37        ( 0.00383652f) 
#define HTC96_38        ( 0.00346078f) 
#define HTC96_39        ( 0.00309313f) 
#define HTC96_40        ( 0.00273256f) 
#define HTC96_41        ( 0.00237813f) 
#define HTC96_42        ( 0.00202896f) 
#define HTC96_43        ( 0.00168421f) 
#define HTC96_44        ( 0.00134309f) 
#define HTC96_45        ( 0.00100484f) 
#define HTC96_46        ( 0.00066872f) 
#define HTC96_47        ( 0.00033401f) 

#endif /* __km7Define_H_INCLUDED_2018_11_12__ */
