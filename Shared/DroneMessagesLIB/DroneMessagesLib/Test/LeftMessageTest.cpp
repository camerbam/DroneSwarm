#include <boost/test/unit_test.hpp>

#include "DroneMessagesLib/Messages/LeftMessage.hpp"
#include "DroneMessagesLib/MessageFactory.hpp"

namespace
{
  messages::LeftMessage goodMessage(double distance)
  {
    messages::LeftMessage leftMessage;
    std::string toParse("left " + std::to_string(distance));
    leftMessage.fromString(toParse);
    return leftMessage;
  }

  void createRapidjson(rapidjson::Document& doc,
                       rapidjson::Value& toReturn,
                       const std::string& toParse)
  {
    doc.Parse(toParse.c_str());
    toReturn = doc.GetObject();
  }
}

BOOST_AUTO_TEST_CASE(LEFT_MESSAGES_CONSTRUCTOR_TEST)
{
  {
    messages::LeftMessage leftMessage(40);
    BOOST_CHECK_EQUAL(leftMessage.getArgument(), 40);
  }
  BOOST_CHECK_THROW(messages::LeftMessage leftMessage(10), std::runtime_error);
  BOOST_CHECK_THROW(messages::LeftMessage leftMessage(510), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(LEFT_MESSAGES_FROM_STRING_TEST)
{
  {
    messages::LeftMessage leftMessage;
    std::string toParse("left 20");
    BOOST_CHECK(leftMessage.fromString(toParse));
  }
  {
    messages::LeftMessage leftMessage;
    std::string toParse("left 1000");
    BOOST_CHECK_THROW(leftMessage.fromString(toParse), std::exception);
  }
  {
    messages::LeftMessage leftMessage;
    std::string toParse("lefts 100");
    BOOST_CHECK_THROW(leftMessage.fromString(toParse), std::exception);
  }
  {
    messages::LeftMessage leftMessage;
    std::string toParse("left x");
    BOOST_CHECK_THROW(leftMessage.fromString(toParse), std::exception);
  }
  {
    messages::LeftMessage leftMessage;
    std::string toParse("left");
    BOOST_CHECK_THROW(leftMessage.fromString(toParse), std::exception);
  }
}

BOOST_AUTO_TEST_CASE(LEFT_MESSAGES_TO_STRING_TEST)
{
  {
    auto message = goodMessage(50.001);
    BOOST_CHECK(message.toString() == "left 50");
  }
  {
    auto message = goodMessage(500);
    BOOST_CHECK(message.toString() == "left 500");
  }
}

BOOST_AUTO_TEST_CASE(LEFT_MESSAGES_FROM_JSON_TEST)
{
  rapidjson::Document doc;
  {
    messages::LeftMessage leftMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"left": 50})");
    BOOST_CHECK_THROW(leftMessage.readJson(json), std::exception);
  }
  {
    messages::LeftMessage leftMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"left": {"distance": "50"}})");
    BOOST_CHECK_THROW(leftMessage.readJson(json), std::exception);
  }
  {
    messages::LeftMessage leftMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"left": {"distance": "1"}})");
    BOOST_CHECK_THROW(leftMessage.readJson(json), std::exception);
  }
  {
    messages::LeftMessage leftMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"left": {"distance": 50}})");
    BOOST_CHECK(leftMessage.readJson(json));
  }
}

BOOST_AUTO_TEST_CASE(LEFT_MESSAGES_FROM_XML_TEST)
{
  auto doc = new rapidxml::xml_document<>;
  {
    messages::LeftMessage leftMessage;
    std::string str("<left></left>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK_THROW(
      leftMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)),
      std::exception);
  }
  {
    messages::LeftMessage leftMessage;
    std::string str("<left><distance>1</distance></left>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK_THROW(
      leftMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)),
      std::exception);
  }
  {
    messages::LeftMessage leftMessage;
    std::string str("<left><distance>50</distance></left>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK(leftMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)));
  }
  delete doc;
}

BOOST_AUTO_TEST_CASE(MESSAGE_FACTORY_LEFT_TEST)
{
  std::string testMessage = "left 58";

  rapidjson::Document doc;
  rapidjson::Value json;
  createRapidjson(
    doc, json, R"({"type":"left","data":{"left": {"distance": 58}}})");
  BOOST_CHECK_EQUAL(
    boost::get<messages::LeftMessage>(messages::getMessage(json)).toString(),
    testMessage);

  auto xmlDoc = new rapidxml::xml_document<>;
  std::string str(
    "<type>left</type><data><left><distance>58</distance></left></data>");
  xmlDoc->parse<0>((char*)str.c_str());
  BOOST_CHECK_EQUAL(
    boost::get<messages::LeftMessage>(messages::getMessage(xmlDoc)).toString(),
    testMessage);
  delete xmlDoc;

  BOOST_CHECK_EQUAL(
    boost::get<messages::LeftMessage>(messages::getMessage(testMessage))
    .toString(),
    testMessage);
}
