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
enum PACKETSTAT {
    PACKETSTAT_BEGIN,
    PACKETSTAT_HANDLE_SEARCH_KEY,
    PACKETSTAT_SAVING_KEY,
    PACKETSTAT_SAVE_W_VALUE_PRE,
    PACKETSTAT_SAVE_W_VALUE,
    PACKETSTAT_COMMA,
    PACKETSTAT_END,
};
static bool begin_func(PacketStateMgr* mgr) {
    if(mgr->cur_c <= ' '){
        mgr->jsongString->inc();
        return true;
    }
    if(mgr->cur_c == '{') {
        mgr->p_state = PACKETSTAT_HANDLE_SEARCH_KEY;
        mgr->jsongString->inc();
        return true;
    } else {
        return false;
    }
}
static bool search_key_func(PacketStateMgr* mgr) {
    if(mgr->cur_c <= ' '){
        mgr->jsongString->inc();
        return true;
    }
    if(mgr->cur_c == '"') {
        mgr->key = "";
        mgr->p_state = PACKETSTAT_SAVING_KEY;
        mgr->pre_c = mgr->cur_c;
        mgr->jsongString->inc();
        return true;
    }
    if(mgr->cur_c == '}') {
        mgr->p_state = PACKETSTAT_END;
        return true;
    } else {
        return false;
    }
}
static bool saving_key_func(PacketStateMgr* mgr) {
    if(mgr->cur_c != '"' || mgr->pre_c == '\\') {
        mgr->key.append(mgr->cur_c);
        mgr->jsongString->inc();
        mgr->pre_c = mgr->cur_c;
        return true;
    } else {
        if(mgr->key == "w") {
            mgr->p_state = PACKETSTAT_SAVE_W_VALUE_PRE;
            mgr->pre_c = 0;
            mgr->jsongString->inc();
            mgr->client->scnotifyurl = "";
            return true;
        }
        if(mgr->key == "ir") {
            
        }
        if(mgr->key == "sn") {
            
        }
        if(mgr->key == "a") {
            
        }
        //bypass
    }
    return true;
}
static bool save_w_value_pre_func(PacketStateMgr* mgr) {
    if(mgr->cur_c == ' ' || mgr->cur_c == ':') {
        mgr->jsongString->inc();
        return true;
    }
    if(mgr->cur_c == '"') {
        mgr->p_state = PACKETSTAT_SAVE_W_VALUE;
        mgr->jsongString->inc();
        mgr->pre_c = 0;
        return true;
    } else {
        return false;
    }
}
static bool save_w_value_func(PacketStateMgr* mgr) {
    if(mgr->cur_c != '"' || mgr->pre_c != '\\') {
        mgr->client->scnotifyurl.append(mgr->cur_c);
        mgr->pre_c = mgr->cur_c;
        mgr->jsongString->inc();
        return true;
    } else {
        mgr->p_state = PACKETSTAT_COMMA;
        mgr->jsongString->inc();
        return true;
    }
}
bool find_comma_func(PacketStateMgr* mgr) {
    if(cur_c == ' ') {
        jsongString->inc();
        return true;;
    }
    if(cur_c == ',') {
        p_state = PACKETSTAT_HANDLE_SEARCH_KEY;
        jsongString->inc();
        return true;
    }
    return false;
}
PacketStateMgr::PacketStateMgr() {
    jsongString = nullptr;
    client = nullptr;
    func_map.resize(PACKETSTAT_END + 1);
    func_map[PACKETSTAT_BEGIN] = begin_func;
    func_map[PACKETSTAT_HANDLE_SEARCH_KEY] = search_key_func;
    func_map[PACKETSTAT_SAVING_KEY] = saving_key_func;
    func_map[PACKETSTAT_SAVE_W_VALUE_PRE] = save_w_value_pre_func;
    func_map[PACKETSTAT_SAVE_W_VALUE] = save_w_value_func;
    func_map[PACKETSTAT_COMMA] = find_comma_func;
}
PacketStateMgr& PacketStateMgr::getInstance() {
    static PacketStateMgr ins;
    return ins;
}
void PacketStateMgr::init(JsonString* jStr, MegaClient* cli) {
    jsongString = jStr;
    client = cli;
}



bool PacketStateMgr::exec() {
    if(jsongString == nullptr) {
        return false;
    }
    
    p_state = PACKETSTAT_BEGIN;
    while(jsongString->getCurChar(cur_c) == 0) {
        if(func_map[p_state](this)) {
            continue;
        }
        return false;
    }
    return false;
}
}
