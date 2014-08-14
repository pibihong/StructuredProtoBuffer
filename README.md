StructuredProtoBuffer
=====================

A general library for package structured binary protocol: 
### 1.Support most data type 
### 2.Serialize or debug easily 
### 3.Friendly interface 
### 4.Support conver to json



License
-----------------------------------
Author: bihongpi<br/>
E-mail: pibihong@163.com<br/>
License: please read the license.txt<br/>



IStructuredProtoBuffer
-----------------------------------
		int ParseFromArray(const void* pBuf, size_t nLen)
		int ParseFromString(const ::std::string& strData)
Initialize struct data from a buffer or a string, return the data length parsed 
		
		int SerializeToString(::std::string& strData) const
		int SerializeToArray(void* pBuf, size_t nLen) const
Serialize struct data to a buffer or a string, return the data length serialized 
		int AppendToString(::std::string& strData) const
Append the serialized data to a string, return the data length serialized

		size_type ArraySize() const
Get the length of serialized data
		
		void clear()
Clear the data
		
		::std::string DebugString(const ::std::string& tab="") const
Get the debug string for debug easily
		
		operator ::Json::Value() const
		int toJson(::Json::Value& json) const
Use the interface to conver data to json data, need libjsoncpp
		

numeric : IStructuredProtoBuffer
-----------------------------------
		void SetNumeric(int8_t n)
		void SetNumeric(int16_t n)
		void SetNumeric(int32_t n)
		void SetNumeric(int64_t n)
		void SetNumeric(uint8_t n)
		void SetNumeric(uint16_t n)
		void SetNumeric(uint32_t n)
		void SetNumeric(uint64_t n)
		void SetNumeric(float n)
		void SetNumeric(double n)
Set the numeric value

		int64_t GetNumeric()
		uint64_t GetUNumeric()
		double GetDNumeric()
Get the numeric value


int8_t/int16_t/int32_t/int64_t : numeric
-----------------------------------
uint8_t/uint16_t/uint32_t/uint64_t : numeric
-----------------------------------
float/double : numeric
-----------------------------------
hex1/hex2/hex4/hex8 : numeric
-----------------------------------

hex data will show data in hex when call DebugString or toJson

		this_type& set(data_type value)
set the value, <b>assign / operator= / operator()</b> are all the same function

		const data_type& get() const
		data_type& get()
		operator data_type() const
get the value



buffer : IStructuredProtoBuffer
-----------------------------------
		size_type size() const
get the buffer size
		char_type& at(size_type index)
		const char_type& at(size_type index) const
		char_type& get(size_type index)
		const char_type& get(size_type index) const
get the char at index, throw exception when out of range

		char_type& operator[](size_type index)
		const char_type& operator[](size_type index) const
get the char at index, no throw exception when out of range

		data_type& get()
		const data_type& get() const
get the buffer array

		this_type& set(const data_type& arr)
		this_type& set(const string_type& str, size_t subpos=0, size_t sublen=string_type::npos)
		this_type& set(char_type ch, size_t n=string_type::npos)
set the buffer data, <b>assign / operator= / operator()</b> are all the same function

		this_type& fill(char_type ch, size_t n=string_type::npos)
fill the data with char



string1/string2/string4/string8 : IStructuredProtoBuffer
-----------------------------------

same use as std::string in STL

		char& get(size_type index)
		const char& get(size_type index) const
get the char at index, same function with std::string::at

		string_type& get()
		const string_type& get() const
		operator string_type() const
		operator string_type&()
		operator const string_type&() const
get the string_type

		this_type& set(...)
		this_type& operator()(...)
set the string, same function with std::string::assign
		

TLV  : IStructuredProtoBuffer
-----------------------------------

to store tag/length/value

		tag_type tag() const
get current tag id

		tag_id_type get_##_tag_id() const
get tag_id value, return default value when tag_id not match

		void set_##_tag_id(...)
change tag_id to it, and set the value, use any method support set method in basic_types



TLVSET  : IStructuredProtoBuffer
-----------------------------------

to store the set of TLV

		bool has_##_tag_id() const
check whether have the tag_id in this set

		tag_data_type_##_tag_id get_##_tag_id() const
get the value of tag_id

		void set_##_tag_id(...)
		void tag_##_tag_id.set(...)
		void tag_##_tag_id.assign(...)
		void tag_##_tag_id.operator=(...)
add tag_id to it, and set the value, use any method support set method in basic_types


list<len_type, pkg_type> : container : IStructuredProtoBuffer
-----------------------------------
vector<len_type, pkg_type> : container : IStructuredProtoBuffer
-----------------------------------
deque<len_type, pkg_type> : container : IStructuredProtoBuffer
-----------------------------------
same use as std::list/vector/deque in STL, do not use these struct direct, use macro to define them

		pkg_type& append()
		this_type& operator+=(T pkg_type)
		this_type& operator<<(T pkg_type)
append a pkg to the container, T is a type which can assign to the package



map<len_type, pkg_type> : IStructuredProtoBuffer
-----------------------------------
same use as std::map in STL, do not use these struct direct, use macro to define them</br>
the pkg_type can only be TLV or object type which have defined key member



