#include <boost/test/unit_test.hpp>

#include "DroneMessagesLib/Messages/RightMessage.hpp"
#include "DroneMessagesLib/MessageFactory.hpp"

namespace
{
  messages::RightMessage goodMessage(double distance)
  {
    messages::RightMessage rightMessage;
    std::string toParse("right " + std::to_string(distance));
    rightMessage.fromString(toParse);
    return rightMessage;
  }

  void createRapidjson(rapidjson::Document& doc,
                       rapidjson::Value& toReturn,
                       const std::string& toParse)
  {
    doc.Parse(toParse.c_str());
    toReturn = doc.GetObject();
  }
}

BOOST_AUTO_TEST_CASE(RIGHT_MESSAGES_CONSTRUCTOR_TEST)
{
  {
    messages::RightMessage rightMessage(40);
    BOOST_CHECK_EQUAL(rightMessage.getArgument(), 40);
  }
  BOOST_CHECK_THROW(messages::RightMessage rightMessage(10), std::runtime_error);
  BOOST_CHECK_THROW(messages::RightMessage rightMessage(510), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(RIGHT_MESSAGES_FROM_STRING_TEST)
{
  {
    messages::RightMessage rightMessage;
    std::string toParse("right 20");
    BOOST_CHECK(rightMessage.fromString(toParse));
  }
  {
    messages::RightMessage rightMessage;
    std::string toParse("right 1000");
    BOOST_CHECK_THROW(rightMessage.fromString(toParse), std::exception);
  }
  {
    messages::RightMessage rightMessage;
    std::string toParse("rights 100");
    BOOST_CHECK_THROW(rightMessage.fromString(toParse), std::exception);
  }
  {
    messages::RightMessage rightMessage;
    std::string toParse("right x");
    BOOST_CHECK_THROW(rightMessage.fromString(toParse), std::exception);
  }
  {
    messages::RightMessage rightMessage;
    std::string toParse("right");
    BOOST_CHECK_THROW(rightMessage.fromString(toParse), std::exception);
  }
}

BOOST_AUTO_TEST_CASE(RIGHT_MESSAGES_TO_STRING_TEST)
{
  {
    auto message = goodMessage(50.001);
    BOOST_CHECK(message.toString() == "right 50");
  }
  {
    auto message = goodMessage(500);
    BOOST_CHECK(message.toString() == "right 500");
  }
}

BOOST_AUTO_TEST_CASE(RIGHT_MESSAGES_FROM_JSON_TEST)
{
  rapidjson::Document doc;
  {
    messages::RightMessage rightMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"right": 50})");
    BOOST_CHECK_THROW(rightMessage.readJson(json), std::exception);
  }
  {
    messages::RightMessage rightMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"right": {"distance": "50"}})");
    BOOST_CHECK_THROW(rightMessage.readJson(json), std::exception);
  }
  {
    messages::RightMessage rightMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"right": {"distance": "1"}})");
    BOOST_CHECK_THROW(rightMessage.readJson(json), std::exception);
  }
  {
    messages::RightMessage rightMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"right": {"distance": 50}})");
    BOOST_CHECK(rightMessage.readJson(json));
  }
}

BOOST_AUTO_TEST_CASE(RIGHT_MESSAGES_FROM_XML_TEST)
{
  auto doc = new rapidxml::xml_document<>;
  {
    messages::RightMessage rightMessage;
    std::string str("<right></right>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK_THROW(
      rightMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)),
      std::exception);
  }
  {
    messages::RightMessage rightMessage;
    std::string str("<right><distance>1</distance></right>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK_THROW(
      rightMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)),
      std::exception);
  }
  {
    messages::RightMessage rightMessage;
    std::string str("<right><distance>50</distance></right>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK(rightMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)));
  }
  delete doc;
}

BOOST_AUTO_TEST_CASE(MESSAGE_FACTORY_RIGHT_TEST)
{
  std::string testMessage = "right 59";

  rapidjson::Document doc;
  rapidjson::Value json;
  createRapidjson(
    doc, json, R"({"type":"right","data":{"right": {"distance": 59}}})");
  BOOST_CHECK_EQUAL(
    boost::get<messages::RightMessage>(messages::getMessage(json)).toString(),
    testMessage);

  auto xmlDoc = new rapidxml::xml_document<>;
  std::string str(
    "<type>right</type><data><right><distance>59</distance></right></data>");
  xmlDoc->parse<0>((char*)str.c_str());
  BOOST_CHECK_EQUAL(
    boost::get<messages::RightMessage>(messages::getMessage(xmlDoc)).toString(),
    testMessage);
  delete xmlDoc;

  BOOST_CHECK_EQUAL(
    boost::get<messages::RightMessage>(messages::getMessage(testMessage))
    .toString(),
    testMessage);
}
