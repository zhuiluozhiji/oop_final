#include <iostream>
#include <cassert>
#include "binary_serialization.h"
#include "xml_serialization.h"


struct UserDefinedType {
    int idx;
    std::string name;
    std::vector<double> data;
    BINARY_SERIALIZABLE(idx, name, data)
};

void test_binary_serialization() {
    int n0 = 256, n1;
    BinarySerialization::serialize(n0, "n.data");
    BinarySerialization::deserialize(n1, "n.data");
    assert(n0 == n1);
    std::cout << "Binary serialization test passed!" << std::endl;
}

void test_xml_serialization() {
    std::pair<int, double> pair0 = {2, 3.1}, pair1;
    xml_serialization::serialize_xml(pair0, "std_pair", "pair.xml");
    xml_serialization::deserialize_xml(pair1, "std_pair", "pair.xml");
    assert(pair0 == pair1);
    std::cout << "XML serialization test passed!" << std::endl;
}


void test_user_defined_type() {
    UserDefinedType u0{42, "hello", {1.1, 2.2, 3.3}}, u1;
    BinarySerialization::serialize(u0, "user.data");
    BinarySerialization::deserialize(u1, "user.data");
    assert(u0.idx == u1.idx && u0.name == u1.name && u0.data == u1.data);
    std::cout << "UserDefinedType binary serialization test passed!" << std::endl;
}

void run_tests() {
    test_binary_serialization();
    test_xml_serialization();
    test_user_defined_type(); // 新增自定义类型测试
}

int main() {
    run_tests();
    std::cout << "All tests completed successfully!" << std::endl;
    return 0;
}