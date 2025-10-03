//
//  json_string.cpp
//  sdk
//
//  Created by marco on 2025/10/2.
//

#include "mega/stateMachine/json_string.h"
namespace mega {
JsonString::JsonString() {
    str = nullptr;
    pos = 0;
    size = 0;
    cur_c = 0;
    pre_c = 0;
}
void JsonString::init(const char* s, uint64_t size) {
    str = s;
    pos = 0;
    if(size>0) {
        pre_c = 0;
        cur_c = str[0];
    }
    this.size = size;
}
bool JsonString::checkCurChar() {
    return pos < size;
}
void JsonString::inc() {
    pos++;
    if(pos<size) {
        cur_c = s[pos];
    }
}
bool JsonString::bypassEmpty() {
    if(cur_c == " ") {
        inc();
        return true;
    }
    return false;
}
bool JsonString::decodeString(std::string& s) {
    if(cur_c != '"' || pre_c == '\\') {
        pre_c = cur_c;
        s.append(cur_c);
        inc();
        return true;
    }
    pre_c = 0;
    return false;
}
bool JsonString::bypassChar(char c) {
    if(cur_c == c) {
        inc();
        return true;
    }
    return false;
}
void JsonString::resetPos(uint64_t p) {
    pos = p;
    if(pos < size) {
        cur_c = str[pos];
    }
    if(pos > 0) {
        pre_c = str[pos-1];
    } else {
        pre_c = 0;
    }
}
int JsonString::findNextStringValue(int& p_state, int state_value, std::string& s) {
    if(bypassEmpty()) {
        return 1;
    }
    if(bypassChar(':')) {
        return 1;
    }
    if(cur_c == '"') {
        inc();
        p_state = state_value
        s = "";
        return 0;
    } else {
        return -1;
    }
}
void JsonString::bypassValue() {
    
}
void JsonString::bypassObjectInside() {
    
}
}
