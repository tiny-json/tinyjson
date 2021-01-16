
#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "..\src\tinyjson.h"

// This tells Catch to provide a main() - only do this in one cpp file

using namespace tinyjson;

using Catch::Matchers::Contains;

TEST_CASE("Simple Json Constructor")
{
    // strings
    json s1("json string object");
    REQUIRE(s1.get_string() == "json string object");

    json s2("");
    REQUIRE(s2.get_string() == "");

    json s3("  ");
    REQUIRE(s3.get_string() == "  ");

    // numbers
    json i1(3256);
    REQUIRE(i1.get_integer() == 3256);

    json i2(-245);
    REQUIRE(i2.get_integer() == -245);

    json i3(0.2356);
    REQUIRE(i3.get_double() == 0.2356);

    json i4(-0.255356);
    REQUIRE(i4.get_double() == -0.255356);

    json i5(000);
    REQUIRE(i5.get_integer() == 0);

    json i6(-000);
    REQUIRE(i6.get_integer() == 0);

    // bools
    json b1(true);
    REQUIRE(b1.get_bool() == true);

    json b2(false);
    REQUIRE(b2.get_bool() == false);

    // null
    json n1;
    REQUIRE(n1.get_null() == nullptr);

    // arrays
    std::vector<json> elms {2134, 781450, 0.187601, -984578, 0, 1, 24.059};
    json a1(elms);
    REQUIRE(a1[0].get_integer() == 2134);
    REQUIRE(a1[1].get_integer() == 781450);
    REQUIRE(a1[2].get_double() == 0.187601);
    REQUIRE(a1[3].get_integer() == -984578);
    REQUIRE(a1[4].get_integer() == 0);
    REQUIRE(a1[5].get_integer() == 1);
    REQUIRE(a1[6].get_double() == 24.059);

    // object
    json o1(json_object{});
    o1.add_member("p1", "helloworld");
    o1.add_member("p2", 3.1415926);
    o1.add_member("p3", 9876543210);
    o1.add_member("p4", json(json_object{}));

    o1["p4"].add_member("p4_1", true);
    o1["p4"].add_member("p4_2", -0.1356456);

    REQUIRE(o1["p1"].get_string() == "helloworld");
    REQUIRE(o1["p2"].get_double() == 3.1415926);
    REQUIRE(o1["p3"].get_integer() == 9876543210);

    REQUIRE(o1["p4"]["p4_1"].get_bool() == true);
    REQUIRE(o1["p4"]["p4_2"].get_double() == -0.1356456);
}

TEST_CASE("Tiny Json String Parsing")
{
    std::u32string u32(U"\x0022" U"2021" U"\x4E16\x754C" U"World " U"\x4F60\x597D"  U"012 Hello!\x0022");
    u32_sstream u32strm(u32);

    auto s = parser::parse_string(u32strm);
    REQUIRE(s.get_string() == "2021世界World 你好012 Hello!");

    u32_sstream ss1(U"\"hello \\\\ world \"");
    auto s1 = parser::parse_string(ss1);
    REQUIRE(s1.get_string() == "hello \\ world ");

    u32_sstream ss2(U"\"hello \\\" world \"");
    auto s2 = parser::parse_string(ss2);
    REQUIRE(s2.get_string() == "hello \" world ");

    u32_sstream ss3(U"\" \\thello \\\"\\r world \\n \\/ \\b \\f \"");
    auto s3 = parser::parse_string(ss3);
    REQUIRE(s3.get_string() == " \thello \"\r world \n / \b \f ");

    u32_sstream ss4(U"\"\"");
    auto s4 = parser::parse_string(ss4);
    REQUIRE(s4.get_string() == "");
}

TEST_CASE("Tiny Json Hex Parsing")
{
    std::u32string u32hex(U"\x0022" U"2021" U"\\u4E16\\u754C" U"World " U"\\u4F60\\u597D"  U"012 Hello! \\u0026\x0022");
    u32_sstream u32strm(u32hex);

    auto s1 = parser::parse_string(u32strm);
    REQUIRE(s1.get_string() == "2021世界World 你好012 Hello! &");
}

