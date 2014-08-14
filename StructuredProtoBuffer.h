/****************************************
The software uses the GPL agreement authorization, please read the license.txt
Author: bihongpi
E-mail: pibihong@163.com
Last-modified: 2014-08-13
****************************************/



#ifndef __BIHONGPI_STRUCTURED_PROTO_BUFFER_H__
#define __BIHONGPI_STRUCTURED_PROTO_BUFFER_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <string>
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <sstream>
#include <stdexcept>
#include <arpa/inet.h>


namespace spb
{

#define SPB_CLASS_TYPE(_name) CStructuredProtoBuffer_##_name
#define _SPB_CLASS_WITH_LEN_TYPE(_name) CStructuredProtoBuffer_len_##_name

#define _SPB_TAB_STRING "  "

class IStructuredProtoBuffer
{
protected:
    static inline ::std::string _any_to_str(int8_t t)
    {
        ::std::stringstream s;
        s<<(uint64_t)t;
        return s.str();
    }
    static inline ::std::string _any_to_str(uint8_t t)
    {
        ::std::stringstream s;
        s<<(uint64_t)t;
        return s.str();
    }
    template<class T>
    static inline ::std::string _any_to_str(T t)
    {
        ::std::stringstream s;
        s<<t;
        return s.str();
    }
    static inline ::std::string _buf_debug_str(const char* buf, size_t len)
    {
        ::std::string ret;
        char s[5] = {0};
        for (size_t i = 0; i < len; ++i)
        {
            if (buf[i] <= ' ' || buf[i] > 127)
            {
                snprintf(s, 5, "\\x%02x", (unsigned)buf[i]);
                ret += s;
            }
            else
            {
                ret += buf[i];
            }
        }
        return ret;
    }
    static inline bool _is_bigend()
    {
        int i=0x12345678;
        return (*((char *)&i) == 0x12);
    };
    static inline uint64_t _swapll(uint64_t x)
    {
        return (((x) & 0xff00000000000000llu) >> 56)
             | (((x) & 0x00ff000000000000llu) >> 40)
             | (((x) & 0x0000ff0000000000llu) >> 24)
             | (((x) & 0x000000ff00000000llu) >> 8)
             | (((x) & 0x00000000ff000000llu) << 8)
             | (((x) & 0x0000000000ff0000llu) << 24)
             | (((x) & 0x000000000000ff00llu) << 40)
             | (((x) & 0x00000000000000ffllu) << 56);
    }
    static inline int8_t   _proto_ntoh(int8_t x){return x;}
    static inline int8_t   _proto_hton(int8_t x){return x;}
    static inline uint8_t  _proto_ntoh(uint8_t x){return x;}
    static inline uint8_t  _proto_hton(uint8_t x){return x;}
    static inline int16_t  _proto_ntoh(int16_t x){return (int16_t)_proto_ntoh((uint16_t)x);}
    static inline int16_t  _proto_hton(int16_t x){return (int16_t)_proto_hton((uint16_t)x);}
    static inline uint16_t _proto_ntoh(uint16_t x){return ntohs(x);}
    static inline uint16_t _proto_hton(uint16_t x){return htons(x);}
    static inline int32_t  _proto_ntoh(int32_t x){return (int32_t)_proto_ntoh((uint32_t)x);}
    static inline int32_t  _proto_hton(int32_t x){return (int32_t)_proto_hton((uint32_t)x);}
    static inline uint32_t _proto_ntoh(uint32_t x){return ntohl(x);}
    static inline uint32_t _proto_hton(uint32_t x){return htonl(x);}
    static inline int64_t  _proto_ntoh(int64_t x){return (int64_t)_proto_ntoh((uint64_t)x);}
    static inline int64_t  _proto_hton(int64_t x){return (int64_t)_proto_hton((uint64_t)x);}
    static inline uint64_t _proto_ntoh(uint64_t x)
    {
        if (_is_bigend())return x;
        return _swapll(x);
    }
    static inline uint64_t _proto_hton(uint64_t x)
    {
        if (_is_bigend())return x;
        return _swapll(x);
    }
    
    static inline float _proto_ntoh(float x)
    {
        uint32_t t;
        memcpy(&t, &x, sizeof(t));
        t = _proto_ntoh(t);
        memcpy(&x, &t, sizeof(t));
        return x;
    }
    static inline float _proto_hton(float x)
    {
        uint32_t t;
        memcpy(&t, &x, sizeof(t));
        t = _proto_hton(t);
        memcpy(&x, &t, sizeof(t));
        return x;
    }
    
    static inline double _proto_ntoh(double x)
    {
        uint64_t t;
        memcpy(&t, &x, sizeof(t));
        t = _proto_ntoh(t);
        memcpy(&x, &t, sizeof(t));
        return x;
    }
    static inline double _proto_hton(double x)
    {
        uint64_t t;
        memcpy(&t, &x, sizeof(t));
        t = _proto_hton(t);
        memcpy(&x, &t, sizeof(t));
        return x;
    }
    template<class T>
    static inline int _proto_encode_to_buffer(T t, void* p)
    {
        T tmp = _proto_hton(t);
        memcpy(p, &tmp, sizeof(t));
        return sizeof(t);
    }
    template<class T>
    static inline int _proto_encode_to_string(T t, ::std::string& p)
    {
        T tmp = _proto_hton(t);
        p.assign((const char*)&tmp, sizeof(t));
        return sizeof(t);
    }
    template<class T>
    static inline int _proto_encode_append_string(T t, ::std::string& p)
    {
        T tmp = _proto_hton(t);
        p.append((const char*)&tmp, sizeof(t));
        return sizeof(t);
    }
    template<class T>
    static inline T _proto_decode_from_buffer(const void* p)
    {
        return _proto_ntoh(*((T*)p));
    }

public:
    typedef size_t size_type;
    
public:
    virtual int ParseFromArray(const void* pBuf, size_t nLen) = 0;
    virtual int ParseFromString(const ::std::string& strData)
    {
        return this->ParseFromArray(strData.data(), strData.size());
    }
    
    virtual int SerializeToString(::std::string& strData) const = 0;
    virtual int SerializeToArray(void* pBuf, size_t nLen) const
    {
        ::std::string strData;
        int iRet = SerializeToString(strData);
        if (iRet < 0)return iRet;
        if (nLen < strData.size())return -1;
        memcpy(pBuf, strData.data(), strData.size());
        return (int)(strData.size());
    }
    virtual int AppendToString(::std::string& strData) const
    {
        ::std::string strTmp;
        int iRet = this->SerializeToString(strTmp);
        if (iRet < 0)return iRet;
        strData.append(strTmp);
        return strTmp.size();
    }
    virtual size_type ArraySize() const = 0;

    virtual ::std::string DebugString(const ::std::string& tab="") const = 0;

#ifdef JSON_JSON_H_INCLUDED
    virtual operator ::Json::Value() const
    {
        ::Json::Value v;
        this->toJson(v);
        return v;
    }
    virtual int toJson(::Json::Value& json) const = 0;
#endif
    
public:
    virtual const char* _GetTypeName() const = 0;
    
