#ifndef __km7Mat__
#define __km7Mat__

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

// base header
#include <cmath>
#include <stdio.h>          // for printf..
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <typeinfo>         // for typeid()
#include <ctime>            // for time_t,struct tm
#include <initializer_list>
#include <sys/time.h>
#include <iostream>            // for cout
#include <atomic>            // for atomic<>
#include <bitset>           // for kmBit
#include <chrono>
#include <thread>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


using namespace std;

// define header
#include "km7Define.h"
#include "km7WinType.h"

///////////////////////////////////////////////////////////////
// template functions

// get the virtual table address of class T
template<class T> void* GetVfptr()     { T a; return (void*) *((int64*)&a); };
template<class T> void* GetVfptr(T& b) { T a; return (void*) *((int64*)&a); };

///////////////////////////////////////////////////////////////
// rand function with tick count

// get random integer between min and max
uint kmrand(uint min, uint max)
{
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);

    return uint((tp.tv_sec*1000ull) + (tp.tv_nsec/1000ull/1000ull))%((int64)max - min + 1) + min;
};
int  kmrand( int min,  int max)
{
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);

    return uint((tp.tv_sec*1000ull) + (tp.tv_nsec/1000ull/1000ull))%((int64)max - min + 1) + min;
};

#define KKT(A) {cout<<"[kkt.jin] "<<__FILE__<<":"<<__func__<<":"<<__LINE__<<", "<<A<<endl;}

#define MAX_FILE_NAME 128
#define MAX_WFILE_NAME 64

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
// class for matrix

///////////////////////////////////////////////////////////////
// complex
template<typename T> class cmplx
{
public:
    T r = 0, i = 0;

    // constructor
    cmplx() {};
    cmplx(T r_, T i_)      { r = r_;  i = i_; };
    cmplx(const cmplx&  a) { r = a.r; i = a.i;};

    cmplx(const initializer_list<T>& val)
    {
        ASSERTFA(val.size() == 2, "[cmplx() in 58]");

        const int64 *pval = val.begin();
        r = *(pval++); i = *(pval);
    };

    // basic math functions        
    T     sq () const { return r*r + i*i; };
    T     ang() const { return atan2(i, r); };
    T     abs() const { return sqrt(r*r + i*i); };
    cmplx exp() const { return ::exp(r)*cmplx(cos(i), sin(i)); };

    // operator
    cmplx operator~() const { return cmplx( r,-i); }; // conjugate
    cmplx operator-() const { return cmplx(-r,-i); };

    cmplx& operator+=(const T& b) { r += b;         return *this; };
    cmplx& operator-=(const T& b) { r -= b;         return *this; };
    cmplx& operator*=(const T& b) { r *= b; i *= b; return *this; };
    cmplx& operator/=(const T& b) { r /= b; i /= b; return *this; };

    cmplx& operator+=(const cmplx& b) { r += b.r; i += b.i; return *this; };
    cmplx& operator-=(const cmplx& b) { r -= b.r; i -= b.i; return *this; };
    cmplx& operator*=(const cmplx& b) { *this = *this*b;    return *this;};
    cmplx& operator/=(const cmplx& b) { (*this *= ~b) /= (b.r*b.r + b.i*b.i); return *this;};
        
    cmplx operator+(const cmplx& b) const { return cmplx(r,i) += b; };
    cmplx operator-(const cmplx& b) const { return cmplx(r,i) -= b; };
    cmplx operator*(const cmplx& b) const { return cmplx(r*b.r - i*b.i, r*b.i + i*b.r) ; };
    cmplx operator/(const cmplx& b) const { return cmplx(r,i) /= b; };

    bool operator==(const cmplx& b) const { return r == b.r && i == b.i; };
    bool operator!=(const cmplx& b) const { return r != b.r || i != b.i; };

    // conversion operator...
    operator T() const { return abs(); };
};

template<typename T> T        abs(const cmplx<T>& a) { return a.abs(); };
template<typename T> T        ang(const cmplx<T>& a) { return a.ang(); };
template<typename T> T        sq (const cmplx<T>& a) { return a.sq (); };
template<typename T> cmplx<T> exp(const cmplx<T>& a) { return a.exp(); };

typedef cmplx<short>   cmplxi16;
typedef cmplx<int>     cmplxi32;
typedef cmplx<int64>   cmplxi64;
typedef cmplx<float>   cmplxf32;
typedef cmplx<double>  cmplxf64;

template<typename T> cmplx<T> operator+(const cmplx<T>& b, const T a) { return cmplx<T>(b.r+a, b.i+a); }
template<typename T> cmplx<T> operator-(const cmplx<T>& b, const T a) { return cmplx<T>(b.r-a, b.i-a); }
template<typename T> cmplx<T> operator*(const cmplx<T>& b, const T a) { return cmplx<T>(b.r*a, b.i*a); }
template<typename T> cmplx<T> operator/(const cmplx<T>& b, const T a) { return cmplx<T>(b.r/a, b.i/a); }

template<typename T> cmplx<T> operator+(const T a, const cmplx<T>& b) { return cmplx<T>(a+b.r, a+b.i); }
template<typename T> cmplx<T> operator-(const T a, const cmplx<T>& b) { return cmplx<T>(a-b.r, a-b.i); }
template<typename T> cmplx<T> operator*(const T a, const cmplx<T>& b) { return cmplx<T>(a*b.r, a*b.i); }
template<typename T> cmplx<T> operator/(const T a, const cmplx<T>& b) { return (a*~b)/sq(b); }

///////////////////////////////////////////////////////////////
// tempalte class for argument

template<typename T1, typename T2>
class kmT2
{    
public:
    T1 v1, *m1; T2 v2, *m2;

    // constructor
    // * Note that a1 and a2 must have objects because they're reference type
    // * and the optimizer can skip over definition of object.
    kmT2(T1& a1, T2& a2)
    {
        m1 = &a1; v1 = a1;
        m2 = &a2; v2 = a2;
    };
    kmT2& operator=(const kmT2& b)
    {
        *m1 = v1 = b.v1;
        *m2 = v2 = b.v2;
        return *this; 
    };
};

template<typename T1, typename T2, typename T3>
class kmT3
{
public:
    T1 v1, *m1; T2 v2, *m2; T3 v3, *m3;

    // constructor
    // * Note that a1, a2 and a3 must have objects because they're reference type
    // * and the optimizer can skip over definition of object.
    kmT3(T1& a1, T2& a2, T3& a3)
    {
        m1 = &a1; v1 = a1;
        m2 = &a2; v2 = a2;
        m3 = &a3; v3 = a3;
    };
    kmT3& operator=(const kmT3& b)
    {
        *m1 = v1 = b.v1;
        *m2 = v2 = b.v2;
        *m3 = v3 = b.v3;
        return *this; 
    };
};

template<typename T1, typename T2, typename T3, typename T4>
class kmT4
{
public:
    T1 v1, *m1; T2 v2, *m2; T3 v3, *m3; T4 v4, *m4;
    
    // constructor
    // * Note that a1, a2, a3 and a4 must have objects because they're reference type
    // * and the optimizer can skip over definition of object.
    kmT4(T1& a1, T2& a2, T3& a3, T4& a4)
    {
        m1 = &a1; v1 = a1;
        m2 = &a2; v2 = a2;
        m3 = &a3; v3 = a3;
        m4 = &a4; v4 = a4;
    };
    kmT4& operator=(const kmT4& b)
    {
        *m1 = v1 = b.v1;
        *m2 = v2 = b.v2;
        *m3 = v3 = b.v3;
        *m4 = v4 = b.v4;
        return *this; 
    };
};

// universal version with variadic template
// * Note that it has no limit for number of arguments,
// * but it is necessary to write template-types one bye one.
template<typename... Ts> class kmT {};

template<typename T, typename... Ts>
class kmT<T, Ts...>
{
public:    
    T _v, *_m;
    kmT<Ts...> _rest;

    kmT(T& a, Ts&... rest) : _v(a), _m(&a), _rest(rest...) {};

    kmT<T,Ts...>& operator=(const kmT<T,Ts...>& b)
    {
        *_m = _v = b._v; _rest = b._rest; return *this;
    };
};

//////////////////////////////////////////////////////////
// index class for kmArr or kmMat
class kmI
{
public:
    int64 s = 0;     // start index
    int64 e = end64; // end   index

    // constructor
    kmI() {};
    kmI(int64 s_)           {s = s_;  e = s_; };
    kmI(int64 s_, int64 e_) {s = s_;  e = e_; };
    kmI(const kmI& a)       {s = a.s; e = a.e;};

    kmI(const initializer_list<int64>& val)
    {    
        ASSERTFA(val.size() == 2, "[kmI() in 44]");

        const int64 *pval = val.begin();
        s = *(pval++); e = *(pval);
    };

    // algorithm
    int64 Len() const
    {
        ASSERTFA(e != end64, "[kmI() in 178]"); return e-s+1; 
    };

    int IsEqualSize(const kmI& b) const
    {
        return Len() == b.Len();
    };
};

//////////////////////////////////////////////////////////////
// state union 
union kmstate
{
    // members
    uint64 val = 0;
    struct
    {
        uint is_created : 1; // memory was allocated
        uint is_pinned  : 1; // memory was pinned
    };
    // * Note that pinned state will ban Expend(), Move() and move constructor

    // constructor
    kmstate() {};

    // copy constructor    
    kmstate(const uint64& a)    { val = a; };

    // assignment operator
    kmstate& operator=(const uint64& a) { val = a; return *this; };

    // conversion operator... (uint64) a
    operator uint64() const { return val; };
};

//////////////////////////////////////////////////////////
// matrix base class
template<typename T> class kmArr
{
protected:
    T*        _p     = nullptr; // pointer of data    
    kmstate _state = 0;       // state of memory allocation
    int64   _size  = 0;       // size of allocated memory (number of data)

    /////////////////////////////////////////////////
    // basic member functions
public:    
    // * Note that _state sholud be initialized seperately.
    virtual void Init() { _p = nullptr; _size = 0;}; 

    // construtor
    kmArr() {};
    kmArr(      int64 size) { Create(size);};
    kmArr(T* p, int64 size) { Set(p, size);};

    kmArr(const initializer_list<T>& val)
    {
        Create(val.size());
        const T *pval = val.begin();
        for(int64 i = 0; i < _size; ++i) *(_p + i) = *(pval++);
    };

    // destructor
    virtual ~kmArr() { Release(); };

    // copy constructor
    kmArr(const kmArr& b)
    {    
        Create(b._size);
        Copy  (b._p);
    };

    // move constructor
    kmArr(kmArr&& b) { Move(b); };
    
    // constructor for other type
    template<typename Y>
    kmArr(const kmArr<Y>& b)
    {
        Create(b.Size());
        CopyTCast(b.P());
    }

    // * Note that the assignment operator of the same type was separated from
    // * the assignment operators of different types, 
    // * because the complier considers there is no assigment operator of the same type
    // * if there is only the definition using tempalate.
    //
    // assignment operator
    kmArr& operator=(const kmArr& b) { RecreateIf(b); Copy(b._p); return *this; };

    // assignment operator for other type
    template<typename Y>
    kmArr& operator=(const kmArr<Y>& b) { RecreateIf(b); CopyTCast(b.P()); return *this; }

    // move assignment operator
    kmArr& operator=(kmArr&& b)
    {
        if(IsPinned()) *this = b; // * Note that this will call the assignment operator since b is an L-value.
        else           Move(b); 
        return *this; 
    };

    // allocate memory... core
    void Create(int64 size)
    {
        ASSERTA(!IsCreated(), "[kmArr::Create in 183] memory has already been created");
        ASSERTA(!IsPinned (), "[kmArr::Create in 184] memory is pinned");

        if(size == 0) { _size = 0; _state = 0; return; }

        _p = new T[size];

        ASSERTA(_p != 0, "[kmArr::Create in 146] %p != 0", _p);

        _size  = size;
        _state = 1;
    };

    // expand memory... core
    // * Note that expand must be call in case that kmArr was created not set.
    void Expand(int64 size)
    {    
        ASSERTA(IsCreated(), "[kmArr::Expand in 206] memory is not created");
        ASSERTA(!IsPinned(), "[kmArr::Expand in 207] memory is pinned");

        const int64 size_new = size + _size;

        T* p = new T[size_new];

        ASSERTA(_p != 0, "[kmArr::Expand in 160] %p != 0", _p);

        // * Note that "delete [] _p" will call the destructor of every elements.
        // * If T is kmat, it will release the memories of elements.
        // * To avoid this, std::move() and the move assignment operator have been used
        // * instead of CopyTo(p).
        // * std:move(A) means that A is rvalue.

        //CopyTo(p);
        for(int64 i = 0; i < _size; ++i) { *(p + i) = std::move(*(_p + i)); }

        delete [] _p;
        
        _size  = size_new;
        _p     = p;
        _state = 1;
    };

    // release memory... core
    virtual void Release()
    {
        if(IsCreated() && _p) delete [] _p;
        _state.is_created = 0; 
        Init();
    };

    // set array... core
    void Set(T* p, int64 size)
    {
        ASSERTA(!IsCreated(), "[kmArr::Set in 181] memory has already been created");

        _size = size;
        _p    = p;
        PinMemory(); // * Note that Set() will automatically pin the memory.
    };    
        
    // copy from (a = b)... core
    void Copy(const T* b)    
    {
        ASSERTA(_size == 0 || _p != 0, "[kmArr::Copy in 190] %p != 0", _p);

        memcpy(_p, b, sizeof(T)*_size);
    };

    // copy from (a = b)... core
    void Copy(const kmArr& b)
    {
        ASSERTA(IsEqualN(b), "[kmArr::Copy in 149]");

        const int64 n = N();

        for(int64 i = 0; i < n; ++i) v(i) = b.v(i);
    };

    // copy to (b = a)... core
    void CopyTo(T* b) const
    {
        memcpy(b, _p, sizeof(T)*_size);
    };

    // copy from with casting(a = b)... core
    template <typename Y> void CopyTCast(const Y* b)
    {    
        ASSERTA(_p != 0, "[kmArr::CopyTCast in 145]");

        T *p = _p;

        for (int64 i = _size; i--;) *(p++) = (T) *(b++);
    }

    // copy form with casting(a = b)... core
    template <typename Y> void CopyTCast(const kmArr<Y>& b)
    {
        ASSERTA(IsEqualN(b), "[kmArr::CopyTCast in 155]");

        const int64 n = N();

        for(int64 i = 0; i < n; ++i) v(i) = (T) b.v(i);
    }
    
    // release and create
    void Recreate(int64 size) { Release();    Create(size); };

    template<typename Y> void Recreate(const kmArr<Y>& b) { Recreate(b.N()); }

    // recreate if
    int RecreateIf(int64 size) { if(size != _size) { Recreate(size); return 1; } return 0; };
        
    template<typename Y> int RecreateIf(const kmArr<Y>& b) { return RecreateIf(b.Size()); }

    // move (b --> a)... core
    void Move(kmArr& b)
    {
        ASSERTA(!IsPinned(), "[kmArr::Move in 298] memory is pinned");

        if(IsCreated()) Release();

        _p     = b._p;
        _size  = b._size;
        _state = b._state; b._state = 0;
    };

    // set array
    void Set(const kmArr& b)   { Set(b._p, b._size); };    

    // operator to get data
    T* P(int64 i1) const
    {
        if(i1 < 0) i1 += _size; // * Note that i1 = -1 is the same as End()

        ASSERTA(0 <= i1 && i1 < _size, "[kmArr::P in 212] %lld < %lld", i1, _size);

        return (_p + i1);
    };

    T* P() const { return _p; };

    T& operator()(int64 i1 = 0) const { return *P(i1); };
    T& a         (int64 i1 = 0) const { return *P(i1); };

    // * Note that you should use v(i) instead of a(i)
    // * if you want to access every elements with one index with N()
    // * in a member function
    virtual T& v (int64 i1 = 0) const { return *P(i1); };

    T* Begin() const { return _p;};
    T* End  () const { return _p + _size - 1; };

    // get class name    
    LPCSTR GetKmClass() const { return typeid(*this).name() + 6; };
    
    // restore virtual function pointer (__vfptr)    
    template<class Y> void RestoreVfptr()     { *((void**)this) = GetVfptr<Y>(); }
    template<class Y> void RestoreVfptr(Y& a) { *((void**)this) = GetVfptr<Y>(); }

    // restore the class including vfptr
    // * Note that this will clear and reset _p, _state, _size without release,
    // * which can cause a memory leak. So, you should use this very carefully.
    kmArr& Restore() { RestoreVfptr<kmArr<T>>(); _state = 0; Init(); return *this; };

    /////////////////////////////////////////////////
    // operator functions
    
    // conversion operator... (T*) a, (T) a
    operator T*() const { return  _p; };
    operator T () const { return *_p; };

    // conversion operator.. (kmArr<Y>) a
    template<typename Y> operator kmArr<Y>() const
    {
        kmArr<Y> b(_size); b.CopyTCast(_p);
        return b;
    }

    /////////////////////////////////////////////////
    // general member functions

    // pin of unpin memory
    void PinMemory  () { _state.is_pinned = 1; };
    void UnpinMemory() { _state.is_pinned = 0; };

    // get info    
    int64 Size()   const { return _size;          };
    int64 Byte()   const { return _size*sizeof(T);};
    int64 State()  const { return _state;         };

    const type_info& GetType() const { return typeid(T); };
    
    bool IsCreated() const { return _state.is_created == 1; };
    bool IsPinned () const { return _state.is_pinned  == 1; };

    template<typename Y> 
    static bool IsArr(kmArr<Y>* a) { return true; }
    static bool IsArr(void*     a) { return false;};
            
    // get the number of real elements
    virtual int64 N() const { return _size; };

    // get bytes of info members which is from _size to end
    virtual int64 GetInfoByte() const { return sizeof(*this) - 24;}; 

    // get the first pointer of info members
    void* GetInfoPt() const { return (void*) &_size; };
    
    // display member info
    virtual void PrintInfo(LPCSTR str = nullptr) const
    {
        if(str != nullptr) PRINTFA("[%s]\n", str);

        PRINTFA("  _p     : %p\n"  , _p);
        PRINTFA("  _state : %llu\n", _state.val);
        PRINTFA("  _size  : %lld\n", _size);
    };

    // display dimension
    virtual void PrintDim(LPCSTR str = nullptr) const
    {
        if(str != nullptr) PRINTFA("[%s]\n", str);

        PRINTFA("  dim : %lld\n", _size);
    };

    // display member value
    void PrintVal(int64 idx) const
    {
        if     (IS_F(T)) PRINTFA("[%lld] %.3f\n", idx, v(idx));
        else if(IS_I(T)) PRINTFA("[%lld] %lld\n", idx, v(idx));
    };

    void  PrintVal(int64 s_idx, int64 e_idx) const
    {
        s_idx = MAX(s_idx, 0);
        e_idx = MIN(e_idx, N()-1);
        
        for(int64 i = s_idx; i <= e_idx; ++i) PrintVal(i);

        PRINTFA("\n");
    };

    void PrintVal() const {    PrintVal(0, N()-1); };

    // set value
    void SetVal(const T val)
    {
        for(int64 i = 0, n = N(); i < n; ++i) v(i) = val;
    };

    // set value in the block
    void SetVal(const T val, int64 i_s, int64 i_e)
    {
        // check idx
        i_s = MAX(i_s, 0);
        i_e = MIN(i_e, N() - 1);
        for (int64 i = i_s; i <= i_e; ++i) v(i) = val;
    };

    void SetVal(initializer_list<T>& val, int64 i_s = 0 )
    {
        const T      *pval = val.begin();
        const int64  i_ep = MIN(N(), val.size() + i_s);

        for(int64 i = i_s; i < i_ep; ++i) v(i) = *(pval++);
    };

    void SetValInc(T val_s, const T delta)
    {
        for(int64 i = 0, n = N(); i < n; ++i, val_s += delta) v(i) = val_s;
    };

    void SetSpaceLin(const T val_s, const T val_e)
    {        
        T delta = (_size < 2) ? 0 : (val_e - val_s) / (_size - 1);

        SetValInc(val_s, delta);
    };

    void SetRand(const T min, const T max)
    {
        // init rand
        static int seed = 1;
        if(seed) {
            struct timeval tick;
            gettimeofday (&tick, 0);
            srand(tick.tv_sec*1000 + tick.tv_usec);
            seed = 0;
        }

        const int64 n = N();

        for(int64 i = 0; i < n; ++i)
        {
            v(i) = (max - min)*((T) rand())/((T) RAND_MAX) + min;
        }
    };

    void SetSpace125(const T val_s, const T val_e)
    {
        const float n   = (float) N();
        const float del = (float) abs(val_e - val_s)/(n + 1.f);

        if(del == 0) { SetVal(0); return; }

        const float logdel   = log10f(del);
        const float logdel_n = floor(logdel);
        const float logdel_f = logdel - logdel_n;

        float step = 0;

        if     (logdel_f < 0.3010f) step = 2.f;
        else if(logdel_f < 0.6990f) step = 5.f;
        else                        step = 10.f;

        step *= powf(10.f, logdel_n);

        T val = (T) (step*(ceilf((float)val_s/step)));

        SetValInc(val, (T) step);
    };

    void SetZero() { memset(_p, 0, Byte()); };

    // swap
    void Swap(int64 i, int64 j)
    {
        T temp = v(i); v(i) = v(j); v(j) = temp;
    };

    // compare
    template<typename Y> bool IsEqualSize   (const kmArr<Y>& b) const { return _size == b.Size();}
    template<typename Y> bool IsEqualSizeDim(const kmArr<Y>& b) const { return _size == b.Size();}
    template<typename Y> bool IsEqualN      (const kmArr<Y>& b) const { return N() == b.N();}

    /////////////////////////////////////////////////
    // math member functions
    //
    // *  Note that general forms of math functions are the followings
    // *  - a.sort(...)    --> a = sort(a) 
    // *  - b.sort(a, ...) --> b = sort(a)

    // get sum
    T Sum() const
    {
        const int64 n = N(); T sum = 0;

        for(int i = 0; i < n; ++i) sum += v(i);

        return sum;
    };

    // get sum of square
    T SumSq() const
    {
        const int64 n = N(); T sum = 0;

        for(int i = 0; i < n; ++i) sum += v(i)*v(i);

        return sum;
    };

    // get mean
    T Mean() const { return  Sum()/(T) N(); };

    // get mean of sqaure
    T MeanSq() const { return SumSq()/(T) N(); };    

    // get median
    T Median() const
    {
        const int64 n = N();
    
        kmArr b = *this; b.Sort(1);
    
        return (n%2 == 0) ? (b(n/2-1) + b(n/2))/(T)2 : b(n/2);
    };

    // get min
    T Min() const
    {
        const int64 n = N(); T min_v = v(0);

        for(int i = 1; i < n; ++i) if(v(i) < min_v) min_v = v(i);

        return min_v;
    };

    // get index of min
    T Min(int64* idx)
    {
        const int64 n = N(); T min_v = v(0); *idx = 0;

        for(int i = 1; i < n; ++i) if(v(i) < min_v) {min_v = v(i); *idx = i;}

        return min_v;
    };

    // get max
    T Max() const
    {
        const int64 n = N(); T max_v = v(0);

        for(int i = 1; i < n; ++i) if(v(i) > max_v) max_v = v(i);

        return max_v;
    };

    // get index of max
    T Max(int64* idx)
    {
        const int64 n = N(); T max_v = v(0); *idx = 0;

        for(int i = 1; i < n; ++i) if(v(i) > max_v) {max_v = v(i); *idx = i;}

        return max_v;
    };

    // get max in the block
    T Max(int64 i_s, int64 i_e)
    {
        // check idx
        i_s = MAX(i_s, 0);
        i_e = MIN(i_e, N()-1);

        T max_v = v(i_s);

        for (int i = i_s; i <= i_e; ++i) if(v(i) > max_v) max_v = v(i);

        return max_v;
    };

    // get min of abs
    T MinAbs() const
    {
        const int64 n = N(); T min_v = abs(v(0));

        for(int i = 1; i < n; ++i) if(v(i) < min_v) min_v = abs(v(i));

        return min_v;
    };

    // get max of abs
    T MaxAbs() const
    {
        const int64 n = N(); T max_v = abs(v(0));

        for(int i = 1; i < n; ++i) if(v(i) > max_v) max_v = abs(v(i));

        return max_v;
    };

    // sort.. a.sort(order)    
    //   dir :  >= 0 (Ascending order), < 0(Descending order)
    void Sort(int dir = 1)
    {
        QuickSort(0, (int)N()-1, 1, dir);
    };

    // sort with b... a.sort(order, b)... b will be also sorted in order of a
    void Sort(kmArr& b, int dir = 1)
    {
        ASSERTA( N() <= b.N(), "[kmArr::Sort in 349] %lld <= %lld", N(), b.N());

        QuickSort(0, (int)N()-1, 1, dir, b);
    };

    // quick sort algorithm
    // int left        : start point
    // int right    : end point
    // int step        : step
    // int order    : >= 0 (Ascending order), < 0(Descending order)
    void QuickSort(int left, int right, int step, int dir)
    {
        if(left < right)
        {
            int j = left; const T pivot = v(left);

            for(int i = left + step; i <= right; i+= step)
            {
                const T vi = v(i);

                if((dir >= 0)? (vi < pivot) : (vi > pivot))
                {
                    j+= step; v(i) = v(j); v(j) = vi;
                }
            }
            v(left) = v(j); v(j) = pivot;

            QuickSort(  left, j-step, step, dir);
            QuickSort(j+step,  right, step, dir);
        }
    }
    
    void QuickSort(int left, int right, int step, int dir, kmArr& b)
    {
        if(left < right)
        {
            int j = left; const T pivot = v(left);

            for(int i = left + step; i <= right; i+= step)
            {
                const T vi = v(i);

                if((dir >= 0)? (vi < pivot) : (vi > pivot))
                {
                    j+= step; v(i) = v(j); v(j) = vi; b.Swap(i, j);
                }
            }
            v(left) = v(j); v(j) = pivot; b.Swap(left, j);

            QuickSort(  left, j-step, step, dir, b);
            QuickSort(j+step,  right, step, dir, b);
        }
    }
};

