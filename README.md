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
		

