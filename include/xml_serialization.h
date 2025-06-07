#ifndef XML_SERIALIZATION_H
#define XML_SERIALIZATION_H

#include <tinyxml2.h>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <type_traits>
#include <utility>

namespace xml_serialization {

// ========== 用户自定义类型优先匹配 ==========
template<typename T>
auto serialize_xml(const T& obj, const std::string& name, const std::string& filename)
    -> decltype(obj.serialize_xml(name, filename), void()) {
    obj.serialize_xml(name, filename);
}

template<typename T>
auto deserialize_xml(T& obj, const std::string& name, const std::string& filename)
    -> decltype(obj.deserialize_xml(name, filename), void()) {
    obj.deserialize_xml(name, filename);
}

// ========== 算术类型 ==========
template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
serialize_xml(const T& obj, const std::string& name, const std::string& filename) {
    tinyxml2::XMLDocument doc;
    auto* root = doc.NewElement("serialization");
    doc.InsertFirstChild(root);
    auto* element = doc.NewElement(name.c_str());
    element->SetAttribute("val", obj);
    root->InsertEndChild(element);
    doc.SaveFile(filename.c_str());
}

template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
deserialize_xml(T& obj, const std::string& name, const std::string& filename) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(filename.c_str());
    auto* root = doc.FirstChildElement("serialization");
    if (!root) return;
    auto* element = root->FirstChildElement(name.c_str());
    if (!element) return;
    element->QueryAttribute("val", &obj);
}

// ========== 算术类型 嵌套节点 ==========
template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
serialize_xml(const T& obj, const char* /*name*/, tinyxml2::XMLElement* parent) {
    parent->SetAttribute("val", obj);
}

template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
deserialize_xml(T& obj, const char* /*name*/, tinyxml2::XMLElement* parent) {
    if (!parent) return;
    parent->QueryAttribute("val", &obj);
}

// ========== std::string ==========
inline void serialize_xml(const std::string& obj, const std::string& name, const std::string& filename) {
    tinyxml2::XMLDocument doc;
    auto* root = doc.NewElement("serialization");
    doc.InsertFirstChild(root);
    auto* element = doc.NewElement(name.c_str());
    element->SetAttribute("val", obj.c_str());
    root->InsertEndChild(element);
    doc.SaveFile(filename.c_str());
}

inline void deserialize_xml(std::string& obj, const std::string& name, const std::string& filename) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(filename.c_str());
    auto* root = doc.FirstChildElement("serialization");
    if (!root) { obj.clear(); return; }
    auto* element = root->FirstChildElement(name.c_str());
    if (!element) { obj.clear(); return; }
    const char* value = element->Attribute("val");
    obj = value ? value : "";
}

// ========== std::string 嵌套节点 ==========
inline void serialize_xml(const std::string& obj, const char* /*name*/, tinyxml2::XMLElement* parent) {
    parent->SetAttribute("val", obj.c_str());
}

inline void deserialize_xml(std::string& obj, const char* /*name*/, tinyxml2::XMLElement* parent) {
    if (!parent) { obj.clear(); return; }
    const char* v = parent->Attribute("val");
    obj = v ? v : "";
}

// ========== std::pair ==========
template<typename T1, typename T2>
void serialize_xml(const std::pair<T1, T2>& obj, const std::string& name, const std::string& filename) {
    tinyxml2::XMLDocument doc;
    auto* root = doc.NewElement("serialization");
    doc.InsertFirstChild(root);
    auto* element = doc.NewElement(name.c_str());
    root->InsertEndChild(element);

    auto* first = doc.NewElement("first");
    serialize_xml(obj.first, "first", first);
    element->InsertEndChild(first);

    auto* second = doc.NewElement("second");
    serialize_xml(obj.second, "second", second);
    element->InsertEndChild(second);

    doc.SaveFile(filename.c_str());
}

template<typename T1, typename T2>
void deserialize_xml(std::pair<T1, T2>& obj, const std::string& name, const std::string& filename) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(filename.c_str());
    auto* root = doc.FirstChildElement("serialization");
    if (!root) return;
    auto* element = root->FirstChildElement(name.c_str());
    if (!element) return;

    auto* first = element->FirstChildElement("first");
    auto* second = element->FirstChildElement("second");
    deserialize_xml(obj.first, "first", first);
    deserialize_xml(obj.second, "second", second);
}

// ========== std::vector ==========
template<typename T>
void serialize_xml(const std::vector<T>& v, const std::string& name, const std::string& filename) {
    tinyxml2::XMLDocument doc;
    auto* root = doc.NewElement("serialization");
    doc.InsertFirstChild(root);
    auto* element = doc.NewElement(name.c_str());
    root->InsertEndChild(element);

    for (const auto& item : v) {
        auto* child = doc.NewElement("item");
        serialize_xml(item, "item", child);
        element->InsertEndChild(child);
    }
    doc.SaveFile(filename.c_str());
}

template<typename T>
void deserialize_xml(std::vector<T>& v, const std::string& name, const std::string& filename) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(filename.c_str());
    auto* root = doc.FirstChildElement("serialization");
    if (!root) return;
    auto* element = root->FirstChildElement(name.c_str());
    if (!element) return;
    v.clear();
    for (auto* child = element->FirstChildElement("item"); child; child = child->NextSiblingElement("item")) {
        T item;
        deserialize_xml(item, "item", child);
        v.push_back(item);
    }
}

