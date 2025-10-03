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
    ACTIONSTATE_SEARCH_A_VALUE,
    ACTIONSTATE_SAVING_A_VALUE,
    ACTIONSTATE_SEARCH_ST_VALUE,
    ACTIONSTATE_SAVING_ST_VALUE,
    ACTIONSTATE_END,
};
typedef bool (*ActionStateMgrFuncPtr)(PacketStateMgr* mgr)
class ActionStateMgr {
public:
    ActionStateMgr();
    void init(JsonString* jStr, MegaClient* cli);
    bool exec();
    bool checkActionPacket();
    bool checkCanRun();
    std::vector<ActionStateMgrFuncPtr> func_map;
    static bool begin_func(ActionStateMgr* mgr);
    static bool into_action_object_func(ActionStateMgr* mgr);
    static bool search_key(ActionStateMgr* mgr);
    static bool saving_key(ActionStateMgr* mgr);
    static bool search_a_value(ActionStateMgr* mgr);
    static bool saving_a_value(ActionStateMgr* mgr);
public:
    JsonString* jsonString;
    MegaClient* client;
    string key;
    string value;
    ACTIONSTATE p_state;
    bool has_i = false;
    bool is_in_checker = false;
    bool is_check_bypass = false;
    std::string cmd;
    
    std::unordered_map<std::string, std::unordered_set<std::string>> cmd_keys = {
        {"u",       {"n", "u", "at", "ts"}},
        {"t",       {"t"/*go tree*/, "u", "ou"}},
        {"d",       {"p", "op", "n", "o", "u", "ou", "ok", "okd", "ha", "r", "ts", "k"}},
        {"s",       {"p", "op", "n", "o", "u", "ou", "ok", "okd", "ha", "r", "ts", "k"}},
        {"s2",      {"p", "op", "n", "o", "u", "ou", "ok", "okd", "ha", "r", "ts", "k"}},
        {"c",       {"u", "ou"}},
        {"fa",      {"fa", "n"}},
        {"ua",      {"u", "ua", "v"}},
        {"psts",    {"it", "p", "r"}},
        {"psts_v2", {"it", "p", "r"}},
        {"ftr",     {"it", "p", "r"}},
        {"pses",    {"ts"}},
        {"ipc",     {"m", "ts", "uts", "rts", "dts", "msg", "clv", "p"}},
        {"opc",     {"e", "m", "ts", "uts", "rts", "dts", "msg", "p"}},
        {"upci",    {"m", "uts", "s", "p", "ou"}},
        {"upco",    {"m", "uts", "s", "p", "ou"}},
        {"ph",      {"h", "ph", "w", "d", "n", "u", "down", "ets", "ts"}},
        {"se",      {"e", "u", "s"}},
        {"mcpc",    {}},
        {"mcc",     {"id", "u", "cs", "n", "g", "ou", "ct", "ts", "m", "ck", "mr", "w", "sr", "oi"}},
        {"mcfpc",   {"id", "f"}},
        {"mcfc",    {"id", "f"}},
        {"mcpna",   {"g", "r", "id", "n", "u"}},
        {"mcna",    {"g", "r", "id", "n", "u"}},
        {"mcsmp",   {"cid", "id", "p", "u", "ou", "tz", "s", "e", "t", "d", "at",
                     "o", "c", "f", "r" /*r is array*/, "cs" /*cs is object*/, "cmd"}},
        {"mcsmr",   {"id", "ou"}},
        {"uac",     {"m"}},
        {"la",      {}},
        {"ub",      {"s", "m"}},
        {"sqac",    {"gb"}},
        {"asp",     {"id", "ph", "at", "u", "k", "ts", "cts", "t"/*int*/}},
        {"ass",     {"s", "ts", "td"/*double check MAKENAMEID2*/, "r", "c"}},
        {"asr",     {"id"}},
        {"aep",     {"id", "s", "h", "at", "o", "ts", "k"}},
        {"aer",     {"id", "s"}},
        {"pk",      {}},
        {"uec",     {"m", "u", }},
        {"cce",     {}}
    };
};
}

#endif
