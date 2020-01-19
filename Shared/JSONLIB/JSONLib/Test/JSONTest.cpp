#define BOOST_TEST_MODULE JSONTest

#include <boost/test/unit_test.hpp>

#include "JSONLib/Deserialize.hpp"
#include "JSONLib/Serialize.hpp"

#include "UtilsLib/Utils.hpp"

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

BOOST_AUTO_TEST_CASE(JSON_OBJECT)
{
  rapidjson::Document doc(rapidjson::kObjectType);
  rapidjson::Value v(rapidjson::kObjectType);
  std::string first("first"), second("second"), third("third"),
    fourth("fourth"), fifth("fifth"), sixth("sixth"), seventh("seventh"),
    eighth("eighth"), nineth("nineth");
  json::addNumberToObject(doc, v, first, 1);
  json::addNumberToObject(doc, v, second, 2);
  json::addIntToObject(doc, v, nineth, 3);
  json::addBoolToObject(doc, v, third, true);
  json::addBoolToObject(doc, v, fourth, false);
  json::addStringToObject(doc, v, sixth, std::string("Hello World"));

  rapidjson::Value v2(rapidjson::kObjectType);
  json::addNumberToObject(doc, v2, first, 1);
  json::addNumberToObject(doc, v2, second, 2);
  json::addIntToObject(doc, v2, nineth, 3);
  json::addBoolToObject(doc, v2, third, true);
  json::addBoolToObject(doc, v2, fourth, false);
  json::addStringToObject(doc, v2, sixth, std::string("Hello World"));
  json::addObjectToObject(doc, v, seventh, v2);

  json::addObjectToDoc(doc, "obj", v);

  auto firstObj = json::getObjectOrArray(doc, "obj");
  BOOST_CHECK(firstObj);

  BOOST_CHECK(json::getBool(firstObj.get(), third) == true);
  BOOST_CHECK(json::getBool(firstObj.get(), fourth) == false);
  BOOST_CHECK(json::getString(firstObj.get(), sixth) ==
              std::string("Hello World"));
  BOOST_CHECK(
    utils::compareTwoDoubles(json::getNumber(firstObj.get(), second), 2));
  BOOST_CHECK(
    utils::compareTwoDoubles(json::getNumber(firstObj.get(), first), 1));

  BOOST_CHECK(
   json::getInt(firstObj.get(), nineth) == 3);

  auto obj = json::getObjectOrArray(firstObj.get(), seventh);
  BOOST_CHECK(obj);

  auto emptyObj = json::getObjectOrArray(firstObj.get(), eighth);
  BOOST_CHECK(!emptyObj);

  BOOST_CHECK(json::getBool(obj.get(), third) == true);
  BOOST_CHECK(json::getBool(obj.get(), fourth) == false);
  BOOST_CHECK(json::getInt(obj.get(), nineth) == 3);
  BOOST_CHECK(json::getString(obj.get(), sixth) == std::string("Hello World"));
  BOOST_CHECK(utils::compareTwoDoubles(json::getNumber(obj.get(), second), 2));
  BOOST_CHECK(
    utils::compareTwoDoubles(json::getNumber(firstObj.get(), first), 1));
}

BOOST_AUTO_TEST_CASE(JSON_DOCUMENT)
{
  rapidjson::Document doc(rapidjson::kObjectType);

  std::string first("first"), second("second"), third("third"),
    fourth("fourth"), fifth("fifth"), sixth("sixth"), seventh("seventh");
  json::addNumberToDoc(doc, first, 1);
  json::addNumberToDoc(doc, second, 2);
  json::addIntToDoc(doc, seventh, 2);
  json::addBoolToDoc(doc, third, true);
  json::addBoolToDoc(doc, fourth, false);
  json::addStringToDoc(doc, sixth, std::string("Hello World"));

  BOOST_CHECK(json::getBool(doc, third) == true);
  BOOST_CHECK(json::getBool(doc, fourth) == false);
  BOOST_CHECK(json::getString(doc, sixth) == std::string("Hello World"));
  BOOST_CHECK(utils::compareTwoDoubles(json::getNumber(doc, second), 2));
  BOOST_CHECK(utils::compareTwoDoubles(json::getNumber(doc, first), 1));
  BOOST_CHECK(json::getInt(doc, seventh) == 2);
}

