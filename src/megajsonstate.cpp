//
//  megastate.c
//  sdk
//
//  Created by marco on 2025/10/1.
//

#include "mega/megajsonstate.h"


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
    PacketStateMgr::PacketStateMgr() {
        jsongString = nullptr;
        client = nullptr;
    }
    PacketStateMgr& PacketStateMgr::getInstance() {
        static PacketStateMgr ins;
        return ins;
    }
    void PacketStateMgr::init(JsonString* jStr, MegaClient* cli) {
        jsongString = jStr;
        client = cli;
    }
    enum PACKETSTAT {
        PACKETSTAT_BEGIN,
        PACKETSTAT_HANDLE_SEARCH_KEY,
        PACKETSTAT_SAVING_KEY,
        PACKETSTAT_SAVE_W_VALUE_PRE,
        PACKETSTAT_END,
    };
    bool PacketStateMgr::exec() {
        if(jsongString == nullptr) {
            return false;
        }
        char cur_c;
        char pre_c = 0;
        string key;
        string value;
        PACKETSTAT p_state = PACKETSTAT_BEGIN;
        while(jsongString->getCurChar(cur_c) == 0) {
            switch(p_state) {
                case PACKETSTAT_BEGIN: {
                    if(cur_c <= " "){
                        jsongString->inc();
                        continue;
                    }
                    if(cur_c == "{") {
                        p_state = PACKETSTAT_HANDLE_SEARCH_KEY;
                        jsongString->inc();
                        continue;
                    } else {
                        return false;
                    }
                    break;
                }
                case PACKETSTAT_HANDLE_SEARCH_KEY: {
                    if(cur_c <= " "){
                        jsongString->inc();
                        continue;
                    }
                    if(cur_c == "\"") {
                        key = "";
                        p_state = PACKETSTAT_SAVING_KEY;
                        pre_c = cur_c;
                        jsongString->inc();
                        continue;
                    }
                    if(cur_c == "}") {
                        p_state = PACKETSTAT_END;
                        continue;
                    } else {
                        return false;
                    }
                    break;
                }
                case PACKETSTAT_SAVING_KEY: {
                    if(cur_c != "\"" || pre_c == "\\") {
                        key.append(cur_c);
                        jsongString->inc();
                        pre_c = cur_c;
                        continue;
                    } else {
                        if(key == "w") {
                            p_state = PACKETSTAT_SAVE_W_VALUE_PRE;
                            pre_c = 0;
                            jsongString->inc();
                            client->scnotifyurl = "";
                            continue;
                        }
                        if(key == "ir") {
                            
                        }
                        if(key == "sn") {
                            
                        }
                        if(key == "a") {
                            
                        }
                        //bypass
                    }
                    break;
                }
                case PACKETSTAT_SAVE_W_VALUE_PRE: {
                    if(cur_c == " " || cur_c == ":") {
                        jsongString->inc();
                        continue;
                    }
                    if(cur_c == "\"") {
                        
                    } else {
                        return false;
                    }
                }
            }
            
            
        }
        return false;
    }
}
