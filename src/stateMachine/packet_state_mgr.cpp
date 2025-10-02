//
//  megastate.c
//  sdk
//
//  Created by marco on 2025/10/1.
//

#include "mega/stteMachine/packet_state_mgr.h"


namespace mega {
enum PACKETSTATE {
    PACKETSTATE_BEGIN,
    PACKETSTATE_HANDLE_SEARCH_KEY,
    PACKETSTATE_SAVING_KEY,
    PACKETSTATE_COMMA,
    PACKETSTATE_SAVE_W_VALUE_PRE,
    PACKETSTATE_SAVE_W_VALUE,
    PACKETSTATE_SAVE_NOLAST_PRE,
    PACKETSTATE_SAVE_NOLAST,
    PACKETSTATE_SAVE_NOLAST_AFTER,
    PACKETSTATE_SAVE_SN_PRE,
    PACKETSTATE_SAVE_SN,
    PACKETSTATE_END,
};

PacketStateMgr::PacketStateMgr() {
    jsonString = nullptr;
    client = nullptr;
    func_map.resize(PACKETSTATE_END + 1);
    func_map[PACKETSTATE_BEGIN] = PacketStateMgr::begin_func;
    func_map[PACKETSTATE_HANDLE_SEARCH_KEY] = PacketStateMgr::search_key_func;
    func_map[PACKETSTATE_SAVING_KEY] = PacketStateMgr::saving_key_func;
    func_map[PACKETSTATE_COMMA] = PacketStateMgr::find_comma_func;
    func_map[PACKETSTATE_SAVE_W_VALUE_PRE] = PacketStateMgr::save_w_value_pre_func;
    func_map[PACKETSTATE_SAVE_W_VALUE] = PacketStateMgr::save_w_value_func;
    func_map[PACKETSTATE_SAVE_NOLAST_PRE] = PacketStateMgr::save_nolast_pre_fun;
    func_map[PACKETSTATE_SAVE_NOLAST] = PacketStateMgr::save_nolast_func;
    func_map[PACKETSTATE_SAVE_NOLAST_AFTER] = PacketStateMgr::save_nolast_after_func;
    func_map[PACKETSTATE_SAVE_SN_PRE] = PacketStateMgr::save_sn_pre_func;
    func_map[PACKETSTATE_SAVE_SN] = PacketStateMgr::save_sn_func;
    
    
}
PacketStateMgr& PacketStateMgr::getInstance() {
    static PacketStateMgr ins;
    return ins;
}
void PacketStateMgr::init(JsonString* jStr, MegaClient* cli) {
    jsonString = jStr;
    client = cli;
}



bool PacketStateMgr::exec() {
    if(jsonString == nullptr) {
        return false;
    }
    
    p_state = PACKETSTATE_BEGIN;
    while(jsonString->checkCurChar()) {
        if(func_map[p_state](this)) {
            continue;
        }
        return false;
    }
    return false;
}

bool PacketStateMgr::checkEnd() {
    if(jsonString->jsonString->cur_c == '}') {
        p_state = PACKETSTATE_END;
        return true;
    }
    return false;
}
bool PacketStateMgr::begin_func(PacketStateMgr* mgr) {
    if(mgr->jsonString->bypassEmpty()) {
        return true;
    }
    if(mgr->jsonString->jsonString->cur_c == '{') {
        mgr->p_state = PACKETSTATE_HANDLE_SEARCH_KEY;
        mgr->jsonString->inc();
        return true;
    } else {
        return false;
    }
}
bool PacketStateMgr::search_key_func(PacketStateMgr* mgr) {
    if(mgr->jsonString->bypassEmpty()) {
        return true;
    }
    if(mgr->jsonString->cur_c == '"') {
        mgr->key = "";
        mgr->p_state = PACKETSTATE_SAVING_KEY;
        mgr->jsonString->inc();
        return true;
    }
    if(mgr->checkEnd()) {
        return true;
    } else {
        return false;
    }
}
bool PacketStateMgr::find_comma_func(PacketStateMgr* mgr) {
    if(mgr->jsonString->bypassEmpty()) {
        return true;
    }
    if(mgr->jsonString->cur_c == ',') {
        mgr->p_state = PACKETSTATE_HANDLE_SEARCH_KEY;
        mgr->jsonString->inc();
        return true;
    }
    if(mgr->checkEnd()) {
        return true;
    }
    return false;
}
bool PacketStateMgr::saving_key_func(PacketStateMgr* mgr) {
    if(mgr->jsonString->decodeString(mgr->key)) {
        return true;
    } else {
        mgr->jsonString->inc();
        if(mgr->key == "w") {
            mgr->p_state = PACKETSTATE_SAVE_W_VALUE_PRE;
            mgr->client->scnotifyurl = "";
            return true;
        }
        if(mgr->key == "ir") {
            mgr->p_state = PACKETSTATE_SAVE_NOLAST_PRE;
            return true;
        }
        if(mgr->key == "sn") {
            mgr->p_state = PACKETSTATE_SAVE_SN_PRE;
            return true;
        }
        if(mgr->key == "a") {
            mgr->action_state_mgr.init(mgr->jsonString, mgr->client);
            return mgr->action_state_mgr.exec();
        }
        //bypass any not expected key
        mgr->jsonString->bypassValue();
    }
    return true;
}
bool PacketStateMgr::save_w_value_pre_func(PacketStateMgr* mgr) {
    if(mgr->jsonString->bypassEmpty()) {
        return true;
    }
    if(mgr->jsonString->bypassChar(':')) {
        return true;
    }
    if(mgr->jsonString->cur_c == '"') {
        mgr->p_state = PACKETSTATE_SAVE_W_VALUE;
        mgr->jsonString->inc();
        mgr->client->scnotifyurl = "";
        return true;
    } else {
        return false;
    }
}
bool PacketStateMgr::save_w_value_func(PacketStateMgr* mgr) {
    if(mgr->jsonString->decodeString(mgr->client->scnotifyurl)) {
        return true;
    } else {
        mgr->p_state = PACKETSTATE_COMMA;
        mgr->jsonString->inc();
        return true;
    }
}
bool PacketStateMgr::save_nolast_pre_fun(PacketStateMgr* mgr) {
    if(mgr->jsonString->bypassEmpty()) {
        return true;
    }
    if(mgr->jsonString->bypassChar(':')) {
        return true;
    }
    mgr->p_state = PACKETSTATE_SAVE_NOLAST;
    return false;
}
bool PacketStateMgr::save_nolast_func(PacketStateMgr* mgr) {
    mgr->client->insca_notlast = (mgr->jsonString->cur_c == '1');
    mgr->p_state =
    mgr->jsonString->inc();
    return true;
}
bool PacketStateMgr::save_nolast_after_func(PacketStateMgr* mgr) {
    if(mgr->checkEnd()) {
        return true;
    }
    if(mgr->jsonString->cur_c != ',') {
        mgr->jsonString->inc();
        return true;
    }
    mgr->p_state = PACKETSTATE_SAVING_KEY;
    mgr->jsonString->inc();
    return false;
}

bool PacketStateMgr::save_sn_pre_func(PacketStateMgr* mgr) {
    if(mgr->jsonString->bypassEmpty()) {
        return true;
    }
    if(mgr->jsonString->bypassChar(':')) {
        return true;
    }
    if(mgr->jsonString->cur_c == '"') {
        mgr->p_state = ;
        mgr->value = "";
        mgr->jsonString->inc();
        return true;
    }
    return false;
}

bool PacketStateMgr::save_sn_func(PacketStateMgr* mgr) {
    if(mgr->jsonString->decodeString(mgr->value)) {
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
    mgr->p_state = PACKETSTATE_HANDLE_SEARCH_KEY;
    mgr->jsonString->inc();
    
    return true;
}
}