//////////////////////////////////////////////////////////
// 1D matrix class
template<typename T> class kmMat1 : public kmArr<T>
{
protected:
    // using for members of parents class
    using kmArr<T>::_p, kmArr<T>::_state, kmArr<T>::_size;

    // member variables
    int64 _n1 = 0;    // number of dim1

public:
    // using for functions of parents class
    using kmArr<T>::Expand;
    using kmArr<T>::Release;
    using kmArr<T>::IsPinned;
    using kmArr<T>::IsCreated;
    using kmArr<T>::GetKmClass;
    using kmArr<T>::Copy;
    using kmArr<T>::RestoreVfptr;

    /////////////////////////////////////////////////
    // basic member functions
public:    
    virtual void Init() { kmArr<T>::Init(); _n1 = 0; };

    // constructor
    kmMat1() {};
    kmMat1(      int64 n1)             { Create(n1);       };
    kmMat1(T* p, int64 n1)             { Set(p, n1);       };
    kmMat1(      int64 n1, int64 size) { Create(n1, size); };
    kmMat1(T* p, int64 n1, int64 size) { Set(p, n1, size); };

    kmMat1(const initializer_list<T>& val)
    {
        _state =0;
        Create(val.size());
        const T *pval = val.begin();
        for(int64 i = 0; i < _size; ++i) *(_p + i) = *(pval++);
    };

    // destructor
    virtual ~kmMat1() {};

    // copy constructor    
    kmMat1(const kmMat1& b) { Create(b.N1()); Copy(b.Begin()); };

    template<typename Y>
    kmMat1(const kmMat1<Y>& b) { Create(b.N1()); CopyTCast(b); }

    // move constructor
    kmMat1(kmMat1&& b) noexcept { Move(b); };

    // assignment operator
    kmMat1& operator=(const kmMat1& b) { RecreateIf(b); Copy(b._p); return *this; };
        
    template<typename Y>
    kmMat1& operator=(const kmMat1<Y>& b) { RecreateIf(b); CopyTCast(b); return *this; }

    // assignment operator for scalar
    kmMat1& operator=(const T b) { SetVal(b); return *this; };

    // move assignment operator
    kmMat1& operator=(kmMat1&& b)
    {
        if(IsPinned()) *this = b; else Move(b);
        return *this; 
    };

    // allocate memory
    void Create(int64 n1)                { Create(n1, n1); };
    void Create(int64 n1, int64 size)
    {
        ASSERTA( n1 <= size, "[kmMat1::Create in 444] %lld <= %lld", n1, size);
        
        _n1 = n1; kmArr<T>::Create(size);
    };
    
    // release and create
    void Recreate(int64 n1)             { Release(); Create(n1); };
    void Recreate(int64 n1, int64 size) { Release(); Create(n1, size); };

    template<typename Y> void Recreate(const kmMat1<Y>& b) { Recreate(b.N1()); }

    // recreate if dimension is different
    int RecreateIf(int64 n1) { if(n1 != _n1) { Recreate(n1); return 1; } return 0; };

    template<typename Y> int RecreateIf(const kmMat1<Y>& b) { return RecreateIf(b.N1()); }

    // move 
    void Move(kmMat1& b) { kmArr<T>::Move(b); _n1 = b._n1; };

    // set array
    void Set(T* p, int64 n1)            { Set(p, n1, n1); };
    void Set(T* p, int64 n1, int64 size)
    {
        ASSERTA( n1 <= size, "[kmMat1::Set in 195]");

        _n1 = n1; kmArr<T>::Set(p, size);
    };

    // set array... a.Set(b)
    void Set(const kmMat1&    b) { Set(b.P(), b.N1(), b.Size()); };
    void Set(const kmArr<T>&  b) { Set(b.P(), b.Size());         };

    // operator to get data    
    T* P(int64 i1) const
    {
        if(i1 < 0) i1 += _n1; // * Note that i1 = -1 is the same as End()

        ASSERTA(0 <= i1 && i1 < _size , "[kmMat1::P in 951] %lld < %lld", i1, _size);

        return (_p + i1);
    };

    T* P() const { return _p; };

    T& operator()(int64 i1 = 0) const { return *P(i1); };
    T& a         (int64 i1 = 0) const { return *P(i1); };
    virtual T& v (int64 i1 = 0) const { return *P(i1); };

    // get linear interpolation
    float Li(float i1) const
    {
        const int64 i1n = MIN(MAX(0, (int)i1),_n1-2);
        const float i1r = i1 - float(i1n);

        return (1.f - i1r)*a(i1n)  + i1r*a(i1n + 1);
    };

    // operator to get data
    T* End () const { return _p + _n1 - 1; };
    T* End1() const { return _p + _n1;     };

    // restore the class including vfptr
    // * Note that this will clear and reset _p, _state, _size without release,
    // * which can cause a memory leak. So, you should use this very carefully.
    kmMat1<T>& Restore() { RestoreVfptr(*this); _state = 0; Init(); return *this; };
        
    /////////////////////////////////////////////////
    // general member functions

    // get mat1
    kmMat1<T> Mat1(kmI i1) const
    {
        i1.e = MIN(i1.e, _n1-1);

        return kmMat1<T>(P(i1.s), i1.Len());
    };

    // get info
    int64 N1() const { return _n1; };

    // get the number of real elements
    virtual int64 N() const { return _n1; };

    // get bytes of info members which is from _size to end
    virtual int64 GetInfoByte() const { return sizeof(*this) - 24;};

    // display member info
    virtual void PrintInfo(LPCSTR str = nullptr) const
    {
        if(str != nullptr) PRINTFA("[%s]\n", str);

        kmArr<T>::PrintInfo();
        PRINTFA("  _n1    : %lld\n", _n1);
    };

    // display dimension
    virtual void PrintDim(LPCSTR str = nullptr) const
    {
        if(str != nullptr) PRINTFA("[%s]", str);
        else               PRINTFA("[%p]", _p);

        PRINTFA(" %lld (%lld) \n", _n1, _size);
    };

    void PrintMat(LPCSTR str = nullptr) const
    {
        if(str != nullptr) PrintDim(str);

        for(int64 i1 = 0; i1 < _n1  ; ++i1)
        {
            cout << i1 << " :\t" << a(i1) << endl;
        }
        cout << endl;
    };

    // compare
    template<typename Y> bool IsEqualSizeDim(const kmMat1<Y>& b) const { return (_n1 == b._n1); }

    // push back
    int64 PushBack()
    {
        if(_n1 == _size)
        {
            ASSERTA(IsCreated(),"[kmMat1::PushBack in 529]");
            Expand(16);
        }
        return ++_n1 - 1;
    };
    int64 PushBack(const T& val)
    {
        PushBack();    *End() = val; return _n1 - 1;
    };

    // puch back using set if val is kmMat    
    int64 PushBackSet(const T& val)
    {
        PushBack(); End()->Set(val); return _n1 - 1;
    };

    // pop back
    T* PopBack()
    {
        if(_n1 == 0) return nullptr;

        --_n1;  return _p + _n1;
    };

    // insert
    kmMat1& Insert(int64 idx, const T& val)
    {
        // check idx
        ASSERTA(0 <= idx && idx <= _n1,"[kmMat1::Insert in 937]");

        if(idx == _n1) { PushBack(val); return *this; }

        // copy to tmp
        kmMat1 tmp0 = Mat1(kmI(idx, _n1 - 1)), tmp = tmp0;

        // pushback
        PushBack();

        // insert val
        a(idx) = val;

        //copy from tmp
        Mat1(kmI(idx + 1, _n1 - 1)) = tmp;

        return *this;
    };

    // erase one element
    kmMat1& Erase(int64 idx)
    {
        // check idx
        if(idx < 0) idx += _n1; // * Note that i1 = -1 is the same as End()

        ASSERTA(0 <= idx && idx < _n1,"[kmMat1::Insert in 937]");

        if(idx == _n1 - 1) { PopBack(); return *this; }

        // copy to temp
        kmMat1 tmp0 = Mat1(kmI(idx + 1, _n1 - 1)), tmp = tmp0;

        // copy from tmp
        Mat1(kmI(idx, _n1 - 2)) = tmp;

        // popback
        PopBack();

        return *this;
    };

    // erase block
    kmMat1& Erase(kmI i1)
    {
        // check idx
        i1.s = MAX(i1.s, 0);
        i1.e = MIN(i1.e, _n1 - 1);

        if(i1.e == _n1 - 1) { _n1 = i1.s; return *this; }
        
        // copy to temp
        kmMat1 tmp0 = Mat1(kmI(i1.e + 1, _n1 - 1)), tmp = tmp0;

        // decrease _n1
        _n1 = _n1 - i1.Len();

        // copy from tmp
        Mat1(kmI(i1.s, _n1 - 1)) = tmp;

        return *this;
    };

    // set n1
    void SetN1(int64 n1)
    {
        ASSERTA(n1 <= _size, "[kmMat1::SetN1 in 950] n1(%lld) <= _size(%lld)", n1, _size);
        _n1 = n1;
    };
    void IncN1(int64 dn1) { SetN1(_n1 + dn1); };
    void SetN1ToSize()    { _n1 = _size; };

    // pack... _size is forced to be _n1
    // * Note that this is recommened before WriteMat() to save capacity
    // * and to avoid errors caused by missing Restore().
    kmMat1& Pack() { _size = _n1; return *this; };

    // find
    int64 Find(const T& val) const
    {
        const int64 n = N();

        for(int64 i = 0; i < n; ++i)
        {
            if(v(i) == val) return i;
        }
        return -1; // not found
    };
};

//////////////////////////////////////////////////////////
// 2D matrix class
template<typename T> class kmMat2 : public kmMat1<T>
{
protected:
    // using for members of parents class
    using kmArr <T>::_p , kmArr <T>::_state, kmArr<T>::_size;
    using kmMat1<T>::_n1;
    
    // member variables
    int64 _p1 = 0;  // pitch of dim1
    int64 _n2 = 1;  // number of dim2

public:
    // using for functions of parents class
    using kmArr<T>::Expand;
    using kmArr<T>::Release;
    using kmArr<T>::IsPinned;
    using kmArr<T>::IsCreated;
    using kmArr<T>::GetKmClass;
    using kmArr<T>::SetVal;
    using kmArr<T>::Copy;
    using kmArr<T>::RestoreVfptr;

    /////////////////////////////////////////////////
    // basic member functions
public:
    virtual void Init() { kmMat1<T>::Init(); _p1 = 0; _n2 = 1; };

    // constructor
    kmMat2() {};
    kmMat2(int64 n) { Create(n, 1);};

    kmMat2(      int64 n1, int64 n2) { Create(n1, n2);};
    kmMat2(T* p, int64 n1, int64 n2) { Set(p, n1, n2);};

    kmMat2(      int64 n1, int64 n2, int64 p1) { CreateP(n1, n2, p1);};
    kmMat2(T* p, int64 n1, int64 n2, int64 p1) { SetP(p, n1, n2, p1);};

    kmMat2(      int64 n1, int64 n2, int64 p1, int64 size) { CreateP(n1, n2, p1, size);};
    kmMat2(T* p, int64 n1, int64 n2, int64 p1, int64 size) { SetP(p, n1, n2, p1, size);};

    kmMat2(const initializer_list<T>& val)
    {    
        Create(val.size(), 1);
        const T *pval = val.begin();
        for(int64 i = 0; i < _size; ++i) *(_p + i) = *(pval++);
    };

    kmMat2(const kmMat1<T>& a) { Set(a); };

    // destructor
    virtual ~kmMat2() {};
    
    // copy constructor    
    kmMat2(const kmMat2& b)
    {        
        Create(b.N1(), b.N2());
        if(IsEqualSizeDimP(b)) Copy(b.Begin()); else Copy(b);
    };

    template<typename Y>
    kmMat2(const kmMat2<Y>& b)
    {
        Create(b.N1(), b.N2());
        CopyTCast(b);
    }

    // move constructor
    kmMat2(kmMat2&& b) { Move(b); };

    // assignment operator
    kmMat2& operator=(const kmMat2& b)
    {
        RecreateIf(b);
        if(IsEqualSizeDimP(b)) Copy(b.Begin()); else Copy(b);
        return *this;
    };

    template<typename Y>
    kmMat2& operator=(const kmMat2<Y>& b) { RecreateIf(b); CopyTCast(b); return *this; }

    // assignment operator for scalar
    kmMat2& operator=(const T b) { SetVal(b); return *this; };

    // move assignment operator
    kmMat2& operator=(kmMat2&& b)
    {
        if(IsPinned()) *this = b; else Move(b);
        return *this; 
    };
    
    // allocate memory
    void Create (int64 n1, int64 n2 = 1)         { CreateP(n1, n2, n1, n1*n2); };
    void Create (int64 n1, int64 n2, int64 size) { CreateP(n1, n2, n1, size ); };
    void CreateP(int64 n1, int64 n2, int64 p1)   { CreateP(n1, n2, p1, p1*n2); };
    void CreateP(int64 n1, int64 n2, int64 p1, int64 size)
    {    
        ASSERTA(p1*n2 <= size, "[kmMat2::CreateP in 261]");

        _n1 = n1; _n2 = n2; _p1 = p1; kmArr<T>::Create(size);
    };

    // release and create
    void Recreate (int64 n1, int64 n2 = 1)         { Release(); Create (n1, n2      ); };
    void Recreate (int64 n1, int64 n2, int64 size) { Release(); Create (n1, n2, size); };
    void RecreateP(int64 n1, int64 n2, int64 p1)   { Release(); CreateP(n1, n2, p1  ); };
    void RecreateP(int64 n1, int64 n2, int64 p1, int64 size)
    {
        Release(); CreateP(n1, n2, p1, size);
    };
    template<typename Y> void Recreate(const kmMat2<Y>& b) { Recreate(b.N1(), b.N2()); }

    // recreate if    
    int RecreateIf(int64 n1, int64 n2)
    {
        if(n1 != _n1 || n2 != _n2) { Recreate(n1, n2); return 1; } return 0;
    };    
    template<typename Y> int RecreateIf(const kmMat2<Y>& b) { return RecreateIf(b.N1(), b.N2()); }

    // move 
    void Move(kmMat2& b) { kmMat1<T>::Move(b); _n2 = b._n2; _p1 = b._p1; };

    // set array    
    void Set (T* p, int64 n1, int64 n2 = 1)         { SetP(p, n1, n2, n1, n1*n2); };
    void Set (T* p, int64 n1, int64 n2, int64 size) { SetP(p, n1, n2, n1, size ); };
    void SetP(T* p, int64 n1, int64 n2, int64 p1)   
    { 
        SetP(p, n1, n2, p1, CalcMinSize(n1,n2,p1)); 
    };
    void SetP(T* p, int64 n1, int64 n2, int64 p1, int64 size)
    {
        const int64 min_size = CalcMinSize(n1,n2,p1);

        ASSERTA(min_size <= size, "[kmMat2::SetP in 284]");

        _n1 = n1; _n2 = n2; _p1 = p1; kmArr<T>::Set(p, size);
    };

    // calculate the minimum size.
    // * Note that (p - n) of the last element is actually not used.
    static int64 CalcMinSize(int64 n1, int64 n2, int64 p1)
    {
        return p1*n2 - (p1 - n1);
    };

    // set array... a.Set(b)
    void Set(const kmMat2<T>& a) { SetP(a.P(), a.N1(), a.N2(), a.P1(), a.Size()); };
    void Set(const kmMat1<T>& a) { Set (a.P(), a.N1(),      1, a.Size()); };
    void Set(const kmArr <T>& a) { Set (a.P(), a.Size(),    1, a.Size()); };
    
    // operator to get data
    T* P(int64 i1, int64 i2) const
    {
        const int64 idx = i1 + _p1*i2;

        ASSERTA(0 <= idx && idx < _size,"[kmMat2::P in 610] idx: %lld (%lld, %lld), _size: %lld", idx, i1, i2, _size);

        return _p + idx;
    };

    T* P(int64 i) const
    {
        if(i < 0) i += _n1*_n2; // * Note that i1 = -1 is the same as End()

        const int64 i2 = i/_n1; i-= i2*_n1;        

        return P(i, i2);
    };

    T* P() const { return _p; };

    T& operator()(int64 i1, int64 i2) const { return *P(i1, i2); };
    T& operator()(int64 i1          ) const { return *P(i1);     };
    T& operator()(                  ) const { return *_p;        };
    T& a         (int64 i1, int64 i2) const { return *P(i1, i2); };
    T& a         (int64 i1          ) const { return *P(i1);     };
    T& a         (                  ) const { return *_p;        };
    virtual T& v (int64 i1          ) const { return *P(i1);     };

    T* End() const { return P(_n1-1, _n2-1); };

    // restore the class including vfptr
    // * Note that this will clear and reset _p, _state, _size without release,
    // * which can cause a memory leak. So, you should use this very carefully.
    kmMat2& Restore() { RestoreVfptr(*this); _state = 0; Init(); return *this; };

    // get bi-linear interpolation    
    float Li(float i1, float i2) const
    {
        const int64 i1n = MIN(MAX(0, (int)i1),_n1-2);
        const int64 i2n = MIN(MAX(0, (int)i2),_n2-2);
        const float i1r = i1 - float(i1n);
        const float i2r = i2 - float(i2n);
        const float v00 = (1.f - i1r)*a(i1n, i2n    )  + i1r*a(i1n + 1, i2n    );
        const float v01 = (1.f - i1r)*a(i1n, i2n + 1)  + i1r*a(i1n + 1, i2n + 1);

        return (1.f - i2r)*v00  + i2r*v01;
    };

    // get linear interpolation    
    float Li(float i1, int64 i2) const
    {
        const int64 i1n = MIN(MAX(0, (int)i1),_n1-2);
        const float i1r = i1 - float(i1n);

        return (1.f - i1r)*a(i1n, i2)  + i1r*a(i1n + 1, i2);
    };
    float Li(float i1, int i2) const { return Li(i1, (int64)i2); };

    // get linear interpolation
    float Li(int64 i1, float i2) const
    {
        const int64 i2n = MIN(MAX(0, (int)i2),_n2-2);
        const float i2r = i2 - float(i2n);

        return (1.f - i2r)*a(i1, i2n)  + i2r*a(i1, i2n + 1);
    };
    float Li(int i1, float i2) const { return Li((int64)i1, i2); };

    // reshape
    void Reshape(int64 n1, int64 n2, int64 p1 = 0)
    {
        // init arguement
        if(p1 == 0) p1 = n1;

        // check size
        ASSERTA(p1*n2 <= _size, "[kmMat2::Reshape in 977]");

        // reset members
        _n1 = n1; _n2 = n2;    _p1 = p1;
    };
        
    /////////////////////////////////////////////////
    // general member functions

    // get mat1
    kmMat1<T> Mat1(int64 i2) const { return kmMat1<T>(P(0,i2), _n1);};

    // get mat2
    kmMat2<T> Mat2(kmI i1, kmI i2) const
    {
        i1.e = MIN(i1.e, _n1-1);
        i2.e = MIN(i2.e, _n2-1);

        return kmMat2<T>(P(i1.s, i2.s), i1.Len(), i2.Len(), _p1);
    };

    // get mat2 with linear interpolation
    kmMat2<T> Mat2Li(float i1s, float d1, int n1, kmI i2) const
    {
        if(i2.e == end64) i2.e = _n2 - 1;

        kmMat2<T> mat(n1, i2.Len());

        for(int64 j2 = i2.s; j2 <= i2.e; ++j2)
        for(int64 j1 =    0; j1 <  n1  ; ++j1)
        {
            mat(j1,j2) = Li(i1s + d1*j1,j2);
        }
        return mat;
    };

    // get flat matrix
    kmMat1<T> Flat() const
    {    
        ASSERTA(_n1 == _p1, "[kmMat2::Flat in 1310]");
    
        return kmMat1<T>(P(), N());
    };

    // set zero
    void SetZero() { SetVal(0); };

    // get info
    int64 N2() const { return _n2; };
    int64 P1() const { return _p1; };
    
    // get the number of real elements
    virtual int64 N() const { return _n1*_n2;};

    // get bytes of info members which is from _size to end
    virtual int64 GetInfoByte() const { return sizeof(*this) - 24;};

    // display member info
    virtual void PrintInfo(LPCSTR str = nullptr) const
    {
        if(str != nullptr) PRINTFA("[%s]\n", str);

        kmMat1<T>::PrintInfo();
        PRINTFA("  _p1    : %lld\n", _p1);
        PRINTFA("  _n2    : %lld\n", _n2);
    };

    // display dimension
    virtual void PrintDim(LPCSTR str = nullptr) const
    {
        if(str != nullptr) PRINTFA("[%s]", str);
        else               PRINTFA("[%p]", _p);

        PRINTFA(" %lld, %lld (%lld) \n", _n1, _n2, _size);
    };

    void PrintMat(LPCSTR str = nullptr) const
    {
        if(str != nullptr) PrintDim(str);

        for(int64 i1 = 0; i1 < _n1  ; ++i1)
        {
            cout << i1 << " :\t";
            for(int64 i2 = 0; i2 < _n2; ++i2) cout << a(i1,i2) << " \t";
            cout << endl;
        }
        cout << endl;
    };

    // compare
    template<typename Y> bool IsEqualSizeDim (const kmMat2<Y>& b) const 
    {
        return _n1 == b.N1() && _n2 == b.N2();
    }

    template<typename Y> bool IsEqualSizeDimP(const kmMat2<Y>& b) const
    {
        return (_p1 == b.P1()) && IsEqualSizeDim(b);
    }

    template<typename Y> bool IsEqualSizeAll (const kmMat2<Y>& b) const 
    {
        return IsEqualSizeDimP(b) && IsEqualSize(b);
    }

    // get transpose matrix
    kmMat2 Tp() const
    {
        kmMat2 des(_n2, _n1);

        for(int64 i2 = 0; i2 < _n2; ++i2)
        for(int64 i1 = 0; i1 < _n1; ++i1)
        {
            des(i2,i1) = *P(i1,i2);
        }

        return des;    // * Note that des is the local variable,
                    // * so the return type cannot be reference type.
    };

    void Cat(const kmMat2& b, const kmMat2& c, const int dim = 1)
    {
        if(dim == 1)      // catenate as 1st dim
        {            
            const int64 n1  = b.N1() + c.N1();
            const int64 n2  = MAX(b.N2(), c.N2());
            const int64 bn1 = b.N1();

            Recreate(n1, n2);

            for(int64 i2 = 0; i2 < _n2; ++i2)
            for(int64 i1 = 0; i1 < _n1; ++i1)
            {
                *P(i1, i2) = (i1 < bn1) ? b(i1, i2) : c(i1 - bn1, i2);
            }
        }
        else if(dim == 2) // catenate along 2nd dim
        {            
            const int64 n1  = MAX(b.N1(), c.N1());
            const int64 n2  = b.N2() + c.N2();
            const int64 bn2 = b.N2();

            Recreate(n1, n2);

            for(int64 i2 = 0; i2 < _n2; ++i2)
            for(int64 i1 = 0; i1 < _n1; ++i1)
            {
                *P(i1, i2) = (i2 < bn2) ? b(i1, i2) : c(i1, i2 - bn2);
            }
        }
        else
        {
            PRINTFA("* [kmMat2::Cat] dim (%d) is not supported\n", dim);
        }
    };

    kmMat2 Repmat(const int64 r1, const int64 r2)
    {
        kmMat2 b(_n1*r1, _n2*r2);

        for(int64 j2 = 0; j2 < r2 ; ++j2)
        for(int64 j1 = 0; j1 < r1 ; ++j1)
        for(int64 i2 = 0; i2 < _n2; ++i2)
        for(int64 i1 = 0; i1 < _n1; ++i1)
        {
            b(i1 + j1*_n1, i2 + j2*_n2) = a(i1, i2);
        }

        return b;
    };

    /////////////////////////////////////////////////
    // math member functions

    // calc determinant
    T Det() const
    {
        // check size
        ASSERTA(_n1 == _n2   ,"[kmMat2::Det in 827]");        

        // calc determinant.. det
        T det = 0;

        if(_n1 == 1)
        {
            det = a(0,0);
        }
        else if(_n1 == 2)
        {
            det = a(0,0)*a(1,1) - a(1,0)*a(0,1);
        }

        return det;
    };

    // inverse matrix a = inv(b)... a.inv(b)
    kmMat2& Inv(const kmMat2& b)
    {
        // check size
        ASSERTA(b._n1 == b._n2   , "[kmMat2::Inv in 828]");
        ASSERTA(IsEqualSizeDim(b), "[kmMat2::Inv in 829]");

        // get det
        T det = b.Det();

        // check determiant
        ASSERTA(abs(det) > 0, "[kmMat2::Inv in 856]");

        if(_n1 == 1)
        {
            a(0,0) = (T) 1/det;
        }
        else if(_n1 == 2)
        {
            a(0,0) =  b(1,1)/det;
            a(1,1) =  b(0,0)/det;
            a(0,1) = -b(0,1)/det;
            a(1,0) = -b(1,0)/det;
        }
        return *this;
    };

    // inverse matrix b = inv(a)... b = a.inv()
    kmMat2 Inv() const
    {
        kmMat2 b(_n1, _n2);
        return b.Inv(*this);
    };

    // get diagonal matrix... a = diag(b)... a.Diag(b)
    kmMat2& Diag(const kmMat2& b)
    {
        // check size
        ASSERTA(b._n1 == b._n2   , "[kmMat2::Diag in 1160]");
        ASSERTA(IsEqualSizeDim(b), "[kmMat2::Diag in 1161]");

        // clear output matrix
        SetZero();

        // set diagonal terms
        for(int i = 0; i < _n1; ++i) a(i,i) = b(i,i);

        return *this;
    };

    // get diagnonal matrix... b = diag(a)... b = a.Diag();
    kmMat2 Diag() const
    {
        kmMat2 b(_n1, _n2);
        return b.Diag(*this);
    };

    // get sub element using bilinear interpolation
    T GetSub(float x, float y) const 
    {
        const int64 x0 = (int64) x, x1 = x0 + 1;
        const int64 y0 = (int64) y, y1 = y0 + 1;
        const float  xr  = x - (float) x0;
        const float  yr  = y - (float) y0;

        T vy0 = a(x0, y0)*(1.f - xr) + a(x1, y0)*xr;
        T vy1 = a(x0, y1)*(1.f - xr) + a(x1, y1)*xr;

        return vy0*(1.f - yr) + vy1*yr;
    };
};