// ========== std::list ==========
template<typename T>
void serialize_xml(const std::list<T>& l, const std::string& name, const std::string& filename) {
    std::vector<T> v(l.begin(), l.end());
    serialize_xml(v, name, filename);
}

template<typename T>
void deserialize_xml(std::list<T>& l, const std::string& name, const std::string& filename) {
    std::vector<T> v;
    deserialize_xml(v, name, filename);
    l.assign(v.begin(), v.end());
}

// ========== std::set ==========
template<typename T>
void serialize_xml(const std::set<T>& s, const std::string& name, const std::string& filename) {
    std::vector<T> v(s.begin(), s.end());
    serialize_xml(v, name, filename);
}

template<typename T>
void deserialize_xml(std::set<T>& s, const std::string& name, const std::string& filename) {
    std::vector<T> v;
    deserialize_xml(v, name, filename);
    s.clear();
    s.insert(v.begin(), v.end());
}

// ========== std::map ==========
template<typename K, typename V>
void serialize_xml(const std::map<K, V>& m, const std::string& name, const std::string& filename) {
    tinyxml2::XMLDocument doc;
    auto* root = doc.NewElement("serialization");
    doc.InsertFirstChild(root);
    auto* element = doc.NewElement(name.c_str());
    root->InsertEndChild(element);

    for (const auto& kv : m) {
        auto* child = doc.NewElement("item");
        serialize_xml(kv.first, "key", child);
        serialize_xml(kv.second, "value", child);
        element->InsertEndChild(child);
    }
    doc.SaveFile(filename.c_str());
}

template<typename K, typename V>
void deserialize_xml(std::map<K, V>& m, const std::string& name, const std::string& filename) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(filename.c_str());
    auto* root = doc.FirstChildElement("serialization");
    if (!root) return;
    auto* element = root->FirstChildElement(name.c_str());
    if (!element) return;
    m.clear();
    for (auto* child = element->FirstChildElement("item"); child; child = child->NextSiblingElement("item")) {
        K key;
        V value;
        auto* keyElem = child->FirstChildElement("key");
        auto* valueElem = child->FirstChildElement("value");
        deserialize_xml(key, "key", keyElem);
        deserialize_xml(value, "value", valueElem);
        m.emplace(std::move(key), std::move(value));
    }
}

// ========== 递归成员展开声明（提前声明） ==========
template<typename T>
void serialize_member(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* parent, const T& value);

template<typename T>
void deserialize_member(tinyxml2::XMLElement* parent, T& value);

inline void serialize_members(tinyxml2::XMLDocument&, tinyxml2::XMLElement*) {}

template<typename First, typename... Rest>
void serialize_members(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* parent, const First& first, const Rest&... rest) {
    serialize_member(doc, parent, first);
    serialize_members(doc, parent, rest...);
}

inline void deserialize_members(tinyxml2::XMLElement*) {}

template<typename First, typename... Rest>
void deserialize_members(tinyxml2::XMLElement* parent, First& first, Rest&... rest) {
    deserialize_member(parent, first);
    deserialize_members(parent, rest...);
}

template<typename T>
void serialize_member(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* parent, const T& value) {
    auto* elem = doc.NewElement("field");
    if constexpr (std::is_arithmetic<T>::value) {
        elem->SetAttribute("val", value);
    } else if constexpr (std::is_same<T, std::string>::value) {
        elem->SetAttribute("val", value.c_str());
    } else {
        serialize_members(doc, elem, value);
    }
    parent->InsertEndChild(elem);
}

template<typename T>
void deserialize_member(tinyxml2::XMLElement* parent, T& value) {
    if (!parent) return;
    auto* elem = parent->FirstChildElement("field");
    if (!elem) return;
    if constexpr (std::is_arithmetic<T>::value) {
        elem->QueryAttribute("val", &value);
    } else if constexpr (std::is_same<T, std::string>::value) {
        const char* v = elem->Attribute("val");
        value = v ? v : "";
    } else {
        deserialize_members(elem, value);
    }
}

// ========== 用户自定义类型宏 ==========
#define XML_SERIALIZABLE(...) \
    void serialize_xml(const std::string& name, const std::string& filename) const { \
        tinyxml2::XMLDocument doc; \
        auto* root = doc.NewElement("serialization"); \
        doc.InsertFirstChild(root); \
        auto* element = doc.NewElement(name.c_str()); \
        root->InsertEndChild(element); \
        xml_serialization::serialize_members(doc, element, __VA_ARGS__); \
        doc.SaveFile(filename.c_str()); \
    } \
    void deserialize_xml(const std::string& name, const std::string& filename) { \
        tinyxml2::XMLDocument doc; \
        doc.LoadFile(filename.c_str()); \
        auto* root = doc.FirstChildElement("serialization"); \
        if (!root) return; \
        auto* element = root->FirstChildElement(name.c_str()); \
        if (!element) return; \
        xml_serialization::deserialize_members(element, __VA_ARGS__); \
    }

} // namespace xml_serialization

#endif // XML_SERIALIZATION_H