TEST_CASE("Tiny Json Escape Parsing")
{
    std::u32string u32(U"\x0022\\\"a\\b\\/c\\bd\\fe  \\fg\\n\\r;\\t\\u4E16\\u754C\x0022");
    u32_sstream u32strm(u32);

    auto s1 = parser::parse_string(u32strm);
    std::string ss1 = s1.get_string();
    REQUIRE(s1.get_string() == "\"a\b/c\bd\fe  \fg\n\r;\t世界");
}

TEST_CASE("Tiny Json Bool Parsing")
{
    u32_sstream bs1(U"true");
    auto b1 = parser::parse_bool(bs1);
    REQUIRE(true == b1.get_bool());

    u32_sstream bs2(U"tRue ");
    auto b2 = parser::parse_bool(bs2);
    REQUIRE(true == b2.get_bool());

    u32_sstream bs3(U" FALSE");
    auto b3 = parser::parse_bool(bs3);
    REQUIRE(false == b3.get_bool());

    u32_sstream bs4(U" falsE ");
    auto b4 = parser::parse_bool(bs4);
    REQUIRE(false == b4.get_bool());
}

TEST_CASE("Tiny Json NULL Parsing")
{
    u32_sstream ns1(U"null");
    auto n1 = parser::parse_null(ns1);
    REQUIRE(nullptr == n1.get_null());

    u32_sstream ns2(U" NULL ");
    auto n2 = parser::parse_null(ns2);
    REQUIRE(nullptr == n2.get_null());

    u32_sstream ns3(U" nUlL");
    auto n3 = parser::parse_null(ns3);
    REQUIRE(nullptr == n3.get_null());
}


TEST_CASE("Tiny Json Number Parsing")
{
    u32_sstream ns1(U"0.124    \t");
    auto n1 = parser::parse_number(ns1);
    REQUIRE(0.124 == n1.get_double());

    u32_sstream ns2(U"-0.10921234567824 ");
    auto n2 = parser::parse_number(ns2);
    REQUIRE(-0.10921234567824 == n2.get_double());

    u32_sstream ns3(U"921.234567824");
    auto n3 = parser::parse_number(ns3);
    REQUIRE(921.234567824 == n3.get_double());

    u32_sstream ns4(U"  .987123654");
    auto n4 = parser::parse_number(ns4);
    REQUIRE(.987123654 == n4.get_double());

    u32_sstream ns5(U".23545E-34  ");
    auto n5 = parser::parse_number(ns5);
    REQUIRE(.23545E-34 == n5.get_double());

    u32_sstream ns6(U"8.7894e+34");
    auto n6 = parser::parse_number(ns6);
    REQUIRE(8.7894e+34 == n6.get_double());

    u32_sstream ns7(U"9874563121555444");
    auto n7 = parser::parse_number(ns7);
    REQUIRE(9874563121555444 == n7.get_integer());

    u32_sstream ns8(U"7895484569216311245.006");
    auto n8 = parser::parse_number(ns8);
    REQUIRE(7895484569216311245.006 == n8.get_double());

    u32_sstream ns9(U"00000");
    auto n9 = parser::parse_number(ns9);
    REQUIRE(0 == n9.get_integer());

    u32_sstream ns10(U"-426981544458458");
    auto n10 = parser::parse_number(ns10);
    REQUIRE(-426981544458458 == n10.get_integer());
}

TEST_CASE("Tiny Json Hex Char Parsing")
{
    u32_sstream hs1(U"0024");
    auto h1 = parser::parse_hex(hs1);
    REQUIRE(U'$' == h1);

    u32_sstream hs2(U"0047");
    auto h2 = parser::parse_hex(hs2);
    REQUIRE(U'G' == h2);

    // 754C
    u32_sstream hs3(U"754C");
    auto h3 = parser::parse_hex(hs3);
    REQUIRE(U'\x754C' == h3);
}


