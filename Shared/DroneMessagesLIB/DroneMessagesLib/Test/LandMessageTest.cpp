#include <boost/test/unit_test.hpp>

#include "DroneMessagesLib/MessageFactory.hpp"
#include "DroneMessagesLib/Messages/LandMessage.hpp"

namespace
{
  messages::LandMessage goodMessage()
  {
    messages::LandMessage landMessage;
    std::string toParse("land");
    landMessage.fromString(toParse);
    return landMessage;
  }

  void createRapidjson(rapidjson::Document& doc,
                       rapidjson::Value& toReturn,
                       const std::string& toParse)
  {
    doc.Parse(toParse.c_str());
    toReturn = doc.GetObject();
  }
}

BOOST_AUTO_TEST_CASE(LAND_MESSAGES_FROM_STRING_TEST)
{
  {
    messages::LandMessage landMessage;
    std::string toParse("lands");
    BOOST_CHECK_THROW(landMessage.fromString(toParse), std::exception);
  }
  {
    messages::LandMessage landMessage;
    std::string toParse("land");
    BOOST_CHECK(landMessage.fromString(toParse));
  }
}

BOOST_AUTO_TEST_CASE(LAND_MESSAGES_TO_STRING_TEST)
{
  auto message = goodMessage();
  BOOST_CHECK(message.toString() == "land");
}

BOOST_AUTO_TEST_CASE(LAND_MESSAGES_FROM_JSON_TEST)
{
  rapidjson::Document doc;
  {
    messages::LandMessage landMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"land": {"foo": "bar"}})");
    BOOST_CHECK(landMessage.readJson(json));
  }
  {
    messages::LandMessage landMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"land": {}})");
    BOOST_CHECK(landMessage.readJson(json));
  }
}

BOOST_AUTO_TEST_CASE(LAND_MESSAGES_FROM_XML_TEST)
{
  auto doc = new rapidxml::xml_document<>;
  {
    messages::LandMessage landMessage;
    std::string str("<land/>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK(landMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)));
  }
  {
    messages::LandMessage landMessage;
    std::string str("<land><distance>50</distance></land>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK(landMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)));
  }
  {
    messages::LandMessage landMessage;
    std::string str("<land></land>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK(landMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)));
  }
  delete doc;
}

BOOST_AUTO_TEST_CASE(MESSAGE_FACTORY_LAND_TEST)
{
  std::string testMessage = "land";

  rapidjson::Document doc;
  rapidjson::Value json;
  createRapidjson(
    doc, json, R"({"type":"land","data":{"land": {}}})");
  BOOST_CHECK_EQUAL(
    boost::get<messages::LandMessage>(messages::getMessage(json)).toString(),
    testMessage);

  auto xmlDoc = new rapidxml::xml_document<>;
  std::string str(
    "<type>land</type><data><land></land></data>");
  xmlDoc->parse<0>((char*)str.c_str());
  BOOST_CHECK_EQUAL(
    boost::get<messages::LandMessage>(messages::getMessage(xmlDoc)).toString(),
    testMessage);
  delete xmlDoc;

  BOOST_CHECK_EQUAL(
    boost::get<messages::LandMessage>(messages::getMessage(testMessage))
    .toString(),
    testMessage);
}
