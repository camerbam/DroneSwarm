#include "FlightPathMsg.hpp"

#include <rapidjson/document.h>
#include <rapidxml/rapidxml.hpp>

#include "JSONLib/Deserialize.hpp"
#include "JSONLib/Serialize.hpp"
#include "XMLLib/Deserialize.hpp"
#include "XMLLib/Serialize.hpp"

#pragma warning(push)
#pragma warning(disable : 4267)
#include "ProtoLib/FlightPathMsg.pb.h"
#pragma warning(pop)

#include "Point.hpp"

namespace
{
  const std::string N_POINTS("points");
} // namespace

msg::FlightPathMsg::FlightPathMsg(const std::vector<Point>& points)
  : m_points(points)
{
}

msg::FlightPathMsg::FlightPathMsg() : m_points()
{
}

bool msg::FlightPathMsg::parseFromJson(const std::string& point)
{
  rapidjson::Document json(rapidjson::kObjectType);
  json.Parse(point.c_str());
  auto points = json::getObjectOrArray(json, N_POINTS);
  if (!points) return false;
  auto& pointsArray = points.get();
  if (!pointsArray.IsArray()) return false;
  msg::Point tPoint;
  for (auto& point : pointsArray.GetArray())
  {
    if (!tPoint.parseFromJson(point)) return false;
    m_points.emplace_back(tPoint);
  }
  return true;
}

bool msg::FlightPathMsg::parseFromProto(const std::string& msg)
{
  proto::FlightPathMsg m;
  m.ParseFromString(msg);
  msg::Point tPoint;
  for (auto&& p : m.points())
  {
    if (!tPoint.parseFromProto(p)) return false;
    m_points.emplace_back(tPoint);
  }
  return true;
}

bool msg::FlightPathMsg::parseFromXml(const std::string& msg)
{
  auto pDoc = new rapidxml::xml_document<>;
  char* cstr = new char[msg.size() + 1];
  strcpy(cstr, msg.c_str());
  pDoc->parse<0>(cstr);
  auto points = xml::getObject(pDoc, N_POINTS);
  msg::Point tPoint;

  for (; points != nullptr; points = points->next_sibling(N_POINTS.c_str()))
  {
    if (!tPoint.parseFromXml(points)) return false;
    m_points.emplace_back(tPoint);
  }

  delete[] cstr;
  delete pDoc;
  return true;
}

std::string msg::FlightPathMsg::toJsonString() const
{
  rapidjson::Document doc(rapidjson::kObjectType);
  rapidjson::Value arr(rapidjson::kArrayType);

  msg::Point tPoint;
  for (auto&& p : m_points)
  {
    rapidjson::Value obj(rapidjson::kObjectType);
    p.toJson(doc, obj);
    json::addObjectToArray(doc, arr, obj);
  }

  json::addArrayToDoc(doc, N_POINTS, arr);
  return json::jsonToString(doc);
}

std::string msg::FlightPathMsg::toProtoString() const
{
  proto::FlightPathMsg msg;
  for (auto&& p : m_points)
  {
    auto point = msg.mutable_points()->Add();
    p.toProto(point);
  }
  return msg.SerializeAsString();
}

std::string msg::FlightPathMsg::toXMLString() const
{
  auto pDoc = new rapidxml::xml_document<>;

  for (auto&& p : m_points)
  {
    auto pointsNode = xml::addDataToNode(pDoc, N_POINTS);
    p.toXML(pointsNode);
  }
  auto toReturn = xml::xmlToString(pDoc);
  delete pDoc;
  return toReturn;
}
