#include <boost/test/unit_test.hpp>

#include <rapidjson/document.h>
#include <rapidxml/rapidxml.hpp>

#pragma warning(push)
#pragma warning(disable : 4267)
#include "ProtoLib/Point.pb.h"
#pragma warning(pop)

#include "MsgLib/Point.hpp"

BOOST_AUTO_TEST_CASE(PointTest)
{
  {
    msg::Point point(3, 2);
    BOOST_CHECK_EQUAL(point.x(), 3);
    BOOST_CHECK_EQUAL(point.y(), 2);
    rapidjson::Document doc(rapidjson::kObjectType);

    point.toJson(doc, doc);
    point.parseFromJson(doc);

    BOOST_CHECK_EQUAL(point.x(), 3);
    BOOST_CHECK_EQUAL(point.y(), 2);
  }
  {
    msg::Point point(3, 2);
    BOOST_CHECK_EQUAL(point.x(), 3);
    BOOST_CHECK_EQUAL(point.y(), 2);
    
    proto::Point* tPoint = new proto::Point();
    point.toProto(tPoint);
    point.parseFromProto(*tPoint);

    BOOST_CHECK_EQUAL(point.x(), 3);
    BOOST_CHECK_EQUAL(point.y(), 2);
  }
  {
    msg::Point point(3, 2);
    BOOST_CHECK_EQUAL(point.x(), 3);
    BOOST_CHECK_EQUAL(point.y(), 2);
    auto pDoc = new rapidxml::xml_document<>;

    point.toXML(pDoc);
    point.parseFromXml(pDoc);

    BOOST_CHECK_EQUAL(point.x(), 3);
    BOOST_CHECK_EQUAL(point.y(), 2);
    delete pDoc;
  }
}
