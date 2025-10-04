//
//  action_state_mgr.h
//  sdk
//
//  Created by marco on 2025/10/2.
//


#ifndef ACTION_STATE_MGR_H
#define ACTION_STATE_MGR_H 1
#include <unordered_set>

#include "mega/stateMachine/json_string.h"
#include "mega/version.h"
#include "mega/types.h"

namespace mega {
enum ACTIONSTATE {
    ACTIONSTATE_BEGIN,
    ACTIONSTATE_INTO_ACTION_OBJECT,
    ACTIONSTATE_SEARCH_KEY,
    ACTIONSTATE_SAVING_KEY,
    ACTIONSTATE_SEARCH_A_VALUE,
    ACTIONSTATE_SAVING_A_VALUE,
    ACTIONSTATE_SEARCH_I_VALUE, //do something
    ACTIONSTATE_SEARCH_ST_VALUE,
    ACTIONSTATE_SAVING_ST_VALUE,
    ACTIONSTATE_SEARCH_N_VALUE,
    ACTIONSTATE_SAVEING_N_VALUE,
    ACTIONSTATE_SEARCH_U_VALUE,
    ACTIONSTATE_SAVEING_U_VALUE,
    ACTIONSTATE_SEARCH_AT_VALUE,
    ACTIONSTATE_SAVEING_AT_VALUE,
    ACTIONSTATE_SEARCH_TS_VALUE,
    ACTIONSTATE_SAVEING_TS_VALUE,
    ACTIONSTATE_SEARCH_OU_VALUE,
    ACTIONSTATE_SAVEING_OU_VALUE,
    ACTIONSTATE_SEARCH_P_VALUE,
    ACTIONSTATE_SAVEING_P_VALUE,
    ACTIONSTATE_BYPASS_OP_VALUE,
    ACTIONSTATE_SEARCH_O_VALUE,
    ACTIONSTATE_SAVEING_O_VALUE,
    ACTIONSTATE_SEARCH_OK_VALUE,
    ACTIONSTATE_SAVEING_OK_VALUE,
    ACTIONSTATE_END,
};
class MegaClient;
class ActionStateMgr;
typedef bool (*ActionStateMgrFuncPtr)(ActionStateMgr* mgr);
typedef bool (*ActionStateKeyFuncPtr)(ActionStateMgr* mgr);
class ActionStateMgr {
public:
    ActionStateMgr();
    void init(JsonString* jStr, MegaClient* cli);
    bool exec();
    bool checkActionPacket();
    bool checkCanRun();
    std::vector<ActionStateMgrFuncPtr> func_map;
    
    /*action state function begin*/
    static bool begin_func(ActionStateMgr* mgr);
    static bool into_action_object_func(ActionStateMgr* mgr);
    static bool search_key(ActionStateMgr* mgr);
    static bool saving_key(ActionStateMgr* mgr);
    static bool search_a_value(ActionStateMgr* mgr);
    static bool saving_a_value(ActionStateMgr* mgr);
    static bool search_st_value(ActionStateMgr* mgr);
    static bool saving_st_value(ActionStateMgr* mgr);
    static bool search_n_value(ActionStateMgr* mgr);
    static bool saving_n_value(ActionStateMgr* mgr);
    static bool search_u_value(ActionStateMgr* mgr);
    static bool saving_u_value(ActionStateMgr* mgr);
    static bool search_at_value(ActionStateMgr* mgr);
    static bool saving_at_value(ActionStateMgr* mgr);
    static bool search_ou_value(ActionStateMgr* mgr);
    static bool saving_ou_value(ActionStateMgr* mgr);
    static bool search_p_value(ActionStateMgr* mgr);
    static bool saving_p_value(ActionStateMgr* mgr);
    static bool bypass_op_value(ActionStateMgr* mgr);
    static bool search_o_value(ActionStateMgr* mgr);
    static bool saving_o_value(ActionStateMgr* mgr);
    static bool search_ok_value(ActionStateMgr* mgr);
    static bool saving_ok_value(ActionStateMgr* mgr);
    /*action state function end*/
    
    
public:
    JsonString* jsonString;
    MegaClient* client;
    std::string key;
    std::string value;
    ACTIONSTATE p_state;
    bool has_i = false;
    bool is_in_checker = false;
    bool is_check_bypass = false;
    std::string cmd;

    //////////////////////////////////////////////
    /*keys cache begin*/
    handle key_n = UNDEF;
    std::string key_u;
    std::string key_at;
    int64_t key_ts;
    handle key_ou;
    handle key_p;
    bool key_op;
    handle key_o;
    std::string key_ok;
    /*keys cache end*/
    //////////////////////////////////////////////

    std::unordered_set<std::string>* keys_ptr = nullptr;
    
    std::unordered_map<std::string, ACTIONSTATE> key_2_state = {
        {"a", ACTIONSTATE_SEARCH_A_VALUE},
        {"i", ACTIONSTATE_SEARCH_I_VALUE},
        {"st", ACTIONSTATE_SEARCH_ST_VALUE},
        {"n", ACTIONSTATE_SEARCH_N_VALUE},
        {"at", ACTIONSTATE_SEARCH_AT_VALUE},
        {"ts", ACTIONSTATE_SEARCH_TS_VALUE},
        {"ou", ACTIONSTATE_SEARCH_OU_VALUE},
        {"p", ACTIONSTATE_SEARCH_P_VALUE},
        {"op", ACTIONSTATE_BYPASS_OP_VALUE},
        {"o", ACTIONSTATE_SEARCH_O_VALUE},
        {"ok", ACTIONSTATE_SEARCH_OK_VALUE},
    };
    
    std::unordered_map<std::string, std::unordered_set<std::string>> cmd_keys = {
        {"u",       {"n", "u", "at", "ts"}},
        {"t",       {"t"/*go tree*/, "u", "ou"}},
        {"d",       {"n", "ou"}},
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
