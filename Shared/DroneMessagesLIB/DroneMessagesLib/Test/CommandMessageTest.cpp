#include <boost/test/unit_test.hpp>

#include "DroneMessagesLib/Messages/CommandMessage.hpp"
#include "DroneMessagesLib/MessageFactory.hpp"

namespace
{
  messages::CommandMessage goodMessage()
  {
    messages::CommandMessage commandMessage;
    std::string toParse("command");
    commandMessage.fromString(toParse);
    return commandMessage;
  }

  void createRapidjson(rapidjson::Document& doc,
                       rapidjson::Value& toReturn,
                       const std::string& toParse)
  {
    doc.Parse(toParse.c_str());
    toReturn = doc.GetObject();
  }
}

BOOST_AUTO_TEST_CASE(COMMAND_MESSAGES_FROM_STRING_TEST)
{
  {
    messages::CommandMessage commandMessage;
    std::string toParse("commands");
    BOOST_CHECK_THROW(commandMessage.fromString(toParse), std::exception);
  }
  {
    messages::CommandMessage commandMessage;
    std::string toParse("command");
    BOOST_CHECK(commandMessage.fromString(toParse));
  }
}

BOOST_AUTO_TEST_CASE(COMMAND_MESSAGES_TO_STRING_TEST)
{
  auto message = goodMessage();
  BOOST_CHECK(message.toString() == "command");
}

BOOST_AUTO_TEST_CASE(COMMAND_MESSAGES_FROM_JSON_TEST)
{
  rapidjson::Document doc;
  {
    messages::CommandMessage commandMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"command": {"foo": "bar"}})");
    BOOST_CHECK(commandMessage.readJson(json));
  }
  {
    messages::CommandMessage commandMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"command": {}})");
    BOOST_CHECK(commandMessage.readJson(json));
  }
}

BOOST_AUTO_TEST_CASE(COMMAND_MESSAGES_FROM_XML_TEST)
{
  auto doc = new rapidxml::xml_document<>;
  {
    messages::CommandMessage commandMessage;
    std::string str("<command/>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK(commandMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)));
  }
  {
    messages::CommandMessage commandMessage;
    std::string str("<command><distance>50</distance></command>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK(commandMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)));
  }
  {
    messages::CommandMessage commandMessage;
    std::string str("<command></command>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK(commandMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)));
  }
  delete doc;
}

BOOST_AUTO_TEST_CASE(MESSAGE_FACTORY_COMMAND_TEST)
{
  std::string testMessage = "command";

  rapidjson::Document doc;
  rapidjson::Value json;
  createRapidjson(doc, json, R"({"type":"command","data":{"command": {}}})");
  BOOST_CHECK_EQUAL(
    boost::get<messages::CommandMessage>(messages::getMessage(json)).toString(),
    testMessage);

  auto xmlDoc = new rapidxml::xml_document<>;
  std::string str("<type>command</type><data><command></command></data>");
  xmlDoc->parse<0>((char*)str.c_str());
  BOOST_CHECK_EQUAL(
    boost::get<messages::CommandMessage>(messages::getMessage(xmlDoc))
    .toString(),
    testMessage);
  delete xmlDoc;

  BOOST_CHECK_EQUAL(
    boost::get<messages::CommandMessage>(messages::getMessage(testMessage))
    .toString(),
    testMessage);
}
