#include <boost/test/unit_test.hpp>

#include "DroneMessagesLib/MessageFactory.hpp"
#include "DroneMessagesLib/Messages/DownMessage.hpp"

namespace
{
  messages::DownMessage goodMessage(double distance)
  {
    messages::DownMessage downMessage;
    std::string toParse("down " + std::to_string(distance));
    downMessage.fromString(toParse);
    return downMessage;
  }
  void createRapidjson(rapidjson::Document& doc,
                       rapidjson::Value& toReturn,
                       const std::string& toParse)
  {
    doc.Parse(toParse.c_str());
    toReturn = doc.GetObject();
  }
}

BOOST_AUTO_TEST_CASE(DOWN_MESSAGES_CONSTRUCTOR_TEST)
{
  {
    messages::DownMessage downMessage(40);
    BOOST_CHECK_EQUAL(downMessage.getArgument(), 40);
  }
  BOOST_CHECK_THROW(messages::DownMessage downMessage(10), std::runtime_error);
  BOOST_CHECK_THROW(messages::DownMessage downMessage(510), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(DOWN_MESSAGES_FROM_STRING_TEST)
{
  {
    messages::DownMessage downMessage;
    std::string toParse("down 20");
    BOOST_CHECK(downMessage.fromString(toParse));
  }
  {
    messages::DownMessage downMessage;
    std::string toParse("down 1000");
    BOOST_CHECK_THROW(downMessage.fromString(toParse), std::exception);
  }
  {
    messages::DownMessage downMessage;
    std::string toParse("downs 100");
    BOOST_CHECK_THROW(downMessage.fromString(toParse), std::exception);
  }
  {
    messages::DownMessage downMessage;
    std::string toParse("down x");
    BOOST_CHECK_THROW(downMessage.fromString(toParse), std::exception);
  }
  {
    messages::DownMessage downMessage;
    std::string toParse("down");
    BOOST_CHECK_THROW(downMessage.fromString(toParse), std::exception);
  }
}

BOOST_AUTO_TEST_CASE(DOWN_MESSAGES_TO_STRING_TEST)
{
  {
    auto message = goodMessage(50.001);
    BOOST_CHECK(message.toString() == "down 50");
  }
  {
    auto message = goodMessage(500);
    BOOST_CHECK(message.toString() == "down 500");
  }
}

BOOST_AUTO_TEST_CASE(DOWN_MESSAGES_FROM_JSON_TEST)
{
  rapidjson::Document doc;
  {
    messages::DownMessage downMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"down": 50})");
    BOOST_CHECK_THROW(downMessage.readJson(json), std::exception);
  }
  {
    messages::DownMessage downMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"down": {"distance": "50"}})");
    BOOST_CHECK_THROW(downMessage.readJson(json), std::exception);
  }
  {
    messages::DownMessage downMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"down": {"distance": "1"}})");
    BOOST_CHECK_THROW(downMessage.readJson(json), std::exception);
  }
  {
    messages::DownMessage downMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"down": {"distance": 50}})");
    BOOST_CHECK(downMessage.readJson(json));
  }
}

BOOST_AUTO_TEST_CASE(DOWN_MESSAGES_FROM_XML_TEST)
{
  auto doc = new rapidxml::xml_document<>;
  {
    messages::DownMessage downMessage;
    std::string str("<down></down>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK_THROW(
      downMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)),
      std::exception);
  }
  {
    messages::DownMessage downMessage;
    std::string str("<down><distance>1</distance></down>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK_THROW(
      downMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)),
      std::exception);
  }
  {
    messages::DownMessage downMessage;
    std::string str("<down><distance>50</distance></down>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK(downMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)));
  }
  delete doc;
}

BOOST_AUTO_TEST_CASE(MESSAGE_FACTORY_DOWN_TEST)
{
  std::string testMessage = "down 45";

  rapidjson::Document doc;
  rapidjson::Value json;
  createRapidjson(
    doc, json, R"({"type":"down","data":{"down": {"distance": 45}}})");
  BOOST_CHECK_EQUAL(
    boost::get<messages::DownMessage>(messages::getMessage(json)).toString(),
    testMessage);

  auto xmlDoc = new rapidxml::xml_document<>;
  std::string str(
    "<type>down</type><data><down><distance>45</distance></down></data>");
  xmlDoc->parse<0>((char*)str.c_str());
  BOOST_CHECK_EQUAL(
    boost::get<messages::DownMessage>(messages::getMessage(xmlDoc)).toString(),
    testMessage);
  delete xmlDoc;

  BOOST_CHECK_EQUAL(
    boost::get<messages::DownMessage>(messages::getMessage(testMessage))
    .toString(),
    testMessage);
}
