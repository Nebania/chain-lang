#include <link_xml.h>
#include <iostream>

namespace LinkXML{
    tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();

    tinyxml2::XMLDocument* readFromFile(const std::string& path){
        std::cout << "[CXML] path: " << path << std::endl;

        auto result = doc->LoadFile(path.c_str());

        std::cout << "[CXML] LoadFile result: "
                << result << std::endl;

        if (result != tinyxml2::XML_SUCCESS)
        {
            std::cout << "[CXML] Failed!" << std::endl;

            delete doc;
            return nullptr;
        }

        std::cout << "[CXML] Loaded successfully!" << std::endl;

        return doc;
    }

    tinyxml2::XMLDocument* readFromString(std::string& string){

        if (doc->Parse(string.c_str()) != tinyxml2::XML_SUCCESS){
            std::cout << "[CXML] Failed to parse XML string" << std::endl;
            return nullptr;
        }

        std::cout << "[CXML] XML string loaded successfully!" << std::endl;
        return doc;
    }
    void destroyXML(){
        doc->Clear();
    }
    tinyxml2::XMLElement* getRoot(){
        if(!doc)
            return nullptr;
        return doc->RootElement();
    }
    tinyxml2::XMLElement* child(tinyxml2::XMLElement* element,std::string name){
        
        if (!element)
            return nullptr;

        return element->FirstChildElement(name.c_str());

    }
    std::string getElementText(tinyxml2::XMLElement* element){
        if (!element || !element->GetText())
            return nullptr;

        return element->GetText();
    }
    tinyxml2::XMLElement* findPath(const std::string& path){
        tinyxml2::XMLElement* current = doc->RootElement();

        // Split path by '.'
        size_t start = 0;

        while (start < path.length())
        {
            size_t end = path.find('.', start);

            std::string part = path.substr(
                start,
                end == std::string::npos
                    ? std::string::npos
                    : end - start
            );

            if (start == 0)
            {
                if (std::string(current->Name()) != part)
                    return nullptr;
            }
            else
            {
                current = current->FirstChildElement(part.c_str());

                if (!current)
                    return nullptr;
            }

            if (end == std::string::npos)
                break;

            start = end + 1;
        }

        return current;
    }
    bool exists(const std::string& path, const std::string& value){
        tinyxml2::XMLElement* element = findPath(path);

        if (!element)
            return false;

        for (auto* current = element;
            current;
            current = current->NextSiblingElement(element->Name()))
        {
            if (current->GetText() &&
                value == current->GetText())
            {
                return true;
            }
        }

        return false;
    }
}