//////////////////////////////////////////////////////////
// 3D matrix class
template<typename T> class kmMat3 : public kmMat2<T>
{
protected:
    // using for members of parents class
    using kmArr <T>::_p , kmArr <T>::_state, kmArr<T>::_size;
    using kmMat1<T>::_n1;
    using kmMat2<T>::_n2, kmMat2<T>::_p1;

    // member variables
    int64 _p2 = 0;  // pitch of dim2
    int64 _n3 = 1;  // number of dim3

public:
    // using functions of parents class
    using kmArr<T>::Expand;
    using kmArr<T>::Release;
    using kmArr<T>::IsPinned;
    using kmArr<T>::IsCreated;
    using kmArr<T>::GetKmClass;
    using kmArr<T>::SetVal;
    using kmArr<T>::Copy;

    /////////////////////////////////////////////////
    // basic member functions
public:
    virtual void Init() { kmMat2<T>::Init(); _p2 = 0; _n3 = 1; };

    // constructor
    kmMat3() {};
    kmMat3(int64 n) { Create(n, 1, 1);};

    kmMat3(      int64 n1, int64 n2, int64 n3) { Create(n1, n2, n3);};
    kmMat3(T* p, int64 n1, int64 n2, int64 n3) { Set(p, n1, n2, n3);};

    kmMat3(      int64 n1, int64 n2, int64 n3, int64 p1) { CreateP(n1, n2, n3, p1);};
    kmMat3(T* p, int64 n1, int64 n2, int64 n3, int64 p1) { SetP(p, n1, n2, n3, p1);};

    kmMat3(      int64 n1, int64 n2, int64 n3, int64 p1, int64 p2) { CreateP(n1, n2, n3, p1, p2);};
    kmMat3(T* p, int64 n1, int64 n2, int64 n3, int64 p1, int64 p2) { SetP(p, n1, n2, n3, p1, p2);};

    kmMat3(      int64 n1, int64 n2, int64 n3, int64 p1, int64 p2, int64 size) { CreateP(n1, n2, n3, p1, p2, size);};
    kmMat3(T* p, int64 n1, int64 n2, int64 n3, int64 p1, int64 p2, int64 size) { SetP(p, n1, n2, n3, p1, p2, size);};
    
    kmMat3(const initializer_list<T>& val)
    {
        Create(val.size(), 1, 1);
        const T *pval = val.begin();
        for(int64 i = 0; i < _size; ++i) *(_p + i) = *(pval++);
    };

    kmMat3(const kmMat2<T>& a) { Set(a); };
    kmMat3(const kmMat1<T>& a) { Set(a); };

    // destructor
    virtual ~kmMat3() {};

    // copy constructor    
    kmMat3(const kmMat3& b)
    {        
        Create(b.N1(), b.N2(), b.N3());
        if(IsEqualSizeDimP(b)) Copy(b.Begin()); else Copy(b);
    };

    template<typename Y>
    kmMat3(const kmMat3<Y>& b)
    {
        Create(b.N1(), b.N2(), b.N3());
        CopyTCast(b);
    }

    // move constructor
    kmMat3(kmMat3&& b) { Move(b); };

    // assignment operator    
    kmMat3& operator=(const kmMat3& b)
    {
        RecreateIf(b);
        if(IsEqualSizeDimP(b)) Copy(b.Begin()); else Copy(b);
        return *this;
    };

    template<typename Y>
    kmMat3& operator=(const kmMat3<Y>& b) { RecreateIf(b); CopyTCast(b); return *this; }

    // assignment operator for scalar
    kmMat3& operator=(const T b) { SetVal(b); return *this; };
    
    // move assignment operator
    kmMat3& operator=(kmMat3&& b)    
    {
        if(IsPinned()) *this = b; else Move(b); 
        return *this; 
    };

    // allocate memory
    void Create (int64 n1, int64 n2, int64 n3 = 1)                 { CreateP(n1, n2, n3, n1, n2, n1*n2*n3); };
    void Create (int64 n1, int64 n2, int64 n3, int64 size)         { CreateP(n1, n2, n3, n1, n2, size    ); };
    void CreateP(int64 n1, int64 n2, int64 n3, int64 p1  )         { CreateP(n1, n2, n3, p1, n2, p1*n2*n3); };
    void CreateP(int64 n1, int64 n2, int64 n3, int64 p1, int64 p2) { CreateP(n1, n2, n3, p1, p2, p1*p2*n3); };
    void CreateP(int64 n1, int64 n2, int64 n3, int64 p1, int64 p2, int64 size)
    {
        ASSERTA(p1*p2*n3 <= size,"[kmMat3::CreateP in 778]");

        _n1 = n1; _n2 = n2; _n3 = n3; _p1 = p1; _p2 = p2; kmArr<T>::Create(size);
    };

    // release and create
    void Recreate (int64 n1, int64 n2, int64 n3 = 1)                 { Release(); Create (n1, n2, n3        ); };
    void Recreate (int64 n1, int64 n2, int64 n3, int64 size)         { Release(); Create (n1, n2, n3, size  ); };
    void RecreateP(int64 n1, int64 n2, int64 n3, int64 p1  )         { Release(); CreateP(n1, n2, n3, p1    ); };
    void RecreateP(int64 n1, int64 n2, int64 n3, int64 p1, int64 p2) { Release(); CreateP(n1, n2, n3, p1, p2); };
    void RecreateP(int64 n1, int64 n2, int64 n3, int64 p1, int64 p2, int64 size)
    {
        Release(); CreateP(n1, n2, n3, p1, p2, size);
    };
    template<typename Y> void Recreate(const kmMat3<Y>& b) { Recreate(b.N1(), b.N2(), b.N3()); }

    // recreate if dimension is different    
    int RecreateIf(int64 n1, int64 n2, int64 n3)
    {
        if(n1 != _n1 || n2 != _n2 || n3 != _n3) { Recreate(n1, n2, n3); return 1; } return 0;
    };    
    template<typename Y> int RecreateIf(const kmMat3<Y>& b) { return RecreateIf(b.N1(), b.N2(), b.N3()); }

    // move 
    void Move(kmMat3& b) { kmMat2<T>::Move(b); _n3 = b._n3; _p2 = b._p2; };

    // set array    
    void Set (T* p, int64 n1, int64 n2, int64 n3 = 1)         { SetP(p, n1, n2, n3, n1, n2);      };
    void Set (T* p, int64 n1, int64 n2, int64 n3, int64 size) { SetP(p, n1, n2, n3, n1, n2, size);};
    void SetP(T* p, int64 n1, int64 n2, int64 n3, int64 p1  ) { SetP(p, n1, n2, n3, p1, n2);      };
    void SetP(T* p, int64 n1, int64 n2, int64 n3, int64 p1, int64 p2)
    {
        SetP(p, n1, n2, n3, p1, p2, CalcMinSize(n1,n2,n3,p1,p2));
    };
    void SetP(T* p, int64 n1, int64 n2, int64 n3, int64 p1, int64 p2, int64 size)
    {
        const int64 min_size = CalcMinSize(n1,n2,n3,p1,p2);

        ASSERTA(min_size <= size, "[kmMat3::SetP in 800] %lld <= %lld", min_size, size);

        _n1 = n1; _n2 = n2; _n3 = n3; _p1 = p1; _p2 = p2; kmArr<T>::Set(p, size);
    };

    // calculate the minimum size.
    // * Note that (p - n) of the last element is actually not used.
    static int64 CalcMinSize(int64 n1, int64 n2, int64 n3, int64 p1, int64 p2)
    {
        return p1*p2*n3 - p1*(p2 - n2) - (p1 - n1);
    };

    // set array... a.Set(b)
    void Set(const kmMat3<T>& b) { SetP(b.P(), b.N1(), b.N2(), b.N3(), b.P1(), b.P2(), b.Size()); };
    void Set(const kmMat2<T>& b) { SetP(b.P(), b.N1(), b.N2(),      1, b.P1(), b.N2(), b.Size()); };
    void Set(const kmMat1<T>& b) { Set (b.P(), b.N1(),      1,      1, b.Size()); };
    void Set(const kmArr <T>& b) { Set (b.P(), b.Size(),    1,      1, b.Size()); };
    
    // operator to get data
    T* P(int64 i1, int64 i2 , int64 i3) const
    {
        const int64 idx = i1 + _p1*(i2 + _p2*i3);

        ASSERTA(0 <= idx && idx < _size, "[kmMat3::P in 816] %lld < %lld", idx, _size);

        return _p + idx;
    };

    T* P(int64 i1, int64 i) const
    {
        const int64 i3 = i/_n2; i-= i3*_n2;        

        return P(i1, i, i3);
    };

    T* P(int64 i) const
    {
        if(i < 0) i += _n1*_n2*_n3; // * Note that i1 = -1 is the same as End()

        const int64 n12 = _n1*_n2;
        const int64 n1  = _n1;

        const int64 i3  = i/n12; i-= i3*n12;
        const int64 i2  = i/n1;  i-= i2*n1;
        
        return P(i, i2, i3);
    };

    T* P() const { return _p; };
    
    T& operator()(int64 i1, int64 i2, int64 i3) const { return *P(i1, i2, i3);};
    T& operator()(int64 i1, int64 i2          ) const { return *P(i1, i2);    };
    T& operator()(int64 i1                    ) const { return *P(i1);        };
    T& operator()(                            ) const { return *_p;           };
    T& a         (int64 i1, int64 i2, int64 i3) const { return *P(i1, i2, i3);};
    T& a         (int64 i1, int64 i2          ) const { return *P(i1, i2);    };
    T& a         (int64 i1                    ) const { return *P(i1);        };
    T& a         (                            ) const { return *_p;           };
    virtual T& v (int64 i1                    ) const { return *P(i1);        };
               
    T* End() const { return P(_n1-1, _n2-1, _n3-1); };

    // restore the class including vfptr
    // * Note that this will clear and reset _p, _state, _size without release,
    // * which can cause a memory leak. So, you should use this very carefully.
    kmMat3& Restore() { RestoreVfptr(*this); _state = 0; Init(); return *this; };

    // reshape
    void Reshape(int64 n1, int64 n2, int64 n3, int64 p1 = 0, int64 p2 = 0)
    {
        // init arguement
        if(p1 == 0) p1 = n1;
        if(p2 == 0) p2 = n2;

        // check size
        ASSERTA(p1*p2*n3 <= _size, "[kmMat3::Reshape in 1944]");

        // reset members
        _n1 = n1; _n2 = n2;    _n3 = n3; _p1 = p1; _p2 = p2;
    };
    
    /////////////////////////////////////////////////
    // general member functions

    // get mat1
    kmMat1<T> Mat1(int64 i23)          const { return kmMat1<T>(P(0,i23)  , _n1);};
    kmMat1<T> Mat1(int64 i2, int64 i3) const { return kmMat1<T>(P(0,i2,i3), _n1);};

    // get mat2
    kmMat2<T> Mat2(int64 i3) const { return kmMat2<T>(P(0,0,i3), _n1, _n2, _p1);};

    // get mat3
    kmMat3<T> Mat3(kmI i1, kmI i2, kmI i3) const
    {
        i1.e = MIN(i1.e, _n1-1);
        i2.e = MIN(i2.e, _n2-1);
        i3.e = MIN(i3.e, _n3-1);

        return kmMat3<T>(P(i1.s, i2.s, i3.s), i1.Len(), i2.Len(), i3.Len(), _p1, _p2);
    };

    // get info
    int64 N3() const { return _n3; };
    int64 P2() const { return _p2; };
    
    // get the number of real elements
    virtual int64 N() const { return _n1*_n2*_n3;};

    // get bytes of info members which is from _size to end
    virtual int64 GetInfoByte() const { return sizeof(*this) - 24;};
    
    // display member info
    virtual void PrintInfo(LPCSTR str = nullptr) const
    {
        if(str != nullptr) PRINTFA("[%s]\n", str);

        kmMat2<T>::PrintInfo();
        PRINTFA("  _p2    : %lld\n", _p2);
        PRINTFA("  _n3    : %lld\n", _n3);        
    };

    // display dimension
    virtual void PrintDim(LPCSTR str = nullptr) const
    {
        if(str != nullptr) PRINTFA("[%s]", str);
        else               PRINTFA("[%p]", _p);

        PRINTFA(" %lld, %lld, %lld (%lld) \n", _n1, _n2, _n3, _size);
    };

    void PrintMat(LPCSTR str = nullptr) const
    {
        if(str != nullptr) PrintDim(str);
                
        const int64 n_mat = N()/(_n1*_n2);

        for(int64 im = 0; im < n_mat; ++im)
        {
            printf("[ %d / %d ]\n", im, n_mat);

            Mat2(im).PrintMat();
        }
        cout << endl;
    };

    // compare
    template<typename Y> bool IsEqualSizeDim(const kmMat3<Y>& b) const
    {
        return _n1 == b.N1() && _n2 == b.N2() && _n3 == b.N3();
    }

    template<typename Y> bool IsEqualSizeDimP(const kmMat3<Y>& b) const
    {
        return (_p1 == b.P1()) && (_p2 == b.P2()) && IsEqualSizeDim(b);
    }

    template<typename Y> bool IsEqualSizeAll(const kmMat3<Y>& b) const
    {
        return IsEqualSizeDimP(b) && IsEqualSize(b);
    }

    // get transpose matrix
    kmMat3 Tp() const
    {
        kmMat3 des(_n2, _n1, _n3);

        for(int64 i3 = 0; i3 < _n3; ++i3)
        for(int64 i2 = 0; i2 < _n2; ++i2)
        for(int64 i1 = 0; i1 < _n1; ++i1)
        {
            des(i2, i1, i3) = *P(i1, i2, i3);
        }

        return des;    // * Note that des is the local variable,
                    // * so the return type cannot be reference type.
    };

    // get swap matrix
    kmMat3 Swap13() const
    {
        kmMat3 des(_n3, _n2, _n1);

        for(int64 i3 = 0; i3 < _n3; ++i3)
        for(int64 i2 = 0; i2 < _n2; ++i2)
        for(int64 i1 = 0; i1 < _n1; ++i1)
        {
            des(i3, i2, i1) = *P(i1, i2, i3);
        }

        return des;    // * Note that des is the local variable,
                    // * so the return type cannot be reference type.
    };
};

