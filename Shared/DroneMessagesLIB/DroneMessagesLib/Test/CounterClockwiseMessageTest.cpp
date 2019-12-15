#include <boost/test/unit_test.hpp>

#include "DroneMessagesLib/MessageFactory.hpp"
#include "DroneMessagesLib/Messages/CounterClockwiseMessage.hpp"

namespace
{
  messages::CounterClockwiseMessage goodMessage(double angle)
  {
    messages::CounterClockwiseMessage counterClockwiseMessage;
    std::string toParse("ccw " + std::to_string(angle));
    counterClockwiseMessage.fromString(toParse);
    return counterClockwiseMessage;
  }

  void createRapidjson(rapidjson::Document& doc,
                       rapidjson::Value& toReturn,
                       const std::string& toParse)
  {
    doc.Parse(toParse.c_str());
    toReturn = doc.GetObject();
  }
}

BOOST_AUTO_TEST_CASE(COUNTER_CLOCKWISE_MESSAGES_CONSTRUCTOR_TEST)
{
  {
    messages::CounterClockwiseMessage counterClockwiseMessage(40);
    BOOST_CHECK_EQUAL(counterClockwiseMessage.getArgument(), 40);
  }
  BOOST_CHECK_THROW(messages::CounterClockwiseMessage counterClockwiseMessage(.1), std::runtime_error);
  BOOST_CHECK_THROW(messages::CounterClockwiseMessage counterClockwiseMessage(510), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(COUNTER_CLOCKWISE_MESSAGES_FROM_STRING_TEST)
{
  {
    messages::CounterClockwiseMessage counterClockwiseMessage;
    std::string toParse("ccw 20");
    BOOST_CHECK(counterClockwiseMessage.fromString(toParse));
  }
  {
    messages::CounterClockwiseMessage counterClockwiseMessage;
    std::string toParse("ccw 1000");
    BOOST_CHECK_THROW(
      counterClockwiseMessage.fromString(toParse), std::exception);
  }
  {
    messages::CounterClockwiseMessage counterClockwiseMessage;
    std::string toParse("ccws 100");
    BOOST_CHECK_THROW(
      counterClockwiseMessage.fromString(toParse), std::exception);
  }
  {
    messages::CounterClockwiseMessage counterClockwiseMessage;
    std::string toParse("ccw x");
    BOOST_CHECK_THROW(
      counterClockwiseMessage.fromString(toParse), std::exception);
  }
  {
    messages::CounterClockwiseMessage counterClockwiseMessage;
    std::string toParse("ccw");
    BOOST_CHECK_THROW(
      counterClockwiseMessage.fromString(toParse), std::exception);
  }
}

BOOST_AUTO_TEST_CASE(COUNTER_CLOCKWISE_MESSAGES_TO_STRING_TEST)
{
  {
    auto message = goodMessage(50.001);
    BOOST_CHECK(message.toString() == "ccw 50");
  }
  {
    auto message = goodMessage(360);
    BOOST_CHECK(message.toString() == "ccw 360");
  }
}

BOOST_AUTO_TEST_CASE(COUNTER_CLOCKWISE_MESSAGES_FROM_JSON_TEST)
{
  rapidjson::Document doc;
  {
    messages::CounterClockwiseMessage counterClockwiseMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"ccw": {"angle": "50"}})");
    BOOST_CHECK_THROW(counterClockwiseMessage.readJson(json), std::exception);
  }
  {
    messages::CounterClockwiseMessage counterClockwiseMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"ccw": {"angle": ".1"}})");
    BOOST_CHECK_THROW(counterClockwiseMessage.readJson(json), std::exception);
  }
  {
    messages::CounterClockwiseMessage counterClockwiseMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"ccw": {"angle": 50}})");
    BOOST_CHECK(counterClockwiseMessage.readJson(json));
  }
}

BOOST_AUTO_TEST_CASE(COUNTER_CLOCKWISE_MESSAGES_FROM_XML_TEST)
{
  auto doc = new rapidxml::xml_document<>;
  {
    messages::CounterClockwiseMessage counterClockwiseMessage;
    std::string str("<ccw></ccw>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK_THROW(
      counterClockwiseMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)),
      std::exception);
  }
  {
    messages::CounterClockwiseMessage counterClockwiseMessage;
    std::string str("<ccw><angles>1</angles></ccw>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK_THROW(
      counterClockwiseMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)),
      std::exception);
  }
  {
    messages::CounterClockwiseMessage counterClockwiseMessage;
    std::string str("<ccw><angle>50</angle></ccw>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK(counterClockwiseMessage.readXml(
      dynamic_cast<rapidxml::xml_node<>*>(doc)));
  }
  delete doc;
}

BOOST_AUTO_TEST_CASE(MESSAGE_FACTORY_COUNTER_CLOCKWISE_TEST)
{
  std::string testMessage = "ccw 55";

  rapidjson::Document doc;
  rapidjson::Value json;
  createRapidjson(doc, json, R"({"type":"ccw","data":{"ccw": {"angle": 55}}})");
  BOOST_CHECK_EQUAL(
    boost::get<messages::CounterClockwiseMessage>(messages::getMessage(json))
    .toString(),
    testMessage);

  auto xmlDoc = new rapidxml::xml_document<>;
  std::string str("<type>ccw</type><data><ccw><angle>55</angle></ccw></data>");
  xmlDoc->parse<0>((char*)str.c_str());
  BOOST_CHECK_EQUAL(
    boost::get<messages::CounterClockwiseMessage>(messages::getMessage(xmlDoc))
    .toString(),
    testMessage);
  delete xmlDoc;

  BOOST_CHECK_EQUAL(boost::get<messages::CounterClockwiseMessage>(
    messages::getMessage(testMessage))
    .toString(),
    testMessage);
}
