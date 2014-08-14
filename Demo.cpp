/****************************************
The software uses the GPL agreement authorization, please read the license.txt
Author: bihongpi
E-mail: pibihong@163.com
Last-modified: 2014-08-13
****************************************/

//#include "json/json.h"
#include "Demo.spb.h"

int main()
{
	spb::SPB_CLASS_TYPE(test_msg) msg, msg2;

#ifdef JSON_JSON_H_INCLUDED
	printf("========================Info========================\n%s\n", msg.Info().toStyledString().c_str());
#else
    printf("========================Info========================\n%s\n", msg.DebugInfo().c_str());
#endif

	msg.u8Test(8);
	msg.u16Test = 16;
	msg.u32Test.set(32);
	msg.u64Test.set(64);

	msg.fltTest(12.34);
	msg.dblTest.set(56.78);

	msg.tlvTest.set_1(80);
	msg.tlvTest.set_2(160);
	msg.tlvTest.set_3(320);
	msg.tlvTest.set_4(640);
	msg.tlvTest.set_TAG_STR_ID("str_tlv", 4);

    msg.tlvsetTest.set_1(80);
	msg.tlvsetTest.tag_2(160);
	msg.tlvsetTest.tag_3 = 320;
	msg.tlvsetTest.tag_4.set(640);
	msg.tlvsetTest.set_TAG_STR_ID("str_tlv", 4);

	msg.msgTest.str1Test("str1");
	msg.msgTest.str2Test = "str2";
	msg.msgTest.str3Test.set("str3");

	spb::SPB_CLASS_TYPE(test_msg_string)& m = msg.lstMSG.append();
	m.str1Test("str1");
	m.str2Test = "str2";
	m.str3Test.set("str3");

	msg.lstTLV.append().set_1(80);
	msg.lstTLV.append().set_2(160);
	msg.lstTLV.append().set_3(320);
	msg.lstTLV.append().set_4(640);
	msg.lstTLV.append().set_TAG_STR_ID("str_tlv");

    msg.mapTLV[1].set_1(800);
    msg.mapTLV[2].set_2(1600);
    msg.mapTLV[3].set_3(3200);
    msg.mapTLV[4].set_4(6400);
    msg.mapTLV[TAG_STR_ID].set_TAG_STR_ID("map_tlv");

	msg.lstSTR.append().set("str_abc");
	msg.lstSTR.append().set("str_bcd");
	msg.lstSTR.append().set("str_efg");
    
    msg.GetExtendBuffer().assign("pbh");

#ifdef JSON_JSON_H_INCLUDED
	Json::Value v;
	msg.toJson(v);

	printf("========================Encode========================\n%s\n", v.toStyledString().c_str());
#else
    printf("========================Encode========================\n%s\n", msg.DebugString().c_str());
#endif
    
	std::string d;
	int r = msg.SerializeToString(d);

	printf("========================[%d]STREAM[%zu/%zu]========================\n", r, msg.ArraySize(), d.size());

	for (size_t i = 0 ; i < d.size(); ++i)
	{
		printf("\\%02x", (unsigned char)d[i]);
	}
	printf("\n");

    r = msg2.ParseFromString(d);

#ifdef JSON_JSON_H_INCLUDED	
	v = msg2;
	printf("========================Decode[%d]========================\n%s\n", r, v.toStyledString().c_str());
#else
    printf("========================Decode[%d]========================\n%s\n", r, msg2.DebugString().c_str());
#endif

	return 0;
}

