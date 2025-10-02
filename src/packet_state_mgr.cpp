//
//  megastate.c
//  sdk
//
//  Created by marco on 2025/10/1.
//

#include "mega/packet_state_mgr.h"


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
    PACKETSTAT_COMMA,
    PACKETSTAT_SAVE_W_VALUE_PRE,
    PACKETSTAT_SAVE_W_VALUE,
    PACKETSTAT_SAVE_NOLAST_PRE,
    PACKETSTAT_SAVE_NOLAST,
    PACKETSTAT_SAVE_NOLAST_AFTER,
    PACKETSTAT_SAVE_SN_PRE,
    PACKETSTAT_SAVE_SN,
    PACKETSTAT_END,
};

PacketStateMgr::PacketStateMgr() {
    jsongString = nullptr;
    client = nullptr;
    func_map.resize(PACKETSTAT_END + 1);
    func_map[PACKETSTAT_BEGIN] = PacketStateMgr::begin_func;
    func_map[PACKETSTAT_HANDLE_SEARCH_KEY] = PacketStateMgr::search_key_func;
    func_map[PACKETSTAT_SAVING_KEY] = PacketStateMgr::saving_key_func;
    func_map[PACKETSTAT_COMMA] = PacketStateMgr::find_comma_func;
    func_map[PACKETSTAT_SAVE_W_VALUE_PRE] = PacketStateMgr::save_w_value_pre_func;
    func_map[PACKETSTAT_SAVE_W_VALUE] = PacketStateMgr::save_w_value_func;
    func_map[PACKETSTAT_SAVE_NOLAST_PRE] = PacketStateMgr::save_nolast_pre_fun;
    func_map[PACKETSTAT_SAVE_NOLAST] = PacketStateMgr::save_nolast_func;
    func_map[PACKETSTAT_SAVE_NOLAST_AFTER] = PacketStateMgr::save_nolast_after_func;
    func_map[PACKETSTAT_SAVE_SN_PRE] = PacketStateMgr::save_sn_pre_func;
    func_map[PACKETSTAT_SAVE_SN] = PacketStateMgr::save_sn_func;
    
    
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
bool PacketStateMgr::bypassEmpty() {
    if(cur_c == " ") {
        jsongString->inc();
        return true;
    }
    return false;
}
bool PacketStateMgr::decodeString(std::string& s) {
    if(cur_c != '"' || pre_c == '\\') {
        s.append(cur_c);
        jsongString->inc();
        pre_c = cur_c;
        return true;
    }
    return false;
}
bool PacketStateMgr::bypassChar(char c) {
    if(cur_c == c) {
        jsongString->inc();
        return true;
    }
    return false;
}
bool PacketStateMgr::checkEnd() {
    if(cur_c == '}') {
        p_state = PACKETSTAT_END;
        return true;
    }
    return false;
}
bool PacketStateMgr::begin_func(PacketStateMgr* mgr) {
    if(mgr->bypassEmpty()) {
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
bool PacketStateMgr::search_key_func(PacketStateMgr* mgr) {
    if(mgr->bypassEmpty()) {
        return true;
    }
    if(mgr->cur_c == '"') {
        mgr->key = "";
        mgr->p_state = PACKETSTAT_SAVING_KEY;
        mgr->pre_c = mgr->cur_c;
        mgr->jsongString->inc();
        return true;
    }
    if(mgr->checkEnd()) {
        return true;
    } else {
        return false;
    }
}
bool PacketStateMgr::find_comma_func(PacketStateMgr* mgr) {
    if(mgr->bypassEmpty()) {
        return true;
    }
    if(mgr->cur_c == ',') {
        mgr->p_state = PACKETSTAT_HANDLE_SEARCH_KEY;
        mgr->jsongString->inc();
        return true;
    }
    if(mgr->checkEnd()) {
        return true;
    }
    return false;
}
bool PacketStateMgr::saving_key_func(PacketStateMgr* mgr) {
    if(mgr->decodeString(mgr->key)) {
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
            mgr->p_state = PACKETSTAT_SAVE_NOLAST_PRE;
            mgr->jsongString->inc();
            return true;
        }
        if(mgr->key == "sn") {
            mgr->p_state = PACKETSTAT_SAVE_SN_PRE;
            mgr->jsongString->inc();
            return true;
            
        }
        if(mgr->key == "a") {
            
        }
        //bypass
    }
    return true;
}
bool PacketStateMgr::save_w_value_pre_func(PacketStateMgr* mgr) {
    if(mgr->bypassEmpty()) {
        return true;
    }
    if(mgr->bypassChar(':')) {
        return true;
    }
    if(mgr->cur_c == '"') {
        mgr->p_state = PACKETSTAT_SAVE_W_VALUE;
        mgr->jsongString->inc();
        mgr->pre_c = 0;
        mgr->client->scnotifyurl = "";
        return true;
    } else {
        return false;
    }
}
bool PacketStateMgr::save_w_value_func(PacketStateMgr* mgr) {
    if(mgr->decodeString(mgr->client->scnotifyurl)) {
        return true;
    } else {
        mgr->p_state = PACKETSTAT_COMMA;
        mgr->jsongString->inc();
        return true;
    }
}
bool PacketStateMgr::save_nolast_pre_fun(PacketStateMgr* mgr) {
    if(mgr->bypassEmpty()) {
        return true;
    }
    if(mgr->bypassChar(':')) {
        return true;
    }
    mgr->p_state = PACKETSTAT_SAVE_NOLAST;
    return false;
}
bool PacketStateMgr::save_nolast_func(PacketStateMgr* mgr) {
    mgr->client->insca_notlast = (mgr->cur_c == '1');
    mgr->p_state =
    mgr->jsongString->inc();
    return true;
}
bool PacketStateMgr::save_nolast_after_func(PacketStateMgr* mgr) {
    if(mgr->checkEnd()) {
        return true;
    }
    if(mgr->cur_c != ',') {
        mgr->jsongString->inc();
        return true;
    }
    mgr->p_state = PACKETSTAT_SAVING_KEY;
    mgr->jsongString->inc();
    return false;
}

bool PacketStateMgr::save_sn_pre_func(PacketStateMgr* mgr) {
    if(mgr->bypassEmpty()) {
        return true;
    }
    if(mgr->bypassChar(':')) {
        return true;
    }
    if(mgr->cur_c == '"') {
        mgr->p_state = ;
        mgr->value = "";
        mgr->jsongString->inc();
        return true;
    }
    return false;
}

bool PacketStateMgr::save_sn_func(PacketStateMgr* mgr) {
    if(mgr->decodeString(mgr->value)) {
        return true;
    }
    //do logic
    handle t;
    auto l = Base64::atob(mgr->value.c_str(), &t, sizeof(t));
    if(sizeof(t) != l) {
        return false;
    }
    {
        auto client = mgr->client;
        client->scsn.setScsn(t);
        assert(!client->mCurrentSeqtagSeen);
        client->notifypurge();
        if (client->sctable)
        {
            if (!client->pendingcs && !client->csretrying && !client->reqs.readyToSend())
            {
                client->sctable->commit();
                client->sctable->begin();
                client->app->notify_dbcommit();
                client->pendingsccommit = false;
            }
            else
            {
                client->pendingsccommit = true;
            }
        }
    }
    mgr->p_state = PACKETSTAT_HANDLE_SEARCH_KEY;
    mgr->jsongString->inc();
    
    return true;
}
}