//////////////////////////////////////////////////////////
// 4D matrix class
template<typename T> class kmMat4 : public kmMat3<T>
{
protected:
    // using for members of parents class
    using kmArr <T>::_p , kmArr <T>::_state, kmArr<T>::_size;
    using kmMat1<T>::_n1;
    using kmMat2<T>::_n2, kmMat2<T>::_p1;
    using kmMat3<T>::_n3, kmMat3<T>::_p2;

    // member variables
    int64 _p3 = 0;   // pitch of dim3
    int64 _n4 = 1;   // number of dim4
    
public:
    // using for functions of parents class
    using kmArr<T>::Expand;
    using kmArr<T>::Release;
    using kmArr<T>::IsPinned;
    using kmArr<T>::IsCreated;
    using kmArr<T>::GetKmClass;
    using kmArr<T>::SetVal;
    using kmArr<T>::RestoreVfptr;
    using kmArr<T>::Copy;

    /////////////////////////////////////////////////
    // basic member functions
public:
    virtual void Init() { kmMat3<T>::Init(); _p3 = 0; _n4 = 1; };

    // constructor
    kmMat4() {};
    kmMat4(int64 n) { Create(n, 1, 1, 1);};

    kmMat4(      int64 n1, int64 n2, int64 n3, int64 n4) { Create(n1, n2, n3, n4);};
    kmMat4(T* p, int64 n1, int64 n2, int64 n3, int64 n4) { Set(p, n1, n2, n3, n4);};

    kmMat4(      int64 n1, int64 n2, int64 n3, int64 n4, int64 p1) { CreateP(n1, n2, n3, n4, p1);};
    kmMat4(T* p, int64 n1, int64 n2, int64 n3, int64 n4, int64 p1) { SetP(p, n1, n2, n3, n4, p1);};

    kmMat4(      int64 n1, int64 n2, int64 n3, int64 n4, int64 p1, int64 p2) { CreateP(n1, n2, n3, n4, p1, p2);};
    kmMat4(T* p, int64 n1, int64 n2, int64 n3, int64 n4, int64 p1, int64 p2) { SetP(p, n1, n2, n3, n4, p1, p2);};

    kmMat4(      int64 n1, int64 n2, int64 n3, int64 n4, int64 p1, int64 p2, int64 p3) { CreateP(n1, n2, n3, n4, p1, p2, p3);};
    kmMat4(T* p, int64 n1, int64 n2, int64 n3, int64 n4, int64 p1, int64 p2, int64 p3) { SetP(p, n1, n2, n3, n4, p1, p2, p3);};
                                                  
    kmMat4(      int64 n1, int64 n2, int64 n3, int64 n4, int64 p1, int64 p2, int64 p3, int64 size) { CreateP(n1, n2, n3, n4, p1, p2, p3, size);};
    kmMat4(T* p, int64 n1, int64 n2, int64 n3, int64 n4, int64 p1, int64 p2, int64 p3, int64 size) { SetP(p, n1, n2, n3, n4, p1, p2, p3, size);};

    kmMat4(const initializer_list<T>& val)
    {
        Create(val.size(), 1, 1, 1);
        const T *pval = val.begin();
        for(int64 i = 0; i < _size; ++i) *(_p + i) = *(pval++);
    };    

    kmMat4(const kmMat3<T>& a) { Set(a); };
    kmMat4(const kmMat2<T>& a) { Set(a); };
    kmMat4(const kmMat1<T>& a) { Set(a); };

    // destructor
    virtual ~kmMat4() {};

    // copy constructor    
    kmMat4(const kmMat4& b)
    {        
        Create(b.N1(), b.N2(), b.N3(), b.N4());
        if(IsEqualSizeDimP(b)) Copy(b.Begin()); else Copy(b);
    };
    
    template<typename Y>
    kmMat4(const kmMat4<Y>& b)
    {
        Create(b.N1(), b.N2(), b.N3(), b.N4());
        CopyTCast(b);
    }

    // move constructor
    kmMat4(kmMat4&& b) { Move(b); };

    // assignment operator
    kmMat4& operator=(const kmMat4& b)
    {
        RecreateIf(b);
        if(IsEqualSizeDimP(b)) Copy(b.Begin()); else Copy(b);
        return *this;
    }

    template<typename Y>
    kmMat4& operator=(const kmMat4<Y>& b) { RecreateIf(b); CopyTCast(b); return *this; }

    // assignment operator for scalar
    kmMat4& operator=(const T b) { SetVal(b); return *this; };

    // move assignment operator
    kmMat4& operator=(kmMat4&& b)
    {
        if(IsPinned()) *this = b; else Move(b);
        return *this; 
    };

    // allocate memory
    void Create (int64 n1, int64 n2, int64 n3, int64 n4 = 1)                           { CreateP(n1, n2, n3, n4, n1, n2, n3, n1*n2*n3*n4); };
    void Create (int64 n1, int64 n2, int64 n3, int64 n4, int64 size)                   { CreateP(n1, n2, n3, n4, n1, n2, n3, size       ); };
    void CreateP(int64 n1, int64 n2, int64 n3, int64 n4, int64 p1  )                   { CreateP(n1, n2, n3, n4, p1, n2, n3, p1*n2*n3*n4); };
    void CreateP(int64 n1, int64 n2, int64 n3, int64 n4, int64 p1, int64 p2)           { CreateP(n1, n2, n3, n4, p1, p2, n3, p1*p2*n3*n4); };
    void CreateP(int64 n1, int64 n2, int64 n3, int64 n4, int64 p1, int64 p2, int64 p3) { CreateP(n1, n2, n3, n4, p1, p2, p3, p1*p2*p3*n4); };
    void CreateP(int64 n1, int64 n2, int64 n3, int64 n4, int64 p1, int64 p2, int64 p3, int64 size)
    {
        ASSERTA(p1*p2*p3*n4 <= size,"[kmMat4::CreateP in 1897]");

         _n1 = n1; _n2 = n2; _n3 = n3; _n4 = n4; _p1 = p1; _p2 = p2; _p3 = p3; kmArr<T>::Create(size);
    };

    // release and create
    void Recreate (int64 n1, int64 n2, int64 n3, int64 n4 =1)                            { Release(); Create (n1, n2, n3, n4      );       };
    void Recreate (int64 n1, int64 n2, int64 n3, int64 n4, int64 size)                   { Release(); Create (n1, n2, n3, n4, size);       };
    void RecreateP(int64 n1, int64 n2, int64 n3, int64 n4, int64 p1  )                   { Release(); CreateP(n1, n2, n3, n4, p1    );     };
    void RecreateP(int64 n1, int64 n2, int64 n3, int64 n4, int64 p1, int64 p2)           { Release(); CreateP(n1, n2, n3, n4, p1, p2);     };
    void RecreateP(int64 n1, int64 n2, int64 n3, int64 n4, int64 p1, int64 p2, int64 p3) { Release(); CreateP(n1, n2, n3, n4, p1, p2, p3); };
    void RecreateP(int64 n1, int64 n2, int64 n3, int64 n4, int64 p1, int64 p2, int64 p3, int64 size)
    {
        Release(); CreateP(n1, n2, n3, n4, p1, p2, p3, size);
    };
    template<typename Y> void Recreate(const kmMat4<Y>& b) { Recreate(b.N1(), b.N2(), b.N3(), b.N4()); }

    // recreate if dimension is different
    int RecreateIf(int64 n1, int64 n2, int64 n3, int64 n4)
    {
        if(n1 != _n1 || n2 != _n2 || n3 != _n3 || n4 != _n4) { Recreate(n1,n2,n3,n4); return 1; }    return 0;
    };    
    template<typename Y> int RecreateIf(const kmMat4<Y>& b) { return RecreateIf(b.N1(), b.N2(), b.N3(), b.N4()); }

    // move
    void Move(kmMat4& b) { kmMat3<T>::Move(b); _n4 = b._n4; _p3 = b._p3; };

    // set array                                                                                                             
    void Set (T* p, int64 n1, int64 n2, int64 n3, int64 n4 = 1)                 { SetP(p, n1, n2, n3, n4, n1, n2, n3);      };
    void Set (T* p, int64 n1, int64 n2, int64 n3, int64 n4, int64 size)         { SetP(p, n1, n2, n3, n4, n1, n2, n3, size);};
    void SetP(T* p, int64 n1, int64 n2, int64 n3, int64 n4, int64 p1  )         { SetP(p, n1, n2, n3, n4, p1, n2, n3);      };
    void SetP(T* p, int64 n1, int64 n2, int64 n3, int64 n4, int64 p1, int64 p2) { SetP(p, n1, n2, n3, n4, p1, p2, n3);      };
    void SetP(T* p, int64 n1, int64 n2, int64 n3, int64 n4, int64 p1, int64 p2, int64 p3) 
    {    
        SetP(p, n1, n2, n3, n4, p1, p2, p3, CalcMinSize(n1,n2,n3,n4,p1,p2,p3)); 
    };
    void SetP(T* p, int64 n1, int64 n2, int64 n3, int64 n4, int64 p1, int64 p2, int64 p3, int64 size)
    {    
        const int64 min_size = CalcMinSize(n1,n2,n3,n4,p1,p2,p3);

        ASSERTA(min_size <= size, "[kmMat4::SetP in 1922] %lld <= %lld", min_size, size);

        _n1 = n1; _n2 = n2; _n3 = n3; _n4 = n4; _p1 = p1; _p2 = p2; _p3 = p3; kmArr<T>::Set(p, size);
    };
        
    // calculate the minimum size.
    // * Note that (p - n) of the last element is actually not used.
    static int64 CalcMinSize(int64 n1, int64 n2, int64 n3, int64 n4, int64 p1, int64 p2, int64 p3)
    {
        return p1*p2*p3*n4 - p1*p2*(p3 - n3) - p1*(p2 - n2) - (p1 - n1);
    };

    // set array... a.Set(b)
    void Set(const kmMat4<T>& b) { SetP(b.P(), b.N1(), b.N2(), b.N3(), b.N4(), b.P1(), b.P2(), b.P3(), b.Size()); };
    void Set(const kmMat3<T>& b) { SetP(b.P(), b.N1(), b.N2(), b.N3(),      1, b.P1(), b.P2(), b.N3(), b.Size()); };
    void Set(const kmMat2<T>& b) { SetP(b.P(), b.N1(), b.N2(),      1,      1, b.P1(), b.N2(),     1 , b.Size()); };
    void Set(const kmMat1<T>& b) { Set (b.P(), b.N1(),     1,       1,      1, b.Size()); };
    void Set(const kmArr <T>& b) { Set (b.P(), b.Size(),   1,       1,      1, b.Size()); };
    
    // operator to get data
    T* P(int64 i1, int64 i2 , int64 i3, int64 i4) const
    {
        const int64 idx = i1 + _p1*(i2 + _p2*(i3 + _p3*i4));

        ASSERTA(0 <= idx && idx < _size, "[kmMat4::P in 1939] %lld < %lld", idx, _size);

        return _p + idx;
    };

    T* P(int64 i1, int64 i2, int64 i) const
    {
        const int64 i4 = i/_n3; i-= i4*_n3;

        return P(i1, i2, i, i4);
    };

    T* P(int64 i1, int64 i) const
    {
        const int64 n23 = _n2*_n3;
        const int64 n2  = _n2;

        const int64 i4 = i/n23; i-= i4*n23;
        const int64 i3 = i/n2;  i-= i3*n2;

        return P(i1, i, i3, i4);
    };

    T* P(int64 i) const
    {
        if(i < 0) i += _n1*_n2*_n3*_n4; // * Note that i1 = -1 is the same as End()

        const int64 n123 = _n1*_n2*_n3;
        const int64 n12  = _n1*_n2;
        const int64 n1   = _n1;

        const int64 i4 = i/n123; i-= i4*n123;
        const int64 i3 = i/n12;  i-= i3*n12;
        const int64 i2 = i/n1;   i-= i2*n1;

        return P(i, i2, i3, i4);
    };

    T* P() const { return _p; };
    
    T& operator()(int64 i1, int64 i2, int64 i3, int64 i4) const { return *P(i1, i2, i3, i4);};
    T& operator()(int64 i1, int64 i2, int64 i3          ) const { return *P(i1, i2, i3);    };
    T& operator()(int64 i1, int64 i2                    ) const { return *P(i1, i2);        };
    T& operator()(int64 i1                              ) const { return *P(i1);            };
    T& operator()(                                      ) const { return *_p;               };
    T& a         (int64 i1, int64 i2, int64 i3, int64 i4) const { return *P(i1, i2, i3, i4);};
    T& a         (int64 i1, int64 i2, int64 i3          ) const { return *P(i1, i2, i3);    };
    T& a         (int64 i1, int64 i2                    ) const { return *P(i1, i2);        };
    T& a         (int64 i1                              ) const { return *P(i1);            };
    T& a         (                                      ) const { return *_p;               };
    virtual T& v (int64 i1                              ) const { return *P(i1);            };
               
    T* End() const { return P(_n1-1, _n2-1, _n3-1, _n4-1); };

    // restore the class including vfptr
    // * Note that this will clear and reset _p, _state, _size without release,
    // * which can cause a memory leak. So, you should use this very carefully.
    kmMat4& Restore() { RestoreVfptr(*this); _state = 0; Init(); return *this; };

    // reshape
    void Reshape(int64 n1, int64 n2, int64 n3, int64 n4, int64 p1 = 0, int64 p2 = 0, int64 p3 = 0)
    {
        // init arguement
        if(p1 == 0) p1 = n1;
        if(p2 == 0) p2 = n2;
        if(p3 == 0) p3 = n3;

        // check size
        ASSERTA(p1*p2*p3*n4 <= _size, "[kmMat4::Reshape in 2280]");

        // reset members
        _n1 = n1; _n2 = n2;    _n3 = n3; _n4 = n4; _p1 = p1; _p2 = p2; _p3 = p3;
    };

    /////////////////////////////////////////////////
    // general member functions

    // get mat1
    kmMat1<T> Mat1(int64 idx)                    const { return kmMat1<T>(P(0,idx)     , _n1);};
    kmMat1<T> Mat1(int64 i2, int64 i3, int64 i4) const { return kmMat1<T>(P(0,i2,i3,i4), _n1);};

    // get mat2
    kmMat2<T> Mat2(int64 idx)          const { return kmMat2<T>(P(0,0,idx)  , _n1, _n2, _p1);};
    kmMat2<T> Mat2(int64 i3, int64 i4) const { return kmMat2<T>(P(0,0,i3,i4), _n1, _n2, _p1);};

    // get mat3
    kmMat3<T> Mat3(int64 idx) const
    {
        return kmMat3<T>(P(0,0,0,idx), _n1, _n2, _n3, _p1, _p2);
    };

    // get mat4
    kmMat4 Mat4(kmI i1, kmI i2, kmI i3, kmI i4) const
    {
        i1.e = MIN(i1.e, _n1-1);
        i2.e = MIN(i2.e, _n2-1);
        i3.e = MIN(i3.e, _n3-1);
        i4.e = MIN(i4.e, _n4-1);

        return kmMat4<T>(P(i1.s, i2.s, i3.s, i4.s), i1.Len(), i2.Len(), i3.Len(), i4.Len(), _p1, _p2, _p3);
    };

    // get info
    int64 N4() const { return _n4; };
    int64 P3() const { return _p3; };
    
    // get the number of real elements
    virtual int64 N() const { return _n1*_n2*_n3*_n4;};

    // get bytes of info members which is from _size to end
    virtual int64 GetInfoByte() const { return sizeof(*this) - 24;};
    
    // display member info
    virtual void PrintInfo(LPCSTR str = nullptr) const
    {
        if(str != nullptr) PRINTFA("[%s]\n", str);

        kmMat3<T>::PrintInfo();
        PRINTFA("  _p3    : %lld\n", _p3);
        PRINTFA("  _n4    : %lld\n", _n4);        
    };

    // display dimension
    virtual void PrintDim(LPCSTR str = nullptr) const
    {
        if(str != nullptr) PRINTFA("[%s]", str);
        else               PRINTFA("[%p]", _p);

        PRINTFA(" %lld, %lld, %lld, %lld (%lld) \n", _n1, _n2, _n3, _n4, _size);
    };

    // compare
    template<typename Y>
    bool IsEqualSizeDim(const kmMat4<Y>& b) const
    {
        return _n1 == b.N1() && _n2 == b.N2() && _n3 == b.N3() && _n4 == b.N4();
    }

    template<typename Y>
    bool IsEqualSizeDimP(const kmMat4<Y>& b) const
    {
        return _p1 == b.P1() && _p2 == b.P2() && _p3 == b.P3() && IsEqualSizeDim(b);
    }

    template<typename Y>
    bool IsEqualSizeAll(const kmMat4<Y>& b) const
    {
        return IsEqualSizeDimP(b) && IsEqualSize(b);
    }

    // get transpose matrix
    kmMat4 Tp() const
    {
        kmMat4 des(_n2, _n1, _n3, _n4);

        for(int64 i4 = 0; i4 < _n4; ++i4)
        for(int64 i3 = 0; i3 < _n3; ++i3)
        for(int64 i2 = 0; i2 < _n2; ++i2)
        for(int64 i1 = 0; i1 < _n1; ++i1)
        {
            des(i2, i1, i3, i4) = *P(i1, i2, i3, i4);
        }
        return des;    // * Note that des is the local variable,
                    // * so the return type cannot be reference type.
    };
};

// define type for kmMat
typedef kmArr <char>            kmArr0i8;
typedef kmMat1<char>            kmMat1i8;
typedef kmMat2<char>            kmMat2i8;
typedef kmMat3<char>            kmMat3i8;
typedef kmMat4<char>            kmMat4i8;

typedef kmArr <uchar>            kmArr0u8;
typedef kmMat1<uchar>            kmMat1u8;
typedef kmMat2<uchar>            kmMat2u8;
typedef kmMat3<uchar>            kmMat3u8;
typedef kmMat4<uchar>            kmMat4u8;

typedef kmArr <short>            kmArr0i16;
typedef kmMat1<short>            kmMat1i16;
typedef kmMat2<short>            kmMat2i16;
typedef kmMat3<short>            kmMat3i16;
typedef kmMat4<short>            kmMat4i16;

typedef kmArr <ushort>            kmArr0u16;
typedef kmMat1<ushort>            kmMat1u16;
typedef kmMat2<ushort>            kmMat2u16;
typedef kmMat3<ushort>            kmMat3u16;
typedef kmMat4<ushort>            kmMat4u16;

typedef kmArr <int>                kmArr0i32;
typedef kmMat1<int>                kmMat1i32;
typedef kmMat2<int>                kmMat2i32;
typedef kmMat3<int>                kmMat3i32;
typedef kmMat4<int>                kmMat4i32;

typedef kmArr <uint>            kmArr0u32;
typedef kmMat1<uint>            kmMat1u32;
typedef kmMat2<uint>            kmMat2u32;
typedef kmMat3<uint>            kmMat3u32;
typedef kmMat4<uint>            kmMat4u32;

typedef kmArr <int64>            kmArr0i64;
typedef kmMat1<int64>            kmMat1i64;
typedef kmMat2<int64>            kmMat2i64;
typedef kmMat3<int64>            kmMat3i64;
typedef kmMat4<int64>            kmMat4i64;

typedef kmArr <float>            kmArr0f32;
typedef kmMat1<float>            kmMat1f32;
typedef kmMat2<float>            kmMat2f32;
typedef kmMat3<float>            kmMat3f32;
typedef kmMat4<float>            kmMat4f32;

typedef kmArr <double>            kmArr0f64;
typedef kmMat1<double>            kmMat1f64;
typedef kmMat2<double>            kmMat2f64;
typedef kmMat3<double>            kmMat3f64;
typedef kmMat4<double>            kmMat4f64;

typedef kmArr <float>            kmArr0f32;
typedef kmMat1<float>            kmMat1f32;
typedef kmMat2<float>            kmMat2f32;
typedef kmMat3<float>            kmMat3f32;
typedef kmMat4<float>            kmMat4f32;

typedef kmArr <cmplxf32>        kmArr0c32;
typedef kmMat1<cmplxf32>        kmMat1c32;
typedef kmMat2<cmplxf32>        kmMat2c32;
typedef kmMat3<cmplxf32>        kmMat3c32;
typedef kmMat4<cmplxf32>        kmMat4c32;

typedef kmArr <f32xy>            kmArr0f32xy;
typedef kmMat1<f32xy>            kmMat1f32xy;
typedef kmMat2<f32xy>            kmMat2f32xy;
typedef kmMat3<f32xy>            kmMat3f32xy;
typedef kmMat4<f32xy>            kmMat4f32xy;

typedef kmArr <f32yz>            kmArr0f32yz;
typedef kmMat1<f32yz>            kmMat1f32yz;
typedef kmMat2<f32yz>            kmMat2f32yz;
typedef kmMat3<f32yz>            kmMat3f32yz;
typedef kmMat4<f32yz>            kmMat4f32yz;

typedef kmArr <f32zx>            kmArr0f32zx;
typedef kmMat1<f32zx>            kmMat1f32zx;
typedef kmMat2<f32zx>            kmMat2f32zx;
typedef kmMat3<f32zx>            kmMat3f32zx;
typedef kmMat4<f32zx>            kmMat4f32zx;

typedef kmArr <f32xyz>            kmArr0f32xyz;
typedef kmMat1<f32xyz>            kmMat1f32xyz;
typedef kmMat2<f32xyz>            kmMat2f32xyz;
typedef kmMat3<f32xyz>            kmMat3f32xyz;
typedef kmMat4<f32xyz>            kmMat4f32xyz;

///////////////////////////////////////////////////////////////
// memory block class for manual memory allocation
template<typename T> class kmMem : public kmMat1<T>
{
protected:
    // using for parents class
    using kmArr <T>::_p , kmArr <T>::_state, kmArr<T>::_size;
    using kmMat1<T>::_n1;

    using kmArr<T>::Byte;
    using kmArr<T>::GetKmClass;
    /////////////////////////////////////////////////
    // basic member functions
public:    
    virtual void Init() { kmMat1<T>::Init(); };

    // constructor
    kmMem() {};
    kmMem(      int64 byte) { Create(byte);};
    kmMem(T* p, int64 byte) { Set(p, byte);};

    // destructor
    virtual ~kmMem() {};
        
    // allocate memory
    void Create(int64 byte) { kmMat1<T>::Create(0, Byte2Size(byte)); };
    
    // release and create
    void Recreate(int64 byte) { kmMat1<T>::Release(); Create(byte); };

    // set array
    void Set(T* p, int64 byte) { kmMat1<T>::Set(p, 0, Byte2Size(byte)); };

    /////////////////////////////////////////////////
    // general member functions

    static int64 Size2Byte(int64 size) {return size*sizeof(T);};
    static int64 Byte2Size(int64 byte) {return (byte - 1)/(int64)sizeof(T) + 1;};

    int64 GetIdx      () const { return _n1; };
    int64 GetByteLeft () const { return (_size - GetIdx())*sizeof(T); };
    int64 GetByteUsed () const { return GetIdx()*sizeof(T); };
    float GetMbyteLeft() const { return GetByteLeft()/(1024.f*1024.f); };
    float GetMbyteUsed() const { return GetByteUsed()/(1024.f*1024.f); };

    void PrintMemState(LPCSTR str = nullptr)
    {
        if(str != nullptr) PRINTFA("* %s : ",str);
        else               PRINTFA("* kmMem : ");
        PRINTFA("%.1f / %lld MB\n", GetMbyteUsed(), Byte()>>20);
    };
    
    // get memory block
    void* GetMem(int64 byte)
    {
        // calc size
        int64 size = Byte2Size(byte); 

        // check size
        ASSERTA(_n1 + size <= _size, "[kmMem::GetMem in 451] over memory");

        // update index
        int64 idx = _n1; _n1 += size;
                
        return (void*) (_p + idx);
    };

    // reset allocated memory block
    void Reset(const int64 idx = 0) { _n1 = idx; };

    // clear allocated memory block
    void Clear() { memset(_p, 0, _size*sizeof(T)); };

    // allocate memory to kmArr and kmMat
    template<typename Y> void Give(kmArr <Y>& a, int64 n ) { a.Set((Y*)this->GetMem(n *sizeof(Y)), n );}
    template<typename Y> void Give(kmMat1<Y>& a, int64 n1) { a.Set((Y*)this->GetMem(n1*sizeof(Y)), n1);}

    // allocate memory to kmMat2
    template<typename Y> 
    void Give(kmMat2<Y>& a, int64 n1, int64 n2, int64 p1 = 0)
    {
        if(p1 == 0) p1 = n1;

        a.SetP((Y*)this->GetMem(p1*n2*sizeof(Y)), n1, n2, p1);
    }

    // allocate memory to kmMat3
    template<typename Y> 
    void Give(kmMat3<Y>& a, int64 n1, int64 n2, int64 n3, int64 p1 = 0, int64 p2 = 0)
    {
        if(p1 == 0) {p1 = n1;}; if(p2 == 0) {p2 = n2;};

        a.SetP((Y*)this->GetMem(p1*p2*n3*sizeof(Y)), n1, n2, n3, p1, p2);
    }

    // allocate memory to kmMat4
    template<typename Y> 
    void Give(kmMat4<Y>& a, int64 n1, int64 n2, int64 n3, int64 n4, int64 p1 = 0, int64 p2 = 0, int64 p3 = 0)
    {
        if(p1 == 0) {p1 = n1;}; if(p2 == 0) {p2 = n2;}; if(p3 == 0) {p3 = n3;};

        a.SetP((Y*)this->GetMem(p1*p2*p3*n4*sizeof(Y)), n1, n2, n3, n4, p1, p2, p3);
    }
};

typedef kmMem<char>    kmMem8;
typedef kmMem<short>   kmMem16;
typedef kmMem<int>     kmMem32;
typedef kmMem<int64>   kmMem64;

///////////////////////////////////////////////////////////////
// bit flag class

class kmBit
{
public:
    uchar* _ptr  = nullptr;
    uchar  _mask = 0;

    // constructor
    kmBit(uchar* ptr, int idx) { _ptr = ptr; SetIdx(idx); };

    // bit  1: set bit as 1
    // bit  0: set bit as 0
    // bit -1: set bit as ~bit
    kmBit& operator=(const int bit)
    {
        if     (bit >  0) *_ptr |=  _mask;
        else if(bit == 0) *_ptr &= ~_mask;
        else              *_ptr ^=  _mask;
        return *this;
    };

    // conversion operator... (uchar) a    
    operator uchar() const { return ((*_ptr)&_mask) ? 1:0; };

    // get bool
    int Get() const { return ((*_ptr)&_mask) ? 1:0; };
    
    // set index
    void SetIdx(int idx) { _mask = uchar(0b10000000)>>idx; };

    // print binary format
    void Print() { cout << "* " << (uint)*_ptr << " : " << bitset<8>(*_ptr) << endl; };    
};

class kmMat1bit: public kmMat1u8
{
public:
    int64 _bit_n = 0;

    // constructor
    kmMat1bit() {};
    kmMat1bit(int64 bit_n) { Create(bit_n); };

    kmMat1bit(const initializer_list<uchar>& val)
    {
        const int64 bit_n = val.size();
        _state =0; Create(bit_n);
        const uchar *pval = val.begin();
        for(int64 i = 0; i < bit_n; ++i) (*this)(i) = (int)*(pval++);
    };

    // destructor
    virtual ~kmMat1bit() {};

    // create matrix
    void Create(int64 bit_n) { _bit_n = bit_n; kmMat1u8::Create(u8_n(bit_n)); };

    // recreate matrix
    void Recreate(int64 bit_n) { Release(); Create(bit_n); };

    // set every bit as 1
    void SetOne() { SetVal(0b11111111); };

    // operator
    kmBit operator()(int64 bit_i) const
    {
        return kmBit(P(u8_i(bit_i)), bit_i & 0b111);
    };

    // get bit_n
    int64 BitN() const { return _bit_n; };

    // true  : if all bits are 1
    // false : if any bits are 0
    bool IsAll() const
    {
        const int64 n = _n1 - 1;

        for(int64 i = 0; i < n; ++i) if(a(i) != 0b11111111) return false;

        const int last_bit_n = int(_bit_n & 0b111);

        if(last_bit_n > 0)
        {
            const uchar mask = 0b11111111 << (8 - last_bit_n);
            if( (*End()&mask) != mask) return false;
        }
        return true;
    };

    // true  : if any bits are 1
    // false : if all bits are 0
    bool IsAny() const
    {
        const int64 n = _n1 - 1;

        for(int64 i = 0; i < n; ++i) if(a(i) != 0) return true;

        const int last_bit_n = int(_bit_n & 0b111);

        if(last_bit_n > 0)
        {
            const uchar mask = 0b11111111 << (8 - last_bit_n);
            if( (*End()&mask) != 0) return true;
        }
        return false;
    };

    // true  : if all bits are 0
    // false : if any bits are 1
    bool IsNone() const { return !IsAny(); };
        
    // print functions
    void PrintVal() const
    {
        cout << "* 0b "; 
        for(int64 i = 0; i < _n1; ++i) cout << bitset<8>(*P(i)) << " "; 
        cout << endl;
    }

protected:
    // inline inner functions
    inline int64 u8_n(int64 bit_n) const { return ((bit_n - 1)>>3) + 1; };
    inline int64 u8_i(int64 bit_i) const { return bit_i>>3;             };
};

///////////////////////////////////////////////////////////////
// block buffer class

// memory block buffer class
class kmMat1blk : public kmMat1i8
{
public:
    uint  _blk_byte = 0;
    uint  _blk_n    = 0;

    // constructor
    kmMat1blk() {};
    kmMat1blk(           int64 byte, uint blk_byte, uint blk_n) { Create(  byte, blk_byte, blk_n); };
    kmMat1blk(char* ptr, int64 byte, uint blk_byte, uint blk_n) { Set(ptr, byte, blk_byte, blk_n); };

    // create
    void Create(int64 byte, uint blk_byte, uint blk_n)
    {
        kmMat1i8::Create(byte); SetBlk(byte, blk_byte, blk_n);
    };

    // recreate
    void Recreate(int64 byte, uint blk_byte, uint blk_n)
    {
        kmMat1i8::Recreate(byte); SetBlk(byte, blk_byte, blk_n);
    };

