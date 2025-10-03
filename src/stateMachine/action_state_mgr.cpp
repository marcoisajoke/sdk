//
//  action_state_mgr.cpp
//  sdk
//
//  Created by marco on 2025/10/2.
//


#include "mega/stteMachine/action_state_mgr.h"


namespace mega {

ActionStateMgr::ActionStateMgr() {
    jsonString = nullptr;
    client = nullptr;
    func_map.resize(ACTIONSTATE_END + 1);
    func_map[ACTIONSTATE_BEGIN] = ActionStateMgr::begin_func;
    func_map[ACTIONSTATE_SEARCH_KEY] = ActionStateMgr::search_key;
    func_map[ACTIONSTATE_SAVING_KEY] = ActionStateMgr::saving_key;
    func_map[ACTIONSTATE_SEARCH_A_VALUE] = ActionStateMgr::search_a_value;
    func_map[ACTIONSTATE_SAVING_A_VALUE] = ActionStateMgr::saving_a_value;
    func_map[ACTIONSTATE_SEARCH_ST_VALUE] = ActionStateMgr::search_st_value;
}
void ActionStateMgr::init(JsonString* jStr, MegaClient* cli) {
    jsonString = jStr;
    client = cli;
    key = "";
    value = "";
    p_state = 0;
    is_in_checker = false;
    cmd = "";
    is_check_bypass = false;
    has_i = false;
}

bool ActionStateMgr::exec() {
    if(jsonString == nullptr) {
        return false;
    }
    
    p_state = ACTIONSTATE_BEGIN;
    while(jsonString->checkCurChar()) {
        if(func_map[p_state](this)) {
            continue;
        }
        return false;
    }
    return false;
}

bool ActionStateMgr::begin_func(ActionStateMgr* mgr) {
    if(mgr->jsonString->bypassEmpty()) {
        return true;
    }
    if(mgr->jsonString->bypassChar(':')) {
        return true;
    }
    if(mgr->jsonString->cur_c == '[') {
        mgr->pstate = ACTIONSTATE_INTO_ACTION_OBJECT;
        mgr->jsonString->inc();
        return true;
    }
    //bypass json value
    mgr->pstate = ACTIONSTATE_END;
    mgr->jsonString->bypassValue();
    return true;
}
bool ActionStateMgr::into_action_object_func(ActionStateMgr* mgr) {
    if(mgr->jsonString->bypassEmpty()) {
        return true;
    }
    if(mgr->jsonString->cur_c == '{') {
        mgr->is_in_checker = true;
        mgr->pstate = ACTIONSTATE_SEARCH_KEY;
        mgr->jsonString->inc();
        return true;
    }
    mgr->pstate = ACTIONSTATE_END;
    mgr->jsonString->bypassValue();
    return true;
}
bool ActionStateMgr::checkEnd() {
    if(jsonString->jsonString->cur_c == '}') {
        p_state = ACTIONSTATE_END;
        return true;
    }
    return false;
}
bool ActionStateMgr::search_key(ActionStateMgr* mgr) {
    if(mgr->jsonString->bypassEmpty()) {
        return true;
    }
    if(mgr->jsonString->cur_c == '"') {
        mgr->key = "";
        mgr->p_state = ACTIONSTATE_SAVING_KEY;
        mgr->jsonString->inc();
        return true;
    }
    if(mgr->checkEnd()) {
        return true;
    } else {
        return false;
    }
}
bool ActionStateMgr::checkActionPacket() {
    if(is_in_checker) {
        if(key != "a" && key != "i" && key != "st") {
            if(cmd == "t" && client->lastAPDeletedNode.get() && dynamic_cast<CommandMoveNode*>(client->reqs.getCurrentCommand(client->mCurrentSeqtagSeen))) {
                LOG_verbose << client->clientname << "st tag implicity not changing for moves";
                is_in_checker = false;
                is_check_bypass = true;
                return true;
            } else  {
                
                // Action Packet with no Sequence Tag.
                is_in_checker = false;
                auto ret = client->sc_checkSequenceTag(string());
                if(ret) {
                    is_check_bypass = true;
                }return ret;
            }
        }
    }
    return true;
}
bool ActionStateMgr::checkCanRun() {
    if(is_check_bypass) {
        is_check_bypass = false;
        if(fetchingnodes) {
            return true;
        }
        if(!has_i) {
            return true;
        }
        if(cmd == "d" || cmd == "t") {
            return true;
        }

    }
    return false;
}
bool ActionStateMgr::saving_key(ActionStateMgr* mgr) {
    if(mgr->jsonString->decodeString(mgr->key)) {
        return true;
    } else {
        if(!mgr->checkActionPacket()) {
            return false;
        }
        mgr->jsonString->inc();
        if(!mgr->checkCanRun()) {
            mgr->jsongString->bypassObjectInside();
            return true;
        }

        if(mgr->key == "a") {
            mgr->p_state = ACTIONSTATE_SEARCH_A_VALUE;
            return true;
        }
        if(mgr->key == "i") {
            mgr->jsonString->bypassValue();
            has_i = true;
            return true;
        }
        if(mgr->key == "st") {
            mgr->p_state = ACTIONSTATE_SEARCH_ST_VALUE;
            return true;
        }
    }
    return true;
}
bool ActionStateMgr::search_a_value(ActionStateMgr* mgr) {
    auto ret = mgr->jsonString->findNextStringValue(mgr->p_state, ACTIONSTATE_SAVING_A_VALUE, mgr->cmd);
    return (ret >= 0);
}
bool ActionStateMgr::saving_a_value(ActionStateMgr* mgr) {
    if(mgr->jsonString->decodeString(mgr->cmd)) {
        return true;
    }
    mgr->pstate = ACTIONSTATE_SEARCH_KEY;
    mgr->jsonString->inc();
    return true;
}
bool ActionStateMgr::search_st_value(ActionStateMgr* mgr) {
    auto ret = mgr->jsonString->findNextStringValue(mgr->p_state, ACTIONSTATE_SAVING_ST_VALUE, mgr->value);
    return (ret >= 0);
}
bool ActionStateMgr::saving_st_value(ActionStateMgr* mgr) {
    if(mgr->jsonString->decodeString(mgr->value)) {
        return true;
    }
    if(!mgr->client->sc_checkSequenceTag(mgr->value)) {
        assert(mgr->client->reqs.cmdsInflight());
        return false;
    }
    
    mgr->is_in_checker = false;
    is_check_bypass = true;
    if (!mgr->client->statecurrent)
    {
        mgr->client->fnstats.actionPackets++;
    }
    return true;
}
}
