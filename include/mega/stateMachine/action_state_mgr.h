//
//  action_state_mgr.h
//  sdk
//
//  Created by marco on 2025/10/2.
//


#ifndef ACTION_STATE_MGR_H
#define ACTION_STATE_MGR_H 1
#include "mega/stateMachine/json_string.h"

namespace mega {
enum ACTIONSTATE {
    ACTIONSTATE_BEGIN,
    ACTIONSTATE_INTO_ACTION_OBJECT,
    ACTIONSTATE_SEARCH_KEY,
    ACTIONSTATE_SAVING_KEY,
    ACTIONSTATE_END,
};
typedef bool (*ActionStateMgrFuncPtr)(PacketStateMgr* mgr)
class ActionStateMgr {
public:
    ActionStateMgr();
    void init(JsonString* jStr, MegaClient* cli);
    bool exec();
    std::vector<ActionStateMgrFuncPtr> func_map;
    static bool begin_func(ActionStateMgr* mgr);
    static bool into_action_object_func(ActionStateMgr* mgr);
    static bool search_key(ActionStateMgr* mgr);
    static bool saving_key(ActionStateMgr* mgr);
public:
    JsonString* jsonString;
    MegaClient* client;
    string key;
    string value;
    ACTIONSTATE p_state;
};
}

#endif
