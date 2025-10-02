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
}
void JsonString::init(const char* s, uint64_t size) {
    str = s;
    pos = 0;
    this.size = size;
}
int JsonString::getCurChar(char& c) {
    if(pos >= size) {
        return -1;
    }
    c = str[pos];
    return 0;
}
void JsonString::inc() {
    pos++;
}
}
