//
//  action_state_mgr.cpp
//  sdk
//
//  Created by marco on 2025/10/2.
//


#include "mega/stteMachine/action_state_mgr.h"


namespace mega {

ActionStateMgr::ActionStateMgr() {
    jsongString = nullptr;
    client = nullptr;
    func_map.resize(ACTIONSTATE_END + 1);
    func_map[ACTIONSTATE_BEGIN] = ActionStateMgr::action_state_begin;
}
void ActionStateMgr::init(JsonString* jStr, MegaClient* cli) {
    jsongString = jStr;
    client = cli;
}

bool ActionStateMgr::exec() {
    if(jsongString == nullptr) {
        return false;
    }
    
    p_state = ACTIONSTATE_BEGIN;
    while(jsongString->getCurChar(cur_c) == 0) {
        if(func_map[p_state](this)) {
            continue;
        }
        return false;
    }
    return false;
}
bool ActionStateMgr::action_state_begin(ActionStateMgr* mgr) {
    
}
}