    // set
    void Set(char* ptr, int64 byte, uint blk_byte, uint blk_n)
    {
        kmMat1i8::Set(ptr, byte); SetBlk(byte, blk_byte, blk_n);
    };

    // set block info
    void SetBlk(int64 byte, uint blk_byte, uint blk_n)
    {
        ASSERTA( byte > (int64)(blk_n - 1)*(int64)blk_byte, "[kmMat1Blk::SetBlk in 2935]");

        _blk_byte = blk_byte; _blk_n = blk_n; 
    };

    // get block ptr
    char* GetBlkPtr(uint iblk)
    {
        ASSERTA( iblk < _blk_n, "[kmMat1Blk::GetBlkPtr in 2937]");

        return P(_blk_byte*iblk); 
    };

    // get block byte
    uint GetBlkByte(uint iblk)
    {
        ASSERTA( iblk < _blk_n, "[kmMat1Blk::GetBlkByte in 2940]");

        return (iblk == _blk_n - 1)? uint(Byte() - _blk_byte*iblk) : _blk_byte;
    };
};

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
// class for string

template<typename T> class kmStr : public kmMat1<T>
{
protected:
    // using for members of parents class
    using kmArr <T>::_p , kmArr <T>::_state, kmArr<T>::_size;
    using kmMat1<T>::_n1;

public:
    // using for functions of parents class
    using kmMat1<T>::IsPinned;
    using kmMat1<T>::IsCreated;
    using kmMat1<T>::Create;
    using kmMat1<T>::Release;
    using kmMat1<T>::Init;
    using kmMat1<T>::a;
    using kmMat1<T>::P;
    using kmMat1<T>::Move;
    using kmArr <T>::RestoreVfptr;
    using kmArr <T>::GetKmClass;
    using kmArr <T>::SetZero;

public:
    // construtor
    kmStr() {};
    kmStr(int64 size) : kmMat1<T>(size) { SetZero(); };

    kmStr(const char* str, ...)
    {
        if(str == nullptr) 
        {
            Create(1); *_p = '\0'; return;
        }
        T buf[1024];
        GETVACHAR(buf, str);
        Create(GetStrLen(buf));
        Copy(buf);
    };

    kmStr(const wchar_t* str, ...)
    {
        if(str == nullptr)
        {
            Create(1); *_p = L'\0'; return;
        }
        T buf[1024];
        GETVAWCHAR(buf, str);
        Create(GetStrLen(buf));
        Copy(buf);
    };

    // destructor
    ~kmStr() {};

    // copy constructor
    kmStr(const kmStr& str)
    {
        Create(str.GetLen());
        Copy  (str._p);
    };

    // move constructor
    kmStr(kmStr&& str) noexcept { Move(str); };

    // assignment operator
    kmStr& operator=(const kmStr& str)
    {
        Recreate(str.GetLen());
        Copy    (str._p);
        return *this;
    };

    // move assignment operator
    kmStr& operator=(kmStr&& str) noexcept
    {
        if(IsPinned()) *this = str;
        else           Move(str); 
        return *this; 
    };

    // assignment operator for LPCTSTR
    kmStr& operator=(const T* str)
    {
        Recreate(GetStrLen(str));
        Copy    (str);
        return *this;
    };

    // recreate for str
    void Recreate(int64 size)
    {
        if(size > _size || size + 128 < _size) { Release(); Create(size); }
    };

    // restore the class including vfptr
    // * Note that this will clear and reset _p, _state, _size without release,
    // * which can cause a memory leak. So, you should use this very carefully.
    kmStr<T>& Restore() { RestoreVfptr(*this); _state = 0; Init(); return *this; };

    /////////////////////////////////////////////////
    // operator functions

    // operator... a == b
    bool operator==(const kmStr& b) const
    {
        if(b.GetLen() != GetLen()) return false;
        return Compare(b._p) == 0;
    };

    // operator... a != b
    bool operator!=(const kmStr& b) const
    {
        if(b.GetLen() != GetLen()) return true;
        return Compare(b._p) != 0;
    };

    // compare with b.. return 0 : the same, not 0 : different
    int Compare(const T* b)  const
    {
        if(typeid(T) == typeid(wchar_t)) {return wcscmp((wchar*)_p, (wchar*)b);}
                                          return strcmp(( char*)_p, ( char*)b);
    };

    // operator... a += b
    kmStr<T>& operator+=(const kmStr& b)
    {
        const int64 an = GetLen(), bn = b.GetLen();

        if(an + bn - 1 > _size) Expand(bn - 1);

        memcpy(P(an-1), b.P(), bn*sizeof(T)); 
        
        _n1 = an + bn - 1;

        return *this;
    };

    // operator... a += b
    kmStr<T>& operator+=(const T* b)
    {
        const int64 an = GetLen(), bn = GetStrLen(b);

        if(an + bn - 1 > _size) Expand(bn - 1);

        memcpy(P(an-1), b, bn*sizeof(T)); 

        _n1 = an + bn - 1;

        return *this;
    };

    // operator... c = a + b
    kmStr<T> operator+(const kmStr& b)
    {
        kmStr<T> c(*this); return c += b;
    };

    // operator... c = a + b
    kmStr<T> operator+(const T* b)
    {
        kmStr<T> c(*this); return c += b;
    };

    // expand for string
    void Expand(int64 size)
    {
        ASSERTA(IsCreated(), "[kmStr::Expand in 3143] memory is not created");
        ASSERTA(!IsPinned(), "[kmStr::Expand in 3144] memory is pinned");

        const int64 size_new = size + _size;

        T* p = new T[size_new];

        ASSERTA(_p != 0, "[kmstr::Expand in 3150] %p != 0", _p);
                
        CopyTo(p);        

        delete [] _p;

        _size  = size_new;
        _p     = p;
        _state = 1;
    };

    // conversion operator
    operator T*() const { return static_cast<T*>(_p); };
    
    /////////////////////////////////////////////////
    // member functions

    // copy str data... core
    void Copy(const T* str)
    {
        ASSERTA(_p != 0, "[kmStr::Copy in 1191]");
        
        const int64 size = GetStrLen(str);

        ASSERTA(size <= _size, "[kmStr::Copy in 646]");

        memcpy(_p, str, sizeof(T)*size);
    };

    // copy to... core
    void CopyTo(T* str) const
    {
        ASSERTA(_p != 0, "[kmStr::CopyTo in 2835]");
        
        memcpy(str, _p, sizeof(T)*GetLen());
    };

    // print string
    void Printf() const
    {
        if(IS_C16(T)) wcout<<_p<<endl;
        else          printf( "%s\n", _p);
    };

    // get string from keyboard
    void Scanf() const
    {
        if(IS_C16(T)) wscanf_s(L"%ls", _p, _size);
        else          scanf_s ( "%s" , _p, _size);
    };

    // set string
    void SetStr(const char* str, ...)
    {
        T buf[1024];
        GETVACHAR(buf, str);
        Recreate(GetStrLen(buf));
        Copy(buf);
    };
    
    void SetStr(const wchar_t* str, ...)
    {
        T buf[1024];
        GETVAWCHAR(buf, str);
        Recreate(GetStrLen(buf));
        Copy(buf);
    };

    void SetAxisFloat(float v, float vmin, float vmax)
    {
        if(fabs(v) < 1e-9f) { *this = L"0"; return; }

        int   vdec = (int)floor(log10(fabs(v))), vdec3 = 3*(vdec/3);
        float ddec = log10(fabs(vmax - vmin));

        if(vdec > 3)
        {
            if     (ddec - vdec3 < -1.5f) this->SetStr(L"%.3fe%d", v*pow(0.1f, vdec3), vdec3);
            else if(ddec - vdec3 < -0.5f) this->SetStr(L"%.2fe%d", v*pow(0.1f, vdec3), vdec3);
            else if(ddec - vdec3 <  0.5f) this->SetStr(L"%.1fe%d", v*pow(0.1f, vdec3), vdec3);
            else                          this->SetStr(L"%.0fe%d", v*pow(0.1f, vdec3), vdec3);
        }
        else if(vdec < -3)
        {
            vdec3 = 3*((vdec-2)/3);

            if     (ddec - vdec3 < -1.5f) this->SetStr(L"%.3fe%d", v*pow(0.1f, vdec3), vdec3);
            else if(ddec - vdec3 < -0.5f) this->SetStr(L"%.2fe%d", v*pow(0.1f, vdec3), vdec3);
            else if(ddec - vdec3 <  0.5f) this->SetStr(L"%.1fe%d", v*pow(0.1f, vdec3), vdec3);
            else                          this->SetStr(L"%.0fe%d", v*pow(0.1f, vdec3), vdec3);
        }
        else
        {    
            if     (ddec < -2.5f) this->SetStr(L"%.4f",v);
            else if(ddec < -1.5f) this->SetStr(L"%.3f",v);
            else if(ddec < -0.5f) this->SetStr(L"%.2f",v);
            else if(ddec <  0.5f) this->SetStr(L"%.1f",v);
            else                  this->SetStr(L"%.0f",v);
        }
    };

    void SetFloat3(const float val)
    {
        if(fabs(val) < 1e-9f) { *this = L"0"; return; }
        
        int dec = (int)floor(log10(fabs(val))), dec3 = 3*(dec/3);

        if(dec3 == 0)
        {
            if     (fabs(floor(val      + 1e-4f) - val      ) < 1e-4f) this->SetStr(L"%.0f", val);
            else if(fabs(floor(val*10.f + 1e-4f) - val*10.f ) < 1e-3f) this->SetStr(L"%.1f", val);
            else if(fabs(floor(val*100.f+ 1e-4f) - val*100.f) < 1e-2f) this->SetStr(L"%.2f", val);
            else                                                       this->SetStr(L"%.3f", val);
        }
        else
        {
            if(dec < -3) dec3 = 3*((dec-2)/3);

            this->SetStr(L"%.1fe%d", val*pow(0.1f, dec3), dec3);
        }
    };

    // set str... core
    void Set(const wchar_t* str)
    {
        ASSERTA(_state == 0, "[kmStr::Set in 779] %lld == 0", _state);

        _size = wcslen(str) + 1; _p = (wchar_t*)(void*)str;
    };
    void Set(const char* str)
    {
        ASSERTA(_state == 0, "[kmStr::Set in 787] %lld == 0", _state);

        _size = strlen(str) + 1; _p = (char*)(void*)str;
    };

    // get a part of string
    // * Note that this will give a new allocated object unlike Mat1().
    // * you should not use Mat1() since it doesn't guarantee null-terminating
    kmStr<T> Get(kmI i1) const
    {
        if(i1.e < 0) {i1.e += _n1;}; i1.e = MIN(i1.e, _n1-2);
        const int n = (int)i1.Len() + 1;

        kmStr<T> b(n);

        memcpy(b._p, _p + i1.s, sizeof(T)*(n - 1));

        b(-1) = T('\0');

        return b;
    };

    // cut back with number of text
    kmStr<T>& Cutback(int cut_n)
    {
        int64 n = GetLen();

        ASSERTA(cut_n < n, "[kmStr::Cutback in 3272]");

        (*this)(n - 1 - cut_n) = T('\0');

        _n1 = n - cut_n;

        return *this;
    };
    kmStr<T>& Cutback(int64 cut_n) { return Cutback(int(cut_n)); };

    // cut back (from 0 to ch's pos - 1) as finding char in reverse order
    kmStr<T>& CutbackRvrs(T ch)
    {
        const int idx = FindRvrs(ch); if(idx < 0) return *this;
    
         (*this)(idx) = T('\0'); _n1 = idx + 1;

        return *this;
    };

    // split as finding char in revers order
    //   return : rear part (from char'pos + 1 to end)     
    kmStr<T> SplitRvrs(T ch)
    {
        const int idx = FindRvrs(ch); if(idx < 0) return kmStr<T>();
        const int n   = (int)GetLen();

        kmStr<T> rear(n - idx - 1);

        memcpy(rear.P(), P(idx + 1), rear.N1()*sizeof(T));

        if(idx >= 0) { (*this)(idx) = T('\0'); _n1 = idx + 1; }

        return rear;
    };

    // split as finding char 
    //   return : rear part (from char'pos + 1 to end)     
    kmStr<T> Split(T ch)
    {
        const int idx = Find(ch); if(idx < 0) return kmStr<T>();
        const int n   = (int)GetLen();

        kmStr<T> rear(n - idx - 1);

        memcpy(rear.P(), P(idx + 1), rear.N1()*sizeof(T));

        if(idx >= 0) { (*this)(idx) = T('\0'); _n1 = idx + 1; }

        return rear;
    };

    // find and replace
    int Replace(T org, T rep) const
    {
        const int idx = Find(org); if(idx < 0) return 0;
        (*this)(idx) = rep;
        return 1;
    };

    // find and replace in reverse order
    int ReplaceRvrs(T org, T rep) const
    {
        const int idx = FindRvrs(org); if(idx < 0) return 0;
        (*this)(idx) = rep;
        return 1;
    };

    // get string length from const T*
    // * Note that the return of GetStrLen includes null-ternminating characters,
    // * while strlen and wsclen don't include it.
    static int64 GetStrLen(const char*    str) { if(str == nullptr) return 0; return strlen(str) + 1; };
    static int64 GetStrLen(const wchar_t* str) { if(str == nullptr) return 0; return wcslen(str) + 1; };

    // get string length including null-terminating character
    int64 GetLen() const { return GetStrLen(_p); };

    // pack string... change n1 as getlen()
    kmStr<T>& Pack() { _n1 = GetLen(); return *this; };

    // conversion function
    int ToInt()
    {
        if(IS_C16(T)) return (int)wcstol((const wchar*) _p, 0, 10);
        else          return  atoi((const  char  *) _p);
    };

    float ToFloat()
    {
        if(IS_C16(T))
        {
            char tmp[256] = {0,};
            wcstombs(tmp, (const wchar_t*)_p, wcslen((const wchar_t*)_p));
            return (float) atof((const char *) tmp);
        }
        else          return (float) atof((const  char  *) _p);
    };

    // find the character from istart (0 <= : position, -1 : not found)
    int Find(T ch, int istart = 0) const
    {
        int n = (int)GetLen() - 1, i = istart;
        for(; i < n; ++i) if(a(i) == ch) break;

        return (i < n) ? i:-1;
    };

    // find the character in reverse order    (0 <= : position, -1 : not found)
    int FindRvrs(T ch, int istart = INT_MAX - 1) const
    {
        int n = (int)GetLen() - 1, i = MIN(n, istart + 1);
        for(; i--;) if(a(i) == ch) break;

        return i;
    };

    // find the first alphabet from istart
    int FindAlpha(int istart = 0) const
    {
        int n = (int)GetLen() - 1, i = istart;
        for(; i < n; ++i) if(iswalpha(a(i))) break;

        return (i < n) ? i:-1;
    };

    // find the first alphabet or number from istart
    int FindAlnum(int istart = 0) const
    {
        int n = (int)GetLen() - 1, i = istart;
        for(; i < n; ++i) if(iswalnum(a(i))) break;

        return (i < n) ? i:-1;
    };

    // find the first non-character from istart
    int FindNonAlpha(int istart = 0) const
    {
        int n = (int)GetLen() - 1, i = istart;
        if(!iswalpha(a(i++))) return -1;
        for(; i < n; ++i) if(!iswalpha(a(i))) break;

        return i;
    };

    // find the first non-alphabet and non-number from istart
    int FindNonAlnum(int istart = 0)
    {
        int n = (int)GetLen() - 1, i = istart;
        if(!iswalnum(a(i++))) return -1;
        for(; i < n; ++i) if(!iswalnum(a(i))) break;

        return i;
    };
};
typedef kmStr<char>             kmStra;
typedef kmStr<wchar>            kmStrw;
typedef kmMat1<kmStra>          kmStras;
typedef kmMat1<kmStrw>          kmStrws;

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
// class for queue

// queue class
template<typename T> class kmQue1 : public kmArr<T>
{
protected:
    // using for members of parents class
    using kmArr <T>::_p , kmArr <T>::_state, kmArr<T>::_size;    

    // member variables
    int64 _n1 = 0; // number of dim1
    int64 _s1 = 0; // start index of dim1

public:
    // using for functions of parents class    
    using kmArr<T>::Release;
    using kmArr<T>::IsPinned;
    using kmArr<T>::IsCreated;
    using kmArr<T>::GetKmClass;
    using kmArr<T>::SetVal;
    using kmArr<T>::Copy;
    using kmArr<T>::RestoreVfptr;
    using kmArr<T>::Create;
    using kmArr<T>::Recreate;

    /////////////////////////////////////////////////
    // basic member functions
public:
    virtual void Init() { kmArr<T>::Init(); _n1 = _s1 = 0; };

    // constructor
    kmQue1() {};    
    kmQue1(int64 size) { Create(size); };

    // destructor
    virtual ~kmQue1() {};

    // copy constructor    
    kmQue1(const kmQue1& b) { Create(b.N1()); Copy(b.Begin()); };

    // assignment operator
    kmQue1& operator=(const kmQue1& b) { RecreateIf(b); Copy(b._p); return *this; };
    
    // operator to get data    
    T* P(int64 i1) const
    {
        if(i1 < 0) i1 += _n1; // * Note that i1 = -1 is the same as End()

        ASSERTA(0 <= i1 && i1 < _n1 , "[kmQue1::P in 3405] %lld < %lld", i1, _n1);

        return _p + _s1 + i1;
    };

    T* P() const { return _p; };

    T& operator()(int64 i1 = 0) const { return *P(i1); };
    T& a         (int64 i1 = 0) const { return *P(i1); };
    virtual T& v (int64 i1 = 0) const { return *P(i1); };

    // operator to get data
    T* End () const { return _p + _s1 + _n1 - 1; };
    T* End1() const { return _p + _s1 + _n1;     };

    // restore the class including vfptr
    // * Note that this will clear and reset _p, _state, _size without release,
    // * which can cause a memory leak. So, you should use this very carefully.
    kmQue1<T>& Restore() { RestoreVfptr(*this); _state = 0; Init(); return *this; };

    // expand memory    
    void Expand(int64 size)
    {
        const int64 size_new = _size + size - _s1;

        T* p = _p;

        if(size - _s1 > 0)
        {
            p = new T[size_new];
            ASSERTA(_p != 0, "[kmQue1::Expand in 3432] %p != 0", _p);
        }

        // * Note that "delete [] _p" will call the destructor of every elements.
        // * If T is kmat, it will release the memories of elements.
        // * To avoid this, std::move() and the move assignment operator have been used
        // * instead of CopyTo(p).
        // * std:move(A) means that A is rvalue.
        for(int64 i = 0; i < _n1; ++i) { *(p + i) = std::move(*(_p + _s1 + i)); }

        if(size - _s1 > 0) delete [] _p;

        _size  = size_new;
        _p     = p;
        _s1    = 0;
        _state = 1;
    };

    /////////////////////////////////////////////////
    // general member functions

    // get info
    int64 N1() const { return _n1; };

    // get the number of real elements
    virtual int64 N() const { return _n1; };

    // enqueue
    int64 Enqueue()
    {
        if(_s1 + _n1 == _size) Expand(16);

        return ++_n1 - 1;
    }
    int64 Enqueue(const T& val)
    {
        Enqueue();  *End() = val;  return _n1 - 1;
    };

    // Dequeue
    T* Dequeue()
    {
        if     (_n1 == 0) return nullptr;
        else if(_n1 == 1) { T* pval = _p + _s1; _s1 = _n1 = 0; return pval; }
        
        --_n1; return _p + _s1++;
    };
};

// main queue class ... this can add elements which have different sizes.
// * Note that kmQue is designed for a struct not using deep copy.
// * So, you should be careful when using a class like a kmMat.
template<typename T> class kmQue
{
public:
    kmMat1<T*> _pobj; // pointer queue
    kmMem8     _obj;  // object queue (memory storage)

    /////////////////////////////////////////////////
    // basic member functions
public:    
    virtual void Init() { _pobj.Init(); _obj.Init(); };

    // constructor
    kmQue() {};    
    kmQue(int64 size, int64 obj_byte) { Create(size, obj_byte); };

    // destructor
    virtual ~kmQue() {};

    // copy constructor    
    //kmQue(const kmQue& b) {};

    // move constructor
    //kmQue(kmQue&& b) {};

    // assignment operator
    //kmQue& operator=(const kmQue& b) { return *this; };

    // move assignment operator
    //kmQue& operator=(kmQue&& b) { return *this; };

    // allocate memory
    void Create(int64 size, int64 obj_byte)
    {
        _pobj.Create(0, size);
        _obj .Create(obj_byte);
    };

    // release memory
    void Release()
    {
        _pobj.Release();
        _obj .Release();
    };

    /////////////////////////////////////////////////
    // general functions

    // push back
    template<class Y> int64 PushBack(const Y& obj)
    {
        // get memory from _obj
        Y* pobj = (Y*)_obj.GetMem(sizeof(Y));

        // copy object to _obj
        memcpy((void*) pobj, &obj, sizeof(Y));

        // pushback pointer of stored object
        return _pobj.PushBack((T*) pobj);
    }

    // push back
    template<class Y> int64 PushBack(const Y* obj, const int64 byte)
    {
        // get memory from _obj
        Y* pobj = (Y*)_obj.GetMem(byte);

        // copy object to _obj
        memcpy((void*) pobj, obj, byte);

        // pushback pointer of stored object
        return _pobj.PushBack((T*) pobj);
    }

    // pop back
    T* PopBack()
    {
        _obj.Reset(_obj.GetIdx() - Byte(_pobj.N()-1));

        return *_pobj.PopBack();
    };

    // get size of obj(idx)
    int64 Byte(int64 idx) const
    {
        int64 n = _pobj.N(), byte = 0;

        if(idx < n-1)
        {
            byte = (int64) _pobj(idx + 1) - (int64) _pobj(idx);
        }
        else if(idx == n-1)
        {
            byte = (int64) _obj.P(_obj.GetIdx()) - (int64) _pobj(idx);
        }
        return byte;
    };

    // get number of objects
    int64 N() const { return _pobj.N(); };

    // get object
    T* Begin() const { return *_pobj.Begin();};
    T* End  () const { return *_pobj.End();  };

    // print info of que
    void PrintInfo(LPCSTR str = nullptr) const
    {
        if(str != nullptr) PRINTFA("[%s]\n", str);
                
        _pobj.PrintInfo("_pobj");

        for(int64 i = 0; i < N(); ++i)
        {
            PRINTFA("  [%d] %lld, %d byte\n", i, _pobj(i), Byte(i));
        }
        PRINTFA("\n");

        _obj.PrintInfo("_obj");
    };

    /////////////////////////////////////////////////
    // operator

    T* operator()(int64 i) const { return _pobj(i); };
};

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
// class for image

// rgb class : 4byte data type (0xrrggbbaa), compatible to COLORREF
// * Note that Intel's byte order is little endian.
// * So, the byte order (or address order) is 0xrrggbbaa, 
// * but the word order is 0xaabbggrr.
class kmRgb
{
public:        
    uchar _r, _g, _b, _a;

    // constructor
    kmRgb()                                   { Set(0, 0, 0, 0);};    
    kmRgb(const int rgb)                      { Set(rgb);       };
    kmRgb(uchar r, uchar g, uchar b)          { Set(r, g, b);   };
    kmRgb(uchar r, uchar g, uchar b, uchar a) { Set(r, g, b, a);};

    // input method
    void Set(const int rgb) { *((int*)this) = rgb; };
    void Set(uchar r, uchar g, uchar b, uchar a = 0)
    {
        _b = b; _g = g; _r = r; _a = a;
    };

    // conversion operator... (COLORREF) a    
    operator COLORREF() const { return *((COLORREF*) this); };
        
    // operator
    kmRgb& operator+=(const kmRgb& b) { _b += b._b; _g += b._g; _r += b._r; _a += b._a; return *this; };
    kmRgb& operator-=(const kmRgb& b) { _b -= b._b; _g -= b._g; _r -= b._r; _a -= b._a; return *this; };
    kmRgb  operator+ (const kmRgb& b) { return kmRgb(_r + b._r, _g + b._g, _b + b._b, _a + b._a);};
    kmRgb  operator- (const kmRgb& b) { return kmRgb(_r - b._r, _g - b._g, _b - b._b, _a - b._a);};
    
