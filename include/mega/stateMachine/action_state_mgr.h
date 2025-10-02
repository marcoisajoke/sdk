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
    ACTIONSTATE_END,
};
typedef bool (*ActionStateMgrFuncPtr)(PacketStateMgr* mgr)
class ActionStateMgr {
public:
    ActionStateMgr();
    void init(JsonString* jStr, MegaClient* cli);
    bool exec();
    std::vector<ActionStateMgrFuncPtr> func_map;
    static bool action_state_begin(ActionStateMgr* mgr);
public:
    JsonString* jsongString;
    MegaClient* client;
    char cur_c = 0;
    char pre_c = 0;
    string key;
    string value;
    ACTIONSTATE p_state;
};
}

#endif