    virtual bool _IsObjectType() const {return false;}
    virtual bool _IsContainerType() const {return false;}
    virtual bool _IsMapType() const {return false;}
    virtual bool _IsNumericType() const {return false;}
    virtual bool _IsStringType() const {return false;}
    virtual bool _IsBasicType() const {return false;}
    virtual bool _IsBasicWithLenType() const {return false;}

public:
    virtual void clear() = 0;
};


#ifdef JSON_JSON_H_INCLUDED
#define _SPB_DEFINE_NUMERIC_JSON_METHOD(_json_type) \
    virtual int toJson(::Json::Value& json) const \
    { \
        json = (_json_type)_data; \
        return 0; \
    }
#else
#define _SPB_DEFINE_NUMERIC_JSON_METHOD(_json_type)
#endif

/////////////////////////////////////////////////////////////////////////////////


class SPB_CLASS_TYPE(numeric) : public IStructuredProtoBuffer
{
public:
    virtual bool _IsNumericType() const {return true;}
    virtual bool _IsBasicType() const {return true;}
    virtual void SetNumeric(int8_t n) = 0;
    virtual void SetNumeric(int16_t n) = 0;
    virtual void SetNumeric(int32_t n) = 0;
    virtual void SetNumeric(int64_t n) = 0;
    virtual void SetNumeric(uint8_t n) = 0;
    virtual void SetNumeric(uint16_t n) = 0;
    virtual void SetNumeric(uint32_t n) = 0;
    virtual void SetNumeric(uint64_t n) = 0;
    virtual void SetNumeric(float n) = 0;
    virtual void SetNumeric(double n) = 0;
    virtual int64_t GetNumeric() = 0;
    virtual uint64_t GetUNumeric() = 0;
    virtual double GetDNumeric() = 0;
};

#define _SPB_DEFINE_NUMERIC_TYPE(_t, _json_type) \
class SPB_CLASS_TYPE(_t) : public SPB_CLASS_TYPE(numeric) \
{ \
public: \
    typedef SPB_CLASS_TYPE(_t) this_type; \
    typedef _t data_type; \
protected: \
    data_type _data; \
public: \
    virtual const char* _GetTypeName() const{return #_t;} \
public: \
    SPB_CLASS_TYPE(_t)(data_type value=0){ set(value); } \
    virtual size_type ArraySize() const \
    { \
        return sizeof(data_type); \
    } \
    virtual int ParseFromArray(const void* pBuf, size_t nLen) \
    { \
        if (nLen < sizeof(data_type))return -1; \
        _data = _proto_decode_from_buffer<data_type>(pBuf); \
        return sizeof(data_type); \
    } \
    virtual int SerializeToString(::std::string& strData) const \
    { \
        int iRet = _proto_encode_to_string(_data, strData); \
        if (iRet < 0)return iRet; \
        return sizeof(data_type); \
    } \
    virtual int SerializeToArray(void* pBuf, size_t nLen) const \
    { \
        if (nLen < sizeof(data_type)) return -1; \
        int iRet = _proto_encode_to_buffer(_data, pBuf); \
        if (iRet < 0)return iRet; \
        return sizeof(data_type); \
    } \
    virtual int AppendToString(::std::string& strData) const \
    { \
        int iRet = _proto_encode_append_string(_data, strData); \
        if (iRet < 0)return iRet; \
        return sizeof(data_type); \
    } \
    virtual void clear() { _data = 0; } \
    inline this_type& assign(data_type value) \
    { \
        _data = value; \
        return *this; \
    } \
    inline this_type& set(data_type value) \
    { \
        _data = value; \
        return *this; \
    } \
    inline this_type& operator()(data_type value) \
    { \
        _data = value; \
        return *this; \
    } \
    inline this_type& operator=(data_type value) \
    { \
        _data = value; \
        return *this; \
    } \
    inline const data_type& get() const \
    { \
        return _data; \
    } \
    inline data_type& get() \
    { \
        return _data; \
    } \
    inline operator data_type() const \
    { \
        return get(); \
    } \
    virtual void SetNumeric(int8_t n){ set((data_type)n); } \
    virtual void SetNumeric(int16_t n){ set((data_type)n); } \
    virtual void SetNumeric(int32_t n){ set((data_type)n); } \
    virtual void SetNumeric(int64_t n){ set((data_type)n); } \
    virtual void SetNumeric(uint8_t n){ set((data_type)n); } \
    virtual void SetNumeric(uint16_t n){ set((data_type)n); } \
    virtual void SetNumeric(uint32_t n){ set((data_type)n); } \
    virtual void SetNumeric(uint64_t n){ set((data_type)n); } \
    virtual void SetNumeric(float n){ set((data_type)n); } \
    virtual void SetNumeric(double n){ set((data_type)n); } \
    virtual int64_t GetNumeric(){ return (int64_t)get();} \
    virtual uint64_t GetUNumeric(){ return (uint64_t)get();} \
    virtual double GetDNumeric(){ return (double)get();} \
    virtual ::std::string DebugString(const ::std::string& tab="") const \
    { \
        return _any_to_str(_data); \
    } \
    _SPB_DEFINE_NUMERIC_JSON_METHOD(_json_type) \
}


_SPB_DEFINE_NUMERIC_TYPE(int8_t,   ::Json::Int);
_SPB_DEFINE_NUMERIC_TYPE(uint8_t,  ::Json::UInt);
_SPB_DEFINE_NUMERIC_TYPE(int16_t,  ::Json::Int);
_SPB_DEFINE_NUMERIC_TYPE(uint16_t, ::Json::UInt);
_SPB_DEFINE_NUMERIC_TYPE(int32_t,  ::Json::Int);
_SPB_DEFINE_NUMERIC_TYPE(uint32_t, ::Json::UInt);
_SPB_DEFINE_NUMERIC_TYPE(int64_t,  ::Json::Int64);
_SPB_DEFINE_NUMERIC_TYPE(uint64_t, ::Json::UInt64);
_SPB_DEFINE_NUMERIC_TYPE(float,  double);
_SPB_DEFINE_NUMERIC_TYPE(double, double);



#define _SPB_DEFINE_LEN_NUMERIC_TYPE(_t, _json_type) \
template<class _LEN_TYPE> \
class _SPB_CLASS_WITH_LEN_TYPE(_t) : public SPB_CLASS_TYPE(numeric) \
{ \
public: \
    typedef _SPB_CLASS_WITH_LEN_TYPE(_t) this_type; \
    typedef _t data_type; \
    typedef _LEN_TYPE len_type; \
protected: \
    typedef char _sign_check[(len_type)-1]; \
    data_type _data; \
public: \
    virtual const char* _GetTypeName() const { return "len_" #_t; } \
public: \
    _SPB_CLASS_WITH_LEN_TYPE(_t)(data_type value=0){ set(value); } \
    virtual size_type ArraySize() const \
    { \
        return sizeof(len_type)+sizeof(data_type); \
    } \
    virtual int ParseFromArray(const void* pBuf, size_t nLen) \
    { \
        if (nLen < sizeof(len_type)+sizeof(data_type))return -1; \
        len_type l = _proto_decode_from_buffer<len_type>(pBuf); \
        if (l != sizeof(data_type))return -2; \
        _data = _proto_decode_from_buffer<data_type>(((const char*)pBuf)+sizeof(len_type)); \
        return sizeof(len_type)+sizeof(data_type); \
    } \
    virtual int SerializeToString(::std::string& strData) const \
    { \
        int iRet = _proto_encode_to_string((len_type)sizeof(data_type), strData); \
        if (iRet < 0)return iRet; \
        iRet = _proto_encode_append_string(_data, strData); \
        if (iRet < 0)return iRet; \
        return sizeof(len_type)+sizeof(data_type); \
    } \
    virtual int SerializeToArray(void* pBuf, size_t nLen) const \
    { \
        if (nLen < sizeof(len_type)+sizeof(data_type)) return -1; \
        int iRet = _proto_encode_to_buffer((len_type)sizeof(data_type), pBuf); \
        if (iRet < 0)return iRet; \
        iRet = _proto_encode_to_buffer(_data, ((char*)pBuf)+sizeof(len_type)); \
        if (iRet < 0)return iRet; \
        return sizeof(len_type)+sizeof(data_type); \
    } \
    virtual int AppendToString(::std::string& strData) const \
    { \
        int iRet = _proto_encode_append_string((len_type)sizeof(data_type), strData); \
        if (iRet < 0)return iRet; \
        iRet = _proto_encode_append_string(_data, strData); \
        if (iRet < 0)return iRet; \
        return sizeof(len_type)+sizeof(data_type); \
    } \
    virtual void clear() { _data = 0; } \
    inline this_type& assign(data_type value) \
    { \
        _data = value; \
        return *this; \
    } \
    inline this_type& set(data_type value) \
    { \
        _data = value; \
        return *this; \
    } \
    inline this_type& operator()(data_type value) \
    { \
        _data = value; \
        return *this; \
    } \
    inline this_type& operator=(data_type value) \
    { \
        _data = value; \
        return *this; \
    } \
    inline const data_type& get() const \
    { \
        return _data; \
    } \
    inline data_type& get() \
    { \
        return _data; \
    } \
    inline operator data_type() const \
    { \
        return get(); \
    } \
    virtual void SetNumeric(int8_t n){ set((data_type)n); } \
    virtual void SetNumeric(int16_t n){ set((data_type)n); } \
    virtual void SetNumeric(int32_t n){ set((data_type)n); } \
    virtual void SetNumeric(int64_t n){ set((data_type)n); } \
    virtual void SetNumeric(uint8_t n){ set((data_type)n); } \
    virtual void SetNumeric(uint16_t n){ set((data_type)n); } \
    virtual void SetNumeric(uint32_t n){ set((data_type)n); } \
    virtual void SetNumeric(uint64_t n){ set((data_type)n); } \
    virtual void SetNumeric(float n){ set((data_type)n); } \
    virtual void SetNumeric(double n){ set((data_type)n); } \
    virtual int64_t GetNumeric(){ return (int64_t)_data;} \
    virtual uint64_t GetUNumeric(){ return (uint64_t)_data;} \
    virtual double GetDNumeric(){ return (double)_data;} \
    virtual ::std::string DebugString(const ::std::string& tab="") const \
    { \
        return _any_to_str(_data); \
    } \
    _SPB_DEFINE_NUMERIC_JSON_METHOD(_json_type) \
}


_SPB_DEFINE_LEN_NUMERIC_TYPE(int8_t,   ::Json::Int);
_SPB_DEFINE_LEN_NUMERIC_TYPE(uint8_t,  ::Json::UInt);
_SPB_DEFINE_LEN_NUMERIC_TYPE(int16_t,  ::Json::Int);
_SPB_DEFINE_LEN_NUMERIC_TYPE(uint16_t, ::Json::UInt);
_SPB_DEFINE_LEN_NUMERIC_TYPE(int32_t,  ::Json::Int);
_SPB_DEFINE_LEN_NUMERIC_TYPE(uint32_t, ::Json::UInt);
_SPB_DEFINE_LEN_NUMERIC_TYPE(int64_t,  ::Json::Int64);
_SPB_DEFINE_LEN_NUMERIC_TYPE(uint64_t, ::Json::UInt64);
_SPB_DEFINE_LEN_NUMERIC_TYPE(float,  double);
_SPB_DEFINE_LEN_NUMERIC_TYPE(double, double);



/////////////////////////////////////////////////////////////////////////////////


#ifdef JSON_JSON_H_INCLUDED
#define _SPB_DEFINE_HEX_JSON_METHOD() \
    virtual int toJson(::Json::Value& json) const \
    { \
        char s[(sizeof(uint64_t)<<1) + 1] = {0}; \
        snprintf(s, sizeof(s), "%0*lx", sizeof(data_type), this->_data); \
        json = s; \
        return 0; \
    }
#else
#define _SPB_DEFINE_HEX_JSON_METHOD()
#endif

#define _SPB_DEFINE_HEX_TYPE(_name, _type) \
class SPB_CLASS_TYPE(_name) : public SPB_CLASS_TYPE(_type) \
{ \
public: \
    typedef SPB_CLASS_TYPE(_name) this_type; \
    typedef _type data_type; \
public: \
    virtual const char* _GetTypeName() const{return #_name;} \
    template<class T1> \
    inline this_type& operator=(T1 t1) \
    { \
        SPB_CLASS_TYPE(_type)::operator=(t1); \
        return *this; \
    } \
public: \
    virtual ::std::string DebugString(const ::std::string& tab="") const \
    { \
        char s[(sizeof(uint64_t)<<1) + 3] = {0}; \
        snprintf(s, sizeof(s), "0x%0*lx", sizeof(data_type), this->_data); \
        return s; \
    } \
    _SPB_DEFINE_HEX_JSON_METHOD() \
}; \
template <class _LEN_TYPE> \
class _SPB_CLASS_WITH_LEN_TYPE(_name) : public _SPB_CLASS_WITH_LEN_TYPE(_type)<_LEN_TYPE> \
{ \
public: \
    typedef _SPB_CLASS_WITH_LEN_TYPE(_name) this_type; \
    typedef _type data_type; \
    typedef _LEN_TYPE len_type; \
public: \
    virtual const char* _GetTypeName() const{return "len_ "#_name;} \
    template<class T1> \
    inline this_type& operator=(T1 t1) \
    { \
        _SPB_CLASS_WITH_LEN_TYPE(_type)<_LEN_TYPE>::operator=(t1); \
        return *this; \
    } \
public: \
    virtual ::std::string DebugString(const ::std::string& tab="") const \
    { \
        char s[(sizeof(uint64_t)<<1) + 3] = {0}; \
        snprintf(s, sizeof(s), "0x%0*lx", sizeof(data_type), this->_data); \
        return s; \
    } \
    _SPB_DEFINE_HEX_JSON_METHOD() \
}


_SPB_DEFINE_HEX_TYPE(hex1, uint8_t);
_SPB_DEFINE_HEX_TYPE(hex2, uint16_t);
_SPB_DEFINE_HEX_TYPE(hex4, uint32_t);
_SPB_DEFINE_HEX_TYPE(hex8, uint64_t);


/////////////////////////////////////////////////////////////////////////////////


template <int _BUF_LEN=4, class _CHAR_TYPE=char>
class SPB_CLASS_TYPE(buffer) : public IStructuredProtoBuffer
{
public:
    enum { BUF_LEN = _BUF_LEN, } ;
    typedef SPB_CLASS_TYPE(buffer) this_type;
    typedef _CHAR_TYPE char_type;
    typedef char_type data_type[BUF_LEN];
    typedef ::std::basic_string<char_type> string_type;
protected:
    data_type _data;
public:
    virtual const char* _GetTypeName() const
    {
        static char s[32] = {0};
        snprintf(s, sizeof(s), "buffer<%d>", BUF_LEN);
        return s;
    }
public:
    SPB_CLASS_TYPE(buffer)(char_type ch=0){ set(ch); }
    virtual bool _IsStringType() const {return true;}
    virtual bool _IsBasicType() const {return true;}
    virtual size_type ArraySize() const
    {
        return sizeof(_data);
    }
    virtual int ParseFromArray(const void* pBuf, size_t nLen)
    {
        if (nLen < sizeof(_data))return -1;
        memcpy(_data, pBuf, sizeof(_data));
        return sizeof(_data);
    }
    virtual int SerializeToString(::std::string& strData) const
    {
        strData.assign((char*)_data, sizeof(_data));
        return sizeof(_data);
    }
    virtual int SerializeToArray(void* pBuf, size_t nLen) const
    {
        if (nLen < sizeof(_data))return -1;
        memcpy(pBuf, _data, sizeof(_data));
        return sizeof(_data);
    }
    virtual int AppendToString(::std::string& strData) const
    {
        strData.append((char*)_data, sizeof(_data));
        return sizeof(_data);
    }