    // member fucntions
    COLORREF GetBgr() const
    {
        return (COLORREF) kmRgb(_b, _g, _r, _a);
    };

    // vary color
    kmRgb Vary(uchar dr, uchar dg, uchar db)
    {
        kmRgb rgb = *this;

        rgb._r += (_r < 128) ? dr:-dr;
        rgb._g += (_g < 128) ? dg:-dg;
        rgb._b += (_b < 128) ? db:-db;

        return rgb;
    };
};

#define kmRgbR kmRgb(255,  0,  0)
#define kmRgbG kmRgb(  0,255,  0)
#define kmRgbB kmRgb(  0,  0,255)
#define kmRgbY kmRgb(255,255,  0)
#define kmRgbM kmRgb(255,  0,255)
#define kmRgbC kmRgb(  0,255,255)
#define kmRgbW kmRgb(255,255,255)
#define kmRgbK kmRgb(  0,  0,  0)

// bgr class : 4byte data type (0xbbggrraa), compatible to Bitmap file
class kmBgr
{
public:
    uchar _b, _g, _r, _a;

    // constructor
    kmBgr()                                   { Set(0, 0, 0, 0);};
    kmBgr(const int bgr)                      { Set(bgr);       };
    kmBgr(uchar b, uchar g, uchar r)          { Set(b, g, r);   };
    kmBgr(uchar b, uchar g, uchar r, uchar a) { Set(b, g, r, a);};

    // input method
    void Set(const int   bgr) { *((int*)this) = bgr; };
    void Set(const kmRgb rgb) { Set(rgb._b, rgb._g, rgb._b, rgb._a); };
    void Set(uchar b, uchar g, uchar r, uchar a = 0)
    {
        _b = b; _g = g; _r = r; _a = a;
    };

    // conversion operator... (kmRgb) a
    operator kmRgb() const { return kmRgb(_r, _g, _b, _a); };

    // conversion operator... (COLORREF) a    
    operator COLORREF() const { return (COLORREF)kmRgb(_r,_g,_b,_a); };

    // operator
    kmBgr& operator+=(const kmBgr& b) { _b += b._b; _g += b._g; _r += b._r; _a += b._a; return *this; };

    // assigne operator
    kmBgr& operator=(const kmRgb& rgb)
    {
        Set(rgb);        
        return *this;
    };
};

// colormap class
// * Note that the type of kmCMpa is kmBgr instead of kmRgb, 
// * since a bitmap is compatible to kmBgr not kmRgb
class kmCMap : public kmMat1<kmBgr>
{
public:
    // constructor
    kmCMap() {};
    kmCMap(kmeCMapType cmtype) { Create(cmtype); };

    // member functions
    virtual void Create(int64 n) { kmMat1<kmBgr>::Create(n);};
    
    void Create(kmeCMapType cmtype)
    {
        if(_size != 256) Recreate(256);

        switch(cmtype)
        {
        case CMAP_JET : ////////////////// jet colormap

            for(int i=0; i<32; ++i)
            {                
                P(i    )->Set( 128+i*4, 0,       0);
                P(i+224)->Set(       0, 0, 252-i*4);
            }
            for(int i=0; i<64; ++i)
            {
                P(i+ 32)->Set(       255,     i*4,   0);
                P(i+ 96)->Set( 252-i*4,     255, i*4);
                P(i+160)->Set(         0, 252-i*4, 255);
            }            
            break;

        case CMAP_GREY : ////////////////// grey colormap

            for(int i=0; i<256; ++i)
            {
                P(i)->Set(i,i,i);
            }
            break;
        }
    };    
};

// image class for RGB
// * Note that the type of kmImg is kmBgr instead of kmRgb, 
// * since a bitmap is compatible to kmBgr not kmRgb
class kmImg : public kmMat3<kmBgr>
{
    /////////////////////////////////////////////////
    // basic member functions
public:

    // constructor
    kmImg() {};
    kmImg(          int64 n1, int64 n2, int64 n3 = 1)                             : kmMat3(   n1, n2, n3)               {};
    kmImg(kmBgr* p, int64 n1, int64 n2, int64 n3 = 1)                             : kmMat3(p, n1, n2, n3)               {};
    kmImg(          int64 n1, int64 n2, int64 n3, int64 p1)                       : kmMat3(   n1, n2, n3, p1)           {};
    kmImg(kmBgr* p, int64 n1, int64 n2, int64 n3, int64 p1)                       : kmMat3(p, n1, n2, n3, p1)           {};
    kmImg(          int64 n1, int64 n2, int64 n3, int64 p1, int64 p2)             : kmMat3(   n1, n2, n3, p1, p2)       {};
    kmImg(kmBgr* p, int64 n1, int64 n2, int64 n3, int64 p1, int64 p2)             : kmMat3(p, n1, n2, n3, p1, p2)       {};
    kmImg(          int64 n1, int64 n2, int64 n3, int64 p1, int64 p2, int64 size) : kmMat3(   n1, n2, n3, p1, p2, size) {};
    kmImg(kmBgr* p, int64 n1, int64 n2, int64 n3, int64 p1, int64 p2, int64 size) : kmMat3(p, n1, n2, n3, p1, p2, size) {};

    /////////////////////////////////////////////////
    // general member functions

    // Convert to BGR image... b (matrix) --> a (BGR image)
    template<typename T> 
    void ConvertBgr(const kmMat3<T>& b, T min_v, T max_v, const kmCMap& cmap = kmCMap(CMAP_JET))
    {        
        // check size        
        ASSERTA(_n1 == b.N1() && _n2 == b.N2(), "[kmImg::ConvertBGR in 1231]");
        ASSERTA(max_v >= min_v                , "[kmImg::ConvertBGR in 1232]");

        // assign colors to elements
        const float coff = 255.f/float(max_v - min_v);

        for(int i3 = 0; i3 < _n3; ++i3)
        for(int i2 = 0; i2 < _n2; ++i2)
        for(int i1 = 0; i1 < _n1; ++i1)
        {
            T val = MIN(MAX(min_v, b(i1, i2, i3)), max_v);

            int idx = int(float(val - min_v)*coff);

            *P(i1, i2, i3) = cmap(MIN(MAX(0, idx),255));
        }
    }

    template<typename T>
    void ConvertBgr(const kmMat2<T>& b, T min_v, T max_v, const kmCMap& cmap = kmCMap(CMAP_JET))
    {
        ConvertBgr(kmMat3<T>(b), min_v, max_v, cmap);
    }    

    // get frame
    kmImg GetFrame(const int64 idx) const 
    {
        kmImg img(P(0,0,idx), _n1, _n2, 1, _p1);

        return img;
    };

    // get info
    int GetW() const { return (int)_n1; };
    int GetH() const { return (int)_n2; };

    int64 GetByteFrame() const { return _p1*_n2*sizeof(kmBgr); };

    /////////////////////////////////////////////////
    // member functions for DIB

    BITMAPFILEHEADER GetBmpFileHeader() const
    {
        const int64 size_fh = sizeof(BITMAPFILEHEADER);
        const int64 size_ih = sizeof(BITMAPINFOHEADER);

        BITMAPFILEHEADER fh = {0,};

        fh.bfType    = 0x4d42;
        fh.bfSize    = size_fh;
        fh.bfOffBits = size_fh + size_ih;

        return fh;
    };

    BITMAPINFOHEADER GetBmpInfoHeader() const
    {
        BITMAPINFOHEADER ih = {0,};

        ih.biSize          = sizeof(BITMAPINFOHEADER);
        ih.biWidth         =    (LONG) _n1;
        ih.biHeight        = -1*(LONG) _n2; // +bottom-up, -up-down
        ih.biPlanes        = 1;
        ih.biBitCount      = 32; 
        ih.biCompression   = BI_RGB;
        ih.biSizeImage     = 0; 
        ih.biXPelsPerMeter = 0;
        ih.biYPelsPerMeter = 0;
        ih.biClrUsed       = 0;
        ih.biClrImportant  = 0;

        return ih;
    };

    BITMAPINFO GetBmpInfo() const
    {
        BITMAPINFO bi = {0,};
        
        bi.bmiHeader = GetBmpInfoHeader();

        return bi;
    };
};

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
// class of time 
// date class... 8byte
class kmDate
{
protected:
    ushort _year{};
    uchar  _mon{}, _date{}, _hour{}, _min{}, _sec{}, _day{};

public:
    kmDate() {};
    kmDate(time_t time) { Set(time); };

    kmDate& Set(time_t time)
    {
        tm t; localtime_r(&time, &t);

        _year = t.tm_year + 1900; _mon = t.tm_mon + 1; _date = t.tm_mday;
        _hour = t.tm_hour;        _min = t.tm_min;      _sec = t.tm_sec;
        _day  = t.tm_wday;

        return *this;
    };
    kmDate& Set(kmStra& str) // yyyy:mm:dd hh:mm:ss
    {
        _year = str.ToInt(); 
        _mon  = str.Get(kmI( 5, 6)).ToInt();
        _date = str.Get(kmI( 8, 9)).ToInt();
        _hour = str.Get(kmI(11,12)).ToInt();
        _min  = str.Get(kmI(14,15)).ToInt();
        _sec  = str.Get(kmI(17,18)).ToInt();
        
        return *this;
    };
    kmDate& SetCur() { return Set(time(NULL)); };

    bool operator==(const kmDate& b) const { return *(int64*)this == *(int64*)&b; };
    bool operator!=(const kmDate& b) const { return *(int64*)this != *(int64*)&b; };

    kmStrw GetStrw() const
    {
        return kmStrw(L"%04d%02d%02d%02d%02d%02d",_year, _mon, _date, _hour, _min, _sec); 
    };
    kmStra GetStr() const
    {
        return kmStra("%04d%02d%02d%02d%02d%02d",_year, _mon, _date, _hour, _min, _sec);
    };
    kmStrw GetStrwPt() const
    {
        return kmStrw(L"%04d-%02d-%02d %02d:%02d:%02d",_year, _mon, _date, _hour, _min, _sec); 
    };
    kmStra GetStrPt() const
    {
        return kmStra("%04d-%02d-%02d %02d:%02d:%02d",_year, _mon, _date, _hour, _min, _sec);
    };
    int64 GetInt() const
    {
        //int64 date = _sec + _min*100 + _hour*10000 + _date*1000000;
        //date += _mon *int64(100000000);
        //date += _year*int64(10000000000);
        struct tm t;
        t.tm_year = _year - 1900;
        t.tm_mon = _mon - 1;
        t.tm_mday = _date;
        t.tm_hour = _hour;
        t.tm_min = _min;
        t.tm_sec = _sec;
        t.tm_wday = _day;

        return mktime(&t);
    };
    kmStrw GetDay() const
    {
        kmStrw str;
        switch(_day)
        {
        case 0: str.SetStr(L"Sun"); break; case 1: str.SetStr(L"Mon"); break;
        case 2: str.SetStr(L"Tue"); break; case 3: str.SetStr(L"Wed"); break;
        case 4: str.SetStr(L"Thu"); break; case 5: str.SetStr(L"Fri"); break;
        case 6: str.SetStr(L"Sat"); break;
        }
        return str;
    };
};

// gps class... 24 byte
class kmGps
{
public:
    double _lat_deg = 0;     // latitude  (deg)
    double _lng_deg = 0;     // longitude (deg)
    float  _alt_m   = 0;     // altitude  (m)
    char   _lat_ref = 'N';   // latitude ref 'N' or 'S'
    char   _lng_ref = 'E';   // latitude ref 'E' or 'W'

    bool operator==(const kmGps& b) const
    {
        return _lat_deg == b._lat_deg && _lng_deg == b._lng_deg && _alt_m == b._alt_m &&
               _lat_ref == b._lat_ref && _lng_ref == b._lng_ref; 
    };
    bool operator!=(const kmGps& b) const
    {
        return _lat_deg != b._lat_deg || _lng_deg != b._lng_deg || _alt_m != b._alt_m ||
               _lat_ref != b._lat_ref || _lng_ref != b._lng_ref;
    };

    void Print()
    {
        wcout<<L"* gps : "<<GetStrw().P()<<endl;
    };

    kmStrw GetStrw()
    {
        return kmStrw(L"%7.3f%c %7.3f%c %3.0fm",
                      _lat_deg, _lat_ref, _lng_deg, _lng_ref, _alt_m);
    };
};


class kmTimer
{
protected:
    int   _state   = 0; // 0: not started, 1: started, 2: paused
    int64 _s_cnt   = 0; // start count
    int64 _p_cnt   = 0; // pause count

public:
    // constructor
    kmTimer() {};

    kmTimer(const int state)
    {
        _state = MIN(MAX(0,state),2);
        if(_state > 0) _s_cnt = _p_cnt = GetCnt();
    };

    // update count
    static int64 GetFreq() { return 1e6; };
    static int64 GetCnt ()
    {
        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        return (now.tv_sec*1e6)+(now.tv_nsec/1e3);
    };

    static double GetTime_sec (int64 del_cnt) { return (double)del_cnt/GetFreq();     };
    static double GetTime_msec(int64 del_cnt) { return (double)del_cnt/GetFreq()*1e3; };
    static double GetTime_usec(int64 del_cnt) { return (double)del_cnt/GetFreq()*1e6; };

    //////////////////////////////////////
    // member fucntions
    inline int GetState    () { return  _state;       };
    inline int IsNotStarted() { return (_state == 0); };
    inline int IsStarted   () { return (_state == 1); };
    inline int IsPaused    () { return (_state == 2); };

    void Start() {                   _s_cnt = GetCnt(); _state = 1;  };
    void Stop () { if(_state == 1)   _p_cnt = GetCnt(); _state = 0;  };
    void Pause() { if(_state == 1) { _p_cnt = GetCnt(); _state = 2; }};

    void Continue()
    {
        if     (_state == 0) Start();
        else if(_state == 2) // paused
        {
            // adjust _s_cnt to remove holding time.
            _s_cnt += (GetCnt() - _p_cnt);

            // set state
            _state = 1;
        }
    };

    // if state is 0, it will return zero.
    // otherwise, it will return the time from starting to now except holding time.
    double GetTime()
    {    
        // calc time_cnt
        int64 time_cnt = 0;

        switch(_state)
        {
        case 1: time_cnt = GetCnt() - _s_cnt; break;
        case 2: time_cnt = _p_cnt   - _s_cnt; break;
        }

        // calc time_msec
        return GetTime_msec(time_cnt);
    };
    inline double GetTime_sec()  { return GetTime()*1e-3;};
    inline double GetTime_msec() { return GetTime();     };
    inline double GetTime_usec() { return GetTime()*1e3; };

    inline double sec()  { return GetTime()*1e-3;};
    inline double msec() { return GetTime();     };
    inline double usec() { return GetTime()*1e3; };

    void Printf(const char* str, ...)
    {
        char buf[256];
        va_list args; // variable argument list

        va_start  (args, str);
        vsprintf  (buf, str, args);
        va_end    (args);

        PRINTFA("* %s : %.3f msec\n", buf, GetTime());
    };

    // wait for t_usec with Sleep(sleep_msec)
    void Wait(float t_usec, uint sleep_msec = 0)
    {
        Start(); while(GetTime_usec() < t_usec) std::this_thread::sleep_for(std::chrono::milliseconds(sleep_msec));
    };
};

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
// class for file control

class kmFile
{
protected:
    FILE* _file;

    /////////////////////////////////////////////////
    // basic member functions
public:
    // constructor
    kmFile() { _file = 0; };
    kmFile(const char   * name, kmeFile mode = KF_READ) { _file = 0; Open(name, mode); };
    kmFile(const wchar_t* name, kmeFile mode = KF_READ) { _file = 0; Open(name, mode); };

    // destructor
    ~kmFile() { Close(); };

    // get class name
    LPCSTR GetKmClass() const {return "kmFile";};

    //////////////////////////////////////////////////
    // general member functions

    // open file
    void Open(const char* name, kmeFile mode = KF_READ)
    {
        ASSERTA(!IsOpen(), "[kmFile::Open in 1830]");
        
        // get fmode
        const char* fmode = NULL;

        switch(mode)
        {
        case KF_READ       : fmode = "rb" ; break;
        case KF_NEW        : fmode = "wb+"; break;
        case KF_ADD        : fmode = "ab+"; break;
        case KF_MODIFY     : fmode = "rb+"; break;
        case KF_READ_TXT   : fmode = "r" ;  break;
        case KF_NEW_TXT    : fmode = "w+";  break;
        case KF_ADD_TXT    : fmode = "a+";  break;
        case KF_MODIFY_TXT : fmode = "r+";  break;
        default            : fmode = "rb" ; break;
        }

        _file = fopen(name, fmode);

        if(_file == nullptr)
        {
            throw KE_CANNOT_OPEN;
        }
    };

    // open file for wide character version
    void Open(const wchar* name, kmeFile mode = KF_READ)
    {
        ASSERTA(!IsOpen(), "[kmFile::Open in 1830]");
        
        // get fmode
        const char* fmode = nullptr;

        switch(mode)
        {
        case KF_READ   : fmode = "rb" ; break;
        case KF_NEW    : fmode = "wb+"; break;
        case KF_ADD    : fmode = "ab+"; break;
        case KF_MODIFY : fmode = "rb+"; break;
        default:         fmode = "rb" ; break;
        }

        char cname[MAX_FILE_NAME] = {0,};
        wcstombs(cname, name, wcslen(name)*2);

        _file = fopen(cname, fmode);

        if(_file == nullptr)
        {
            throw KE_CANNOT_OPEN;
        }
    };

    // close file
    void Close() { if(IsOpen()) { fclose(_file); _file = 0; } };

    // check file
    int IsOpen() { return (_file == 0)? 0:1; };

    // get file size byte
    int64 GetByte()
    {
        const int64 cur_pos = GetPos(), byte = SeekEnd().GetPos(); Seek(cur_pos);        
        return byte;
    };

    // -D_FILE_OFFSET_BITS=64  lseek
    kmFile& Seek   (int64 offset = 0) { fseek(_file, offset, SEEK_SET); return *this; };
    kmFile& SeekEnd(int64 offset = 0) { fseek(_file, offset, SEEK_END); return *this; };
    kmFile& SeekCur(int64 offset = 0) { fseek(_file, offset, SEEK_CUR); return *this; };

    // get file position
    // -D_FILE_OFFSET_BITS=64  lseek
    int64 GetPos() const { return ftell(_file); };

    //////////////////////////////////////////////////
    // static file functions

    static int Exist(const char*  name, bool show_err = false)
    {
        errno_t err = access(name, 0);
        if(show_err) switch(err)
        {
        case EACCES: PRINTFA("[kmFile::Exist] access denied\n");               break;
        case ENOENT: PRINTFA("[kmFile::Exist] file name or path not found\n"); break;
        case EINVAL: PRINTFA("[kmFile::Exist] invalide parameter\n");          break;
        }
        return (err == 0)? 1:0;
    };
    static int Exist(const wchar* name, bool show_err = false)
    {
        char cname[100] = {0,};
        wcstombs(cname, name, wcslen(name)*2);
        errno_t err = access(cname, 0);
        if(show_err) switch(err)
        {
        case EACCES: PRINTFA("[kmFile::Exist] access denied\n");               break;
        case ENOENT: PRINTFA("[kmFile::Exist] file name or path not found\n"); break;
        case EINVAL: PRINTFA("[kmFile::Exist] invalide parameter\n");          break;
        }
        return (err == 0)? 1:0;
    };

    static int Remove(const wchar* name) 
    {
        char cname[100] = {0,};
        wcstombs(cname, name, wcslen(name)*2);
        return remove(cname);
    }
    static int Remove(const  char* name) { return   remove(name); };

    static int Rename(const wchar* cur_name, const wchar* new_name) 
    {
        char cur_cname[MAX_FILE_NAME] = {0,};
        wcstombs(cur_cname, cur_name, wcslen(cur_name)*2);
        char new_cname[MAX_FILE_NAME] = {0,};
        wcstombs(new_cname, new_name, wcslen(new_name)*2);

        return rename(cur_cname, new_cname);
    }
    static int Rename(const  char* cur_name, const  char* new_name) { return   rename(cur_name, new_name); };

/*
        S_IRUSR : (00400) - owner에 대한 읽기 권한
        S_IWUSR : (00200) - owner에 대한 쓰기 권한
        S_IXUSR : (00100) - owner에 대한 search 권한
        S_IRGRP : (00040) - Group에 대한 읽기 권한
        S_IWGRP : (00020) - Group에 대한 쓰기 권한
        S_IXGRP : (00010) - Group에 대한 search 권한
        S_IROTH : (00004) - Other에 대한 읽기 권한
        S_IWOTH : (00002) - Other에 대한 쓰기 권한
        S_IXOTH : (00001) - Other에 대한 search 권한
*/
    static int MakeDir(const wchar* name)
    {
        char cname[MAX_FILE_NAME] = {0,};
        wcstombs(cname, name, wcslen(name)*2);
        return mkdir(cname, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH | S_IXOTH);
    };
    static int MakeDir(const  char* name) { return mkdir (name, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH | S_IXOTH); };

    static int RemoveDir(const wchar* name)
    {
        char cname[MAX_FILE_NAME] = {0,};
        wcstombs(cname, name, wcslen(name)*2);
        return rmdir(cname);
    };
    static int RemoveDir(const  char* name) { return rmdir (name); };

    // make the directory including sub-folder
    static int MakeDirs(const kmStrw& path)
    {
        if(kmFile::Exist(path.P())) return 1;

        if(MakeDirs(path) == 1)
        {
            path(path.GetLen()-1) = L'/';

            MakeDir(path.P());
        }
        return 1;
    };

    //////////////////////////////////////////////////
    // read and write functions

    // write str
    void WriteStr(const char* str, ...)
    {
        ASSERTA(IsOpen(),"[kmFile::WriteStr in 1852]");

        va_list args; // variable argument list

        va_start(args, str);
        vfprintf(_file, str, args);
        va_end(args);
    };

    // write data
    template<typename Y> void Write(const Y* str, const size_t cnt = 1)
    {        
        ASSERTA(IsOpen(), "[kmFile::Write in 1865]");

        fwrite((void*) str, sizeof(Y), cnt, _file);
    }

    // read data    
    template<typename Y> int64 Read(Y* str, const size_t cnt = 1)
    {        
        ASSERTA(IsOpen(), "[kmFile::Read in 1874]");

        return fread((void*) str, sizeof(Y), cnt, _file);
    }

    //////////////////////////////////////////////////
    // member functions to read and write kmMat

    // write kmMat
    template<typename Y> void WriteMat(const kmArr<Y>* b)
    {
        ASSERTA(IsOpen(), "[kmFile::WriteMat in 2066]");

        // write header
        kmStra hd(b->GetKmClass()); if(hd.GetLen() > 63) hd(63) = '\0';
        fwrite((void*)hd, 1, 64, _file);

        // write info of kmArr
        fwrite(b->GetInfoPt(), b->GetInfoByte(), 1, _file);

        // write data
        if(kmArr<Y>::IsArr(b->Begin()))
        {    
            for(int64 n = b->N(), i = 0; i < n; ++i) WriteMat(&b->v(i));
        }
        else
        {
            int64 size = sizeof(Y);
            fwrite(     &size,    8,         1, _file);
            fwrite(b->Begin(), size, b->Size(), _file);
        }
    }

    // read kmMat
    template<typename Y> void ReadMat(kmArr<Y>* b)
    {
        ASSERTA(IsOpen(), "[kmFile::ReadMat in 2084]");

        // read header
        kmStra hd(64);
        fread((void*)hd, 1, 64, _file);    hd(63) = '\0';
        
        // get correct header
        kmStra hd0(b->GetKmClass()); if(hd.GetLen() > 63) hd(63) = '\0';
                
        // check header
        if(hd != hd0)
        {
            PRINTFA("[kmFile::Read] the type of kmFile(%s) is different from (%s)\n",
                    (char*)hd, (char*)hd0);
            throw KE_WRONG_CONFIG;
        }
        // release b
        b->Release();
        
        // read info of kmMat
        fread(b->GetInfoPt(), b->GetInfoByte(), 1, _file);

        // recreate mat
        b->Create(b->Size());

        // read data
        if(kmArr<Y>::IsArr(b->Begin()))
        {
            for(int64 n = b->N(), i = 0; i < n; ++i) ReadMat(&b->v(i));
        }
        else
        {
            uint64 size;
            fread(&size, 8, 1, _file);

            if(size == sizeof(Y)) fread(b->Begin(), sizeof(Y), b->Size(), _file);
            else if(size < sizeof(Y)) // in case that class Y was bigger than before
            {
                print("[kmFile::Read, %s] size of class was bigger than before\n", b->GetKmClass());

                for(int i = 0; i < b->Size(); ++i) fread(b->P(i), size, 1, _file);
            }
            else // in case that class Y was smaller than before
            {
                print("[kmFile::Read, %s] size of class was smaller than before\n", b->GetKmClass());

                char* buf = new char[size];

                for(int i = 0; i < b->Size(); ++i)
                {
                    fread(buf, size, 1, _file);
                    memcpy(b->P(i), buf, sizeof(Y));
                }
                delete[] buf;
            }
        }
    }

