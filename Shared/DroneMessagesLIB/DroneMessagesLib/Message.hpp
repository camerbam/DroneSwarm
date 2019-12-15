#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include <string>

#include <rapidjson/document.h>
#include <rapidxml/rapidxml.hpp>

#include <boost/variant.hpp>

namespace messages
{
  class Message
  {
  public:
    Message(const std::string& name);
    bool fromString(std::string& toParse);

    virtual std::string toString() const;
    bool readJson(rapidjson::Value& jsonToParse);
    virtual bool readXml(rapidxml::xml_node<>* xmlToParse);

  protected:
    virtual bool readJsonImpl(rapidjson::Value& jsonToParse) = 0;
    virtual bool fromStringImpl(std::string& toParse) = 0;
    virtual bool readXmlImpl(rapidxml::xml_node<>* xmlToParse) = 0;

    std::string m_name;

    std::string getNextWord(std::string& toParse);

    std::string removeUnimportantZeros(std::string toTrim) const;

    virtual bool verifyMessage(std::string& toParse,
                               const std::string& messageName);
  };
} // namespace messages

#endif