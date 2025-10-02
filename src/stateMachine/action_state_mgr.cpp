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
    func_map[ACTIONSTATE_BEGIN] = ActionStateMgr::action_state_begin_func;
    func_map[ACTIONSTATE_INTO_ACTION_OBJECT] = ActionStateMgr::action_state_into_action_object_func;
    func_map[ACTIONSTATE_SEARCH_KEY] = ActionStateMgr::action_state_search_key;
    func_map[ACTIONSTATE_SAVING_KEY] = ActionStateMgr::action_state_saving_key;
    
}
void ActionStateMgr::init(JsonString* jStr, MegaClient* cli) {
    jsonString = jStr;
    client = cli;
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

bool ActionStateMgr::action_state_begin_func(ActionStateMgr* mgr) {
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
bool ActionStateMgr::action_state_into_action_object_func(ActionStateMgr* mgr) {
    if(mgr->jsonString->bypassEmpty()) {
        return true;
    }
    if(mgr->jsonString->cur_c == '{') {
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
bool ActionStateMgr::action_state_search_key(ActionStateMgr* mgr) {
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
bool ActionStateMgr::action_state_saving_key(ActionStateMgr* mgr) {
    if(mgr->jsonString->decodeString(mgr->key)) {
        return true;
    } else {
        mgr->jsonString->inc();
        if(mgr->key == "w") {
        }
    }
    return true;
}
}