BOOST_AUTO_TEST_CASE(JSON_ARRAY)
{
  rapidjson::Document doc(rapidjson::kObjectType);
  rapidjson::Value stringArray(rapidjson::kArrayType);
  rapidjson::Value doubleArray(rapidjson::kArrayType);
  rapidjson::Value boolArray(rapidjson::kArrayType);
  rapidjson::Value intArray(rapidjson::kArrayType);
  std::string first("first"), second("second"), third("third"), fourth("fourth");

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
  size_t j = 0;
  for (double i = 0; i < 5; i++, j++)
    BOOST_CHECK(utils::compareTwoDoubles(doubleResult[j], i));

  for (int i = 0; i < 5; i++)
    json::addIntToArray(doc, intArray, i);

  json::addArrayToDoc(doc, fourth, intArray);

  auto intResult = json::getVectorInt(doc, fourth);
  for (int i = 0; i < 5; i++)
    BOOST_CHECK(utils::compareTwoDoubles(intResult[i], i));
}

BOOST_AUTO_TEST_CASE(JSON_OBJECT_TO_ARRAY)
{
  rapidjson::Document doc(rapidjson::kObjectType);
  rapidjson::Value arrayValue(rapidjson::kArrayType);
  rapidjson::Value objectValue(rapidjson::kObjectType);
  std::string first("first"), second("second"), third("third");
  json::addNumberToObject(doc, objectValue, first, 1);
  json::addNumberToObject(doc, objectValue, second, 2);
  json::addNumberToObject(doc, objectValue, third, 3);
  json::addObjectToArray(doc, arrayValue, objectValue);
  json::addArrayToDoc(doc, first, arrayValue);

  auto arr = json::getObjectOrArray(doc, first);
  BOOST_CHECK(arr);
}

BOOST_AUTO_TEST_CASE(JSON_ARRAY_TO_OBJ)
{
  rapidjson::Document doc(rapidjson::kObjectType);
  rapidjson::Value arrayValue(rapidjson::kArrayType);
  rapidjson::Value objectValue(rapidjson::kObjectType);
  std::string first("first");
  json::addNumberToArray(doc, arrayValue, 1);
  json::addNumberToArray(doc, arrayValue, 2);
  json::addNumberToArray(doc, arrayValue, 3);
  json::addArrayToObject(doc, objectValue, "arr", arrayValue);
  json::addObjectToDoc(doc, first, objectValue);

  auto arr = json::getObjectOrArray(doc, first);
  BOOST_CHECK(arr);
}

BOOST_AUTO_TEST_CASE(JSON_DOC_TO_STRING)
{
  rapidjson::Document doc(rapidjson::kObjectType);
  std::string first("first"), second("second"), third("third");
  json::addNumberToDoc(doc, first, 1);
  json::addNumberToDoc(doc, second, 2);
  json::addNumberToDoc(doc, third, 3);

  auto expected = "{\"first\":1.0,\"second\":2.0,\"third\":3.0}";
  auto result = json::jsonToString(doc);
  BOOST_CHECK(expected == result);
}

BOOST_AUTO_TEST_CASE(JSON_OBJ_TO_STRING)
{
  rapidjson::Document doc(rapidjson::kObjectType);
  rapidjson::Value objectValue(rapidjson::kObjectType);
  std::string first("first"), second("second"), third("third");
  json::addNumberToObject(doc, objectValue, first, 1);
  json::addNumberToObject(doc, objectValue, second, 2);
  json::addNumberToObject(doc, objectValue, third, 3);

  auto expected = "{\"first\":1.0,\"second\":2.0,\"third\":3.0}";
  auto result = json::jsonToString(objectValue);
  BOOST_CHECK(expected == result);
}
