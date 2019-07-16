#define BOOST_TEST_MODULE JSONTest
#include <boost/test/unit_test.hpp>

#include "JSONLib/Serialize.hpp"
#include "JSONLib/Deserialize.hpp"

BOOST_AUTO_TEST_CASE(JSON_OBJECT)
{
  rapidjson::Document doc;
  rapidjson::Value v(rapidjson::kObjectType);
  std::string first("first"), second("second"), third("third"),
    fourth("fourth"), fifth("fifth"), sixth("sixth"), seventh("seventh");
  json::addNumberToObject(doc, v, first, 1);
  json::addNumberToObject(doc, v, second, 2);
  json::addBoolToObject(doc, v, third, true);
  json::addBoolToObject(doc, v, fourth, false);
  json::addStringToObject(doc, v, sixth, std::string("Hello World"));

  rapidjson::Value v2(rapidjson::kObjectType);
  json::addNumberToObject(doc, v2, first, 1);
  json::addNumberToObject(doc, v2, second, 2);
  json::addBoolToObject(doc, v2, third, true);
  json::addBoolToObject(doc, v2, fourth, false);
  json::addStringToObject(doc, v2, sixth, std::string("Hello World"));
  json::addObjectToObject(doc, v, seventh, v2);

  BOOST_CHECK(json::getBool(v, third) == true);
  BOOST_CHECK(json::getBool(v, fourth) == false);
  BOOST_CHECK(json::getString(v, sixth) == std::string("Hello World"));
  BOOST_CHECK(json::getNumber(v, second) == 2);
  BOOST_CHECK(json::getNumber(v, first) == 1);

  BOOST_CHECK(json::getBool(v2, third) == true);
  BOOST_CHECK(json::getBool(v2, fourth) == false);
  BOOST_CHECK(json::getString(v2, sixth) == std::string("Hello World"));
  BOOST_CHECK(json::getNumber(v2, second) == 2);
  BOOST_CHECK(json::getNumber(v, first) == 1);
  BOOST_CHECK(json::getObjectOrArray(v, seventh) == v2);
}

BOOST_AUTO_TEST_CASE(JSON_ARRAY)
{
  rapidjson::Document doc;
  rapidjson::Value v(rapidjson::kArrayType);
  json::addNumberToArray(doc, v, 1);
  json::addNumberToArray(doc, v, 2);
  json::addBoolToArray(doc, v, true);
  json::addBoolToArray(doc, v, false);
  json::addStringToArray(doc, v, std::string("Hello World"));

  QVERIFY(int1 == 1);
  QVERIFY(int2 == 1);
  QVERIFY(bool1 == 1);
  QVERIFY(bool2 == 1);
  QVERIFY(null1 == 1);
  QVERIFY(string1 == 1);
}

void MessagesTest::testAddToDoc()
{
  rapidjson::Document d;
  d.SetObject();
  std::string first("first"), second("second"), third("third"),
    fourth("fourth"), fifth("fifth"), sixth("sixth");
  json::addIntToDoc(d, first, 1);
  json::addIntToDoc(d, second, 2);
  json::addBoolToDoc(d, third, true);
  json::addBoolToDoc(d, fourth, false);
  json::addNullToDoc(d, fifth);
  json::addStringToDoc(d, sixth, std::string("Hello World"));

  json::JSONParser parser(d);
  auto map = parser.getMap();

  int int1(0), int2(0), bool1(0), bool2(0), null1(0), string1(0);

  for (auto&& value : map)
  {
    if (value.first == "/first" && value.second == "1") int1 += 1;
    if (value.first == "/second" && value.second == "2") int2 += 1;
    if (value.first == "/third" && value.second == "true") bool1 += 1;
    if (value.first == "/fourth" && value.second == "false") bool2 += 1;
    if (value.first == "/fifth" && value.second == "null") null1 += 1;
    if (value.first == "/sixth" && value.second == "Hello World") string1 += 1;
  }

  QVERIFY(int1 == 1);
  QVERIFY(int2 == 1);
  QVERIFY(bool1 == 1);
  QVERIFY(bool2 == 1);
  QVERIFY(null1 == 1);
  QVERIFY(string1 == 1);
}

void MessagesTest::testJSONToString()
{
  rapidjson::Document d;
  d.SetObject();
  std::string first("first"), second("second"), third("third"),
    fourth("fourth"), fifth("fifth"), sixth("sixth");
  json::addIntToDoc(d, first, 1);
  json::addIntToDoc(d, second, 2);
  json::addBoolToDoc(d, third, true);
  json::addBoolToDoc(d, fourth, false);
  json::addNullToDoc(d, fifth);
  json::addStringToDoc(d, sixth, std::string("Hello World"));

  std::string expected("{\"first\":1,\"second\":2,\"third\":true,\"fourth\":"
    "false,\"fifth\":null,\"sixth\":\"Hello World\"}");

  auto jsonString = json::jsonToString(d);

  QVERIFY(expected == jsonString);
}
