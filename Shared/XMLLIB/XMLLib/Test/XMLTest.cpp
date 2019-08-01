#define BOOST_TEST_MODULE XMLTest

#include <iostream>

#include <boost/test/unit_test.hpp>

#include <rapidxml/rapidxml.hpp>

#include "XMLLib/Deserialize.hpp"
#include "XMLLib/Serialize.hpp"

bool compareDoubles(const double& a, const double& b)
{
  return std::abs(a - b) < .25;
}
std::string first("first"), second("second"), third("third"), fourth("fourth"),
  fifth("fifth"), sixth("sixth"), seventh("seventh"), eighth("eigtht"),
  happy("happy"), birthday("birthday");

BOOST_AUTO_TEST_CASE(XML_OBJECT)
{
  auto pDoc = new rapidxml::xml_document<>;
  auto pNode = xml::addDataToNode(pDoc, "myNode");
  xml::addDataToNode(pNode, first, first);
  xml::addDataToNode(pNode, second, 0.0);
  xml::addDataToNode(pNode, third, true);
  xml::addDataToNode(pNode, fourth, false);
  xml::addDataToNode(pNode, fifth, 1.1);
  xml::addDataToNode(pNode, sixth, happy);
  xml::addDataToNode(pNode, sixth, birthday);
  xml::addDataToNode(pNode, seventh, true);
  xml::addDataToNode(pNode, seventh, true);
  xml::addDataToNode(pNode, eighth, 2.1);
  xml::addDataToNode(pNode, eighth, 2.2);

  auto pRespNode = xml::getObject(pDoc, "myNode");

  BOOST_CHECK(xml::getString(pRespNode, first) == first);
  BOOST_CHECK(compareDoubles(xml::getNumber(pRespNode, second), 0.0));
  BOOST_CHECK(xml::getBool(pRespNode, third) == true);
  BOOST_CHECK(xml::getBool(pRespNode, fourth) == false);
  BOOST_CHECK(compareDoubles(xml::getNumber(pRespNode, fifth), 1.1));
  BOOST_CHECK(xml::getVectorString(pRespNode, sixth)[0] == happy);
  BOOST_CHECK(xml::getVectorString(pRespNode, sixth)[1] == birthday);
  BOOST_CHECK(xml::getVectorBool(pRespNode, seventh)[0] == true);
  BOOST_CHECK(xml::getVectorBool(pRespNode, seventh)[1] == true);
  BOOST_CHECK(compareDoubles(xml::getVectorDouble(pRespNode, eighth)[0], 2.1));
  BOOST_CHECK(compareDoubles(xml::getVectorDouble(pRespNode, eighth)[1], 2.2));
  delete pDoc;
}

BOOST_AUTO_TEST_CASE(XML_DOCUMENT)
{
  auto pDoc = new rapidxml::xml_document<>;
  xml::addDataToNode(pDoc, first, first);
  xml::addDataToNode(pDoc, second, 0.0);
  xml::addDataToNode(pDoc, third, true);
  xml::addDataToNode(pDoc, fourth, false);
  xml::addDataToNode(pDoc, fifth, 1.1);
  xml::addDataToNode(pDoc, sixth, happy);
  xml::addDataToNode(pDoc, sixth, birthday);
  xml::addDataToNode(pDoc, seventh, true);
  xml::addDataToNode(pDoc, seventh, true);
  xml::addDataToNode(pDoc, eighth, 2.1);
  xml::addDataToNode(pDoc, eighth, 2.2);

  auto response = xml::xmlToString(pDoc);

  BOOST_CHECK(xml::getString(pDoc, first) == first);
  BOOST_CHECK(compareDoubles(xml::getNumber(pDoc, second), 0.0));
  BOOST_CHECK(xml::getBool(pDoc, third) == true);
  BOOST_CHECK(xml::getBool(pDoc, fourth) == false);
  BOOST_CHECK(compareDoubles(xml::getNumber(pDoc, fifth), 1.1));
  BOOST_CHECK(xml::getVectorString(pDoc, sixth)[0] == happy);
  BOOST_CHECK(xml::getVectorString(pDoc, sixth)[1] == birthday);
  BOOST_CHECK(xml::getVectorBool(pDoc, seventh)[0] == true);
  BOOST_CHECK(xml::getVectorBool(pDoc, seventh)[1] == true);
  BOOST_CHECK(compareDoubles(xml::getVectorDouble(pDoc, eighth)[0], 2.1));
  BOOST_CHECK(compareDoubles(xml::getVectorDouble(pDoc, eighth)[1], 2.2));
  delete pDoc;
}
