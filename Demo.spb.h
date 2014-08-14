/****************************************
The software uses the GPL agreement authorization, please read the license.txt
Author: bihongpi
E-mail: pibihong@163.com
Last-modified: 2014-08-13
****************************************/

#include "StructuredProtoBuffer.h"


#define TAG_STR_ID 5
#define TAG_ID_TYPE uint8_t
#define TAG_DATA_LEN_TYPE uint16_t

SPB_BEGIN_TLV(test_tlv, TAG_ID_TYPE, TAG_DATA_LEN_TYPE)
    SPB_DEF_TLV_TAG(1, uint8_t)
    SPB_DEF_TLV_TAG(2, uint16_t)
    SPB_DEF_TLV_TAG(3, uint32_t)
    SPB_DEF_TLV_TAG(4, uint64_t)
    SPB_DEF_TLV_TAG(TAG_STR_ID, string)
SPB_END_TLV()

SPB_BEGIN_TLVSET(test_tlvset, uint16_t, TAG_ID_TYPE, TAG_DATA_LEN_TYPE)
    SPB_DEF_TLVSET_TAG(1, hex1)
    SPB_DEF_TLVSET_TAG(2, hex2)
    SPB_DEF_TLVSET_TAG(3, hex4)
    SPB_DEF_TLVSET_TAG(4, hex8)
    SPB_DEF_TLVSET_TAG(TAG_STR_ID, string)
SPB_END_TLVSET()

SPB_BEGIN_OBJECT(test_msg_string)
    SPB_DEF_MEMBER(string1, str1Test)
    SPB_DEF_MEMBER(string2, str2Test)
    SPB_DEF_MEMBER(string4, str3Test)
SPB_END_OBJECT()

SPB_BEGIN_OBJECT(test_msg)
    SPB_DEF_LEN_MEMBER(uint8_t) //will fill the ArraySize of the object, can only use this macro only once to define a len member
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


