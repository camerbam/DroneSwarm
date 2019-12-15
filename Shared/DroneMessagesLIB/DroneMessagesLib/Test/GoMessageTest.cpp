#include <boost/test/unit_test.hpp>

#include "DroneMessagesLib/MessageFactory.hpp"
#include "DroneMessagesLib/Messages/GoMessage.hpp"

namespace
{
  messages::GoMessage goodMessage(const double& x,
                                  const double& y,
                                  const double& z,
                                  const double& speed)
  {
    messages::GoMessage goMessage;
    std::string toParse("go " + std::to_string(x) + " " + std::to_string(y) +
                        " " + std::to_string(z) + " " + std::to_string(speed));
    goMessage.fromString(toParse);
    return goMessage;
  }

  void createRapidjson(rapidjson::Document& doc,
                       rapidjson::Value& toReturn,
                       const std::string& toParse)
  {
    doc.Parse(toParse.c_str());
    toReturn = doc.GetObject();
  }
}

BOOST_AUTO_TEST_CASE(GO_MESSAGES_FROM_STRING_TEST)
{
  std::string command("go");
  {
    messages::GoMessage goMessage;
    BOOST_CHECK_THROW(goMessage.fromString(command), std::exception);
  }
  {
    command = "go 50";
    messages::GoMessage goMessage;
    BOOST_CHECK_THROW(goMessage.fromString(command), std::exception);
  }
  {
    command = "go 100 100";
    messages::GoMessage goMessage;
    BOOST_CHECK_THROW(goMessage.fromString(command), std::exception);
  }
  {
    command = "go 50 50 10";
    messages::GoMessage goMessage;
    BOOST_CHECK_THROW(goMessage.fromString(command), std::exception);
  }
  {
    command = "go 50 501 50 40";
    messages::GoMessage goMessage;
    BOOST_CHECK_THROW(goMessage.fromString(command), std::exception);
  }
  {
    command = "go 50 10 50 50";
    messages::GoMessage goMessage;
    BOOST_CHECK_THROW(goMessage.fromString(command), std::exception);
  }
  {
    command = "go 50 52 50 5";
    messages::GoMessage goMessage;
    BOOST_CHECK_THROW(goMessage.fromString(command), std::exception);
  }
  {
    command = "go 50 52 50 105";
    messages::GoMessage goMessage;
    BOOST_CHECK_THROW(goMessage.fromString(command), std::exception);
  }
  {
    command = "go 50 40 60 40";
    messages::GoMessage goMessage;
    BOOST_CHECK(goMessage.fromString(command));
  }
}

BOOST_AUTO_TEST_CASE(GO_MESSAGES_CONSTRUCTOR_TEST)
{
  {
    messages::GoMessage goMessage(40, 50, 60, 70);
    BOOST_CHECK_EQUAL(goMessage.getXDistance(), 40);
    BOOST_CHECK_EQUAL(goMessage.getYDistance(), 50);
    BOOST_CHECK_EQUAL(goMessage.getZDistance(), 60);
    BOOST_CHECK_EQUAL(goMessage.getSpeed(), 70);
  }
  BOOST_CHECK_THROW(messages::GoMessage goMessage(10, 40, 50, 60), std::runtime_error);
  BOOST_CHECK_THROW(messages::GoMessage goMessage(40, 50, 60, 9), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(GO_MESSAGES_TO_STRING_TEST)
{
  {
    auto message = goodMessage(50, 50, 50, 50);
    BOOST_CHECK(message.toString() == "go 50 50 50 50");
  }
  {
    auto message = goodMessage(50, 50, 40, 60);
    BOOST_CHECK(message.toString() == "go 50 50 40 60");
  }
  {
    auto message = goodMessage(450, 452, 82, 41);
    BOOST_CHECK(message.toString() == "go 450 452 82 41");
  }
}

BOOST_AUTO_TEST_CASE(GO_MESSAGES_FROM_JSON_TEST)
{
  rapidjson::Document doc;
  {
    messages::GoMessage goMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"gos": 50})");
    BOOST_CHECK_THROW(goMessage.readJson(json), std::exception);
  }
  {
    messages::GoMessage goMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"go": 50})");
    BOOST_CHECK_THROW(goMessage.readJson(json), std::exception);
  }
  {
    messages::GoMessage goMessage;
    rapidjson::Value json;
    createRapidjson(doc, json, R"({"go": {"xDirection": "up"}})");
    BOOST_CHECK_THROW(goMessage.readJson(json), std::exception);
  }
  {
    messages::GoMessage goMessage;
    rapidjson::Value json;
    createRapidjson(
      doc,
      json,
      R"({"go": {"xDirection": 50, "yDirection": 50, "zDirection": 50})");
    BOOST_CHECK_THROW(goMessage.readJson(json), std::exception);
  }
  {
    messages::GoMessage goMessage;
    rapidjson::Value json;
    createRapidjson(doc,
                    json,
                    R"({"go": {"xDirection": 50, 
                "yDirection": 50, 
                "zDirection": 50,
                "speed": 50}})");
    BOOST_CHECK(goMessage.readJson(json));
  }
}