object : IStructuredProtoBuffer
-----------------------------------
use macro to define an object, you can operator the member directly with the method on the member type

		const ::std::string DebugInfo(const ::std::string& tab="") const
		const ::Json::Value& Info() const
get the type/offset/size of the members on this message

		::std::string& GetExtendBuffer()
    const ::std::string& GetExtendBuffer() const
if you use SPB_DEF_LEN_MEMBER declare a length member in object, you can use this interface to get the extend buffer
    


How to declare a TLV type
-----------------------------------

		#define TAG_STR_ID 5  //no need to define, if you define this tag_name, you can access this tag_id by name
		#define TAG_ID_TYPE uint8_t //no need to define
		#define TAG_DATA_LEN_TYPE uint16_t //no need to define
		
		SPB_BEGIN_TLV(test_tlv, TAG_ID_TYPE, TAG_DATA_LEN_TYPE)
		    SPB_DEF_TLV_TAG(1, uint8_t)
		    SPB_DEF_TLV_TAG(2, uint16_t)
		    SPB_DEF_TLV_TAG(3, uint32_t)
		    SPB_DEF_TLV_TAG(4, uint64_t)
		    SPB_DEF_TLV_TAG(TAG_STR_ID, string)
		SPB_END_TLV()



How to declare a TLVSET type
-----------------------------------

		#define TAG_STR_ID 5  //no need to define, if you define this tag_name, you can access this tag_id by name
		#define TAG_ID_TYPE uint8_t //no need to define
		#define TAG_DATA_LEN_TYPE uint16_t //no need to define
		
		SPB_BEGIN_TLVSET(test_tlvset, uint16_t, TAG_ID_TYPE, TAG_DATA_LEN_TYPE)
		    SPB_DEF_TLVSET_TAG(1, hex1)
		    SPB_DEF_TLVSET_TAG(2, hex2)
		    SPB_DEF_TLVSET_TAG(3, hex4)
		    SPB_DEF_TLVSET_TAG(4, hex8)
		    SPB_DEF_TLVSET_TAG(TAG_STR_ID, string)
		SPB_END_TLVSET()


How to declare a object type
-----------------------------------

		SPB_BEGIN_OBJECT(test_msg_string)
		    SPB_DEF_MEMBER(string1, str1Test)
		    SPB_DEF_MEMBER(string2, str2Test)
		    SPB_DEF_MEMBER(string4, str3Test)
		SPB_END_OBJECT()
		
		SPB_BEGIN_OBJECT(test_msg)
		    SPB_DEF_LEN_MEMBER(uint8_t)  //will fill the ArraySize of the object, can only use this macro only once to define a len member
		    SPB_DEF_MEMBER(uint8_t, u8Test)
		    SPB_DEF_MEMBER(uint16_t, u16Test)
		    SPB_DEF_KEY_MEMBER(uint32_t, u32Test) //will be used for map type, can only use this macro only once to define a key member
		    SPB_DEF_MEMBER(uint64_t, u64Test)
		    SPB_DEF_MEMBER(float, fltTest)
		    SPB_DEF_MEMBER(double, dblTest)
		    SPB_DEF_MEMBER(test_tlv, tlvTest)
		    SPB_DEF_MEMBER(test_tlvset, tlvsetTest)
		    SPB_DEF_MEMBER(buffer<4>, bufTest1)
		    SPB_DEF_MEMBER(test_msg_string, msgTest)
		    SPB_DEF_LIST1_MEMBER(test_tlv, lstTLV)
		    SPB_DEF_MAP1_MEMBER(test_tlv, mapTLV)
		    SPB_DEF_VECTOR2_MEMBER(test_msg_string, lstMSG)
		    SPB_DEF_DEQUE4_MEMBER(string1, lstSTR)
		    SPB_DEF_MEMBER(buffer<32>, bufTest2)
		SPB_END_OBJECT()
		
		
		SPB_DEF_LEN_MEMBER(_type)
use this macro to define a length member

		SPB_DEF_KEY_MEMBER(_type, _name)
use this macro to define a key member
		
		SPB_DEF_LIST1_MEMBER(_type, _name)
		SPB_DEF_LIST2_MEMBER(_type, _name)
		SPB_DEF_LIST4_MEMBER(_type, _name)
		SPB_DEF_LIST8_MEMBER(_type, _name)
		SPB_DEF_VECTOR1_MEMBER(_type, _name)
		SPB_DEF_VECTOR2_MEMBER(_type, _name)
		SPB_DEF_VECTOR4_MEMBER(_type, _name)
		SPB_DEF_VECTOR8_MEMBER(_type, _name)
		SPB_DEF_DEQUE1_MEMBER(_type, _name)
		SPB_DEF_DEQUE2_MEMBER(_type, _name)
		SPB_DEF_DEQUE4_MEMBER(_type, _name)
		SPB_DEF_DEQUE8_MEMBER(_type, _name)
use these macro to define a container member
                                    
		SPB_DEF_MAP1_MEMBER(_type, _name)
		SPB_DEF_MAP2_MEMBER(_type, _name)
		SPB_DEF_MAP4_MEMBER(_type, _name)
		SPB_DEF_MAP8_MEMBER(_type, _name)
use these macro to define a map member





