//
//  json_string.h
//  sdk
//
//  Created by marco on 2025/10/2.
//


#ifndef MEGA_JSON_STRING_H
#define MEGA_JSON_STRING_H 1


namespace mega {

class JsonString {
public:
    JsonString();
    void init(const char* s, uint64_t size);
    int getCurChar(char& c);
    void inc();
private:
    char* str;
    uint64_t pos;
    uint64_t size;
};
}
#endif
