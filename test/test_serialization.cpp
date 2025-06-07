#include <iostream>
#include <cassert>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <string>
#include "binary_serialization.h"
#include "xml_serialization.h"

struct UserDefinedType {
    int idx;
    std::string name;
    std::vector<double> data;
    BINARY_SERIALIZABLE(idx, name, data)
    XML_SERIALIZABLE(idx, name, data)
};

void test_binary_serialization() {
    int n0 = 256, n1;
    BinarySerialization::serialize(n0, "n.data");
    BinarySerialization::deserialize(n1, "n.data");
    assert(n0 == n1);

    std::string s0 = "hello", s1;
    BinarySerialization::serialize(s0, "s.data");
    BinarySerialization::deserialize(s1, "s.data");
    assert(s0 == s1);

    std::vector<int> v0{1,2,3}, v1;
    BinarySerialization::serialize(v0, "v.data");
    BinarySerialization::deserialize(v1, "v.data");
    assert(v0 == v1);

    std::list<double> l0{1.1,2.2,3.3}, l1;
    BinarySerialization::serialize(l0, "l.data");
    BinarySerialization::deserialize(l1, "l.data");
    assert(std::vector<double>(l0.begin(), l0.end()) == std::vector<double>(l1.begin(), l1.end()));

    std::set<std::string> set0{"a","b","c"}, set1;
    BinarySerialization::serialize(set0, "set.data");
    BinarySerialization::deserialize(set1, "set.data");
    assert(set0 == set1);

    std::map<int, std::string> map0{{1,"a"},{2,"b"}}, map1;
    BinarySerialization::serialize(map0, "map.data");
    BinarySerialization::deserialize(map1, "map.data");
    assert(map0 == map1);

    std::pair<int, double> pair0 = {2, 3.1}, pair1;
    BinarySerialization::serialize(pair0, "pair.data");
    BinarySerialization::deserialize(pair1, "pair.data");
    assert(pair0 == pair1);

    std::cout << "Binary serialization test passed!" << std::endl;
}

void test_xml_serialization() {
    int n0 = 256, n1;
    xml_serialization::serialize_xml(n0, "n", "n.xml");
    xml_serialization::deserialize_xml(n1, "n", "n.xml");
    assert(n0 == n1);

    std::string s0 = "hello", s1;
    xml_serialization::serialize_xml(s0, "s", "s.xml");
    xml_serialization::deserialize_xml(s1, "s", "s.xml");
    assert(s0 == s1);

    std::vector<int> v0{1,2,3}, v1;
    xml_serialization::serialize_xml(v0, "v", "v.xml");
    xml_serialization::deserialize_xml(v1, "v", "v.xml");
    assert(v0 == v1);

    std::list<double> l0{1.1,2.2,3.3}, l1;
    xml_serialization::serialize_xml(l0, "l", "l.xml");
    xml_serialization::deserialize_xml(l1, "l", "l.xml");
    assert(std::vector<double>(l0.begin(), l0.end()) == std::vector<double>(l1.begin(), l1.end()));

    std::set<std::string> set0{"a","b","c"}, set1;
    xml_serialization::serialize_xml(set0, "set", "set.xml");
    xml_serialization::deserialize_xml(set1, "set", "set.xml");
    assert(set0 == set1);

    // std::map<int, std::string> map0{{1,"a"},{2,"b"}}, map1;
    // xml_serialization::serialize_xml(map0, "map", "map.xml");
    // xml_serialization::deserialize_xml(map1, "map", "map.xml");
    // assert(map0 == map1);

    std::pair<int, double> pair0 = {2, 3.1}, pair1;
    xml_serialization::serialize_xml(pair0, "std_pair", "pair.xml");
    xml_serialization::deserialize_xml(pair1, "std_pair", "pair.xml");
    assert(pair0 == pair1);

    std::cout << "XML serialization test passed!" << std::endl;
}

void test_user_defined_type() {
    UserDefinedType u0{42, "hello", {1.1, 2.2, 3.3}}, u1, u2;
    BinarySerialization::serialize(u0, "user.data");
    BinarySerialization::deserialize(u1, "user.data");
    assert(u0.idx == u1.idx && u0.name == u1.name && u0.data == u1.data);



    xml_serialization::serialize_xml(u0, "user", "user.xml");
    xml_serialization::deserialize_xml(u2, "user", "user.xml");
    assert(u0.idx == u2.idx && u0.name == u2.name && u0.data == u2.data);

    std::cout << "UserDefinedType serialization test passed!" << std::endl;
}

void run_tests() {
    test_binary_serialization();
    test_xml_serialization();
    test_user_defined_type();
}

int main() {
    run_tests();
    std::cout << "All tests completed successfully!" << std::endl;
    return 0;
}