    virtual void clear() { set(0); }
    inline size_type size() const{ return BUF_LEN; }
    
    inline char_type& at(size_type index)
    {
        if (index >= BUF_LEN) throw ::std::out_of_range("out of buffer range");
        return _data[index];
    }
    inline const char_type& at(size_type index) const
    {
        if (index >= BUF_LEN) throw ::std::out_of_range("out of buffer range");
        return _data[index];
    }
    inline char_type& get(size_type index)
    {
        if (index >= BUF_LEN) throw ::std::out_of_range("out of buffer range");
        return _data[index];
    }
    inline const char_type& get(size_type index) const
    {
        if (index >= BUF_LEN) throw ::std::out_of_range("out of buffer range");
        return _data[index];
    }
    
    inline data_type& get()
    {
        return _data;
    }
    inline const data_type& get() const
    {
        return _data;
    }
    
    inline char_type& operator[](size_type index)
    {
        return _data[index];
    }
    inline const char_type& operator[](size_type index) const
    {
        return _data[index];
    }
    
    inline this_type& set(const data_type& arr)
    {
        memcpy(_data, arr, sizeof(_data));
        return *this;
    }
    inline this_type& set(const string_type& str, size_t subpos=0, size_t sublen=string_type::npos)
    {
        memset(_data, 0, sizeof(_data));
        size_t l = sublen;
        if (subpos >= str.size())
        {
            l = 0;
        }
        else if (l == string_type::npos)
        {
            l = str.size() - subpos;
        }
        memcpy(_data, str.data()+subpos, sizeof(char_type)*(l<BUF_LEN?l:BUF_LEN));
        return *this;
    }
    inline this_type& set(char_type ch, size_t n=string_type::npos)
    {
        for (size_t i = 0; i < BUF_LEN; ++i)
        {
            _data[i] = i < n ? ch : 0;
        }
        return *this;
    }
    inline this_type& fill(char_type ch, size_t n=string_type::npos)
    {
        for (size_t i = 0; i < BUF_LEN; ++i)
        {
            _data[i] = i < n ? ch : 0;
        }
        return *this;
    }
    template <class T1> inline this_type& assign(T1 t1){ return set(t1); }
    template <class T1, class T2> inline this_type& assign(T1 t1, T2 t2){ return set(t1, t2); }
    template <class T1, class T2, class T3> inline this_type& assign(T1 t1, T2 t2, T3 t3){ return set(t1, t2, t3); }
    template <class T1> inline this_type& operator()(T1 t1){ return set(t1); }
    template <class T1, class T2> inline this_type& operator()(T1 t1, T2 t2){ return set(t1, t2); }
    template <class T1, class T2, class T3> inline this_type& operator()(T1 t1, T2 t2, T3 t3){ return set(t1, t2, t3); }
    template <class T1> inline this_type& operator=(T1 t1){ return set(t1); }

    virtual ::std::string DebugString(const ::std::string& tab="") const
    {
        return _buf_debug_str(_data, BUF_LEN);
    }
    
#ifdef JSON_JSON_H_INCLUDED
    virtual int toJson(::Json::Value& json) const
    {
        json = ::std::string((const char*)_data, sizeof(_data));
        return 0;
    }
#endif
};



/////////////////////////////////////////////////////////////////////////////////


template <class _LEN_TYPE>
class SPB_CLASS_TYPE(len_string) : public IStructuredProtoBuffer, public ::std::string
{
public:
    typedef SPB_CLASS_TYPE(len_string) this_type;
    typedef _LEN_TYPE len_type;
    typedef ::std::string string_type;
    typedef string_type data_type;
    typedef IStructuredProtoBuffer::size_type size_type;
    virtual const char* _GetTypeName() const
    {
        static char s[32] = {0};
        snprintf(s, sizeof(s), "string%d", sizeof(len_type));
        return s;
    }
protected:
    typedef char _sign_check[(len_type)-1];
public:
    SPB_CLASS_TYPE(len_string)() {}
    template<class T1>
    SPB_CLASS_TYPE(len_string)(T1 t1) : ::std::string(t1) {}
    template<class T1, class T2>
    SPB_CLASS_TYPE(len_string)(T1 t1, T2 t2) : ::std::string(t1, t2) {}
    template<class T1, class T2, class T3>
    SPB_CLASS_TYPE(len_string)(T1 t1, T2 t2, T3 t3) : ::std::string(t1, t2, t3) {}
    
    virtual bool _IsStringType() const {return true;}
    virtual bool _IsBasicType() const {return true;}
    virtual bool _IsBasicWithLenType() const {return true;}
    
    virtual size_type ArraySize() const
    {
        return string_type::size() + sizeof(len_type);
    }
    
    virtual int ParseFromArray(const void* pBuf, size_t nLen)
    {
        if (nLen < sizeof(len_type)) return -1;
        len_type n = _proto_decode_from_buffer<len_type>((const char*)pBuf);
        if ((int)nLen-sizeof(len_type) < (int)n)return -1;
        string_type::assign(((const char*)pBuf)+sizeof(len_type), (size_t)n);
        return (int)(n + sizeof(len_type));
    }
    virtual int SerializeToString(::std::string& strData) const
    {
        int iRet = _proto_encode_to_string<len_type>(string_type::size(), strData);
        if (iRet < 0)return iRet;
        strData.append(string_type::data(), string_type::size());
        return strData.size();
    }
    virtual int SerializeToArray(void* pBuf, size_t nLen) const
    {
        if (nLen < sizeof(len_type)+string_type::size()) return -1;
        int iRet = _proto_encode_to_buffer<len_type>(string_type::size(), pBuf);
        if (iRet < 0)return iRet;
        memcpy(((char*)pBuf)+sizeof(len_type), string_type::data(), string_type::size());
        return sizeof(len_type)+string_type::size();
    }
    virtual int AppendToString(::std::string& strData) const
    {
        int iRet = _proto_encode_append_string<len_type>(string_type::size(), strData);
        if (iRet < 0)return iRet;
        strData.append(string_type::data(), string_type::size());
        return string_type::size() + sizeof(len_type);
    }

    virtual void clear() { string_type::clear(); }
    
    inline char& get(size_type index)
    {
        return string_type::at(index);
    }
    inline const char& get(size_type index) const
    {
        return string_type::at(index);
    }
    inline char& operator[](size_type index)
    {
        return string_type::operator[](index);
    }
    inline const char& operator[](size_type index) const
    {
        return string_type::operator[](index);
    }
    inline string_type& get()
    {
        return *((string_type*)this);
    }
    inline const string_type& get() const
    {
        return *((string_type*)this);
    }
    template<class T1>
    inline this_type& set(T1 t1)
    {
        string_type::assign(t1);
        return *this;
    }
    template<class T1, class T2>
    inline this_type& set(T1 t1, T2 t2)
    {
        string_type::assign(t1, t2);
        return *this;
    }
    template<class T1, class T2, class T3>
    inline this_type& set(T1 t1, T2 t2, T3 t3)
    {
        string_type::assign(t1, t2, t3);
        return *this;
    }
    template<class T1>
    inline void operator()(T1 t1)
    {
        string_type::assign(t1);
    }
    template<class T1, class T2>
    inline void operator()(T1 t1, T2 t2)
    {
        string_type::assign(t1, t2);
    }
    template<class T1, class T2, class T3>
    inline void operator()(T1 t1, T2 t2, T3 t3)
    {
        string_type::assign(t1, t2, t3);
    }
    template<class T1>
    inline this_type& operator=(T1 t1)
    {
        string_type::assign(t1);
        return *this;
    }
    template<class T1>
    inline this_type& operator+=(T1 t1)
    {
        string_type::append(t1);
        return *this;
    }
    
    inline operator string_type() const
    {
        return get();
    }
    inline operator string_type&()
    {
        return get();
    }
    inline operator const string_type&() const
    {
        return get();
    }
    
