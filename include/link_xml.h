#ifndef LINK_XML_H //*chain-lang
#define LINK_XML_H

#include <tinyxml2.h>
#include <iostream>

namespace LinkXML
{
    tinyxml2::XMLDocument* readFromFile(const std::string& path);
    tinyxml2::XMLDocument* readFromString(std::string& string);
    void destroyXML();
    tinyxml2::XMLElement* getRoot();
    tinyxml2::XMLElement* child(tinyxml2::XMLElement* element,std::string name);
    std::string getElementText(tinyxml2::XMLElement* element);
    tinyxml2::XMLElement* findPath(const std::string& path);
    bool exists(const std::string& path, const std::string& value);
} // namespace LinkXML


#endif