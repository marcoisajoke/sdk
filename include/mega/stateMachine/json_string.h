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
    bool checkCurChar();
    void inc();
    bool bypassEmpty();
    bool decodeString(std::string& s);
    handle getHandler(int size = 6);
    bool bypassChar(char c);
    void resetPos(uint64_t p);
    void bypassValue();
    void bypassString();
    void bypassObjectInside();
    bool decodeInt64(int64_t& ret);
    int findNextStringValue(int& p_state, int state_value, std::string& s);
    
    uint64_t pos;
    char cur_c = 0;
    char pre_c = 0;
private:
    char* str;
    uint64_t size;
};
}
#endif