    virtual ::std::string DebugString(const ::std::string& tab="") const
    {
        return _buf_debug_str(string_type::data(), string_type::size());
    }

#ifdef JSON_JSON_H_INCLUDED
    virtual int toJson(::Json::Value& json) const
    {
        json = get();
        return 0;
    }
#endif
};

typedef SPB_CLASS_TYPE(len_string<uint8_t>)  SPB_CLASS_TYPE(string1), SPB_CLASS_TYPE(str1);
typedef SPB_CLASS_TYPE(len_string<uint16_t>) SPB_CLASS_TYPE(string2), SPB_CLASS_TYPE(str2);
typedef SPB_CLASS_TYPE(len_string<uint32_t>) SPB_CLASS_TYPE(string4), SPB_CLASS_TYPE(str4);
typedef SPB_CLASS_TYPE(len_string<uint64_t>) SPB_CLASS_TYPE(string8), SPB_CLASS_TYPE(str8);


/////////////////////////////////////////////////////////////////////////////////

template <class _LEN_TYPE, class _CONTAINER_TYPE>
class SPB_CLASS_TYPE(len_container) : public IStructuredProtoBuffer, public _CONTAINER_TYPE
{
public:
    typedef SPB_CLASS_TYPE(len_container) this_type;
    typedef _LEN_TYPE len_type;
    typedef _CONTAINER_TYPE container_type;
    typedef typename container_type::value_type pkg_type;
    typedef pkg_type  data_type;
    typedef IStructuredProtoBuffer::size_type size_type;
protected:
    typedef char _sign_check[(len_type)-1]; \
public:
    virtual const char* _GetTypeName() const
    {
        static char s[1024] = {0};
        pkg_type tmp;
        snprintf(s, sizeof(s), "container%d<%s>", sizeof(len_type), tmp._GetTypeName());
        return s;
    }
public:
    virtual bool _IsContainerType() const {return true;}
    virtual size_type ArraySize() const
    {
        size_t r = sizeof(len_type);
        for (typename container_type::const_iterator it = container_type::begin(); it != container_type::end(); ++it)
        {
            r += it->ArraySize();
        }
        return r;
    }
    virtual int ParseFromArray(const void* pBuf, size_t nLen)
    {
        if (nLen < sizeof(len_type))return -1;
        len_type n = _proto_decode_from_buffer<len_type>(pBuf);
        int iRet = sizeof(len_type);
        container_type::resize(n);
        for (typename container_type::iterator it = container_type::begin(); it != container_type::end(); ++it)
        {
            if ((int)nLen-iRet < 0)return -1;
            int iPkgRet = it->ParseFromArray(((const char*)pBuf)+iRet, nLen-iRet);
            if (iPkgRet < 0)
            {
                container_type::clear();
                return iPkgRet;
            }
            iRet += iPkgRet;
        }
        return iRet;
    }
    virtual int SerializeToString(::std::string& strData) const
    {
        int iRet = _proto_encode_to_string((len_type)container_type::size(), strData);
        if (iRet < 0)return iRet;
        for (typename container_type::const_iterator it = container_type::begin(); it != container_type::end(); ++it)
        {
            iRet = it->AppendToString(strData);
            if (iRet < 0)return iRet;
        }
        return strData.size();
    }
    virtual int SerializeToArray(void* pBuf, size_t nLen) const
    {
        if (nLen < sizeof(len_type))return -1;
        int iRet = _proto_encode_to_buffer((len_type)container_type::size(), pBuf);
        if (iRet < 0)return iRet;
        for (typename container_type::const_iterator it = container_type::begin(); it != container_type::end(); ++it)
        {
            int iPkgRet = it->SerializeToArray(((char*)pBuf)+iRet, nLen-iRet);
            if (iPkgRet < 0)return iPkgRet;
            iRet += iPkgRet;
        }
        return iRet;
    }
    virtual int AppendToString(::std::string& strData) const
    {
        int iRet = _proto_encode_append_string((len_type)container_type::size(), strData);
        if (iRet < 0)return iRet;
        for (typename container_type::const_iterator it = container_type::begin(); it != container_type::end(); ++it)
        {
            int iPkgRet = it->AppendToString(strData);
            if (iPkgRet < 0)return iPkgRet;
            iRet += iPkgRet;
        }
        return iRet;
    }
    
    virtual void clear()
    {
        container_type::clear();
    }
    
    inline pkg_type& append()
    {
        container_type::resize(container_type::size()+1);
        return *(container_type::rbegin());
    }
    template<class T1>
    inline this_type& operator+=(T1 t1)
    {
        this->append() = t1;
        return *this;
    }
    template<class T1>
    inline this_type& operator<<(T1 t1)
    {
        this->append() = t1;
        return *this;
    }

    virtual ::std::string DebugString(const ::std::string& tab="") const
    {
        ::std::string ret = "[\n";
        size_t i = 0;
        for (typename container_type::const_iterator it = container_type::begin(); it != container_type::end(); ++it)
        {
            ret += tab + _SPB_TAB_STRING + _any_to_str(i) + " : " + it->DebugString(tab + _SPB_TAB_STRING) + "\n";
            i++;
        }
        ret += tab + "]";
        return ret;
    }
    
#ifdef JSON_JSON_H_INCLUDED
    virtual int toJson(::Json::Value& json) const
    {
        json = ::Json::Value(::Json::arrayValue);
        for (typename container_type::const_iterator it = container_type::begin(); it != container_type::end(); ++it)
        {
            it->toJson( json[json.size()] );
        }
        return 0;
    }
#endif
};



template <class _LEN_TYPE, class _PKG_TYPE>
class SPB_CLASS_TYPE(len_list) : public SPB_CLASS_TYPE(len_container)< _LEN_TYPE, ::std::list<_PKG_TYPE> >
{
public:
    typedef SPB_CLASS_TYPE(len_list) this_type;
    typedef _PKG_TYPE pkg_type;
    typedef _LEN_TYPE len_type;
    typedef pkg_type  data_type;
    typedef IStructuredProtoBuffer::size_type size_type;
    virtual const char* _GetTypeName() const
    {
        static char s[1024] = {0};
        pkg_type tmp;
        snprintf(s, sizeof(s), "list%d<%s>", sizeof(len_type), tmp._GetTypeName());
        return s;
    }
    template<class T1>
    inline this_type& operator+=(T1 t1)
    {
        this->append() = t1;
        return *this;
    }
    template<class T1>
    inline this_type& operator<<(T1 t1)
    {
        this->append() = t1;
        return *this;
    }
};

template <class _LEN_TYPE, class _PKG_TYPE>
class SPB_CLASS_TYPE(len_vector) : public SPB_CLASS_TYPE(len_container)< _LEN_TYPE, ::std::vector<_PKG_TYPE> >
{
public:
    typedef SPB_CLASS_TYPE(len_vector) this_type;
    typedef _PKG_TYPE pkg_type;
    typedef _LEN_TYPE len_type;
    typedef pkg_type  data_type;
    typedef IStructuredProtoBuffer::size_type size_type;
    virtual const char* _GetTypeName() const
    {
        static char s[1024] = {0};
        pkg_type tmp;
        snprintf(s, sizeof(s), "vector%d<%s>", sizeof(len_type), tmp._GetTypeName());
        return s;
    }
    template<class T1>
    inline this_type& operator+=(T1 t1)
    {
        this->append() = t1;
        return *this;
    }
    template<class T1>
    inline this_type& operator<<(T1 t1)
    {
        this->append() = t1;
        return *this;
    }
};

template <class _LEN_TYPE, class _PKG_TYPE>
class SPB_CLASS_TYPE(len_deque) : public SPB_CLASS_TYPE(len_container)< _LEN_TYPE, ::std::deque<_PKG_TYPE> >
{
public:
    typedef SPB_CLASS_TYPE(len_deque) this_type;
    typedef _PKG_TYPE pkg_type;
    typedef _LEN_TYPE len_type;
    typedef pkg_type  data_type;
    typedef IStructuredProtoBuffer::size_type size_type;
    virtual const char* _GetTypeName() const
    {
        static char s[1024] = {0};
        pkg_type tmp;
        snprintf(s, sizeof(s), "deque%d<%s>", sizeof(len_type), tmp._GetTypeName());
        return s;
    }
    template<class T1>
    inline this_type& operator+=(T1 t1)
    {
        this->append() = t1;
        return *this;
    }
    template<class T1>
    inline this_type& operator<<(T1 t1)
    {
        this->append() = t1;
        return *this;
    }
};


template <class _LEN_TYPE, class _PKG_TYPE>
class SPB_CLASS_TYPE(len_map) : public IStructuredProtoBuffer, public ::std::map< typename _PKG_TYPE::key_type, _PKG_TYPE >
{
public:
    typedef SPB_CLASS_TYPE(len_map) this_type;
    typedef _LEN_TYPE len_type;
    typedef ::std::map< typename _PKG_TYPE::key_type, _PKG_TYPE > container_type;
    typedef _PKG_TYPE pkg_type;
    typedef typename pkg_type::key_type key_type;
    typedef pkg_type  data_type;
    typedef IStructuredProtoBuffer::size_type size_type;
protected:
    typedef char _sign_check[(len_type)-1]; \
public:
    virtual const char* _GetTypeName() const
    {
        static char s[1024] = {0};
        pkg_type tmp;
        snprintf(s, sizeof(s), "map%d<%s>", sizeof(len_type), tmp._GetTypeName());
        return s;
    }
public:
    virtual bool _IsContainerType() const {return true;}
    virtual size_type ArraySize() const
    {
        size_t r = sizeof(len_type);
        for (typename container_type::const_iterator it = container_type::begin(); it != container_type::end(); ++it)
        {
            r += it->second.ArraySize();
        }
        return r;
    }
    virtual int ParseFromArray(const void* pBuf, size_t nLen)
    {
        if (nLen < sizeof(len_type))return -1;
        len_type n = _proto_decode_from_buffer<len_type>(pBuf);
        int iRet = sizeof(len_type);
        for (size_t i = 0; i < n; ++i)
        {
            if ((int)nLen-iRet < 0)return -1;
            pkg_type pkg;
            int iPkgRet = pkg.ParseFromArray(((const char*)pBuf)+iRet, nLen-iRet);
            if (iPkgRet < 0)
            {
                container_type::clear();
                return iPkgRet;
            }
            container_type::operator[](pkg._GetKey()) = pkg;
            iRet += iPkgRet;
        }
        return iRet;
    }
    virtual int SerializeToString(::std::string& strData) const
    {
        int iRet = _proto_encode_to_string((len_type)container_type::size(), strData);
        if (iRet < 0)return iRet;
        for (typename container_type::const_iterator it = container_type::begin(); it != container_type::end(); ++it)
        {
            iRet = it->second.AppendToString(strData);
            if (iRet < 0)return iRet;
        }
        return strData.size();
    }
    virtual int SerializeToArray(void* pBuf, size_t nLen) const
    {
        if (nLen < sizeof(len_type))return -1;
        int iRet = _proto_encode_to_buffer((len_type)container_type::size(), pBuf);
        if (iRet < 0)return iRet;
        for (typename container_type::const_iterator it = container_type::begin(); it != container_type::end(); ++it)
        {
            int iPkgRet = it->second.SerializeToArray(((char*)pBuf)+iRet, nLen-iRet);
            if (iPkgRet < 0)return iPkgRet;
            iRet += iPkgRet;
        }
        return iRet;
    }
    virtual int AppendToString(::std::string& strData) const
    {
        int iRet = _proto_encode_append_string((len_type)container_type::size(), strData);
        if (iRet < 0)return iRet;
        for (typename container_type::const_iterator it = container_type::begin(); it != container_type::end(); ++it)
        {
            int iPkgRet = it->second.AppendToString(strData);
            if (iPkgRet < 0)return iPkgRet;
            iRet += iPkgRet;
        }
        return iRet;
    }
    
    virtual void clear()
    {
        container_type::clear();
    }
    
    inline const pkg_type& get(const key_type& index) const
    {
        return container_type::operator[](index);
    }
    inline pkg_type& get(const key_type& index)
    {
        return container_type::operator[](index);
    }

