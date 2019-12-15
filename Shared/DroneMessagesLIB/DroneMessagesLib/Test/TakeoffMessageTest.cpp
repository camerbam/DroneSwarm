#include <boost/test/unit_test.hpp>

#include "DroneMessagesLib/MessageFactory.hpp"
#include "DroneMessagesLib/Messages/TakeoffMessage.hpp"

namespace
{
  messages::TakeoffMessage goodMessage()
  {
    messages::TakeoffMessage takeoffMessage;
    std::string toParse("takeoff");
    takeoffMessage.fromString(toParse);
    return takeoffMessage;
  }

  void createRapidjson(rapidjson::Document& doc,
                       rapidjson::Value& toReturn,
                       const std::string& toParse)
  {
    doc.Parse(toParse.c_str());
    toReturn = doc.GetObject();
  }
}

BOOST_AUTO_TEST_CASE(TAKEOFF_MESSAGES_FROM_STRING_TEST)
{
  {
    messages::TakeoffMessage takeoffMessage;
    std::string toParse("takeoffs");
    BOOST_CHECK_THROW(takeoffMessage.fromString(toParse), std::exception);
  }
  {
    messages::TakeoffMessage takeoffMessage;
    std::string toParse("takeoff");
    BOOST_CHECK(takeoffMessage.fromString(toParse));
  }
}

BOOST_AUTO_TEST_CASE(TAKEOFF_MESSAGES_TO_STRING_TEST)
{
  auto message = goodMessage();
  BOOST_CHECK(message.toString() == "takeoff");
}

BOOST_AUTO_TEST_CASE(TAKEOFF_MESSAGES_FROM_JSON_TEST)
{
  rapidjson::Document doc;
  {
    messages::TakeoffMessage takeoffMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"takeoff": {"foo": "bar"}})");
    BOOST_CHECK(takeoffMessage.readJson(json));
  }
  {
    messages::TakeoffMessage takeoffMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"takeoff": {}})");
    BOOST_CHECK(takeoffMessage.readJson(json));
  }
}

BOOST_AUTO_TEST_CASE(TAKEOFF_MESSAGES_FROM_XML_TEST)
{
  auto doc = new rapidxml::xml_document<>;
  {
    messages::TakeoffMessage takeoffMessage;
    std::string str("<takeoff/>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK(
      takeoffMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)));
  }
  {
    messages::TakeoffMessage takeoffMessage;
    std::string str("<takeoff><distance>50</distance></takeoff>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK(
      takeoffMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)));
  }
  {
    messages::TakeoffMessage takeoffMessage;
    std::string str("<takeoff></takeoff>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK(
      takeoffMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)));
  }
  delete doc;
}

BOOST_AUTO_TEST_CASE(MESSAGE_FACTORY_TAKEOFF_TEST)
{
  std::string testMessage = "takeoff";

  rapidjson::Document doc;
  rapidjson::Value json;
  createRapidjson(doc, json, R"({"type":"takeoff","data":{"takeoff": {}}})");
  BOOST_CHECK_EQUAL(
    boost::get<messages::TakeoffMessage>(messages::getMessage(json)).toString(),
    testMessage);

  auto xmlDoc = new rapidxml::xml_document<>;
  std::string str("<type>takeoff</type><data><takeoff></takeoff></data>");
  xmlDoc->parse<0>((char*)str.c_str());
  BOOST_CHECK_EQUAL(
    boost::get<messages::TakeoffMessage>(messages::getMessage(xmlDoc)).toString(),
    testMessage);
  delete xmlDoc;

  BOOST_CHECK_EQUAL(
    boost::get<messages::TakeoffMessage>(messages::getMessage(testMessage))
    .toString(),
    testMessage);
}
