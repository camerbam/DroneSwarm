#include <boost/test/unit_test.hpp>

#include "DroneMessagesLib/MessageFactory.hpp"
#include "DroneMessagesLib/Messages/TimeMessage.hpp"

namespace
{
  messages::TimeMessage goodMessage()
  {
    messages::TimeMessage backMessage;
    std::string toParse("time?");
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

BOOST_AUTO_TEST_CASE(TIME_MESSAGES_FROM_STRING_TEST)
{
  {
    messages::TimeMessage timeMessage;
    std::string toParse("time");
    BOOST_CHECK_THROW(timeMessage.fromString(toParse), std::exception);
  }
  {
    messages::TimeMessage timeMessage;
    std::string toParse("times");
    BOOST_CHECK_THROW(timeMessage.fromString(toParse), std::exception);
  }
  {
    messages::TimeMessage timeMessage;
    std::string toParse("time?");
    BOOST_CHECK(timeMessage.fromString(toParse));
  }
}

BOOST_AUTO_TEST_CASE(TIME_MESSAGES_TO_STRING_TEST)
{
  auto message = goodMessage();
  BOOST_CHECK(message.toString() == "time?");
}

BOOST_AUTO_TEST_CASE(TIME_MESSAGES_FROM_JSON_TEST)
{
  rapidjson::Document doc;
  {
    messages::TimeMessage timeMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"time": {"foo": "bar"}})");
    BOOST_CHECK(timeMessage.readJson(json));
  }
  {
    messages::TimeMessage timeMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"time": {}})");
    BOOST_CHECK(timeMessage.readJson(json));
  }
}

BOOST_AUTO_TEST_CASE(TIME_MESSAGES_FROM_XML_TEST)
{
  auto doc = new rapidxml::xml_document<>;
  {
    messages::TimeMessage timeMessage;
    std::string str("<time/>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK(timeMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)));
  }
  {
    messages::TimeMessage timeMessage;
    std::string str("<time><distance>50</distance></time>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK(timeMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)));
  }
  {
    messages::TimeMessage timeMessage;
    std::string str("<time></time>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK(timeMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)));
  }
  delete doc;
}

BOOST_AUTO_TEST_CASE(MESSAGE_FACTORY_TIME_TEST)
{
  std::string testMessage = "time?";

  rapidjson::Document doc;
  rapidjson::Value json;
  createRapidjson(doc, json, R"({"type":"time","data":{"time": {}}})");
  BOOST_CHECK_EQUAL(
    boost::get<messages::TimeMessage>(messages::getMessage(json)).toString(),
    testMessage);

  auto xmlDoc = new rapidxml::xml_document<>;
  std::string str("<type>time</type><data><time></time></data>");
  xmlDoc->parse<0>((char*)str.c_str());
  BOOST_CHECK_EQUAL(
    boost::get<messages::TimeMessage>(messages::getMessage(xmlDoc)).toString(),
    testMessage);
  delete xmlDoc;

  BOOST_CHECK_EQUAL(
    boost::get<messages::TimeMessage>(messages::getMessage(testMessage))
    .toString(),
    testMessage);
}