    // dummy functions for a input which isn't a kmArr-based class
    void WriteMat(const void* b)
    {
        PRINTFA("[kmFile::WriteMat] called with a not kmArr\n");
    };
    void ReadMat(const void* b)
    {
        PRINTFA("[kmFile::ReadMat] called with a not kmArr\n");
    };

    //////////////////////////////////////////////////
    // member functions to read and write kmQue

    // write kmQue
    template<typename Y> void WriteQue(const kmQue<Y>* b)
    {
        ASSERTA(IsOpen(), "[kmFile::WriteQue in 3286]");

        // write header
        kmStra hd(16);  hd.SetVal(0);
        hd.SetStr("kmQue");

        fwrite((void*)hd, 1, 16, _file);

        // write _pobj
        WriteMat(&(b->_pobj));
        
        // write _obj
        WriteMat(&(b->_obj));
    }

    // read kmQue
    // * Note that if Y is a class with virtual function,
    // * you should reset a virtual funtion pointer (__vfptr).
    template<typename Y> void ReadQue(kmQue<Y>* b)
    {
        ASSERTA(IsOpen(), "[kmFile::ReadQue in 3309]");

        // read header
        kmStra hd(16);

        fread(hd, 1, 16, _file);

        // get correct header
        kmStra hd0(16);  hd0.SetVal(0);
        hd0.SetStr("kmQue");

        // check header
        if(hd != hd0)
        {
            PRINTFA("[kmFile::ReadQue] the type of kmFile(%s) is different from (%s)\n",
                    (char*)hd, (char*)hd0);
            throw KE_WRONG_CONFIG;
        }

        // read _pobj
        ReadMat(&(b->_pobj));

        // read _obj
        ReadMat(&(b->_obj));

        // re-arrange address of _obj        
        const int64 offset = (int64) b->_obj.Begin() - (int64) b->_pobj(0);

        for(int64 i = 0; i < b->N(); ++i)
        {
            b->_pobj(i) = (Y*)(void*)((int64) b->_pobj(i) + offset);
        }
    }
    
    //////////////////////////////////////////////////
    // member functions to read and write DIB file

    // write DIB... core
    void WriteDib(const kmImg& img)
    {
        // get headers
        BITMAPFILEHEADER fh = img.GetBmpFileHeader();
        BITMAPINFOHEADER ih = img.GetBmpInfoHeader();

        // write header
        Write(&fh);
        Write(&ih);

        // write
        Write(&img(0), img.Size());
    };

    // write DIB 
    template<typename T>
    static void WriteDib(const kmImg& img, const T* file_name)
    {
        kmFile file(file_name, KF_NEW); file.WriteDib(img);    file.Close();
    }

    // read DIB... core
    void ReadDib(kmImg& img)
    {
        // get headers
        BITMAPFILEHEADER fh;
        BITMAPINFOHEADER ih;

        // write header
        Read(&fh);
        Read(&ih);

        // init parameters
        const int64 n1 = (int64) abs(ih.biWidth );
        const int64 n2 = (int64) abs(ih.biHeight);

        // create img
        img.Recreate(n1, n2);

        // write        
        Read(&img(0), img.Size());
    };

    // read DIB
    template<typename T>
    static void ReadDib(kmImg& img, const T* file_name)
    {
        kmFile file(file_name, KF_READ); file.ReadDib(img);    file.Close();
    }
};

// file block buffer class
class kmFileBlk : public kmFile
{
public:
    int64    _byte        = 0;
    uint     _blk_byte    = 0;
    uint     _blk_n       = 0;
    int64    _writed_byte = 0;

    // constructor
    kmFileBlk() {};

    // open to read
    void OpenToRead(const wchar* name, uint blk_byte)
    {
        kmFile::Open(name, KF_READ);
        
        _byte     = kmFile::GetByte();
        _blk_byte = blk_byte;
        _blk_n    = uint((_byte - 1)/blk_byte +1);
    };

    // open to write
    void OpenToWrite(const wchar* name, int64 byte, uint blk_byte, uint blk_n)
    {
        kmFile::Open(name, KF_NEW);

        _byte = byte; _blk_byte = blk_byte; _blk_n = blk_n;
    };

    // get members
    uint  GetBlkByte() { return _blk_byte; };
    uint  GetBlkN   () { return _blk_n;    };
    int64 GetByte   () { return _byte;     };

    // read block 
    void ReadBlk(char* ptr, uint iblk)
    {
        Seek(iblk*(int64)_blk_byte); Read(ptr, GetBlkByte(iblk));
    };

    // write block
    void WriteBlk(uint iblk, char* ptr)
    {
        Seek(iblk*(int64)_blk_byte); Write(ptr, GetBlkByte(iblk));
    };

    // get block byte
    uint GetBlkByte(uint iblk)
    {
        ASSERTA( iblk < _blk_n, "[kmFileBlk::GetBlkByte in 1624]");
        
        return (iblk == _blk_n - 1)? uint(_byte - _blk_byte*(int64)iblk) : _blk_byte;
    };
};

// class for log file
class kmLog : public kmFile
{    
    /////////////////////////////////////
    // member variables
protected:

    char _file_name[256]; // file name including path

    ////////////////////////////////////
    // member functions
public:
    // constructor
    kmLog() : kmFile() { _file_name[0] = 0; }
    kmLog(const char* file_name) : kmFile()
    {
        SetFileName(file_name);        
    };

    // destructor
    ~kmLog() {}

    // set file name
    void SetFileName(const char* file_name)
    {
        ASSERTA(strlen(file_name) < 128, "kmLog::SetFileName");

        strncpy(_file_name, file_name, sizeof(_file_name));
    };

    // check state 
    int IsFileNameValid() { return (_file_name[0]==0) ? 0:1; }
    
    // write string into log-files
    void Write(const char* str, ...)
    {
        ASSERTA(IsFileNameValid(), "kmLog::Write");

        // open file
        Open(_file_name, KF_ADD);

        // get time stamp            
        char time_str[32];
        GetTimeStr(time_str, sizeof(time_str));

        kmFile::WriteStr(time_str);
                
        // write str        
        va_list ap; // variable argument list

        va_start(ap, str);        
        vfprintf(_file, str, ap);
        va_end(ap);

        // write
        kmFile::WriteStr("\r\n");

        // close file
        Close();
    };

private:
    // get string of time stamp
    void GetTimeStr(char* str, int str_size)
    {
        // get time
        time_t time_now = time(NULL);

        // convert time into struct tm
        tm time;
        localtime_r(&time_now, &time);

        // convert struct to str
        snprintf(str, str_size,
                "%04d-%02d-%02d %02d:%02d:%02d> ",
                time.tm_year+1900, time.tm_mon+1, time.tm_mday,
                time.tm_hour,      time.tm_min,   time.tm_sec);
    };
};

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
// class for ini file control

#define ReadKey(section, key)              Read(section, ""#key"", key)
#define ReadKeyDefault(section, key, val0) Read(section, ""#key"", key, val0)



///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
// class of thread

typedef void* kmThreadFun(void *);

// * Note that you sholud very carefully use a capture of lambda function,
// * since the captured variables cannot be available during thread's running.
// * So, it is recommanded using static or global for variables to be captured
// * One more thing, lambda function cannot capture any local variable
// * if it is called in any other thread.
class kmThread
{
protected:
    int _h      = 0;
    pthread_t _tid;
    void*  _lambda = nullptr; // pointer of lambdafun
    void*  _arg0   = nullptr; // addtional argument0
    void*  _arg1   = nullptr; // addtional argument1
    void*  _arg2   = nullptr; // addtional argument2

    ////////////////////////////////////////////
    // static functions
    template<class L> static
    void* _kmThreadRun(void* arg)
    {    
        // get a thread object
        kmThread* const thrd = (kmThread*) arg;

        // exec lambda function
        (*(L*)(thrd->_lambda))();

        // terminate the thread
        thrd->Close(); pthread_exit((void*)0); return 0;
    }

    template<class L, class A> static
    void* _kmThreadRunArg(void* arg)
    {    
        // get a thread object
        kmThread* const thrd = (kmThread*) arg;

        // exec lambda function
        (*(L*)(thrd->_lambda))((A)thrd->_arg0);

        // terminate the thread
        thrd->Close(); pthread_exit((void*)0); return 0;
    }

    template<class L, class A, class B> static
    void* _kmThreadRunArg2(void* arg)
    {
        // get a thread object
        kmThread* const thrd = (kmThread*) arg;

        // exec lambda function
        (*(L*)(thrd->_lambda))((A)thrd->_arg0, (B)thrd->_arg1);

        // terminate the thread
        thrd->Close(); pthread_exit((void*)0); return 0;
    }

    template<class L, class A, class B, class C> static
    void* _kmThreadRunArg3(void* arg)
    {
        // get a thread object
        kmThread* const thrd = (kmThread*) arg;

        // exec lambda function
        (*(L*)(thrd->_lambda))((A)thrd->_arg0, (B)thrd->_arg1, (C)thrd->_arg2);

        // terminate the thread
        thrd->Close(); pthread_exit((void*)0); return 0;
    }
    ////////////////////////////////////////////
    // member functions
public:
    // constructors
    kmThread() {};
    kmThread(kmThreadFun cbfun, void* arg = nullptr) { Begin(cbfun, arg); };

    // constructor with lambda function
    // * Note that these constructors have been removed 
    // * since there is a risk that thread members can be unavailable.
    // * So you should use kmThread object as member or static or global variable.
    // * Later, Let's apply self-destroy mode (sdm) to use thread without an object
    //
    //template<class L>                   kmThread(L lambdafun)   { Begin(lambdafun);}
    //template<class L, class A>          kmThread(L l, A a)      { Begin(l, a);     }
    //template<class L, class A, class B> kmThread(L l, A a, B b) { Begin(l, a, b);  }

    // destructor
    ~kmThread() { Close(); };

    // begin a thread... core
    void Begin(kmThreadFun cbfun, void* arg = nullptr)
    {
        if(_h == 0)
        {
            _h = pthread_create(&_tid, NULL, cbfun, arg);

            if(_h != 0) { PRINTFA("* kmThread: launching failed.\n"); throw KE_THREAD_FAILED; }
        }
        else PRINTFA("* kmThread: the same thread is already running.\n");
    };

    // begin a thread with lambda function
    template<class L> void Begin(L lfun)
    {
        _lambda = (void*)&lfun;
    
        Begin(_kmThreadRun<L>, (void*)this);
    }

    // begin a thread with lambda function
    template<class L, class A> void Begin(L lfun, A arg0)
    {
        _lambda = (void*)&lfun;
        _arg0   = (void*)arg0;
    
        Begin(_kmThreadRunArg<L,A>, (void*)this);
    }

    // begin a thread with lambda function
    template<class L, class A, class B> void Begin(L lfun, A arg0, B arg1)
    {
        _lambda = (void*)&lfun;
        _arg0   = (void*) arg0;
        _arg1   = (void*) arg1;
    
        Begin(_kmThreadRunArg2<L,A,B>, (void*)this);
    }

    // begin a thread with lambda function
    template<class L, class A, class B, class C> void Begin(L lfun, A arg0, B arg1, C arg2)
    {
        _lambda = (void*)&lfun;
        _arg0   = (void*) arg0;
        _arg1   = (void*) arg1;
        _arg2   = (void*) arg2;

        Begin(_kmThreadRunArg3<L,A,B,C>, (void*)this);
    }

    // wait for the thread finishing
    DWORD Wait(DWORD msec = INFINITE) { return pthread_join(_tid, NULL); };

    // wait for the thread starting
    void WaitStart() { while(!IsRunning()) { std::this_thread::sleep_for(std::chrono::milliseconds(0)); } };

    // suspend the thread
    //DWORD Suspend() { return SuspendThread(_h); };

    // resume the thread
    //DWORD Resume() { return ResumeThread(_h); };

    // check status of thread
    bool IsRunning() { return _h == 0; };

    // close the handle
    bool Close()
    {
        if(_h != 0) return false;

        bool ret = close((int)_h);
        
        if(ret) _h = 0;

        return ret;
    };
};

// critical section class
// * Note that this class is just an example. Use kmLock instead.
class __kmCs
{
protected:
    pthread_mutex_t m;
public:
    // constructor
    __kmCs()
    {
        //create mutex attribute variable
        pthread_mutexattr_t mAttr;

        // setup recursive mutex for mutex attribute
        pthread_mutexattr_settype(&mAttr, PTHREAD_MUTEX_RECURSIVE_NP);

        // Use the mutex attribute to create the mutex
        pthread_mutex_init(&m, &mAttr);

        // Mutex attribute can be destroy after initializing the mutex variable
        pthread_mutexattr_destroy(&mAttr);
    }

    // destructor
    ~__kmCs() { pthread_mutex_destroy (&m); };

    // enter critical section
    void Enter() { pthread_mutex_lock (&m); };

    // leave critical section
    void Leave() { pthread_mutex_unlock (&m); };
};

// atomic critical section class
// * Note that this class is just an example. Use kmLock instead.
class __kmCsat
{
protected:
    atomic_flag _lck = ATOMIC_FLAG_INIT;    
public:
    // enter critical section
    void Enter() noexcept {    while(_lck.test_and_set(memory_order_acquire));};

    // leaver critical section    
    void Leave() noexcept { _lck.clear(memory_order_release); };
};

// atomic mutex (mutual exclusion) class
// * Note that char or bool is slower than int or short.
class kmLock
{
protected:
    atomic<ushort> _lck = {0}, _cnt = {0};
public:    
    // constructor
    kmLock() {};

    // copy-construtor and assignment operator will be banned
    kmLock           (const kmLock&) = delete;
    kmLock& operator=(const kmLock&) = delete;

    // lock
    // * Note that this will ban both locking and entering of another thread.
    kmLock* Lock() noexcept
    {
        ushort b = 0;        
        while(!_lck.compare_exchange_strong(b, 1, memory_order_acquire)) b = 0;
        while(_cnt > 0);
        return this;
    };

    // unlock
    void Unlock() noexcept { _lck.store(0, memory_order_release); };

    // enter
    // * Note that this will ban only locking of another thread, not entering.    
    kmLock& Enter() noexcept
    {
        while(_lck == 1);
        ++_cnt;
        if(_lck == 1) { --_cnt; Enter(); }
        return *this;
    };

    // leave 
    void Leave() noexcept { if(_cnt > 0) --_cnt; };

    // Is functions
    bool IsLock() const { return (_lck == 1); };

    // get functions
    ushort GetCount() const noexcept { return _cnt; };
    ushort GetLock () const noexcept { return _lck; };
};

class kmLockGuard
{
protected:
    kmLock* _plck = nullptr;
    int     _type = 0;      // 0 : lock, 1 : enter
public:
    
    // constructor
    kmLockGuard(kmLock* plck) { _type = 0; _plck = plck; }; // lock
    kmLockGuard(kmLock&  lck) { _type = 1; _plck = &lck; }; // enter

    // destructor
    ~kmLockGuard() { if(_type == 0) _plck->Unlock(); else _plck->Leave(); };

    // copy-construtor and assignment operator will be banned
    kmLockGuard           (const kmLockGuard&) = delete;
    kmLockGuard& operator=(const kmLockGuard&) = delete;
};

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
// loop functions for lambda function
//
// * Note that const on "const M<T>&" is only for compatibility.
// * The elements of "const M<T>&" can be changed.

template<template<typename> class M, typename T, class L>
void foreach(const M<T>& a, L lambdafun)
{
    for(int64 i = a.N(); i--;) lambdafun(a(i)); 
}

template<template<typename> class M, typename T1, typename T2, class L>
void foreach(const M<T1>& a, const M<T2>& b, L lambdafun)
{
    for(int64 i = a.N(); i--;) lambdafun(a(i), b(i));
}

template<template<typename> class M, typename T1, typename T2, typename T3, class L>
void foreach(const M<T1>& a, const M<T2>& b, const M<T3>& c, L lambdafun)
{
    for(int64 i = a.N(); i--;) lambdafun(a(i), b(i), c(i));
}