TEST_CASE("Tiny Json Array Parsing")
{
    u32_sstream as1(U" [124, -2.534, \"hello world  \", null, false ]");
    auto a1 = parser::parse_array(as1);

    REQUIRE(5 == a1.size());
    REQUIRE(json_t::array == a1.type());
    REQUIRE(124 == a1[0].get_integer());
    REQUIRE(-2.534 == a1[1].get_double());
    REQUIRE("hello world  " == a1[2].get_string());
    REQUIRE(nullptr == a1[3].get_null());
    REQUIRE(false == a1[4].get_bool());


    u32_sstream as2(U" [  ] ");
    auto a2 = parser::parse_array(as2);

    REQUIRE(0 == a2.size());
    REQUIRE(json_t::array == a2.type());

    u32_sstream as3(U" [ \" elem1\" ] ");
    auto a3 = parser::parse_array(as3);

    REQUIRE(1 == a3.size());
    REQUIRE(json_t::array == a3.type());
    REQUIRE(" elem1" == a3[0].get_string());


    u32_sstream as4(U" [ \" elem1\", {}, {\"p1\" : 498723123} ] ");
    auto a4 = parser::parse_array(as4);

    REQUIRE(3 == a4.size());
    REQUIRE(json_t::array == a4.type());
    REQUIRE(" elem1" == a4[0].get_string());
    REQUIRE(0 == a4[1].size());
    REQUIRE(1 == a4[2].size());
    REQUIRE(498723123 == a4[2]["p1"].get_integer());

    u32_sstream as5(U" [[]  ] ");
    auto a5 = parser::parse_array(as5);

    REQUIRE(a5.type() == json_t::array);
    REQUIRE(a5.size() == 1);
    REQUIRE(a5[0].type() == json_t::array);
    REQUIRE(a5[0].size() == 0);

    u32_sstream as6(U" [[1984.2020], 2020.1984  ] ");
    auto a6 = parser::parse_array(as6);

    REQUIRE(a6.type() == json_t::array);
    REQUIRE(a6.size() == 2);
    REQUIRE(a6[1].get_double() == 2020.1984);
    REQUIRE(a6[0].type() == json_t::array);
    REQUIRE(a6[0].size() == 1);
    REQUIRE(a6[0][0].get_double() == 1984.2020);

    u32_sstream as7(U" [[1984, \"1984\"], 2020, 1984 ] ");
    auto a7 = parser::parse_array(as7);
    REQUIRE(a7.type() == json_t::array);
    REQUIRE(a7.size() == 3);
    REQUIRE(a7[0].type() == json_t::array);
    REQUIRE(a7[0][0].get_integer() == 1984);
    REQUIRE(a7[0][1].get_string() == "1984");
    REQUIRE(a7[1].get_integer() == 2020);
    REQUIRE(a7[2].get_integer() == 1984);
}

TEST_CASE("Tiny Json Object Parsing")
{
    u32_sstream os1(U" {}");
    auto o1 = parser::parse_object(os1);
    REQUIRE(o1.type() == json_t::object);
    REQUIRE(o1.size() == 0);

    u32_sstream os2(U" {\"p1\" : 987135}");
    auto o2 = parser::parse_object(os2);
    REQUIRE(o2.type() == json_t::object);
    REQUIRE(o2.size() == 1);
    REQUIRE(o2["p1"].get_integer() == 987135);

    u32_sstream os3(U" {\"p1\" : 987135, \"P_1\" : true}");
    auto o3 = parser::parse_object(os3);
    REQUIRE(o3.type() == json_t::object);
    REQUIRE(o3.size() == 2);
    REQUIRE(o3["p1"].get_integer() == 987135);
    REQUIRE(o3["P_1"].get_bool() == true);

    u32_sstream os4(U" {\"p1\" : -0.26897415, \"P_1\" : { \"_53245\": -235235 } }");
    auto o4 = parser::parse_object(os4);
    REQUIRE(o4.type() == json_t::object);
    REQUIRE(o4.size() == 2);
    REQUIRE(o4["p1"].get_double() == -0.26897415);
    REQUIRE(o4["P_1"].type() == json_t::object);
    REQUIRE(o4["P_1"]["_53245"].get_integer() == -235235);
}

