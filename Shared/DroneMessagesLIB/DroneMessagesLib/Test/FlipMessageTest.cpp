#include <boost/test/unit_test.hpp>

#include "DroneMessagesLib/Messages/FlipMessage.hpp"
#include "DroneMessagesLib/MessageFactory.hpp"

namespace
{
  messages::FlipMessage goodMessage(const std::string& direction)
  {
    messages::FlipMessage flipMessage;
    std::string toParse("flip " + direction);
    flipMessage.fromString(toParse);
    return flipMessage;
  }

  void createRapidjson(rapidjson::Document& doc,
                       rapidjson::Value& toReturn,
                       const std::string& toParse)
  {
    doc.Parse(toParse.c_str());
    toReturn = doc.GetObject();
  }

  void flipMessageFromStringTest(std::string& toParse, bool succeed)
  {
    messages::FlipMessage flipMessage;
    BOOST_CHECK(flipMessage.fromString(toParse) == succeed);
  }
}

BOOST_AUTO_TEST_CASE(FLIP_MESSAGES_CONSTRUCTOR_TEST)
{
  {
    messages::FlipMessage flipMessage("f");
    BOOST_CHECK(flipMessage.getDirection() == messages::Direction::FORWARD);
  }
  BOOST_CHECK_THROW(messages::FlipMessage flipMessage("u"), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(FLIP_MESSAGES_FROM_STRING_TEST)
{
  std::string command("flip f");
  flipMessageFromStringTest(command, true);
  command = "flip r";
  flipMessageFromStringTest(command, true);
  command = "flip b";
  flipMessageFromStringTest(command, true);
  command = "flip l";
  flipMessageFromStringTest(command, true);
  command = "flip u";
  flipMessageFromStringTest(command, false);
  command = "flip";
  flipMessageFromStringTest(command, false);
}

BOOST_AUTO_TEST_CASE(FLIP_MESSAGES_TO_STRING_TEST)
{
  {
    auto message = goodMessage("f");
    BOOST_CHECK(message.toString() == "flip f");
  }
  {
    auto message = goodMessage("r");
    BOOST_CHECK(message.toString() == "flip r");
  }
  {
    auto message = goodMessage("l");
    BOOST_CHECK(message.toString() == "flip l");
  }
  {
    auto message = goodMessage("b");
    BOOST_CHECK(message.toString() == "flip b");
  }
}

BOOST_AUTO_TEST_CASE(FLIP_MESSAGES_FROM_JSON_TEST)
{
  rapidjson::Document doc;
  {
    messages::FlipMessage flipMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"flips": 50})");
    BOOST_CHECK_THROW(flipMessage.readJson(json), std::exception);
  }
  {
    messages::FlipMessage flipMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"flip": 50})");
    BOOST_CHECK_THROW(flipMessage.readJson(json), std::exception);
  }
  {
    messages::FlipMessage flipMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"flip": {"direction": "u"}})");
    BOOST_CHECK_THROW(flipMessage.readJson(json), std::exception);
  }
  {
    messages::FlipMessage flipMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"flip": {"direction": }})");
    BOOST_CHECK_THROW(flipMessage.readJson(json), std::exception);
  }
  {
    messages::FlipMessage flipMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"flip": {"direction": "f"}})");
    BOOST_CHECK(flipMessage.readJson(json));
  }
  {
    messages::FlipMessage flipMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"flip": {"direction": "r"}})");
    BOOST_CHECK(flipMessage.readJson(json));
  }
  {
    messages::FlipMessage flipMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"flip": {"direction": "b"}})");
    BOOST_CHECK(flipMessage.readJson(json));
  }
  {
    messages::FlipMessage flipMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"flip": {"direction": "l"}})");
    BOOST_CHECK(flipMessage.readJson(json));
  }
}

BOOST_AUTO_TEST_CASE(FLIP_MESSAGES_FROM_XML_TEST)
{
  auto doc = new rapidxml::xml_document<>;
  {
    messages::FlipMessage flipMessage;
    std::string str("<flip></flip>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK_THROW(
      flipMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)),
      std::exception);
  }
  {
    messages::FlipMessage flipMessage;
    std::string str("<flip><direction>u</direction></flip>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK_THROW(
      flipMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)),
      std::exception);
  }
  {
    messages::FlipMessage flipMessage;
    std::string str("<flip><direction>f</direction></flip>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK(flipMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)));
  }
  {
    messages::FlipMessage flipMessage;
    std::string str("<flip><direction>r</direction></flip>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK(flipMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)));
  }
  {
    messages::FlipMessage flipMessage;
    std::string str("<flip><direction>b</direction></flip>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK(flipMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)));
  }
  {
    messages::FlipMessage flipMessage;
    std::string str("<flip><direction>l</direction></flip>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK(flipMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)));
  }
  delete doc;
}

BOOST_AUTO_TEST_CASE(MESSAGE_FACTORY_FLIP_TEST)
{
  std::string testMessage = "flip f";

  rapidjson::Document doc;
  rapidjson::Value json;
  createRapidjson(
    doc, json, R"({"type":"flip","data":{"flip": {"direction": "f"}}})");
  BOOST_CHECK_EQUAL(
    boost::get<messages::FlipMessage>(messages::getMessage(json)).toString(),
    testMessage);

  auto xmlDoc = new rapidxml::xml_document<>;
  std::string str(
    "<type>flip</type><data><flip><direction>f</direction></flip></data>");
  xmlDoc->parse<0>((char*)str.c_str());
  BOOST_CHECK_EQUAL(
    boost::get<messages::FlipMessage>(messages::getMessage(xmlDoc)).toString(),
    testMessage);
  delete xmlDoc;

  BOOST_CHECK_EQUAL(
    boost::get<messages::FlipMessage>(messages::getMessage(testMessage))
    .toString(),
    testMessage);
}
