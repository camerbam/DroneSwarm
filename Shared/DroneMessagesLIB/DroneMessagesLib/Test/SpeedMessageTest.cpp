#include <boost/test/unit_test.hpp>

#include "DroneMessagesLib/MessageFactory.hpp"
#include "DroneMessagesLib/Messages/SpeedMessage.hpp"

namespace
{
  messages::SpeedMessage goodMessage()
  {
    messages::SpeedMessage backMessage;
    std::string toParse("speed?");
    backMessage.fromString(toParse);
    return backMessage;
  }

  void createRapidjson(rapidjson::Document& doc,
                       rapidjson::Value& toReturn,
                       const std::string& toParse)
  {
    doc.Parse(toParse.c_str());
    toReturn = doc.GetObject();
  }
}

BOOST_AUTO_TEST_CASE(SPEED_MESSAGES_FROM_STRING_TEST)
{
  {
    messages::SpeedMessage speedMessage;
    std::string toParse("speed");
    BOOST_CHECK_THROW(speedMessage.fromString(toParse), std::exception);
  }
  {
    messages::SpeedMessage speedMessage;
    std::string toParse("speeds");
    BOOST_CHECK_THROW(speedMessage.fromString(toParse), std::exception);
  }
  {
    messages::SpeedMessage speedMessage;
    std::string toParse("speed?");
    BOOST_CHECK(speedMessage.fromString(toParse));
  }
}

BOOST_AUTO_TEST_CASE(SPEED_MESSAGES_TO_STRING_TEST)
{
  auto message = goodMessage();
  BOOST_CHECK(message.toString() == "speed?");
}

BOOST_AUTO_TEST_CASE(SPEED_MESSAGES_FROM_JSON_TEST)
{
  rapidjson::Document doc;
  {
    messages::SpeedMessage speedMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"speed": {"foo": "bar"}})");
    BOOST_CHECK(speedMessage.readJson(json));
  }
  {
    messages::SpeedMessage speedMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"speed": {}})");
    BOOST_CHECK(speedMessage.readJson(json));
  }
}

BOOST_AUTO_TEST_CASE(SPEED_MESSAGES_FROM_XML_TEST)
{
  auto doc = new rapidxml::xml_document<>;
  {
    messages::SpeedMessage speedMessage;
    std::string str("<speed/>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK(speedMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)));
  }
  {
    messages::SpeedMessage speedMessage;
    std::string str("<speed><distance>50</distance></speed>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK(speedMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)));
  }
  {
    messages::SpeedMessage speedMessage;
    std::string str("<speed></speed>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK(speedMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)));
  }
  delete doc;
}

BOOST_AUTO_TEST_CASE(MESSAGE_FACTORY_SPEED_TEST)
{
  std::string testMessage = "speed?";

  rapidjson::Document doc;
  rapidjson::Value json;
  createRapidjson(doc, json, R"({"type":"speed","data":{"speed": {}}})");
  BOOST_CHECK_EQUAL(
    boost::get<messages::SpeedMessage>(messages::getMessage(json)).toString(),
    testMessage);

  auto xmlDoc = new rapidxml::xml_document<>;
  std::string str("<type>speed</type><data><speed></speed></data>");
  xmlDoc->parse<0>((char*)str.c_str());
  BOOST_CHECK_EQUAL(
    boost::get<messages::SpeedMessage>(messages::getMessage(xmlDoc)).toString(),
    testMessage);
  delete xmlDoc;

  BOOST_CHECK_EQUAL(
    boost::get<messages::SpeedMessage>(messages::getMessage(testMessage))
    .toString(),
    testMessage);
}
