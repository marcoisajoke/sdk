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
    func_map[ACTIONSTATE_SAVING_ST_VALUE] = ActionStateMgr::saving_a_value;
    func_map[ACTIONSTATE_SEARCH_N_VALUE] = ActionStateMgr::search_n_value;
    func_map[ACTIONSTATE_SAVING_N_VALUE] = ActionStateMgr::saving_n_value;
    func_map[ACTIONSTATE_SEARCH_U_VALUE] = ActionStateMgr::search_u_value;
    func_map[ACTIONSTATE_SAVEING_U_VALUE] = ActionStateMgr::saving_u_value;
    func_map[ACTIONSTATE_SEARCH_AT_VALUE] = ActionStateMgr::search_at_value;
    func_map[ACTIONSTATE_SAVEING_AT_VALUE] = ActionStateMgr::saving_at_value;
    func_map[ACTIONSTATE_SEARCH_TS_VALUE] = ActionStateMgr::search_ts_value;
    func_map[ACTIONSTATE_SAVEING_TS_VALUE] = ActionStateMgr::saving_ts_value;
    func_map[ACTIONSTATE_SEARCH_OU_VALUE] = ActionStateMgr::search_ou_value;
    func_map[ACTIONSTATE_SAVEING_OU_VALUE] = ActionStateMgr::saving_ou_value;
    func_map[ACTIONSTATE_SEARCH_P_VALUE] = ActionStateMgr::search_p_value;
    func_map[ACTIONSTATE_SAVEING_P_VALUE] = ActionStateMgr::saving_p_value;
    func_map[ACTIONSTATE_BYPASS_OP_VALUE] = ActionStateMgr::bypass_op_value;
    func_map[ACTIONSTATE_SEARCH_O_VALUE] = ActionStateMgr::search_o_value;
    func_map[ACTIONSTATE_SAVEING_O_VALUE] = ActionStateMgr::saving_o_value;
    func_map[ACTIONSTATE_SEARCH_OK_VALUE] = ActionStateMgr::search_ok_value;
    func_map[ACTIONSTATE_SAVEING_OK_VALUE] = ActionStateMgr::saving_ok_value;

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
    keys_ptr = nullptr;
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
    if(mgr->jsonString->bypassChar(',')) {
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
            //do check only after get element a/i/st values
            //only do one time check for each object
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
        //check one time in each object
        is_check_bypass = false;
        if(fetchingnodes || !has_i || cmd == "d" || cmd == "t") {
            auto itr = cmd_keys.find(cmd);
            if(itr != cmd_keys.end()) {
                keys_ptr = &itr.second;
            }
            return true;
        } else {
            return false;
        }
    }
    return true;
}
bool ActionStateMgr::saving_key(ActionStateMgr* mgr) {
    if(mgr->jsonString->decodeString(mgr->key)) {
        return true;
    } else {
        if(!mgr->checkActionPacket()) {
            return false;
        }
        mgr->jsonString->inc();
        if(keys_ptr != nullptr && keys_ptr.find(mgr->key) == keys_ptr.end()) {
            mgr->jsonString->bypassValue();
            continue;
        }
        
        if(!mgr->checkCanRun()) {
            mgr->jsongString->bypassObjectInside();
            return true;
        }
        auto itr = mgr->key_2_state.find(mgr->key);
        if(itr == mgr->key_func.end()) {
            mgr->jsonString->bypassValue();
            return true;
        }
        mgr->p_state = itr.second;
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
    mgr->pstate = ACTIONSTATE_SEARCH_KEY;
    mgr->jsonString->inc();
    return true;
}
bool ActionStateMgr::search_n_value(ActionStateMgr* mgr) {
    auto ret = mgr->jsonString->findNextStringValue(mgr->p_state, ACTIONSTATE_SAVING_N_VALUE, mgr->value);
    return (ret >= 0);
}
bool ActionStateMgr::saving_n_value(ActionStateMgr* mgr) {
    mgr->key_n = mgr->jsonString->getHandler();
    mgr->pstate = ACTIONSTATE_SEARCH_KEY;
    return true;

}
bool ActionStateMgr::search_u_value(ActionStateMgr* mgr) {
    auto ret = mgr->jsonString->findNextStringValue(mgr->p_state, ACTIONSTATE_SAVING_U_VALUE, mgr->key_u);
    return (ret >= 0);
}
bool ActionStateMgr::saving_u_value(ActionStateMgr* mgr) {
    if(mgr->jsonString->decodeString(mgr->key_u)) {
        return true;
    }
    mgr->pstate = ACTIONSTATE_SEARCH_KEY;
    mgr->jsonString->inc();
    return true;
}
bool ActionStateMgr::search_at_value(ActionStateMgr* mgr) {
    auto ret = mgr->jsonString->findNextStringValue(mgr->p_state, ACTIONSTATE_SAVING_AT_VALUE, mgr->key_at);
    return (ret >= 0);
}
bool ActionStateMgr::saving_at_value(ActionStateMgr* mgr) {
    if(mgr->jsonString->decodeString(mgr->key_at)) {
        return true;
    }
    mgr->pstate = ACTIONSTATE_SEARCH_KEY;
    mgr->jsonString->inc();
    return true;
}
bool ActionStateMgr::search_ts_value(ActionStateMgr* mgr) {
    if(mgr->jsonString->bypassEmpty()) {
        return true;
    }
    if('0' <= cur_c && cur_c <= '9') {
        mgr->key_ts = 0;
        mgr->p_state = ACTIONSTATE_SAVING_TS_VALUE;
        return true;
    }
    if(cur_c == '-') {
        mgr->key_ts = -1;
        return true;
    }
    return false;
}
bool ActionStateMgr::saving_ts_value(ActionStateMgr* mgr) {
    if(mgr->jsonString->decodeInt64(mgr->key_ts)) {
        return true;
    }
    mgr->pstate = ACTIONSTATE_SEARCH_KEY;
    return true;
}
bool ActionStateMgr::search_ou_value(ActionStateMgr* mgr) {
    auto ret = mgr->jsonString->findNextStringValue(mgr->p_state, ACTIONSTATE_SAVING_OU_VALUE, mgr->value);
    return (ret >= 0);
}
bool ActionStateMgr::saving_ou_value(ActionStateMgr* mgr) {
    mgr->key_ou = mgr->jsonString->getHandler(USERHANDLE);
    mgr->pstate = ACTIONSTATE_SEARCH_KEY;
    return true;
}
bool ActionStateMgr::search_p_value(ActionStateMgr* mgr) {
    auto ret = mgr->jsonString->findNextStringValue(mgr->p_state, ACTIONSTATE_SAVING_P_VALUE, mgr->value);
    return (ret >= 0);
}
bool ActionStateMgr::saving_p_value(ActionStateMgr* mgr) {
    mgr->key_p = mgr->jsonString->getHandler(PCRHANDLE);
    mgr->pstate = ACTIONSTATE_SEARCH_KEY;
    return true;
}
bool ActionStateMgr::bypass_op_value(ActionStateMgr* mgr) {
    mgr->key_op = true;
    mgr->jsonString->bypassValue();
    mgr->pstate = ACTIONSTATE_SEARCH_KEY;
    return true;
}
bool ActionStateMgr::search_o_value(ActionStateMgr* mgr) {
    auto ret = mgr->jsonString->findNextStringValue(mgr->p_state, ACTIONSTATE_SAVING_O_VALUE, mgr->value);
    return (ret >= 0);
}
bool ActionStateMgr::saving_o_value(ActionStateMgr* mgr) {
    mgr->key_o = mgr->jsonString->getHandler(USERHANDLE);
    mgr->pstate = ACTIONSTATE_SEARCH_KEY;
    return true;
}
bool ActionStateMgr::search_ok_value(ActionStateMgr* mgr) {
    auto ret = mgr->jsonString->findNextStringValue(mgr->p_state, ACTIONSTATE_SAVING_AT_VALUE, mgr->key_ok);
    return (ret >= 0);
}
bool ActionStateMgr::saving_ok_value(ActionStateMgr* mgr) {
    if(mgr->jsonString->decodeString(mgr->key_ok)) {
        return true;
    }
    mgr->pstate = ACTIONSTATE_SEARCH_KEY;
    mgr->jsonString->inc();
    return true;
}
}