TEST_CASE("Tiny Json Member Parsing")
{
    u32_sstream ss1(U"\"\x4F60\x202F2 hello\"");
    auto member_name = parser::parse_member(ss1);
    REQUIRE(member_name == "你𠋲 hello");

    u32_sstream ss2(U"\"_hello world!\"");
    member_name = parser::parse_member(ss2);
    REQUIRE(member_name == "_hello world!");

    u32_sstream ss3(U"\"__\\u0069_\\u005E\"");
    member_name = parser::parse_member(ss3);
    REQUIRE(member_name == "__i_^");

    u32_sstream ss4(U"\"hello \\\"world\\\"\"");
    member_name = parser::parse_member(ss4);
    REQUIRE(member_name == "hello \"world\"");

    u32_sstream ss5(U"\"hello { world }\"");
    member_name = parser::parse_member(ss5);
    REQUIRE(member_name == "hello { world }");

    u32_sstream ss6(U"\"hello ,{ world }\"");
    member_name = parser::parse_member(ss6);
    REQUIRE(member_name == "hello ,{ world }");

    u32_sstream ss7(U"\"[hello ,{ world }]\"");
    member_name = parser::parse_member(ss7);
    REQUIRE(member_name == "[hello ,{ world }]");

    u32_sstream ss8(U"\"[\\thello ,{ world }]\"");
    member_name = parser::parse_member(ss8);
    REQUIRE(member_name == "[\thello ,{ world }]");
}

TEST_CASE("SimpleJson Serialization")
{
    json a = parser::parse("{}");

    a.add_member("p1", 13525.4235);
    a.add_member("p2", "abcdefg hijklmnop rsq  uvwxyz ");
    a.add_member("_p3_", false);
    a.add_member("p4_____________", -0.9876540321);
    a.add_member("_______p5", std::vector<json>{"abc", 0.31424, false});
    a.add_member("示例一", false);
    a.add_member("你好 hello", "world\\u0039");
    a.add_member("世界__\\u0069_\\u005E", "你好");

    std::string a1 = a.to_string();
    json b = parser::parse(a1.c_str());

    REQUIRE(b.size() == 8);
    REQUIRE(b["p1"].get_double() == 13525.4235);
    REQUIRE(b["p2"].get_string() == "abcdefg hijklmnop rsq  uvwxyz ");
    REQUIRE(b["_p3_"].get_bool() == false);
    REQUIRE(b["p4_____________"].get_double() == Approx(-0.9876540321).margin(0.00001));

    REQUIRE(b["_______p5"].type() == json_t::array);
    REQUIRE(b["_______p5"][0].get_string() == "abc");
    REQUIRE(b["_______p5"][1].get_double() == 0.31424);
    REQUIRE(b["_______p5"][2].get_bool() == false);

    REQUIRE(b["示例一"].get_bool() == false);
    REQUIRE(b["你好 hello"].get_string() == "world9");
    REQUIRE(b["世界__i_^"].get_string() == "你好");

    json c(json_object{});
    REQUIRE(c.to_string() == "{}");

    json d(json_array{});
    REQUIRE(d.to_string() == "[]");
}

