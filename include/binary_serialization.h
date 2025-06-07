#ifndef BINARY_SERIALIZATION_H
#define BINARY_SERIALIZATION_H

#include <fstream>
#include <string>
#include <type_traits>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <utility>
#include <stdexcept>

namespace BinarySerialization {

// 优先匹配有成员 serialize/deserialize 的类型（SFINAE）
template<typename T>
auto serialize(const T& obj, std::ostream& os) -> decltype(obj.serialize(os), void()) {
    obj.serialize(os);
}

template<typename T>
auto deserialize(T& obj, std::istream& is) -> decltype(obj.deserialize(is), void()) {
    obj.deserialize(is);
}

// 算术类型
template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
serialize(const T& obj, std::ostream& os) {
    os.write(reinterpret_cast<const char*>(&obj), sizeof(T));
}

template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
deserialize(T& obj, std::istream& is) {
    is.read(reinterpret_cast<char*>(&obj), sizeof(T));
}

// std::string
inline void serialize(const std::string& str, std::ostream& os) {
    size_t len = str.size();
    os.write(reinterpret_cast<const char*>(&len), sizeof(len));
    os.write(str.data(), len);
}

inline void deserialize(std::string& str, std::istream& is) {
    size_t len;
    is.read(reinterpret_cast<char*>(&len), sizeof(len));
    str.resize(len);
    is.read(&str[0], len);
}

// std::pair
template<typename T1, typename T2>
void serialize(const std::pair<T1, T2>& p, std::ostream& os) {
    serialize(p.first, os);
    serialize(p.second, os);
}

template<typename T1, typename T2>
void deserialize(std::pair<T1, T2>& p, std::istream& is) {
    deserialize(p.first, is);
    deserialize(p.second, is);
}

// std::vector
template<typename T>
void serialize(const std::vector<T>& v, std::ostream& os) {
    size_t size = v.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    for (const auto& item : v) serialize(item, os);
}

template<typename T>
void deserialize(std::vector<T>& v, std::istream& is) {
    size_t size;
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    v.resize(size);
    for (auto& item : v) deserialize(item, is);
}

// std::list
template<typename T>
void serialize(const std::list<T>& l, std::ostream& os) {
    size_t size = l.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    for (const auto& item : l) serialize(item, os);
}

template<typename T>
void deserialize(std::list<T>& l, std::istream& is) {
    size_t size;
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    l.clear();
    for (size_t i = 0; i < size; ++i) {
        T item;
        deserialize(item, is);
        l.push_back(item);
    }
}

// std::set
template<typename T>
void serialize(const std::set<T>& s, std::ostream& os) {
    size_t size = s.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    for (const auto& item : s) serialize(item, os);
}

template<typename T>
void deserialize(std::set<T>& s, std::istream& is) {
    size_t size;
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    s.clear();
    for (size_t i = 0; i < size; ++i) {
        T item;
        deserialize(item, is);
        s.insert(item);
    }
}

// std::map
template<typename K, typename V>
void serialize(const std::map<K, V>& m, std::ostream& os) {
    size_t size = m.size();
    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    for (const auto& kv : m) {
        serialize(kv.first, os);
        serialize(kv.second, os);
    }
}

template<typename K, typename V>
void deserialize(std::map<K, V>& m, std::istream& is) {
    size_t size;
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    m.clear();
    for (size_t i = 0; i < size; ++i) {
        K key;
        V value;
        deserialize(key, is);
        deserialize(value, is);
        m.emplace(std::move(key), std::move(value));
    }
}

// 文件接口
template<typename T>
void serialize(const T& obj, const std::string& filename) {
    std::ofstream ofs(filename, std::ios::binary);
    if (!ofs) throw std::runtime_error("Failed to open file for writing");
    serialize(obj, ofs);
}

template<typename T>
void deserialize(T& obj, const std::string& filename) {
    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs) throw std::runtime_error("Failed to open file for reading");
    deserialize(obj, ifs);
}

// 用户自定义类型宏
#define BINARY_SERIALIZABLE(...) \
    template<typename Stream> \
    void serialize(Stream& os) const { \
        BinarySerialization::serialize_members(os, __VA_ARGS__); \
    } \
    template<typename Stream> \
    void deserialize(Stream& is) { \
        BinarySerialization::deserialize_members(is, __VA_ARGS__); \
    }

inline void serialize_members(std::ostream&) {}
inline void deserialize_members(std::istream&) {}

template<typename Stream, typename First, typename... Rest>
void serialize_members(Stream& os, const First& first, const Rest&... rest) {
    BinarySerialization::serialize(first, os);
    serialize_members(os, rest...);
}

template<typename Stream, typename First, typename... Rest>
void deserialize_members(Stream& is, First& first, Rest&... rest) {
    BinarySerialization::deserialize(first, is);
    deserialize_members(is, rest...);
}

} // namespace BinarySerialization



#endif // BINARY_SERIALIZATION_H