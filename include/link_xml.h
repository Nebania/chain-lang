#ifndef LINK_XML_H //*chain-lang
#define LINK_XML_H

#include <tinyxml2.h>
#include <iostream>

namespace LinkXML
{
    //read/write functions
    tinyxml2::XMLDocument* readFromFile(const std::string& path);
    void readFromString(std::string& string);
    tinyxml2::XMLElement* getRoot();
    tinyxml2::XMLElement* child(tinyxml2::XMLElement* element,std::string name);
    std::string getElementText(tinyxml2::XMLElement* element);
    tinyxml2::XMLElement* findPath(const std::string& path);
} // namespace LinkXML


#endif