TEST_CASE("SimpleJson Json Class Test")
{
    json a, b;
    REQUIRE(a == b);

    json a1(1984), b1(1984);
    REQUIRE(a1 == b1);

    json a2(-0.326345), b2(-0.326345);
    REQUIRE(a2 == b2);

    a2 = json(0.326345);
    REQUIRE(a2 != b2);

    json a3(true), b3(true);
    REQUIRE(a3 == b3);

    a3 = json(false);
    REQUIRE(a3 != b3);

    json a4("hello world"), b4("hello world");
    REQUIRE(a4 == b4);

    a4 = json("hello world!");
    REQUIRE(a4 != b4);

    json a5(json_object{}), b5(json_object{});
    REQUIRE(a5 == b5);

    a5.add_member("p1", "hello1984");
    REQUIRE(a5 != b5);

    b5.add_member("p1", "hello1984");
    REQUIRE(a5 == b5);

    json a6(json_array{}), b6(json_array{});
    REQUIRE(a6 == b6);

    a6.add_element(1984);
    REQUIRE(a6 != b6);

    REQUIRE(a1 != a2);
    REQUIRE(a1 != a3);
    REQUIRE(a1 != a4);
    REQUIRE(a1 != a5);
    REQUIRE(a1 != a6);

    REQUIRE(a2 != a3);
    REQUIRE(a2 != a4);
    REQUIRE(a2 != a5);
    REQUIRE(a2 != a6);

    REQUIRE(a3 != a4);
    REQUIRE(a3 != a5);
    REQUIRE(a3 != a6);

    REQUIRE(a4 != a5);
    REQUIRE(a4 != a6);

    REQUIRE(a5 != a6);

    json c(1984);
    json d(c);
    REQUIRE(c == d);
    REQUIRE(d.get_integer() == 1984);

    json e(json("1984"));
    REQUIRE(e.get_string() == "1984");
}

TEST_CASE("SimpleJson Nubmer Parsing Failure")
{
    u32_sstream ns1(U"0.124abc");
    REQUIRE_THROWS_WITH(parser::parse_number(ns1), Contains("Unexpected number"));

    u32_sstream ns2(U"124abc");
    REQUIRE_THROWS_WITH(parser::parse_number(ns2), Contains("Unexpected number"));

    u32_sstream ns3(U"124 000");
    REQUIRE_THROWS_WITH(parser::parse_number(ns3), Contains("Unexpected number"));

    u32_sstream ns4(U".124 000");
    REQUIRE_THROWS_WITH(parser::parse_number(ns4), Contains("Unexpected number"));

    u32_sstream ns5(U"not a number 00.23");
    REQUIRE_THROWS(parser::parse_number(ns5));
}

TEST_CASE("SimpleJson Bool Parsing Failure")
{
    u32_sstream bs1(U"falt");
    REQUIRE_THROWS_WITH(parser::parse_bool(bs1), Contains("invalid boolean string"));

    u32_sstream bs2(U"Falsa");
    REQUIRE_THROWS_WITH(parser::parse_bool(bs2), Contains("invalid boolean string"));
}

TEST_CASE("SimpleJson String Parsing Failure")
{
    u32_sstream ss1(U"\"hello world \\\"");
    REQUIRE_THROWS_WITH(parser::parse_string(ss1), Contains("expected char '\"'"));

    u32_sstream ss2(U"\"hello world \\u00A");
    REQUIRE_THROWS_WITH(parser::parse_string(ss2), Contains("not hex number"));

    u32_sstream ss3(U"\"hello \\a world ");
    REQUIRE_THROWS_WITH(parser::parse_string(ss3), Contains("backslash is followed by invalid character"));

    u32_sstream ss4(U"\"hello \\uworld\"");
    REQUIRE_THROWS_WITH(parser::parse_string(ss4), Contains("not hex number"));
}