template<template<typename> class M, typename T1,typename T2, typename T3, typename T4, class L>
void foreach(const M<T1>& a, const M<T2>& b, const M<T3>& c, const M<T4>& d, L lambdafun)
{
    for(int64 i = a.N(); i--;) lambdafun(a(i), b(i), c(i), d(i));
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
// class of group
template<typename T>
class kmGrp
{
protected:
    kmMat1<T*> _member;

public:
    // constructor
    kmGrp() { _member.Create(0,8); };    

    // operator()
    T& operator()(int64 idx) { return *_member(idx); };

    // add members
    template<typename ... Ts>
    kmGrp& Add(T& a, Ts& ... as) { _member.PushBack((T*)&a); Add(as...); return *this; }
    void   Add() {}; // to finish a recursive call.

    // call lambdafun for every members
    //  * ex : kmGrp<kmwChild> _grp; ... ; _grp.All([](kmwChild* a){ a->Hide(); });
    template<class L>
    void All(L lambdafun) { foreach(_member, lambdafun); }
};

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
// operation functions for kmMat

/////////////////////////////////////////////////////
// operator+=

// general form of operator+=... a+=b
template<template<typename> class M, typename T> 
M<T>& operator+=(M<T>& a, const M<T>& b)
{
    ASSERTFA(a.IsEqualSizeDim(b), "[operator+= in 3453]");

    for(int64 n = a.N(), i = 0; i < n; ++i)
    {
        a(i) += b(i);
    }
    return a;
}

// general form of operator+=... a+=b
template<template<typename> class M, typename T> 
M<T>& operator+=(M<T>& a, const T b)
{
    for(int64 n = a.N(), i = 0; i < n; ++i)
    {
        a(i) += b;
    }
    return a;
}
/////////////////////////////////////////////////////
// operator-=

// general form of operator-=... a-=b
template<template<typename> class M, typename T> 
M<T>& operator-=(M<T>& a, const M<T>& b)
{    
    ASSERTFA(a.IsEqualSizeDim(b), "[operator-= in 3469]");
        
    for(int64 n = a.N(), i = 0; i < n; ++i)
    {
        a(i) -= b(i);
    }
    return a;
}

// general form of operator-=... a-=b
template<template<typename> class M, typename T> 
M<T>& operator-=(M<T>& a, const T b)
{
    for(int64 n = a.N(), i = 0; i < n; ++i)
    {
        a(i) -= b;
    }
    return a;
}

/////////////////////////////////////////////////////
// operator *= ...subtraction and assign

// general form of operator*=... a*=b
template<template<typename> class M, typename T> 
M<T>& operator*=(M<T>& a, const M<T>& b)
{
    ASSERTFA(a.IsEqualSizeDim(b), "[operator*= in 3485]");

    for(int64 n = a.N(), i = 0; i < n; ++i)
    {
        a(i) *= b(i);
    }
    return a;
}

// general form of operator*=... a*=b
template<template<typename> class M, typename T> 
M<T>& operator*=(M<T>& a, const T b)
{
    for(int64 n = a.N(), i = 0; i < n; ++i)
    {
        a(i) *= b;
    }
    return a;
}

/////////////////////////////////////////////////////
// operator /= 

// general form of operator/=... a/=b
template<template<typename> class M, typename T> 
M<T>& operator/=(M<T>& a, const M<T>& b)
{
    ASSERTFA(a.IsEqualSizeDim(b), "[operator/= in 3501]");

    for(int64 n = a.N(), i = 0; i < n; ++i)
    {
        a(i) /= b(i);
    }
    return a;
}

// general form of operator/=... a/=b
template<template<typename> class M, typename T> 
M<T>& operator/=(M<T>& a, const T b)
{
    for(int64 n = a.N(), i = 0; i < n; ++i)
    {
        a(i) /= b;
    }
    return a;
}

/////////////////////////////////////////////////////
// operator + ...addition

// general form of operator+... c = a + b
template<template<typename> class M, typename T>
M<T> operator+(const M<T>& a, const M<T>& b)
{
    ASSERTFA(a.N() == b.N(), "[operator+ in 3509]");

    M<T> c(a);
    for(int64 n = a.N(), i = 0; i < n; ++i)
    {
        c(i) = a(i) + b(i);
    }
    return c;
}

// general form of operator+... c = a + b
template<template<typename> class M, typename T>
M<T> operator+(const M<T>& a, const T b)
{    
    M<T> c(a);
    for(int64 n = a.N(), i = 0; i < n; ++i)
    {
        c(i) = a(i) + b;
    }
    return c;
}

// general form of operator+... c = a + b
template<template<typename> class M, typename T>
M<T> operator+(const T b, const M<T>& a)
{    
    M<T> c(a);
    for(int64 n = a.N(), i = 0; i < n; ++i)
    {
        c(i) = a(i) + b;
    }
    return c;
}

// kmMat2 operator+... c = a + b
template<typename T> 
kmMat2<T> operator+(const kmMat2<T>& a, const kmMat2<T>& b)
{    
    const int64 n1 = a.N1(), n2 = a.N2();

    ASSERTFA(a.IsEqualSizeDim(b), "[operator+ in 1443]");

    kmMat2<T> c(n1,n2);
    
    for(int64 i2 = 0; i2 < n2; ++i2)
    for(int64 i1 = 0; i1 < n1; ++i1)
    {
        c(i1, i2) = a(i1, i2) + b(i1, i2);
    }
    return c;
}

// kmMat2 operator+... c = a + b
template<typename T>
kmMat2<T> operator+(const kmMat2<T>& a, const T b)
{    
    const int64 n1 = a.N1(), n2 = a.N2();

    kmMat2<T> c(n1, n2);
                
    for(int64 i2 = 0; i2 < n2; ++i2)
    for(int64 i1 = 0; i1 < n1; ++i1)
    {
        c(i1, i2) = a(i1, i2) + b;
    }
    return c;
}

// kmMat2 operator+... c = b + a
template<typename T>
kmMat2<T> operator+(const T b, const kmMat2<T>& a)
{    
    const int64 n1 = a.N1(), n2 = a.N2();    

    kmMat2<T> c(n1, n2);

    for(int64 i2 = 0; i2 < n2; ++i2)
    for(int64 i1 = 0; i1 < n1; ++i1)
    {
        c(i1, i2) = b + a(i1, i2);
    }
    return c;
}

/////////////////////////////////////////////////////
// operator * ...mutification

// general form of operator*... c = a * b
template<template<typename> class M, typename T>
M<T> operator*(const M<T>& a, const T b)
{    
    M<T> c(a);
    for(int64 n = a.N(), i = 0; i < n; ++i)
    {
        c(i) = b*a(i);
    }
    return c;
}

// general form of operator*... c = b * a
template<template<typename> class M, typename T>
M<T> operator*(const T b, const M<T>& a)
{    
    M<T> c(a);
    for(int64 n = a.N(), i = 0; i < n; ++i)
    {
        c(i) = b*a(i);
    }
    return c;
}

// kmMat2 operator*... c = a * b
template<typename T>
kmMat2<T> operator*(const kmMat2<T>& a, const T b)
{    
    const int64 n1 = a.N1(), n2 = a.N2();

    kmMat2<T> c(n1, n2);

    for(int64 i2 = 0; i2 < n2; ++i2)
    for(int64 i1 = 0; i1 < n1; ++i1)
    {
        c(i1, i2) = a(i1, i2)*b;
    }
    return c;
}

// kmMat2 operator*... c = b * a
template<typename T>
kmMat2<T> operator*(const T b, const kmMat2<T>& a)
{
    const int64 n1 = a.N1(), n2 = a.N2();

    kmMat2<T> c(n1, n2);

    for(int64 i2 = 0; i2 < n2; ++i2)
    for(int64 i1 = 0; i1 < n1; ++i1)
    {
        c(i1, i2) = a(i1, i2)*b;
    }
    return c;
}

// kmMat2 operator*... c = a * b
template<typename T>
kmMat2<T> operator*(const kmMat2<T>& a, const kmMat2<T>& b)
{        
    const int64 an1 = a.N1(), an2 = a.N2();
    const int64 bn1 = b.N1(), bn2 = b.N2();

    ASSERTFA(an2 == bn1, "[operator* in 1691]");

    kmMat2<T> c(an1, bn2);
                
    for(int64 i2 = 0; i2 < bn2; ++i2)
    for(int64 i1 = 0; i1 < an1; ++i1)
    {
        T sum = 0; 
        for(int64 i = 0; i  < an2; ++i ) sum += a(i1, i)*b(i, i2);
        c(i1, i2) = sum;
    }
    return c;        
}

// kmMat3 operator*... c = a * b
template<typename T>
kmMat3<T> operator*(const kmMat3<T>& a, const kmMat3<T>& b)
{        
    const int64 an1 = a.N1(), an2 = a.N2(), an3 = a.N3();
    const int64 bn1 = b.N1(), bn2 = b.N2(), bn3 = b.N3();

    ASSERTFA(an2 == bn1, "[operator* in 3621]");
    ASSERTFA(an3 == bn3, "[operator* in 3622]");

    kmMat3<T> c(an1, bn2, an3);
    
    for(int64 i3 = 0; i3 < an3; ++i3)
    for(int64 i2 = 0; i2 < bn2; ++i2)
    for(int64 i1 = 0; i1 < an1; ++i1)
    {
        T sum = 0;
        for(int64 i = 0; i  < an2; ++i ) sum += a(i1, i, i3)*b(i, i2, i3);
        c(i1, i2, i3) = sum;
    }
    return c;
}

// kmMat4 operator*... c = a * b
template<typename T>
kmMat4<T> operator*(const kmMat4<T>& a, const kmMat4<T>& b)
{        
    const int64 an1 = a.N1(), an2 = a.N2(), an3 = a.N3(), an4 = a.N4();
    const int64 bn1 = b.N1(), bn2 = b.N2(), bn3 = b.N3(), bn4 = b.N4();

    ASSERTFA(an2 == bn1, "[operator* in 3643]");
    ASSERTFA(an3 == bn3, "[operator* in 3644]");
    ASSERTFA(an4 == bn4, "[operator* in 3645]");

    kmMat4<T> c(an1, bn2, an3, an4);
    
    for(int64 i4 = 0; i4 < an4; ++i4)
    for(int64 i3 = 0; i3 < an3; ++i3)
    for(int64 i2 = 0; i2 < bn2; ++i2)
    for(int64 i1 = 0; i1 < an1; ++i1)
    {
        T sum = 0; 
        for(int64 i = 0; i  < an2; ++i ) sum += a(i1, i, i3, i4)*b(i, i2, i3, i4);
        c(i1, i2, i3, i4) = sum;
    }
    return c;
}

/////////////////////////////////////////////////////
// operator - ...subtraction

// general form of operator-... c = a - b
template<template<typename> class M, typename T>
M<T> operator-(const M<T>& a, const M<T>& b)
{    
    ASSERTFA(a.IsEqualSizeDim(b), "[operator- in 3476]");

    M<T> c(a);
    for(int64 n = a.N(), i = 0; i < n; ++i)
    {
        c(i) = a(i) - b(i);
    }
    return c;
}

// general form of operator-... c = a - b
template<template<typename> class M, typename T>
M<T> operator-(const M<T>& a, const T b)
{    
    M<T> c(a);
    for(int64 n = a.N(), i = 0; i < n; ++i)
    {
        c(i) = a(i) - b;
    }
    return c;
}

// general form of operator-... c = b - a
template<template<typename> class M, typename T>
M<T> operator-(const T b, const M<T>& a)
{    
    M<T> c(a);
    for(int64 n = a.N(), i = 0; i < n; ++i)
    {
        c(i) = b - a(i);
    }
    return c;
}

// kmMat2 operator-... c = a - b
template<typename T>
kmMat2<T> operator-(const kmMat2<T>& a, const kmMat2<T>& b)
{    
    const int64 n1 = a.N1(), n2 = a.N2();

    ASSERTFA(a.IsEqualSizeDim(b), "[operator- in 2157]");

    kmMat2<T> c(n1, n2);
    
    for(int64 i2 = 0; i2 < n2; ++i2)
    for(int64 i1 = 0; i1 < n1; ++i1)
    {
        c(i1, i2) = a(i1, i2) - b(i1, i2);
    }
    return c;
}

// kmMat2 operator-... c = a - b
template<typename T>
kmMat2<T> operator-(const kmMat2<T>& a, const T b)
{        
    const int64 n1 = a.N1(), n2 = a.N2();

    kmMat2<T> c(n1, n2);

    for(int64 i2 = 0; i2 < n2; ++i2)
    for(int64 i1 = 0; i1 < n1; ++i1)
    {
        c(i1, i2) = a(i1, i2) - b;
    }
    return c;
}

// kmMat2 operator-... c = b - a
template<typename T>
kmMat2<T> operator-(const T b, const kmMat2<T>& a)
{    
    const int64 n1 = a.N1(), n2 = a.N2();

    kmMat2<T> c(n1, n2);

    for(int64 i2 = 0; i2 < n2; ++i2)
    for(int64 i1 = 0; i1 < n1; ++i1)
    {
        c(i1, i2) = b - a(i1, i2);
    }
    return c;
}

/////////////////////////////////////////////////////
// operator / ...division

// general form of operator/... c = a / b
template<template<typename> class M, typename T>
M<T> operator/(const M<T>& a, const T b)
{    
    M<T> c(a);
    for(int64 n = a.N(), i = 0; i < n; ++i)
    {
        c(i) = a(i) / b;
    }
    return c;
}

// general form of operator/... c = b / a
template<template<typename> class M, typename T>
M<T> operator/(const T b, const M<T>& a)
{    
    M<T> c(a);
    for(int64 n = a.N(), i = 0; i < n; ++i)
    {
        c(i) = b / a(i);
    }
    return c;
}

// kmMat2 operator/... c = a / b
template<typename T>
kmMat2<T> operator/(const kmMat2<T>& a, const T b)
{
    // init parameters        
    const int64 n1 = a.N1(), n2 = a.N2();
        
    // subtract matrix
    kmMat2<T> c(n1, n2);
                
    for(int64 i2 = 0; i2 < n2; ++i2)
    for(int64 i1 = 0; i1 < n1; ++i1)
    {        
        c(i1, i2) = a(i1, i2) / b;
    }
    return c;
}

// kmMat2 operator/... c = b / a
template<typename T>
kmMat2<T> operator/(const T b, const kmMat2<T>& a)
{
    // init parameters        
    const int64 n1 = a.N1(), n2 = a.N2();    
        
    // divide matrix
    kmMat2<T> c(n1, n2);
                
    for(int64 i2 = 0; i2 < n2; ++i2)
    for(int64 i1 = 0; i1 < n1; ++i1)
    {        
        c(i1, i2) = b / a(i1, i2);
    }
    return c;
}

/////////////////////////////////////////////////////
// operator ^ ...power

// general form of operator^... c = a^b ... c = pow(a, b)
template<template<typename> class M, typename T>
M<T> operator^(const M<T>& a, const T b)
{    
    M<T> c(a);
    for(int64 n = a.N(), i = 0; i < n; ++i)
    {
        c(i) = pow(a(i), b);
    }
    return c;
}

// kmMat2 operator^... c = a^b ... c = pow(a, b)
template<typename T>
kmMat2<T> operator^(const kmMat2<T>& a, const T b)
{    
    const int64 n1 = a.N1(), n2 = a.N2();

    kmMat2<T> c(n1, n2);

    for(int64 i2 = 0; i2 < n2; ++i2)
    for(int64 i1 = 0; i1 < n1; ++i1)
    {
        c(i1, i2) = pow(a(i1, i2), b);
    }
    return c;
}

//////////////////////////////////////////////////////
// hash class

// chaining type
//   T : datat type, K : key type
template<typename T, typename K, int hash_n, int node_n_max> 
class kmHashC
{
    class kmNode
    {
    public:
        T*      data = nullptr;
        kmNode* prv  = nullptr;
        kmNode* nxt  = nullptr;
    };
    kmNode _table[hash_n];
    kmNode _node [node_n_max];
    int    _node_n = 0;

public:
    static const int _hash_n     = hash_n;
    static const int _node_n_max = node_n_max;
    
    // add data's address to hash table
    void Add(T* data)
    {
        const K key  = GetKey(data);        
        kmNode* node = &_table[GetHashCode(key)];

        if(node->data == nullptr) node->data = data;
        else
        {
            while(node->nxt != nullptr) node = node->nxt;

            node->nxt = &_node[_node_n++];
            node->nxt->prv  = node;
            node->nxt->data = data;
        }
    };    

    T* Find(K key) // if not found. it will return nullptr
    {
        kmNode* node = &_table[GetHashCode(key)];

        if(node->data == nullptr) return nullptr;

        while(!IsOpen(node->data, key))
        {
            if(node->nxt == nullptr) return nullptr; // cannot find
            node = node->nxt;
        }
        return node->data;
    };

    int Erase(K key)
    {
        kmNode* node = &_table[GetHashCode(key)];

        while(!IsOpen(node->data, key))
        {
            if(node->nxt == nullptr) return -1; // cannot find
            node = node->nxt;
        }        
        if(node->nxt == nullptr) node->prv->nxt = nullptr;
        else 
        {
            node->prv->nxt = node->nxt;
            node->nxt->prv = node->prv;
        }
        return int(node - _node)/sizeof(kmNode); // index of erase node
    };

    ////////////////////////////////////////
    // pure virtual functions
    virtual K    GetKey     (T* data       ) = 0;
    virtual int  GetHashCode(         K key) = 0;
    virtual bool IsOpen     (T* data, K key) = 0;
};

// indexing type
//   T : datat type, K : key type
template<typename T, typename K, int hash_n, int node_n_max>
class kmHashI
{
    int _table[hash_n][node_n_max] = {0,}; // it will save idx as idx + 1
    
public:
    static const int _hash_n     = hash_n;
    static const int _node_n_max = node_n_max;
    
    // init or clear talbe
    void Init()
    {
        const int n   = _hash_n*_node_n_max;
        int*      ptr = (int*)_table;
        for(int i = 0; i < n; ++i) *(ptr + i) = 0;
    };

    // add data's index to hash table
    void Add(T* data, int idx)
    {
        const K key = GetKey(data);

        int* node = (int*)_table[GetHashCode(key)];

        if(*node == 0) *node = idx + 1;
        else
        {
            while(*node != 0) ++node;
            *node = idx + 1;
        }
    };    

    // find data from key... if not found, it'll return nullptr
    T* Find(T* data0, K key)
    {
        int* node = (int*)_table[GetHashCode(key)];

        if(*node == 0) return nullptr;

        while(!IsOpen(data0 + *node - 1, key))
        {
            ++node;    if(*node == 0) return nullptr; // cannot find
        }
        return data0 + *node - 1;
    };

    // erase data of the key from hash table.. if not found, it'll return -1
    int Erase(T* data0, K key)
    {
        int* node = (int*)_table[GetHashCode(key)];

        if(*node == 0) return -1;
        
        // find data
        while(!IsOpen(data0 + *node - 1, key))
        {
            ++node; if(*node == 0) return -1;
        }
        const int erased_idx = *node - 1;

        // erase 
        while(*(node + 1) != 0)
        {
            *node = *(node + 1); ++node;
        }
        *node = 0;

        return erased_idx;
    };

    ////////////////////////////////////////
    // pure virtual functions
    virtual K    GetKey     (T* data       ) = 0;
    virtual int  GetHashCode(         K key) = 0;
    virtual bool IsOpen     (T* data, K key) = 0;
};

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
// media file info for kmMat

// media file type... jpg, png, bmp, gif, mp4, mpg, avi, unknown
enum class kmMdfType { jpg, png, bmp, gif, mp4, mpg, avi, unknown };

// tiff IFD (image file directory) structure... 12byte
class kmTiffIfd
{
public: ushort tag, fmt; uint obj_n, data;
};

// media file class
class kmMdf
{
public:
    kmMdfType _type{};
    kmDate    _date{};
    kmGps     _gps{};
    //kmMdfOrnt _ornt{};

    // constructor
    kmMdf() {};    
    kmMdf(kmFile& fobj) { Init(fobj); };
    kmMdf(const wchar* fname) { kmFile fobj(fname); Init(fobj); }; 

    // init from file
    int Init(kmFile& fobj)
    {
        // get type
        _type = GetType(fobj); 

        // read header info
        switch(_type)
        {
        case kmMdfType::jpg : ReadJpg(fobj); return 1;
        default: break;
        }
        return 0;
    };

    // print info
    void Print()
    {
        print("* mdf type :");
        switch(_type)
        {
        case kmMdfType::jpg : print("jpg"); break;
        case kmMdfType::png : print("png"); break;
        case kmMdfType::bmp : print("bmp"); break;
        case kmMdfType::gif : print("gif"); break;
        case kmMdfType::mp4 : print("mp4"); break;
        case kmMdfType::mpg : print("mpg"); break;
        case kmMdfType::avi : print("avi"); break;
        case kmMdfType::unknown : print("unknown");
        }
        print("\n");

        wcout<<L"* date : "<<_date.GetStrwPt().P()<<endl;
        _gps.Print();
    };

    //////////////////////////////////////////////////
    // static functions

    // get mdf type from file
    static kmMdfType GetType(kmFile& fobj)
    {
        // read firt 8 bytes
        uchar buf[8];

        fobj.Seek().Read(buf, 8);

        if     (IsJpg(buf)) return kmMdfType::jpg;
        else if(IsBmp(buf)) return kmMdfType::bmp;
        else if(IsPng(buf)) return kmMdfType::png;

        return kmMdfType::unknown;
    };

    static bool IsJpg(uchar* hd) { return hd[0] == 0xFF && hd[1] == 0xD8; };
    static bool IsBmp(uchar* hd) { return hd[0] == 0x42 && hd[1] == 0x4D; };
    static bool IsPng(uchar* hd)
    {
        const uchar hd0[] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };

        for(uint i = 0; i < sizeof(hd0); ++i) if(hd[i] != hd0[i]) return false;

        return true;
    };

    //////////////////////////////////////////////////
    // jpeg functions 

    // read jpg
    void ReadJpg(kmFile& fobj)
    {
        fobj.Seek(2); // pos after header (SOI)

        for(uchar mk[2] = {}; 1; mk[0] = 0)
        {
            fobj.Read(mk, 2); // read marker

            int64 eop = ReadEop(fobj); // end of position

            if(mk[0] == 0xff) switch(mk[1])
            {
            case 0xE0 : ReadJpgApp0(fobj); break; // app0
            case 0xE1 : ReadJpgApp1(fobj); break; // app1
            case 0xDB : ReadJpgDqt (fobj); break; // define quantization table
            case 0xC0 : ReadJpgSof0(fobj); break; // start of frame (baseline DCT)
            case 0xC2 : ReadJpgSof2(fobj); break; // start of frame (progressive DCT)
            case 0xC4 : ReadJpgDht (fobj); break; // define huffman table
            case 0xDA : ReadJpgSos (fobj); break; // start of scan
            case 0xFE : ReadJpgCmt (fobj); break; // comment
            default   : print("*** %c\n", mk[1]);
            }
            else break;

            fobj.Seek(eop);
        }
    };

    // read end of position
    int64 ReadEop(kmFile& fobj)
    {
        const int64 pos0 = fobj.GetPos();

        uchar buf[2] = {}; fobj.Read(buf, 2);

        const ushort len = ((ushort)buf[0]<<8) | buf[1];

        return len + pos0;
    };
    
    // start of scan
    void ReadJpgSos(kmFile& fobj) {};

    // define quantization table
    void ReadJpgDqt(kmFile& fobj) {};

    // define huffman table
    void ReadJpgDht(kmFile& fobj) {};

    // start of frame (baseline DCT)
    void ReadJpgSof0(kmFile& fobj)
    {
        fobj.SeekCur(1);

        ushort h_pix, w_pix;

        fobj.Read(&h_pix); fobj.Read(&w_pix);

        h_pix = ((0x00ff&h_pix)<<8) | ((0xff00&h_pix)>>8);
        w_pix = ((0x00ff&w_pix)<<8) | ((0xff00&w_pix)>>8);
    };

    // start of frame (progressive DCT)
    void ReadJpgSof2(kmFile& fobj) {};

    // comment
    void ReadJpgCmt(kmFile& fobj)  {};

    // app0 Jfif
    void ReadJpgApp0(kmFile& fobj) {};

    // app1 exif
    void ReadJpgApp1(kmFile& fobj) { ReadExif(fobj); };

    // read exif
    void ReadExif(kmFile& fobj)
    {
        uchar buf[6] = {}; fobj.Read(buf, 6);

        if(strcmp((char*)&buf[0], "Exif") == 0) // exif
        {
            // read tiff
            ReadTiff(fobj);
        }
    };

    // read tiff
    void ReadTiff(kmFile& fobj)
    {
        int64 pos0 = fobj.GetPos();

        uchar buf[4] = {}; fobj.Read(buf, 2);

        if(buf[0] == 0x49 && buf[1] == 0x49) // little endian
        {
            fobj.Read(buf, 2); // tag mark   ...0x2A00
            fobj.Read(buf, 4); // IFD offset ...0x08000000

            ReadIfdLe(fobj, pos0);
        }
        else if(buf[0] == 0x4d && buf[1] == 0x4d) // big endian
        {
            fobj.Read(buf, 2); // tag mark   ...0x002A
            fobj.Read(buf, 4); // IFD offset ...0x00000008

            ReadIfdBe(fobj, pos0);
        }
        else print("[kmTiffInfo] it is not tiff\n");
    };

    // read ifd for big endian
    void ReadIfdBe(kmFile& fobj, int64 pos0)
    {    
        ushort entry_n; fobj.Read(&entry_n); entry_n = ENDIAN16(entry_n); print_i(entry_n);

        for(int i = 0; i < entry_n; ++i)
        {
            kmTiffIfd ifd; fobj.Read(&ifd);

            ifd.data  = ENDIAN32(ifd.data);
            ifd.fmt   = ENDIAN16(ifd.fmt);
            ifd.obj_n = ENDIAN32(ifd.obj_n);
            ifd.tag   = ENDIAN16(ifd.tag);

            if(ifd.fmt == 3) ifd.data>>=16; // ushort

            int64 pos_old = fobj.GetPos(); fobj.Seek(pos0).SeekCur(ifd.data);

            switch(ifd.tag)
            {
            case 0x010f : ReadIfdMaker(fobj,  ifd); break; 
            case 0x0110 : ReadIfdModel(fobj,  ifd); break;
            case 0x0112 : ReadIfdOrnt (fobj,  ifd); break;
            case 0x0132 : ReadIfdDate (fobj,  ifd); break;
            case 0x8825 : ReadIfdGps  (fobj, pos0); break;
            }
            fobj.Seek(pos_old);
        }
        uchar buf[2] = {}; fobj.Read(buf,2);

        ushort len = ((ushort)buf[0]<<8) | buf[1];

        fobj.Seek(pos0 + len).Read(buf,2);
    };

    // read ifd for little endian
    void ReadIfdLe(kmFile& fobj, int64 pos0)
    {
        ushort entry_n; fobj.Read(&entry_n);

        for(int i = 0; i < entry_n; ++i)
        {
            kmTiffIfd ifd; fobj.Read(&ifd);

            int64 pos_old = fobj.GetPos(); fobj.Seek(pos0).SeekCur(ifd.data);

            switch(ifd.tag)
            {
            case 0x010f : ReadIfdMaker(fobj,  ifd); break;
            case 0x0110 : ReadIfdModel(fobj,  ifd); break;
            case 0x0112 : ReadIfdOrnt (fobj,  ifd); break;
            case 0x0132 : ReadIfdDate (fobj,  ifd); break;
            case 0x8825 : ReadIfdGps  (fobj, pos0); break;
            }
            fobj.Seek(pos_old);
        }
        uchar buf[2] = {}; fobj.Read(buf,2);

        ushort len = ((ushort)buf[0]<<8) | buf[1];

        fobj.Seek(pos0 + len).Read(buf,2);
    };
    void ReadIfdOrnt(kmFile& fobj, kmTiffIfd& ifd) // orientation
    {
        // 1 : upper left, 3: lower right, 6: upper right, 8: lower left
        //_ornt = (kmMdfOrnt) ifd.data;
    };
    void ReadIfdMaker(kmFile& fobj, kmTiffIfd& ifd)
    {
        //kmStra maker(ifd.obj_n); fobj.Read(maker.P(), ifd.obj_n);
    };
    void ReadIfdModel(kmFile& fobj, kmTiffIfd& ifd)
    {
        //kmStra model(ifd.obj_n); fobj.Read(model.P(), ifd.obj_n);
    };
    void ReadIfdDate(kmFile& fobj, kmTiffIfd& ifd)
    {
        kmStra date(ifd.obj_n); fobj.Read(date.P(), ifd.obj_n);

        _date.Set(date);
    };

    // read ifd gps for little endian
    void ReadIfdGps(kmFile& fobj, int64 pos0)
    {
        ushort entry_n; fobj.Read(&entry_n);

        for(int k = 0; k < entry_n; ++k)
        {
            kmTiffIfd ifd; fobj.Read(&ifd);

            int64 pos_old = fobj.GetPos(); fobj.Seek(pos0).SeekCur(ifd.data);

            switch(ifd.tag)
            {            
            case 0x0001: ReadIfdGpsLatRef(fobj, ifd); break;
            case 0x0002: ReadIfdGpsLat   (fobj, ifd); break;
            case 0x0003: ReadIfdGpsLngRef(fobj, ifd); break;
            case 0x0004: ReadIfdGpsLng   (fobj, ifd); break;
            case 0x0005: ReadIfdGpsAltRef(fobj, ifd); break;
            case 0x0006: ReadIfdGpsAlt   (fobj, ifd); break;
            case 0x0007: ReadIfdGpsTime  (fobj, ifd); break;
            case 0x001d: ReadIfdGpsDate  (fobj, ifd); break;
            }            
            fobj.Seek(pos_old);
        }
    };
    void ReadIfdGpsLatRef(kmFile& fobj, kmTiffIfd& ifd)
    {
        _gps._lat_ref = ifd.data;
    };
    void ReadIfdGpsLat(kmFile& fobj, kmTiffIfd& ifd)
    {
        int rat[6]; fobj.Read(&rat[0], ifd.obj_n*2);

        double deg = rat[0]/double(rat[1]);
        double min = rat[2]/double(rat[3]);
        double sec = rat[4]/double(rat[5]);

        _gps._lat_deg = deg + min/60. + sec/3600.;
    };
    void ReadIfdGpsLngRef(kmFile& fobj, kmTiffIfd& ifd)
    {
        _gps._lng_ref = ifd.data;
    };
    void ReadIfdGpsLng(kmFile& fobj, kmTiffIfd& ifd)
    {
        int rat[6]; fobj.Read(&rat[0], ifd.obj_n*2);

        double deg = rat[0]/double(rat[1]);
        double min = rat[2]/double(rat[3]);
        double sec = rat[4]/double(rat[5]);

        _gps._lng_deg = deg + min/60. + sec/3600.;
    };
    void ReadIfdGpsAltRef(kmFile& fobj, kmTiffIfd& ifd)
    {
    };
    void ReadIfdGpsAlt(kmFile& fobj, kmTiffIfd& ifd)
    {
        int rat[2]; fobj.Read(&rat[0], ifd.obj_n*2);

        _gps._alt_m = float(rat[0]/double(rat[1]));
    };
    void ReadIfdGpsDate(kmFile& fobj, kmTiffIfd& ifd)
    {
        //kmStra date(ifd.obj_n); fobj.Read(date.P(), ifd.obj_n);
    };
    void ReadIfdGpsTime(kmFile& fobj, kmTiffIfd& ifd)
    {
        //int rat[6]; fobj.Read(&rat[0], ifd.obj_n*2);
    };    
};

#endif /* __km7Mat__ */
