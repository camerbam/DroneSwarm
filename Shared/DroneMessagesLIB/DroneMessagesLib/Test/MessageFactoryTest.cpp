#include <boost/test/unit_test.hpp>

#include "DroneMessagesLib/MessageFactory.hpp"

BOOST_AUTO_TEST_CASE(MESSAGE_FACTORY_TEST_JSON)
{
  {
    std::string str("[]");
    rapidjson::Document doc;
    doc.Parse(str.c_str());
    rapidjson::Value value = doc.GetArray();
    BOOST_CHECK_THROW(messages::getMessage(value), std::exception);
  }
  {
    std::string str(R"({"type":"commands","data":{"commands":{}}})");
    rapidjson::Document doc;
    doc.Parse(str.c_str());
    rapidjson::Value value = doc.GetObject();
    BOOST_CHECK_THROW(messages::getMessage(value), std::exception);
  }
}

BOOST_AUTO_TEST_CASE(MESSAGE_FACTORY_TEST_JSON2)
{
  {
    std::string str("[]");
    rapidjson::Document doc;
    doc.Parse(str.c_str());
    rapidjson::Value value = doc.GetArray();
    BOOST_CHECK_THROW(messages::getMessage(value), std::exception);
  }
}

BOOST_AUTO_TEST_CASE(MESSAGE_FACTORY_TEST_XML)
{
  {
    std::string str(
      R"(<root><type>command</type><data><command/></data></root>)");
    rapidxml::xml_document<> doc;
    doc.parse<0>((char*)str.c_str());
    rapidxml::xml_node<>* node = doc.first_node();
    BOOST_CHECK_NO_THROW(messages::getMessage(node));
  }
  {
    std::string str(
      R"(<root><type>commands</type><data><command/></data></root>)");
    rapidxml::xml_document<> doc;
    doc.parse<0>((char*)str.c_str());
    rapidxml::xml_node<>* node = doc.first_node();
    BOOST_CHECK_THROW(messages::getMessage(node), std::exception);
  }
}

BOOST_AUTO_TEST_CASE(MESSAGE_FACTORY_TEST_STRING)
{
  std::string str = "ups";
  BOOST_CHECK_THROW(messages::getMessage(str), std::exception);
}