BOOST_AUTO_TEST_CASE(GO_MESSAGES_FROM_XML_TEST)
{
  auto doc = new rapidxml::xml_document<>;
  {
    messages::GoMessage goMessage;
    std::string str("<go></go>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK_THROW(
      goMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)),
      std::exception);
  }
  {
    messages::GoMessage goMessage;
    std::string str("<gos>50</gos>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK_THROW(
      goMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)),
      std::exception);
  }
  {
    messages::GoMessage goMessage;
    std::string str("<go>50</go>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK_THROW(
      goMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)),
      std::exception);
  }
  {
    messages::GoMessage goMessage;
    std::string str("<go><xDirection>50</xDirection></go>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK_THROW(
      goMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)),
      std::exception);
  }
  {
    messages::GoMessage goMessage;
    std::string str("<go><xDirection>50</xDirection><yDirection>50</"
                    "yDirection><zDirection>50</zDirection></go>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK_THROW(
      goMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)),
      std::exception);
  }
  {
    messages::GoMessage goMessage;
    std::string str(
      "<go><xDirection>50</xDirection><yDirection>50</"
      "yDirection><zDirection>50</zDirection><speed>50</speed></go>");
    doc->parse<0>((char*)str.c_str());
    BOOST_CHECK(goMessage.readXml(dynamic_cast<rapidxml::xml_node<>*>(doc)));
  }
  delete doc;
}

BOOST_AUTO_TEST_CASE(MESSAGE_FACTORY_GO_TEST)
{
  std::string testMessage = "go 51 52 53 54";

  rapidjson::Document doc;
  rapidjson::Value json;
  createRapidjson(
    doc, json, R"({"type":"go","data":{"go":{"xDirection": 51, 
                "yDirection": 52, 
                "zDirection": 53,
                "speed": 54}}})");
  BOOST_CHECK_EQUAL(
    boost::get<messages::GoMessage>(messages::getMessage(json)).toString(),
    testMessage);

  auto xmlDoc = new rapidxml::xml_document<>;
  std::string str(
    "<type>go</type><data><go><xDirection>51</xDirection><yDirection>52</"
    "yDirection><zDirection>53</zDirection><speed>54</speed></go></data>");
  xmlDoc->parse<0>((char*)str.c_str());
  BOOST_CHECK_EQUAL(
    boost::get<messages::GoMessage>(messages::getMessage(xmlDoc)).toString(),
    testMessage);
  delete xmlDoc;

  BOOST_CHECK_EQUAL(
    boost::get<messages::GoMessage>(messages::getMessage(testMessage))
    .toString(),
    testMessage);
}
