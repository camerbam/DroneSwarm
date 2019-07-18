#define BOOST_TEST_MODULE JSONTest
#include <iostream>

#include <boost/test/unit_test.hpp>

#include "JSONLib/Deserialize.hpp"
#include "JSONLib/Serialize.hpp"

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

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

  auto obj = json::getObjectOrArray(v, seventh);
  BOOST_CHECK(obj);

  BOOST_CHECK(json::getBool(obj.get(), third) == true);
  BOOST_CHECK(json::getBool(obj.get(), fourth) == false);
  BOOST_CHECK(json::getString(obj.get(), sixth) == std::string("Hello World"));
  BOOST_CHECK(json::getNumber(obj.get(), second) == 2);
  BOOST_CHECK(json::getNumber(v, first) == 1);
}

BOOST_AUTO_TEST_CASE(JSON_DOCUMENT)
{
  rapidjson::Document doc(rapidjson::kObjectType);

  std::string first("first"), second("second"), third("third"),
    fourth("fourth"), fifth("fifth"), sixth("sixth"), seventh("seventh");
  json::addNumberToDoc(doc, first, 1);
  json::addNumberToDoc(doc, second, 2);
  json::addBoolToDoc(doc, third, true);
  json::addBoolToDoc(doc, fourth, false);
  json::addStringToDoc(doc, sixth, std::string("Hello World"));

  BOOST_CHECK(json::getBool(doc, third) == true);
  BOOST_CHECK(json::getBool(doc, fourth) == false);
  BOOST_CHECK(json::getString(doc, sixth) == std::string("Hello World"));
  BOOST_CHECK(json::getNumber(doc, second) == 2);
  BOOST_CHECK(json::getNumber(doc, first) == 1);
}

BOOST_AUTO_TEST_CASE(JSON_ARRAY)
{
  rapidjson::Document doc(rapidjson::kObjectType);
  rapidjson::Value stringArray(rapidjson::kArrayType);
  rapidjson::Value doubleArray(rapidjson::kArrayType);
  rapidjson::Value boolArray(rapidjson::kArrayType);
  std::string first("first"), second("second"), third("third");

  json::addStringToArray(doc, stringArray, "a");
  json::addStringToArray(doc, stringArray, "b");
  json::addStringToArray(doc, stringArray, "c");
  json::addStringToArray(doc, stringArray, "d");
  json::addStringToArray(doc, stringArray, "e");

  json::addArrayToDoc(doc, first, stringArray);

  auto stringResult = json::getVectorString(doc, first);
  BOOST_CHECK(stringResult[0] == "a");
  BOOST_CHECK(stringResult[1] == "b");
  BOOST_CHECK(stringResult[2] == "c");
  BOOST_CHECK(stringResult[3] == "d");
  BOOST_CHECK(stringResult[4] == "e");

  json::addBoolToArray(doc, boolArray, true);
  json::addBoolToArray(doc, boolArray, false);

  json::addArrayToDoc(doc, second, boolArray);

  auto boolResult = json::getVectorBool(doc, second);
  BOOST_CHECK(boolResult[0] == true);
  BOOST_CHECK(boolResult[1] == false);

  for (auto i = 0; i < 5; i++)
    json::addNumberToArray(doc, doubleArray, i);

  json::addArrayToDoc(doc, third, doubleArray);

  auto doubleResult = json::getVectorDouble(doc, third);
  for (auto i = 0; i < 5; i++)
    BOOST_CHECK(doubleResult[i] == i);
}
