#include <iostream>
#include <map>
#include <vector>
#include <string>

struct OriginFormType {
    std::string m_Path;

    // 기본 생성자
    OriginFormType() {}

    // 기본 소멸자
    ~OriginFormType() {}
};

struct AbsoluteFormType {
    unsigned short m_Port;
    std::string m_Host;
    std::string m_Path;

    // 기본 생성자
    AbsoluteFormType() : m_Port(0) {}

    // 기본 소멸자
    ~AbsoluteFormType() {}
};

struct ResponseRecipe {
    typedef std::map<std::string, std::vector<std::string> > QueryMap_t;

    bool isChunk;
    unsigned char m_Method;
    unsigned char m_Status;
    unsigned char m_Version;
    unsigned short m_Headers;
    unsigned int m_ContentLength;

    std::string m_Host;
    std::string m_ContentType;

    std::string m_RequestTarget;

    std::vector<unsigned char> m_BodyMessage();
    std::map<std::string, std::string> m_Cookie;
    QueryMap_t m_Query;

	union {
        OriginFormType m_OriginData;
        AbsoluteFormType m_AbsoluteForm;
    };

    ResponseRecipe(const std::string& test) {
        m_OriginData.m_Path = test;
    }
    // 소멸자
    ~ResponseRecipe() {}
};

int main() {
    ResponseRecipe test("test");
}