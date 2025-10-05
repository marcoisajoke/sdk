//
//  action_state_mgr.cpp
//  sdk
//
//  Created by marco on 2025/10/2.
//


#include "mega/stateMachine/action_state_mgr.h"


namespace mega {

ActionStateMgr::ActionStateMgr() {
    jsonString = nullptr;
    client = nullptr;
    func_map.resize(ACTIONSTATE_END + 1);
    func_map[ACTIONSTATE_BEGIN] = ActionStateMgr::begin_func;
    func_map[ACTIONSTATE_INTO_ACTION_OBJECT] = ActionStateMgr::into_action_object_func;
    func_map[ACTIONSTATE_SEARCH_KEY] = ActionStateMgr::search_key;
    func_map[ACTIONSTATE_SAVING_KEY] = ActionStateMgr::saving_key;
    func_map[ACTIONSTATE_SEARCH_ST_VALUE] = ActionStateMgr::search_st_value;
    func_map[ACTIONSTATE_SAVING_ST_VALUE] = ActionStateMgr::saving_st_value;
    func_map[ACTIONSTATE_SEARCH_STRING_VALUE] = ActionStateMgr::search_string_value;
    func_map[ACTIONSTATE_SAVING_STRING_VALUE] = ActionStateMgr::saving_string_value;
    func_map[ACTIONSTATE_SEARCH_INT64_VALUE] = ActionStateMgr::search_int64_value;
    func_map[ACTIONSTATE_SAVING_INT64_VALUE] = ActionStateMgr::saving_int64_value;
    func_map[ACTIONSTATE_SEARCH_ARRAY_OF_STRING_VALUE] = ActionStateMgr::search_arrayOfString_value;
    func_map[ACTIONSTATE_SEARCH_ARRAY_OF_STRING_STRING_VALUE] = ActionStateMgr::search_arrayOfString_string_value;
    func_map[ACTIONSTATE_SAVING_ARRAY_OP_STRING_VALUE] = ActionStateMgr::saving_arrayOfString_value;
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
            cur_data = nullptr;
            return true;
        }
        mgr->cur_data = &itr->second;
        mgr->p_state = mgr->cur_data->search_state
    }
    return true;
}
bool ActionStateMgr::search_int64_value(ActionStateMgr* mgr) {
    return mgr->jsonString->findNextInt64(mgr->p_state, ACTIONSTATE_SAVING_INT64_VALUE, *((int64_t*)(mgr->cur_data->value)));
}
bool ActionStateMgr::saving_int64_value(ActionStateMgr* mgr) {
    if(mgr->jsonString->decodeInt64(*((int64_t*)(mgr->cur_data->value)))) {
        return true;
    }
    mgr->pstate = ACTIONSTATE_SEARCH_KEY;
    mgr->jsonString->inc();
    return true;
}
bool ActionStateMgr::search_string_value(ActionStateMgr* mgr) {
    auto ret = mgr->jsonString->findNextStringValue(mgr->p_state, ACTIONSTATE_SAVING_STRING_VALUE, *((std::string*)(mgr->cur_data->value)));
    return (ret >= 0);
}
bool ActionStateMgr::saving_string_value(ActionStateMgr* mgr) {
    if(mgr->jsonString->decodeString(*((std::string*)(mgr->cur_data->value)))) {
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
bool ActionStateMgr::search_arrayOfString_value(ActionStateMgr* mgr) {
    if(mgr->jsonString->bypassEmpty()) {
        return true;
    }
    if(mgr->jsonString->bypassChar(':')) {
        return true;
    }
    if(mgr->jsonString->cur_c == '[') {
        mgr->jsonString->inc();
        mgr->p_state = ACTIONSTATE_SEARCH_ARRAY_OF_STRING_STRING_VALUE;
        std::vector<std::string>* v = (std::vector<std::string>*)(mgr->cur_data->value);
        v->clear();
        return true;
    }
    return false;
}
bool ActionStateMgr::search_arrayOfString_string_value(ActionStateMgr* mgr) {
    if(mgr->jsonString->bypassEmpty()) {
        return true;
    }

    if(mgr->jsonString->cur_c == '"') {
        mgr->value = "";
        mgr->jsonString->inc();
        return true;    
    }
    if(mgr->jsonString->cur_c == ']') {
        mgr->pstate = ACTIONSTATE_SEARCH_KEY;
        mgr->jsonString->inc();
        return true;
    }

    return false;
}
bool ActionStateMgr::saving_arrayOfString_value(ActionStateMgr* mgr) {
    if(mgr->jsonString->decodeString(mgr->value)) {
        return true;
    }
    if(mgr->value.length() > 0) {
        ((std::vector<std::string>*)(mgr->cur_data->value))->push_back(mgr->value);
    }
    mgr->value.clear();
    mgr->p_state = ACTIONSTATE_SEARCH_ARRAY_OF_STRING_STRING_VALUE;
    mgr->jsonString->inc();
    return true;

}

}