TEST_CASE("SimpleJson Array Parsing Failure")
{
    u32_sstream as1(U"[a]");
    REQUIRE_THROWS(parser::parse_array(as1));

    u32_sstream as2(U"[,]");
    REQUIRE_THROWS(parser::parse_array(as2), Contains("invalid array format"));

    u32_sstream as3(U"[1.5,]");
    REQUIRE_THROWS(parser::parse_array(as3), Contains("parsing error: unexpected character"));

    u32_sstream as4(U"[\"abc\":]");
    REQUIRE_THROWS(parser::parse_array(as4), Contains("parsing error: unexpected character"));

    u32_sstream as5(U"[3.14 , \"\", a]");
    REQUIRE_THROWS(parser::parse_array(as5), Contains("parsing error: unexpected character"));

    u32_sstream as6(U"[[3.14 , \"\", a]");
    REQUIRE_THROWS(parser::parse_array(as6), Contains("invalid array formatr"));
}

TEST_CASE("SimpleJson Object Parsing Failure")
{
    u32_sstream os1(U"{a}");
    REQUIRE_THROWS(parser::parse_object(os1));

    u32_sstream os2(U"{");
    REQUIRE_THROWS(parser::parse_object(os2));

    u32_sstream os3(U"{1}");
    REQUIRE_THROWS(parser::parse_object(os3));

    u32_sstream os4(U"{\"hello\"}");
    REQUIRE_THROWS(parser::parse_object(os4));

    u32_sstream os5(U"{\"hello\":}");
    REQUIRE_THROWS(parser::parse_object(os5));

    u32_sstream os6(U"{\"hello: 124 }");
    REQUIRE_THROWS(parser::parse_object(os6));
}

TEST_CASE("SimpleJson Member Parsing Failure")
{
    u32_sstream ms1(U"\"hello");
    REQUIRE_THROWS(parser::parse_member(ms1));
}

TEST_CASE("SimpleJson Json Parsing Failure")
{
    REQUIRE_THROWS(parser::parse(""));
    REQUIRE_THROWS(parser::parse("{"));
    REQUIRE_THROWS(parser::parse("}"));
    REQUIRE_THROWS(parser::parse("{}}"));
    REQUIRE_THROWS(parser::parse("]"));
    REQUIRE_THROWS(parser::parse("["));
    REQUIRE_THROWS(parser::parse("[]]"));
    REQUIRE_THROWS(parser::parse("hello"));
}

TEST_CASE("SimpleJson Json Parsing 1")
{
    json j = parser::parse(R"RAW(
        {
        "command": "launch",
        "arguments": {
            "name": "Debug",
            "type": "windbg",
            "request": "launch",
            "program": "C:\\VC\\helloworld/helloworld.exe",
            "args": [

            ],
            "cwd": "C:\\VC\\helloworld/",
            "debugServer": 8888,
            "__configurationTarget": 5,
            "__sessionId": "df7ac0b9-4d27-4aba-a73f-7fd8207bb590"
        },
        "type": "request",
        "seq": 2
        }
    )RAW");

    REQUIRE(j.type() == json_t::object);
    REQUIRE(j["command"].get_string() == "launch");
    REQUIRE(j["arguments"]["name"].get_string() == "Debug");
    REQUIRE(j["arguments"]["type"].get_string() == "windbg");
    REQUIRE(j["arguments"]["request"].get_string() == "launch");
    REQUIRE(j["arguments"]["program"].get_string() == "C:\\VC\\helloworld/helloworld.exe");
    REQUIRE(j["arguments"]["args"].size() == 0);
    REQUIRE(j["arguments"]["cwd"].get_string() == "C:\\VC\\helloworld/");
    REQUIRE(j["arguments"]["debugServer"].get_integer() == 8888);
    REQUIRE(j["arguments"]["__configurationTarget"].get_integer() == 5);
    REQUIRE(j["arguments"]["__sessionId"].get_string() == "df7ac0b9-4d27-4aba-a73f-7fd8207bb590");
    REQUIRE(j["type"].get_string() == "request");
    REQUIRE(j["seq"].get_integer() == 2);
}


TEST_CASE("SimpleJson Json Parsing 2")
{
    json j = parser::parse(R"RAW(
        ["computer"]
    )RAW");

    REQUIRE(j[0].get_string() == "computer");
}