    virtual ::std::string DebugString(const ::std::string& tab="") const
    {
        ::std::string ret = "{\n";
        for (typename container_type::const_iterator it = container_type::begin(); it != container_type::end(); ++it)
        {
            ret += tab + _SPB_TAB_STRING + _any_to_str(it->first) + " : " + it->second.DebugString(tab + _SPB_TAB_STRING) + "\n";
        }
        ret += tab + "}";
        return ret;
    }
    
#ifdef JSON_JSON_H_INCLUDED
    virtual int toJson(::Json::Value& json) const
    {
        json = ::Json::Value(::Json::objectValue);
        for (typename container_type::const_iterator it = container_type::begin(); it != container_type::end(); ++it)
        {
            it->second.toJson( json[_any_to_str(it->first)] );
        }
        return 0;
    }
#endif
};




class SPB_CLASS_TYPE(object) : public IStructuredProtoBuffer
{
public:
    virtual bool _IsObjectType() const {return true;}
    virtual const ::std::string DebugInfo(const ::std::string& tab="") const = 0;
#ifdef JSON_JSON_H_INCLUDED
    virtual const ::Json::Value& Info() const = 0;
#endif
};



/////////////////////////////////////////////////////////////////////////////////


}



/////////////////////////////////////////////////////////////////////////////////



#define SPB_JOIN_1(x, y) SPB_JOIN_2(x, y)
#define SPB_JOIN_2(x, y) SPB_JOIN_3(x, y)
#define SPB_JOIN_3(x, y) x##y


#ifdef JSON_JSON_H_INCLUDED

#define _SPB_DEFINE_TLV_DATA_TO_JSON_METHOD_DECLARE() \
virtual bool _DataToJson(const ::std::string& data, ::Json::Value& json) const = 0;

#define _SPB_DEFINE_TLV_DATA_TO_JSON_METHOD() \
    virtual bool _DataToJson(const ::std::string& data, ::Json::Value& json) const \
    { \
        cur_tag_id_type tmp; \
        if (tmp.ParseFromString(data) == data.size()) \
        { \
            tmp.toJson(json); \
            return true; \
        } \
        return false; \
    }

#else

#define _SPB_DEFINE_TLV_DATA_TO_JSON_METHOD_DECLARE()
#define _SPB_DEFINE_TLV_DATA_TO_JSON_METHOD()

#endif


#define SPB_BEGIN_TLV(_name, _tag_type, _len_type) \
namespace spb { \
class SPB_CLASS_TYPE(_name) : public IStructuredProtoBuffer \
{ \
public: \
    typedef SPB_CLASS_TYPE(_name) this_type; \
    typedef _tag_type tag_type; \
    typedef tag_type  key_type; \
    typedef _len_type len_type; \
protected: \
    typedef char _sign_check[(len_type)-1]; \
    tag_type _tag; \
    ::std::string _data; \
public: \
    virtual const char* _GetTypeName() const { return #_name; } \
public: \
    SPB_CLASS_TYPE(_name)(){ clear(); } \
    virtual size_type ArraySize() const \
    { \
        return sizeof(tag_type) + _data.size(); \
    } \
    virtual int ParseFromArray(const void* pBuf, size_t nLen) \
    { \
        if (nLen < sizeof(tag_type)+sizeof(len_type))return -1; \
        const char* pCur = ((const char*)pBuf)+sizeof(tag_type); \
        len_type c = _proto_decode_from_buffer<len_type>(pCur); \
        if ((int)nLen-sizeof(tag_type)-sizeof(len_type) < (int)c)return -1; \
        _data.assign(pCur, sizeof(len_type)+(size_t)c); \
        _tag = _proto_decode_from_buffer<tag_type>(pCur-sizeof(tag_type)); \
        return sizeof(tag_type)+sizeof(len_type) + c; \
    } \
    virtual int SerializeToString(::std::string& strData) const \
    { \
        int iRet = _proto_encode_to_string(_tag, strData); \
        if (iRet < 0) return iRet; \
        strData.append(_data.data(), _data.size()); \
        return strData.size(); \
    } \
    virtual int SerializeToArray(void* pBuf, size_t nLen) const \
    { \
        if (nLen < sizeof(tag_type)+_data.size()) return -1; \
        int iRet = _proto_encode_to_buffer(_tag, pBuf); \
        if (iRet < 0) return iRet; \
        memcpy(((char*)pBuf)+sizeof(tag_type), _data.data(), _data.size()); \
        return sizeof(tag_type)+_data.size(); \
    } \
    virtual int AppendToString(::std::string& strData) const \
    { \
        int iRet = _proto_encode_append_string(_tag, strData); \
        if (iRet < 0) return iRet; \
        strData.append(_data.data(), _data.size()); \
        return sizeof(tag_type)+_data.size(); \
    } \
    virtual void clear() { _tag = 0; _data.assign(sizeof(len_type), '\0'); } \
    inline tag_type tag() const \
    { \
        return _tag; \
    } \
    inline const tag_type& _GetKey() const \
    { \
        return _tag; \
    } \
    inline ::std::string _GetString() const \
    { \
        return _data.substr(sizeof(len_type)); \
    } \
    inline uint64_t _GetUValue() const \
    { \
        switch (_data.size()-sizeof(len_type)) \
        { \
        case 1: \
             return (uint8_t)_data[sizeof(len_type)]; \
        case 2: \
            return _proto_ntoh(*((uint16_t*)&(_data[sizeof(len_type)]))); \
        case 4: \
            return _proto_ntoh(*((uint32_t*)&(_data[sizeof(len_type)]))); \
        case 8: \
            return _proto_ntoh(*((uint64_t*)&(_data[sizeof(len_type)]))); \
        default: \
            return 0; \
        } \
    } \
    inline int64_t _GetValue() const \
    { \
        return (signed)(_GetUValue()); \
    } \
protected: \
    class __IMember \
    { \
    public: \
        inline ::std::string _GetString(const ::std::string& data) const \
        { \
            return data.substr(sizeof(len_type)); \
        } \
        inline uint64_t _GetUValue(const ::std::string& data) const \
        { \
            switch (data.size()-sizeof(len_type)) \
            { \
            case 1: \
                 return (uint8_t)data[sizeof(len_type)]; \
            case 2: \
                return _proto_ntoh(*((uint16_t*)&(data[sizeof(len_type)]))); \
            case 4: \
                return _proto_ntoh(*((uint32_t*)&(data[sizeof(len_type)]))); \
            case 8: \
                return _proto_ntoh(*((uint64_t*)&(data[sizeof(len_type)]))); \
            default: \
                return 0; \
            } \
        } \
        virtual tag_type _GetTag() const = 0; \
        virtual size_t _ClassSize() const = 0; \
        virtual const char* _GetTagName() const {return NULL;} \
        virtual ::std::string DebugString(const ::std::string& data) const = 0; \
        _SPB_DEFINE_TLV_DATA_TO_JSON_METHOD_DECLARE() \
    }; \
    size_t __begin_member;

    

#define SPB_DEF_TLV_TAG(_tag_id, _tag_id_type) \
public: \
    typedef _SPB_CLASS_WITH_LEN_TYPE(_tag_id_type)<len_type> tag_type_##_tag_id; \
    typedef tag_type_##_tag_id::data_type tag_data_type_##_tag_id; \
    template<class T1> \
    inline void set_##_tag_id(T1 v1) \
    { \
        tag_type_##_tag_id tmp; \
        tmp.set(v1); \
        tmp.SerializeToString(_data); \
        _tag = _tag_id; \
    } \
    template<class T1, class T2> \
    inline void set_##_tag_id(T1 v1, T2 v2) \
    { \
        tag_type_##_tag_id tmp; \
        tmp.set(v1, v2); \
        tmp.SerializeToString(_data); \
        _tag = _tag_id; \
    } \
    template<class T1, class T2, class T3> \
    inline void set_##_tag_id(T1 v1, T2 v2, T3 v3) \
    { \
        tag_type_##_tag_id tmp; \
        tmp.set(v1, v2, v3); \
        tmp.SerializeToString(_data); \
        _tag = _tag_id; \
    } \
    inline tag_data_type_##_tag_id get_##_tag_id() const \
    { \
        if (_tag != _tag_id) return tag_data_type_##_tag_id(); \
        tag_type_##_tag_id tmp; \
        if (tmp.ParseFromString(_data) == _data.size()) \
        { \
            return tmp.get(); \
        } \
        return tag_data_type_##_tag_id(); \
    } \
protected: \
    class _tag_mem_type_##_tag_id : public __IMember \
    { \
    protected: \
        typedef tag_type_##_tag_id cur_tag_id_type; \
    public: \
        virtual tag_type _GetTag() const { return _tag_id; } \
        virtual size_t _ClassSize() const { return ((sizeof(*this)+7)/8)*8; } \
        virtual const char* _GetTagName() const { return #_tag_id; } \
        virtual ::std::string DebugString(const ::std::string& data) const \
        { \
            cur_tag_id_type tmp; \
            if (tmp.ParseFromString(data) == data.size()) \
            { \
                return tmp.DebugString(); \
            } \
            return _any_to_str(_GetUValue(data)) + "/" \
                 + _buf_debug_str(_GetString(data).data(), _GetString(data).size()); \
        } \
        _SPB_DEFINE_TLV_DATA_TO_JSON_METHOD() \
    }; \
    _tag_mem_type_##_tag_id tag_##_tag_id; \
protected: \
    char __mem_type_align_##_tag_id[(8-(sizeof(_tag_mem_type_##_tag_id)%8))%8]; \
    
    
#define SPB_END_TLV() \
protected: \
    size_t __end_member; \
    inline const __IMember* _GetTagInterface(const tag_type& tag) const \
    { \
        static ::std::map< tag_type, const __IMember*> mapTags; \
        if (mapTags.empty()) \
        { \
            const __IMember* m = (__IMember*)(&(__begin_member)+1); \
            while ((size_t)m < (size_t)(&__end_member)) \
            { \
                mapTags[m->_GetTag()] = m; \
                m = (const __IMember*)(((char*)m) + m->_ClassSize()); \
            } \
        } \
        ::std::map< tag_type, const __IMember*>::const_iterator it = mapTags.find(tag); \
        if (mapTags.end() != it) \
        { \
            return it->second; \
        } \
        return NULL; \
    } \
public: \
    inline const char* _GetTagName(tag_type tag) const \
    { \
        const __IMember* m = _GetTagInterface(tag); \
        if (NULL != m)return m->_GetTagName(); \
        return NULL; \
    } \
public: \
    virtual ::std::string DebugString(const ::std::string& tab="") const \
    { \
        const __IMember* m = _GetTagInterface(_tag); \
        if (NULL != m) \
        { \
            const char* name = m->_GetTagName(); \
            if (NULL != name) \
            { \
                return ::std::string(name) + "=" + m->DebugString(_data); \
            } \
        } \
        return _any_to_str((uint64_t)(_tag)) + "=" \
             + _any_to_str(_GetUValue()) + "/" \
             + _buf_debug_str(_GetString().data(), _GetString().size()); \
    } \
    _SPB_DEFINE_TLV_JSON_METHOD() \
}; \
}


#ifdef JSON_JSON_H_INCLUDED
#define _SPB_DEFINE_TLV_JSON_METHOD() \
    virtual int toJson(::Json::Value& json) const \
    { \
        const __IMember* m = _GetTagInterface(_tag); \
        if (NULL != m) \
        { \
            const char* name = m->_GetTagName(); \
            if (NULL != name) \
            { \
                if (m->_DataToJson(_data, json["v"])) \
                { \
                    json["t"] = name; \
                    return 0; \
                } \
            } \
        } \
        json["t"] = _any_to_str((uint64_t)(_tag)); \
        json["s"] = _GetString(); \
        json["n"] = (::Json::UInt64)_GetUValue(); \
        return 0; \
    }
#else
#define _SPB_DEFINE_TLV_JSON_METHOD()
#endif



/////////////////////////////////////////////////////////////////////////////////



#define SPB_BEGIN_TLVSET(_name, _len_type, _tag_type, _tag_len_type) \
namespace spb { \
class SPB_CLASS_TYPE(_name) : public IStructuredProtoBuffer \
{ \
public: \
    typedef SPB_CLASS_TYPE(_name) this_type; \
    typedef _tag_type tag_type; \
    typedef _len_type len_type; \
    typedef _tag_len_type tag_len_type; \
    typedef ::std::map< tag_type, ::std::string > container_type; \
protected: \
    typedef char _sign_check[(len_type)-1]; \
    container_type _data; \
public: \
    virtual bool _IsContainerType() const {return true;} \
    virtual const char* _GetTypeName() const { return #_name; } \
public: \
    SPB_CLASS_TYPE(_name)(){ clear(); } \
    virtual size_type ArraySize() const \
    { \
        size_type r = sizeof(len_type); \
        for (container_type::const_iterator it = _data.begin(); it != _data.end(); ++it) \
        { \
            r += sizeof(tag_type) + it->second.size(); \
        } \
        return r; \
    } \
    virtual int ParseFromArray(const void* pBuf, size_t nLen) \
    { \
        if (nLen < sizeof(len_type))return -1; \
        len_type n = _proto_decode_from_buffer<len_type>(pBuf); \
        int iRet = sizeof(len_type); \
        for (size_t i = 0; i < n; ++i) \
        { \
            if ((int)nLen-iRet-sizeof(tag_type)-sizeof(tag_len_type) < 0)return -1; \
            tag_type tag = _proto_decode_from_buffer<tag_type>(((const char*)pBuf)+iRet); \
            iRet += sizeof(tag_type); \
            tag_len_type len = _proto_decode_from_buffer<tag_len_type>(((const char*)pBuf)+iRet); \
            if ((int)nLen-iRet-sizeof(tag_len_type)-len < 0)return -1; \
            _data[tag].assign(((const char*)pBuf)+iRet, sizeof(tag_len_type)+len); \
            iRet += sizeof(tag_len_type)+len; \
        } \
        return iRet; \
    } \
    virtual int SerializeToString(::std::string& strData) const \
    { \
        int iRet = _proto_encode_to_string((len_type)_data.size(), strData); \
        if (iRet < 0)return iRet; \
        for (typename container_type::const_iterator it = _data.begin(); it != _data.end(); ++it) \
        { \
            int r = _proto_encode_append_string((tag_type)it->first, strData); \
            if (r < 0)return r; \
            strData.append(it->second); \
        } \
        return strData.size(); \
    } \
    virtual int SerializeToArray(void* pBuf, size_t nLen) const \
    { \
        if (nLen < sizeof(len_type))return -1; \
        char* pCur = (char*)pBuf; \
        int iRet = _proto_encode_to_buffer((len_type)_data.size(), pCur); \
        if (iRet < 0)return iRet; \
        pCur += sizeof(len_type); \
        nLen -= sizeof(len_type); \
        for (typename container_type::const_iterator it = _data.begin(); it != _data.end(); ++it) \
        { \
            if (nLen < sizeof(tag_type)+it->second.size())return -1; \
            int r = _proto_encode_to_buffer((tag_type)it->first, pCur); \
            if (r < 0)return r; \
            memcpy(pCur+sizeof(tag_type), it->second.data(), it->second.size()); \
            pCur += sizeof(tag_type)+it->second.size(); \
            nLen -= sizeof(tag_type)+it->second.size(); \
        } \
        return (int)(((size_t)pCur)-((size_t)pBuf)); \
    } \
    virtual int AppendToString(::std::string& strData) const \
    { \
        int iRet = _proto_encode_append_string((len_type)_data.size(), strData); \
        if (iRet < 0)return iRet; \
        for (typename container_type::const_iterator it = _data.begin(); it != _data.end(); ++it) \
        { \
            int r = _proto_encode_append_string((tag_type)it->first, strData); \
            if (r < 0)return r; \
            strData.append(it->second); \
            iRet += sizeof(tag_type)+it->second.size(); \
        } \
        return iRet; \
    } \
    virtual void clear() { _data.clear(); } \
protected: \
    inline ::std::string _GetString(const ::std::string& s) const \
    { \
        return s.substr(sizeof(tag_len_type)); \
    } \
    inline uint64_t _GetUValue(const ::std::string& d) const \
    { \
        switch (d.size()-sizeof(tag_len_type)) \
        { \
        case 1: \
             return (uint8_t)d[sizeof(tag_len_type)]; \
        case 2: \
            return _proto_ntoh(*((uint16_t*)&(d[sizeof(tag_len_type)]))); \
        case 4: \
            return _proto_ntoh(*((uint32_t*)&(d[sizeof(tag_len_type)]))); \
        case 8: \
            return _proto_ntoh(*((uint64_t*)&(d[sizeof(tag_len_type)]))); \
        default: \
            return 0; \
        } \
    } \
    inline int64_t _GetValue(const ::std::string& s) const \
    { \
        return (signed)(_GetUValue(s)); \
    } \
public: \
    inline ::std::string _GetString(const tag_type& tag) const \
    { \
        container_type::const_iterator it = _data.find(tag); \
        if (it == _data.end()) return ""; \
        return _GetString(it->second); \
    } \
    inline uint64_t _GetUValue(const tag_type& tag) const \
    { \
        container_type::const_iterator it = _data.find(tag); \
        if (it == _data.end()) return 0; \
        return _GetUValue(it->second); \
    } \
    inline int64_t _GetValue(const tag_type& tag) const \
    { \
        container_type::const_iterator it = _data.find(tag); \
        if (it == _data.end()) return 0; \
        return _GetValue(it->second); \
    } \
protected: \
    class __IMember \
    { \
    public: \
        inline ::std::string _GetString(const ::std::string& data) const \
        { \
            return data.substr(sizeof(len_type)); \
        } \
        inline uint64_t _GetUValue(const ::std::string& data) const \
        { \
            switch (data.size()-sizeof(len_type)) \
            { \
            case 1: \
                 return (uint8_t)data[sizeof(len_type)]; \
            case 2: \
                return _proto_ntoh(*((uint16_t*)&(data[sizeof(len_type)]))); \
            case 4: \
                return _proto_ntoh(*((uint32_t*)&(data[sizeof(len_type)]))); \
            case 8: \
                return _proto_ntoh(*((uint64_t*)&(data[sizeof(len_type)]))); \
            default: \
                return 0; \
            } \
        } \
        virtual tag_type _GetTag() const = 0; \
        virtual size_t _ClassSize() const = 0; \
        virtual const char* _GetTagName() const {return NULL;} \
        virtual ::std::string DebugString(const ::std::string& data) const = 0; \
        _SPB_DEFINE_TLV_DATA_TO_JSON_METHOD_DECLARE() \
    }; \
    size_t __begin_member;
    

#define SPB_DEF_TLVSET_TAG(_tag_id, _tag_id_type) \
public: \
    typedef _SPB_CLASS_WITH_LEN_TYPE(_tag_id_type)<len_type> tag_type_##_tag_id; \
    typedef tag_type_##_tag_id::data_type tag_data_type_##_tag_id; \
    template<class T1> \
    inline void set_##_tag_id(T1 v1) \
    { \
        tag_type_##_tag_id tmp; \
        tmp.set(v1); \
        tmp.SerializeToString(_data[_tag_id]); \
    } \
    template<class T1, class T2> \
    inline void set_##_tag_id(T1 v1, T2 v2) \
    { \
        tag_type_##_tag_id tmp; \
        tmp.set(v1, v2); \
        tmp.SerializeToString(_data[_tag_id]); \
    } \
    template<class T1, class T2, class T3> \
    inline void set_##_tag_id(T1 v1, T2 v2, T3 v3) \
    { \
        tag_type_##_tag_id tmp; \
        tmp.set(v1, v2, v3); \
        tmp.SerializeToString(_data[_tag_id]); \
    } \
    inline tag_data_type_##_tag_id get_##_tag_id() const \
    { \
        container_type::const_iterator it = _data.find(_tag_id); \
        if (it == _data.end()) return tag_data_type_##_tag_id(); \
        tag_type_##_tag_id tmp; \
        if (tmp.ParseFromString(it->second) == it->second.size()) \
        { \
            return tmp.get(); \
        } \
        return tag_data_type_##_tag_id(); \
    } \
    inline bool has_##_tag_id() const \
    { \
        container_type::const_iterator it = _data.find(_tag_id); \
        if (it == _data.end()) return false; \
        return true; \
    } \
protected: \
    class _tag_mem_type_##_tag_id : public __IMember \
    { \
    protected: \
        typedef tag_type_##_tag_id cur_tag_id_type; \
    public: \
        virtual tag_type _GetTag() const { return _tag_id; } \
        virtual size_t _ClassSize() const { return ((sizeof(*this)+7)/8)*8; } \
        virtual const char* _GetTagName() const { return #_tag_id; } \
        virtual ::std::string DebugString(const ::std::string& data) const \
        { \
            cur_tag_id_type tmp; \
            if (tmp.ParseFromString(data) == data.size()) \
            { \
                return tmp.DebugString(); \
            } \
            return _any_to_str(_GetUValue(data)) + "/" \
                 + _buf_debug_str(_GetString(data).data(), _GetString(data).size()); \
        } \
        _SPB_DEFINE_TLV_DATA_TO_JSON_METHOD() \
    protected: \
        this_type* _parent; \
    public: \
        _tag_mem_type_##_tag_id() \
        { \
            _parent = (this_type*)(((char*)this)-((size_t)(&(((this_type*)8)->tag_##_tag_id))-8)); \
        } \
        template <class T1> \
        inline _tag_mem_type_##_tag_id& operator=(T1 t1) \
        { \
            _parent->set_##_tag_id(t1); \
            return *this; \
        } \
        template <class T1> \
        inline _tag_mem_type_##_tag_id& set(T1 t1) \
        { \
            _parent->set_##_tag_id(t1); \
            return *this; \
        } \
        template <class T1, class T2> \
        inline _tag_mem_type_##_tag_id& set(T1 t1, T2 t2) \
        { \
            _parent->set_##_tag_id(t1, t2); \
            return *this; \
        } \
        template <class T1, class T2, class T3> \
        inline _tag_mem_type_##_tag_id& set(T1 t1, T2 t2, T3 t3) \
        { \
            _parent->set_##_tag_id(t1, t2, t3); \
            return *this; \
        } \
        template <class T1> \
        inline _tag_mem_type_##_tag_id& assign(T1 t1) \
        { \
            _parent->set_##_tag_id(t1); \
            return *this; \
        } \
        template <class T1, class T2> \
        inline _tag_mem_type_##_tag_id& assign(T1 t1, T2 t2) \
        { \
            _parent->set_##_tag_id(t1, t2); \
            return *this; \
        } \
        template <class T1, class T2, class T3> \
        inline _tag_mem_type_##_tag_id& assign(T1 t1, T2 t2, T3 t3) \
        { \
            _parent->set_##_tag_id(t1, t2, t3); \
            return *this; \
        } \
        template <class T1> \
        inline _tag_mem_type_##_tag_id& operator()(T1 t1) \
        { \
            _parent->set_##_tag_id(t1); \
            return *this; \
        } \
        template <class T1, class T2> \
        inline _tag_mem_type_##_tag_id& operator()(T1 t1, T2 t2) \
        { \
            _parent->set_##_tag_id(t1, t2); \
            return *this; \
        } \
        template <class T1, class T2, class T3> \
        inline _tag_mem_type_##_tag_id& operator()(T1 t1, T2 t2, T3 t3) \
        { \
            _parent->set_##_tag_id(t1, t2, t3); \
            return *this; \
        } \
        inline tag_data_type_##_tag_id get() const \
        { \
            return _parent->get_##_tag_id(); \
        } \
        inline operator tag_data_type_##_tag_id() const \
        { \
            return _parent->get_##_tag_id(); \
        } \
        inline _tag_mem_type_##_tag_id& operator=(const _tag_mem_type_##_tag_id& other) \
        { \
            _parent = (this_type*)(((char*)this)-((size_t)(&(((this_type*)8)->tag_##_tag_id))-8)); \
            return *this; \
        } \
    }; \
public: \
    _tag_mem_type_##_tag_id tag_##_tag_id; \
protected: \
    char __mem_type_align_##_tag_id[(8-(sizeof(_tag_mem_type_##_tag_id)%8))%8]; \
    
    
#define SPB_END_TLVSET() \
protected: \
    size_t __end_member; \
    inline const __IMember* _GetTagInterface(const tag_type& tag) const \
    { \
        static ::std::map< tag_type, const __IMember*> mapTags; \
        if (mapTags.empty()) \
        { \
            const __IMember* m = (__IMember*)(&(__begin_member)+1); \
            while ((size_t)m < (size_t)(&__end_member)) \
            { \
                mapTags[m->_GetTag()] = m; \
                m = (const __IMember*)(((char*)m) + m->_ClassSize()); \
            } \
        } \
        ::std::map< tag_type, const __IMember*>::const_iterator it = mapTags.find(tag); \
        if (mapTags.end() != it) \
        { \
            return it->second; \
        } \
        return NULL; \
    } \
public: \
    inline const char* _GetTagName(tag_type tag) const \
    { \
        const __IMember* m = _GetTagInterface(tag); \
        if (NULL != m)return m->_GetTagName(); \
        return NULL; \
    } \
public: \
    virtual ::std::string DebugString(const ::std::string& tab="") const \
    { \
        ::std::string ret = "{\n"; \
        for (typename container_type::const_iterator it = _data.begin(); it != _data.end(); ++it) \
        { \
            const __IMember* m = _GetTagInterface(it->first); \
            if (NULL != m) \
            { \
                const char* name = m->_GetTagName(); \
                if (NULL != name) \
                { \
                    ret += tab + _SPB_TAB_STRING + name + " : " + m->DebugString(it->second) + "\n"; \
                    continue; \
                } \
            } \
            ret += tab + _SPB_TAB_STRING + _any_to_str((uint64_t)(it->first)) + " : " \
                 + _any_to_str(_GetUValue(it->second)) + "/" \
                 + _buf_debug_str(_GetString(it->second).data(), _GetString(it->second).size()) + "\n"; \
        } \
        ret += tab + "}"; \
        return ret;\
    } \
    _SPB_DEFINE_TLVSET_JSON_METHOD() \
}; \
}


#ifdef JSON_JSON_H_INCLUDED
#define _SPB_DEFINE_TLVSET_JSON_METHOD() \
    virtual int toJson(::Json::Value& json) const \
    { \
        json = ::Json::Value(::Json::objectValue); \
        for (typename container_type::const_iterator it = _data.begin(); it != _data.end(); ++it) \
        { \
            const __IMember* m = _GetTagInterface(it->first); \
            if (NULL != m) \
            { \
                const char* name = m->_GetTagName(); \
                if (NULL != name) \
                { \
                    if (m->_DataToJson(it->second, json[name]))continue; \
                } \
            } \
            ::Json::Value& v = json[_any_to_str((uint64_t)(it->first))]; \
            v["s"] = _GetString(it->second); \
            v["n"] = (::Json::UInt64)_GetUValue(it->second); \
        } \
        return 0; \
    }
#else
#define _SPB_DEFINE_TLVSET_JSON_METHOD()
#endif



/////////////////////////////////////////////////////////////////////////////////



#define SPB_BEGIN_OBJECT(_name) \
namespace spb { \
class SPB_CLASS_TYPE(_name) : public SPB_CLASS_TYPE(object) \
{ \
public: \
    typedef SPB_CLASS_TYPE(_name) this_type; \
    virtual const char* _GetTypeName() const { return #_name; } \
protected: \
    class __IMember \
    { \
    public: \
        virtual size_t _ClassSize() const = 0; \
        virtual const char* _GetMemberName() const {return NULL;} \
        virtual IStructuredProtoBuffer* _GetIStructuredProtoBuffer() = 0; \
        virtual const IStructuredProtoBuffer* _GetIStructuredProtoBuffer() const = 0; \
        virtual bool _IsMessageLenMem() const {return false;} \
    }; \
    ::std::string __ext_buffer; \
    SPB_CLASS_TYPE(numeric)* __len_mem; \
public: \
    SPB_CLASS_TYPE(_name)() \
    { \
        __len_mem = NULL; \
        __IMember* m = (__IMember*)(&(__begin_members)+1); \
        uint64_t len = 0; \
        while ((size_t)m < (size_t)(&__end_members)) \
        { \
            if (m->_IsMessageLenMem()) \
            { \
                __len_mem = (SPB_CLASS_TYPE(numeric)*)m->_GetIStructuredProtoBuffer(); \
                break; \
            } \
            m = (__IMember*)(((char*)m) + m->_ClassSize()); \
        } \
    } \
protected: \
    uint64_t __begin_members;

#define SPB_DEF_LEN_MEMBER(_type) \
protected: \
    class __len_type : public __IMember, public SPB_CLASS_TYPE(_type) \
    { \
    public: \
        virtual size_t _ClassSize() const { return ((sizeof(*this)+7)/8)*8; } \
        virtual IStructuredProtoBuffer* _GetIStructuredProtoBuffer() { return this; } \
        virtual const IStructuredProtoBuffer* _GetIStructuredProtoBuffer() const { return this; } \
        virtual bool _IsMessageLenMem() const {return true;} \
    } __msg_len; \
public: \
    ::std::string& GetExtendBuffer() { return __ext_buffer; } \
    const ::std::string& GetExtendBuffer() const { return __ext_buffer; } \
protected: \
    char __len_type_align[(8-(sizeof(__len_type)%8))%8];
    
#define SPB_DEF_MEMBER(_type, _name) \
protected: \
    class __mem_type_##_name : public __IMember, public SPB_CLASS_TYPE(_type) \
    { \
    public: \
        virtual size_t _ClassSize() const { return ((sizeof(*this)+7)/8)*8; } \
        virtual IStructuredProtoBuffer* _GetIStructuredProtoBuffer() { return this; } \
        virtual const IStructuredProtoBuffer* _GetIStructuredProtoBuffer() const { return this; } \
        template<class _OPT_EQ_TYPE> \
        __mem_type_##_name& operator=(const _OPT_EQ_TYPE& value) \
        { \
            SPB_CLASS_TYPE(_type)::operator=(value); \
            return *this; \
        } \
        virtual const char* _GetMemberName() const {return #_name;} \
    }; \
public: \
    __mem_type_##_name _name; \
protected: \
    char __mem_type_align_##_name[(8-(sizeof(__mem_type_##_name)%8))%8];


#define SPB_DEF_KEY_MEMBER(_type, _name) \
SPB_DEF_MEMBER(_type, _name) \
public: \
    typedef _type key_type; \
    inline const key_type& _GetKey() const \
    { \
        return _name.get(); \
    } \


#define SPB_DEF_CONTAINER_MEMBER(_container_type, _len_type, _type, _name) \
protected: \
    class __mem_type_##_name : public __IMember, public _SPB_CLASS_WITH_LEN_TYPE(_container_type)< _len_type, SPB_CLASS_TYPE(_type) > \
    { \
    public: \
        virtual size_t _ClassSize() const { return ((sizeof(*this)+7)/8)*8; } \
        virtual IStructuredProtoBuffer* _GetIStructuredProtoBuffer() { return this; } \
        virtual const IStructuredProtoBuffer* _GetIStructuredProtoBuffer() const { return this; } \
        virtual const char* _GetMemberName() const { return #_name; } \
    }; \
public: \
    __mem_type_##_name _name; \
protected: \
    char __mem_type_align_##_name[(8-(sizeof(__mem_type_##_name)%8))%8];



#define SPB_DEF_CONTAINER1_MEMBER(_container_type, _type, _name) SPB_DEF_CONTAINER_MEMBER(_container_type, uint8_t, _type, _name)
#define SPB_DEF_CONTAINER2_MEMBER(_container_type, _type, _name) SPB_DEF_CONTAINER_MEMBER(_container_type, uint16_t, _type, _name)
#define SPB_DEF_CONTAINER4_MEMBER(_container_type, _type, _name) SPB_DEF_CONTAINER_MEMBER(_container_type, uint32_t, _type, _name)
#define SPB_DEF_CONTAINER8_MEMBER(_container_type, _type, _name) SPB_DEF_CONTAINER_MEMBER(_container_type, uint64_t, _type, _name)

#define SPB_DEF_LIST1_MEMBER(_type, _name) SPB_DEF_CONTAINER1_MEMBER(list, _type, _name)
#define SPB_DEF_LIST2_MEMBER(_type, _name) SPB_DEF_CONTAINER2_MEMBER(list, _type, _name)
#define SPB_DEF_LIST4_MEMBER(_type, _name) SPB_DEF_CONTAINER4_MEMBER(list, _type, _name)
#define SPB_DEF_LIST8_MEMBER(_type, _name) SPB_DEF_CONTAINER8_MEMBER(list, _type, _name)

#define SPB_DEF_VECTOR1_MEMBER(_type, _name) SPB_DEF_CONTAINER1_MEMBER(vector, _type, _name)
#define SPB_DEF_VECTOR2_MEMBER(_type, _name) SPB_DEF_CONTAINER2_MEMBER(vector, _type, _name)
#define SPB_DEF_VECTOR4_MEMBER(_type, _name) SPB_DEF_CONTAINER4_MEMBER(vector, _type, _name)
#define SPB_DEF_VECTOR8_MEMBER(_type, _name) SPB_DEF_CONTAINER8_MEMBER(vector, _type, _name)

#define SPB_DEF_DEQUE1_MEMBER(_type, _name) SPB_DEF_CONTAINER1_MEMBER(deque, _type, _name)
#define SPB_DEF_DEQUE2_MEMBER(_type, _name) SPB_DEF_CONTAINER2_MEMBER(deque, _type, _name)
#define SPB_DEF_DEQUE4_MEMBER(_type, _name) SPB_DEF_CONTAINER4_MEMBER(deque, _type, _name)
#define SPB_DEF_DEQUE8_MEMBER(_type, _name) SPB_DEF_CONTAINER8_MEMBER(deque, _type, _name)


#define SPB_DEF_MAP_MEMBER(_len_type, _type, _name) \
protected: \
    class __mem_type_##_name : public __IMember, public SPB_CLASS_TYPE(len_map)< _len_type, SPB_CLASS_TYPE(_type) > \
    { \
    public: \
        virtual size_t _ClassSize() const { return ((sizeof(*this)+7)/8)*8; } \
        virtual IStructuredProtoBuffer* _GetIStructuredProtoBuffer() { return this; } \
        virtual const IStructuredProtoBuffer* _GetIStructuredProtoBuffer() const { return this; } \
        virtual const char* _GetMemberName() const { return #_name; } \
    }; \
public: \
    __mem_type_##_name _name; \
protected: \
    char __mem_type_align_##_name[(8-(sizeof(__mem_type_##_name)%8))%8];


#define SPB_DEF_MAP1_MEMBER(_type, _name) SPB_DEF_MAP_MEMBER(uint8_t,  _type, _name)
#define SPB_DEF_MAP2_MEMBER(_type, _name) SPB_DEF_MAP_MEMBER(uint16_t, _type, _name)
#define SPB_DEF_MAP4_MEMBER(_type, _name) SPB_DEF_MAP_MEMBER(uint32_t, _type, _name)
#define SPB_DEF_MAP8_MEMBER(_type, _name) SPB_DEF_MAP_MEMBER(uint64_t, _type, _name)


#define SPB_END_OBJECT() \
protected: \
    uint64_t __end_members; \
public: \
    virtual size_type ArraySize() const \
    { \
        size_t n = 0; \
        if (NULL != __len_mem)n = __ext_buffer.size(); \
        __IMember* m = (__IMember*)(&(__begin_members)+1); \
        while ((size_t)m < (size_t)(&__end_members)) \
        { \
            n += m->_GetIStructuredProtoBuffer()->ArraySize(); \
            m = (__IMember*)(((char*)m) + m->_ClassSize()); \
        } \
        return n; \
    } \
    virtual void clear() \
    { \
        __IMember* m = (__IMember*)(&(__begin_members)+1); \
        while ((size_t)m < (size_t)(&__end_members)) \
        { \
            m->_GetIStructuredProtoBuffer()->clear(); \
            m = (__IMember*)(((char*)m) + m->_ClassSize()); \
        } \
    } \
    virtual int ParseFromArray(const void* pBuf, size_t nLen) \
    { \
        SPB_CLASS_TYPE(numeric)* len_mem = NULL; \
        int iRet = 0; \
        __IMember* m = (__IMember*)(&(__begin_members)+1); \
        uint64_t len = 0; \
        while ((size_t)m < (size_t)(&__end_members)) \
        { \
            if (nLen < iRet)return -1; \
            if (NULL != len_mem && len < iRet) return -1; \
            int iPkgRet = m->_GetIStructuredProtoBuffer()->ParseFromArray(((const char*)pBuf)+iRet, nLen-iRet); \
            if (iPkgRet < 0)return iPkgRet; \
            if (m->_IsMessageLenMem()) \
            { \
                len_mem = (SPB_CLASS_TYPE(numeric)*)(m->_GetIStructuredProtoBuffer()); \
                len = len_mem->GetUNumeric(); \
                if (len > nLen) return -1; \
            } \
            iRet += iPkgRet; \
            m = (__IMember*)(((char*)m) + m->_ClassSize()); \
        } \
        if (NULL != len_mem) \
        { \
            __ext_buffer.assign(((const char*)pBuf)+iRet, len-iRet); \
            return (int)len; \
        } \
        return iRet; \
    } \
    virtual int SerializeToString(::std::string& strData) const \
    { \
        strData.clear(); \
        return AppendToString(strData); \
    } \
    virtual int SerializeToArray(void* pBuf, size_t nLen) const \
    { \
        if (NULL != __len_mem) \
        { \
            size_t len = this->ArraySize(); \
            if (len > nLen) return -1; \
            uint64_t max = (uint64_t)((1<<((__len_mem->ArraySize())<<3))-1); \
            if (len > max) return -1; \
            __len_mem->SetNumeric(len); \
        } \
        int iRet = 0; \
        __IMember* m = (__IMember*)(&(__begin_members)+1); \
        while ((size_t)m < (size_t)(&__end_members)) \
        { \
            int iPkgRet = m->_GetIStructuredProtoBuffer()->SerializeToArray(((char*)pBuf)+iRet, nLen-iRet); \
            if (iPkgRet < 0) return iPkgRet; \
            iRet += iPkgRet; \
            m = (__IMember*)(((char*)m) + m->_ClassSize()); \
        } \
        memcpy(((char*)pBuf)+iRet, __ext_buffer.data(), __ext_buffer.size()); \
        iRet += __ext_buffer.size(); \
        return iRet; \
    } \
    virtual int AppendToString(::std::string& strData) const \
    { \
        if (NULL != __len_mem) \
        { \
            size_t len = this->ArraySize(); \
            uint64_t max = (uint64_t)((1<<((__len_mem->ArraySize())<<3))-1); \
            if (len > max) return -1; \
            __len_mem->SetNumeric(len); \
        } \
        int iRet = 0; \
        __IMember* m = (__IMember*)(&(__begin_members)+1); \
        while ((size_t)m < (size_t)(&__end_members)) \
        { \
            int iPkgRet = m->_GetIStructuredProtoBuffer()->AppendToString(strData); \
            if (iPkgRet < 0) return iPkgRet; \
            iRet += iPkgRet; \
            m = (__IMember*)(((char*)m) + m->_ClassSize()); \
        } \
        strData.append(__ext_buffer); \
        iRet += __ext_buffer.size(); \
        return iRet; \
    } \
public: \
    virtual ::std::string DebugString(const ::std::string& tab="") const \
    { \
        ::std::string ret = "{\n"; \
        __IMember* m = (__IMember*)(&(__begin_members)+1); \
        while ((size_t)m < (size_t)(&__end_members)) \
        { \
            if (NULL != m->_GetMemberName()) \
            { \
                ret += tab + _SPB_TAB_STRING + m->_GetMemberName() + " : " + m->_GetIStructuredProtoBuffer()->DebugString(tab + _SPB_TAB_STRING) + "\n"; \
            } \
            m = (__IMember*)(((char*)m) + m->_ClassSize()); \
        } \
        ret += tab + "}"; \
        return ret; \
    } \
    virtual const ::std::string DebugInfo(const ::std::string& tab="") const \
    { \
        ::std::string ret = _GetTypeName(); \
        size_t offset = 0; \
        __IMember* m = (__IMember*)(&(__begin_members)+1); \
        while ((size_t)m < (size_t)(&__end_members)) \
        { \
            if (NULL != m->_GetMemberName()) \
            { \
                ret += "\n" + tab + _SPB_TAB_STRING + m->_GetMemberName() \
                     + "[" \
                     + m->_GetIStructuredProtoBuffer()->_GetTypeName() \
                     + "]" \
                       " : " \
                       "offset=" + _any_to_str(offset) \
                     + ", " \
                       "len=" + _any_to_str(m->_GetIStructuredProtoBuffer()->ArraySize()); \
                if (m->_GetIStructuredProtoBuffer()->_IsObjectType()) \
                { \
                    SPB_CLASS_TYPE(object)* obj = (SPB_CLASS_TYPE(object)*)(m->_GetIStructuredProtoBuffer()); \
                    ret += "\n" + tab + _SPB_TAB_STRING + obj->DebugInfo(tab + _SPB_TAB_STRING); \
                } \
            } \
            offset += m->_GetIStructuredProtoBuffer()->ArraySize(); \
            m = (__IMember*)(((char*)m) + m->_ClassSize()); \
        } \
        return ret; \
    } \
    _SPB_DEFINE_OBJECT_JSON_METHOD() \
}; \
}



#ifdef JSON_JSON_H_INCLUDED
#define _SPB_DEFINE_OBJECT_JSON_METHOD() \
    virtual int toJson(::Json::Value& json) const \
    { \
        json = ::Json::Value(::Json::objectValue); \
        __IMember* m = (__IMember*)(&(__begin_members)+1); \
        while ((size_t)m < (size_t)(&__end_members)) \
        { \
            if (NULL != m->_GetMemberName())m->_GetIStructuredProtoBuffer()->toJson(json[m->_GetMemberName()]); \
            m = (__IMember*)(((char*)m) + m->_ClassSize()); \
        } \
        return 0; \
    } \
    virtual const ::Json::Value& Info() const \
    { \
        static ::Json::Value v; \
        size_t offset = 0; \
        __IMember* m = (__IMember*)(&(__begin_members)+1); \
        while ((size_t)m < (size_t)(&__end_members)) \
        { \
            if (NULL != m->_GetMemberName()) \
            { \
                v[m->_GetMemberName()]["type"] = m->_GetIStructuredProtoBuffer()->_GetTypeName(); \
                v[m->_GetMemberName()]["len"] = (::Json::UInt64)m->_GetIStructuredProtoBuffer()->ArraySize(); \
                v[m->_GetMemberName()]["offset"] = (::Json::UInt64)offset; \
                if (m->_GetIStructuredProtoBuffer()->_IsObjectType()) \
                { \
                    SPB_CLASS_TYPE(object)* obj = (SPB_CLASS_TYPE(object)*)(m->_GetIStructuredProtoBuffer()); \
                    v[m->_GetMemberName()]["info"] = obj->Info(); \
                } \
            } \
            offset += m->_GetIStructuredProtoBuffer()->ArraySize(); \
            m = (__IMember*)(((char*)m) + m->_ClassSize()); \
        } \
        return v; \
    }
#else
#define _SPB_DEFINE_OBJECT_JSON_METHOD()
#endif



/////////////////////////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////////////////


#endif



