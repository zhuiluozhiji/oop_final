#ifndef XML_SERIALIZATION_H
#define XML_SERIALIZATION_H

#include <tinyxml2.h>
#include <string>
#include <utility>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <type_traits>

namespace xml_serialization {

    template<typename T>
    void serialize_xml(const T& obj, const std::string& name, const std::string& filename) {
        tinyxml2::XMLDocument doc;
        tinyxml2::XMLElement* root = doc.NewElement("serialization");
        doc.InsertFirstChild(root);

        tinyxml2::XMLElement* element = doc.NewElement(name.c_str());
        root->InsertEndChild(element);

        // Serialize arithmetic types and std::string
        if constexpr (std::is_arithmetic<T>::value) {
            element->SetAttribute("val", obj);
        } else if constexpr (std::is_same<T, std::string>::value) {
            element->SetAttribute("val", obj.c_str());
        } else if constexpr (std::is_same<T, std::pair<int, double>>::value) {
            tinyxml2::XMLElement* first = doc.NewElement("first");
            first->SetAttribute("val", obj.first);
            element->InsertEndChild(first);

            tinyxml2::XMLElement* second = doc.NewElement("second");
            second->SetAttribute("val", obj.second);
            element->InsertEndChild(second);
        } else {
            // Add serialization for other types as needed
        }

        doc.SaveFile(filename.c_str());
    }

    template<typename T>
    void deserialize_xml(T& obj, const std::string& name, const std::string& filename) {
        tinyxml2::XMLDocument doc;
        doc.LoadFile(filename.c_str());

        tinyxml2::XMLElement* root = doc.FirstChildElement("serialization");
        tinyxml2::XMLElement* element = root->FirstChildElement(name.c_str());

        // Deserialize arithmetic types and std::string
        if constexpr (std::is_arithmetic<T>::value) {
            int value;
            element->QueryIntAttribute("val", &value);
            obj = static_cast<T>(value);
        } else if constexpr (std::is_same<T, std::string>::value) {
            const char* value = element->Attribute("val");
            obj = std::string(value);
        } else if constexpr (std::is_same<T, std::pair<int, double>>::value) {
            tinyxml2::XMLElement* first = element->FirstChildElement("first");
            tinyxml2::XMLElement* second = element->FirstChildElement("second");

            int firstValue;
            double secondValue;

            first->QueryIntAttribute("val", &firstValue);
            second->QueryDoubleAttribute("val", &secondValue);

            obj = std::make_pair(firstValue, secondValue);
        } else {
            // Add deserialization for other types as needed
        }
    }

} // namespace xml_serialization

#endif // XML_SERIALIZATION_H