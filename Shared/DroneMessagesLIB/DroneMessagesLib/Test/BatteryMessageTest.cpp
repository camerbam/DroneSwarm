#include <boost/test/unit_test.hpp>

#include "DroneMessagesLib/MessageFactory.hpp"
#include "DroneMessagesLib/Messages/BatteryMessage.hpp"

namespace
{
  messages::BatteryMessage goodMessage()
  {
    messages::BatteryMessage backMessage;
    std::string toParse("battery?");
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

BOOST_AUTO_TEST_CASE(BATTERY_MESSAGES_FROM_STRING_TEST)
{
  {
    messages::BatteryMessage batteryMessage;
    std::string toParse("battery");
    BOOST_CHECK_THROW(batteryMessage.fromString(toParse), std::exception);
  }
  {
    messages::BatteryMessage batteryMessage;
    std::string toParse("batterys");
    BOOST_CHECK_THROW(batteryMessage.fromString(toParse), std::exception);
  }
  {
    messages::BatteryMessage batteryMessage;
    std::string toParse("battery?");
    BOOST_CHECK(batteryMessage.fromString(toParse));
  }
}

BOOST_AUTO_TEST_CASE(BATTERY_MESSAGES_TO_STRING_TEST)
{
  auto message = goodMessage();
  BOOST_CHECK(message.toString() == "battery?");
}

BOOST_AUTO_TEST_CASE(BATTERY_MESSAGES_FROM_JSON_TEST)
{
  rapidjson::Document doc;
  {
    messages::BatteryMessage batteryMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"battery": {"foo": "bar"}})");
    BOOST_CHECK(batteryMessage.readJson(json));
  }
  {
    messages::BatteryMessage batteryMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"battery": {}})");
    BOOST_CHECK(batteryMessage.readJson(json));
  }
}

BOOST_AUTO_TEST_CASE(BATTERY_MESSAGES_FROM_XML_TEST)
{
  auto doc = new rapidxml::xml_document<>;
  {
    messages::BatteryMessage batteryMessage;
    std::string str("<battery/>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK(
      batteryMessage.readXml((dynamic_cast<rapidxml::xml_node<>*>(doc))));
  }
  {
    messages::BatteryMessage batteryMessage;
    std::string str("<battery><distance>50</distance></battery>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK(
      batteryMessage.readXml((dynamic_cast<rapidxml::xml_node<>*>(doc))));
  }
  {
    messages::BatteryMessage batteryMessage;
    std::string str("<battery></battery>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK(
      batteryMessage.readXml((dynamic_cast<rapidxml::xml_node<>*>(doc))));
  }
  delete doc;
}

BOOST_AUTO_TEST_CASE(MESSAGE_FACTORY_BATTERY_TEST)
{
  std::string testMessage = "battery?";

  rapidjson::Document doc;
  rapidjson::Value json;
  createRapidjson(doc, json, R"({"type":"battery","data":{"battery": {}}})");
  BOOST_CHECK_EQUAL(
    boost::get<messages::BatteryMessage>(messages::getMessage(json)).toString(),
    testMessage);

  auto xmlDoc = new rapidxml::xml_document<>;
  std::string str("<type>battery</type><data><battery></battery></data>");
  xmlDoc->parse<0>((char*)str.c_str());
  BOOST_CHECK_EQUAL(
    boost::get<messages::BatteryMessage>(messages::getMessage(xmlDoc))
    .toString(),
    testMessage);
  delete xmlDoc;

  BOOST_CHECK_EQUAL(
    boost::get<messages::BatteryMessage>(messages::getMessage(testMessage))
    .toString(),
    testMessage);
}
