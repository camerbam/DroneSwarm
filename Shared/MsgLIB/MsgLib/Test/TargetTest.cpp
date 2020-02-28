#include <boost/test/unit_test.hpp>

#include <rapidjson/document.h>
#include <rapidxml/rapidxml.hpp>

#pragma warning(push)
#pragma warning(disable : 4267)
#include "ProtoLib/Target.pb.h"
#pragma warning(pop)

#include "MsgLib/TargetMsg.hpp"

BOOST_AUTO_TEST_CASE(TargetTest)
{
  {
    msg::TargetMsg target(3, 2);
    BOOST_CHECK_EQUAL(target.x(), 3);
    BOOST_CHECK_EQUAL(target.y(), 2);
    rapidjson::Document doc(rapidjson::kObjectType);

    target.toJson(doc, doc);
    target.parseFromJson(doc);

    BOOST_CHECK_EQUAL(target.x(), 3);
    BOOST_CHECK_EQUAL(target.y(), 2);
  }
  {
    msg::TargetMsg target(3, 2);
    BOOST_CHECK_EQUAL(target.x(), 3);
    BOOST_CHECK_EQUAL(target.y(), 2);

    proto::Target* tTarget = new proto::Target();
    target.toProto(tTarget);
    target.parseFromProto(*tTarget);

    BOOST_CHECK_EQUAL(target.x(), 3);
    BOOST_CHECK_EQUAL(target.y(), 2);
    delete tTarget;
  }
  {
    msg::TargetMsg target(3, 2);
    BOOST_CHECK_EQUAL(target.x(), 3);
    BOOST_CHECK_EQUAL(target.y(), 2);
    auto pDoc = new rapidxml::xml_document<>;

    target.toXML(pDoc);
    target.parseFromXml(pDoc);

    BOOST_CHECK_EQUAL(target.x(), 3);
    BOOST_CHECK_EQUAL(target.y(), 2);
    delete pDoc;
  }
}
