#include <boost/test/unit_test.hpp>

#include "DroneMessagesLib/Messages/ClockwiseMessage.hpp"
#include "DroneMessagesLib/MessageFactory.hpp"

namespace
{
  messages::ClockwiseMessage goodMessage(double distance)
  {
    messages::ClockwiseMessage clockwiseMessage;
    std::string toParse("cw " + std::to_string(distance));
    clockwiseMessage.fromString(toParse);
    return clockwiseMessage;
  }

  void createRapidjson(rapidjson::Document& doc,
                       rapidjson::Value& toReturn,
                       const std::string& toParse)
  {
    doc.Parse(toParse.c_str());
    toReturn = doc.GetObject();
  }
}

BOOST_AUTO_TEST_CASE(CLOCKWISE_MESSAGES_CONSTRUCTOR_TEST)
{
  {
    messages::ClockwiseMessage clockwiseMessage(40);
    BOOST_CHECK_EQUAL(clockwiseMessage.getArgument(), 40);
  }
  BOOST_CHECK_THROW(messages::ClockwiseMessage clockwiseMessage(.1), std::runtime_error);
  BOOST_CHECK_THROW(messages::ClockwiseMessage clockwiseMessage(510), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(CLOCKWISE_MESSAGES_FROM_STRING_TEST)
{
  {
    messages::ClockwiseMessage clockwiseMessage;
    std::string toParse("cw 20");
    BOOST_CHECK(clockwiseMessage.fromString(toParse));
  }
  {
    messages::ClockwiseMessage clockwiseMessage;
    std::string toParse("cw 1000");
    BOOST_CHECK_THROW(clockwiseMessage.fromString(toParse), std::exception);
  }
  {
    messages::ClockwiseMessage clockwiseMessage;
    std::string toParse("cws 100");
    BOOST_CHECK_THROW(clockwiseMessage.fromString(toParse), std::exception);
  }
  {
    messages::ClockwiseMessage clockwiseMessage;
    std::string toParse("cw x");
    BOOST_CHECK_THROW(clockwiseMessage.fromString(toParse), std::exception);
  }
  {
    messages::ClockwiseMessage clockwiseMessage;
    std::string toParse("cw");
    BOOST_CHECK_THROW(clockwiseMessage.fromString(toParse), std::exception);
  }
}

BOOST_AUTO_TEST_CASE(CLOCKWISE_MESSAGES_TO_STRING_TEST)
{
  {
    auto message = goodMessage(50.001);
    BOOST_CHECK(message.toString() == "cw 50");
  }
  {
    auto message = goodMessage(360);
    BOOST_CHECK(message.toString() == "cw 360");
  }
}

BOOST_AUTO_TEST_CASE(CLOCKWISE_MESSAGES_FROM_JSON_TEST)
{
  rapidjson::Document doc;
  {
    messages::ClockwiseMessage clockwiseMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"cw": {"angle": "50"}})");
    BOOST_CHECK_THROW(clockwiseMessage.readJson(json), std::exception);
  }
  {
    messages::ClockwiseMessage clockwiseMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"cw": {"angle": ".1"}})");
    BOOST_CHECK_THROW(clockwiseMessage.readJson(json), std::exception);
  }
  {
    messages::ClockwiseMessage clockwiseMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"cw": {"angle": 50}})");
    BOOST_CHECK(clockwiseMessage.readJson(json));
  }
}

BOOST_AUTO_TEST_CASE(CLOCKWISE_MESSAGES_FROM_XML_TEST)
{
  auto doc = new rapidxml::xml_document<>;
  {
    messages::ClockwiseMessage clockwiseMessage;
    std::string str("<cw></cw>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK_THROW(
      clockwiseMessage.readXml((dynamic_cast<rapidxml::xml_node<>*>(doc))),
      std::exception);
  }
  {
    messages::ClockwiseMessage clockwiseMessage;
    std::string str("<cw><angles>1</angles></cw>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK_THROW(
      clockwiseMessage.readXml((dynamic_cast<rapidxml::xml_node<>*>(doc))),
      std::exception);
  }
  {
    messages::ClockwiseMessage clockwiseMessage;
    std::string str("<cw><angle>50</angle></cw>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK(
      clockwiseMessage.readXml((dynamic_cast<rapidxml::xml_node<>*>(doc))));
  }
  delete doc;
}

BOOST_AUTO_TEST_CASE(MESSAGE_FACTORY_CLOCKWISE_TEST)
{
  std::string testMessage = "cw 75";

  rapidjson::Document doc;
  rapidjson::Value json;
  createRapidjson(doc, json, R"({"type":"cw","data":{"cw": {"angle": 75}}})");
  BOOST_CHECK_EQUAL(
    boost::get<messages::ClockwiseMessage>(messages::getMessage(json))
    .toString(),
    testMessage);

  auto xmlDoc = new rapidxml::xml_document<>;
  std::string str("<type>cw</type><data><cw><angle>75</angle></cw></data>");
  xmlDoc->parse<0>((char*)str.c_str());
  BOOST_CHECK_EQUAL(
    boost::get<messages::ClockwiseMessage>(messages::getMessage(xmlDoc))
    .toString(),
    testMessage);
  delete xmlDoc;

  BOOST_CHECK_EQUAL(
    boost::get<messages::ClockwiseMessage>(messages::getMessage(testMessage))
    .toString(),
    testMessage);
}
