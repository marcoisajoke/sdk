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
    ACTIONSTATE_SEARCH_ST_VALUE,
    ACTIONSTATE_SAVING_ST_VALUE,
    ACTIONSTATE_SEARCH_INT64_VALUE,
    ACTIONSTATE_SAVING_INT64_VALUE,
    ACTIONSTATE_SEARCH_STRING_VALUE,
    ACTIONSTATE_SAVING_STRING_VALUE,
    ACTIONSTATE_SEARCH_ARRAY_OF_STRING_VALUE,
    ACTIONSTATE_SEARCH_ARRAY_OF_STRING_STRING_VALUE,
    ACTIONSTATE_SAVING_ARRAY_OP_STRING_VALUE,
    ACTIONSTATE_SEARCH_STRING_OR_INT_VALUE,
    ACTIONSTATE_SEARCH_STRING_OR_INT_OR_BOOL_VALUE,
    ACTIONSTATE_SEARCH_BOOL_VALUE,
    ACTIONSTATE_SAVING_BOOL_VALUE,
    ACTIONSTATE_END,
};
class KeyStateData {
public:
    KeyStateData(ACTIONSTATE search_s, void* v) {
        search_state = search_s;
        value = v;
    }
    ACTIONSTATE search_state;
    void* value;
};

union StringOrInt {
    std::string str;
    int64_t i;
};
union StringOrIntOrBool {
    std::string str;
    int64_t i;
    bool b;
};

class MegaClient;
class ActionStateMgr;
typedef bool (*ActionStateMgrFuncPtr)(ActionStateMgr* mgr);
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
    static bool search_string_value(ActionStateMgr* mgr);
    static bool saving_string_value(ActionStateMgr* mgr);
    static bool search_int64_value(ActionStateMgr* mgr);
    static bool saving_int64_value(ActionStateMgr* mgr);
    static bool search_st_value(ActionStateMgr* mgr);
    static bool saving_st_value(ActionStateMgr* mgr);
    static bool search_arrayOfString_value(ActionStateMgr* mgr);
    static bool search_arrayOfString_string_value(ActionStateMgr* mgr);
    static bool saving_arrayOfString_value(ActionStateMgr* mgr);
    static bool search_string_or_int_value(ActionStateMgr* mgr);
    static bool search_string_or_int_or_bool_value(ActionStateMgr* mgr); 
    static bool search_bool_value(ActionStateMgr* mgr);
    static bool saving_bool_value(ActionStateMgr* mgr);
    /*action state function end*/
    
    
public:
    JsonString* jsonString;
    MegaClient* client;
    std::string key;
    std::string value;
    std::string cmd;
    ACTIONSTATE p_state;
    bool has_i = false;
    bool is_in_checker = false;
    bool is_check_bypass = false;
    KeyStateData* cur_data;

    //////////////////////////////////////////////
    /*keys cache begin*/
    std::string key_a;
    std::string key_i;
    std::string key_st;
    std::string key_u;  //sc_contacts() is array of object. implement after a while. string/int/array
    std::string key_at;
    std::string key_ou;
    std::string key_ok;
    std::string key_ha;
    std::string key_k;
    std::string key_fa;
    std::string key_msg;
    std::string key_h;
    std::string key_ph;
    std::string key_w;
    std::string key_id;
    std::string key_cid;
    std::string key_tz;
    std::string key_t; //maybe a array of object or int/string
    std::string key_td;
    int64_t key_okd;
    int64_t key_ts;
    int64_t key_it;
    int64_t key_uts;
    int64_t key_rts;
    int64_t key_dts;
    int64_t key_clv;
    int64_t key_down;
    int64_t key_ets;
    int64_t key_cs; //maybe a array or object in sc_scheduledmeetings
    int64_t key_f;
    int64_t key_c;
    int64_t key_gd;
    StringOrInt key_m;
    StringOrInt key_o;
    StringOrInt key_e;
    StringOrInt key_s;
    StringOrInt key_d;
    StringOrInt key_p;
    StringOrIntOrBool key_r; //may be a object in sc_scheduledmeetings
    StringOrInt key_n; // may be array of object sc_chatupdate
    std::vector<std::string> key_ua;
    std::vector<std::string> key_v;
    std::vector<std::string> key_cmd;
    bool key_g;
    bool key_op; // no value.
    /*keys cache end*/
    //////////////////////////////////////////////

    std::unordered_set<std::string>* keys_ptr = nullptr;
    
    
    std::unordered_map<std::string, KeyStateData> key_2_state = {
        {"a",    KeyStateData(ACTIONSTATE_SEARCH_STRING_VALUE,                &cmd)},
        {"i",    KeyStateData(ACTIONSTATE_SEARCH_STRING_VALUE,                &key_i)},
        {"u",    KeyStateData(ACTIONSTATE_SEARCH_STRING_VALUE,                &key_u)},
        {"at",   KeyStateData(ACTIONSTATE_SEARCH_STRING_VALUE,                &key_at)},
        {"ou",   KeyStateData(ACTIONSTATE_SEARCH_STRING_VALUE,                &key_ou)},
        {"op",   KeyStateData(ACTIONSTATE_SEARCH_STRING_VALUE,                &key_op)},
        {"okd",  KeyStateData(ACTIONSTATE_SEARCH_STRING_VALUE,                &key_okd)},
        {"ha",   KeyStateData(ACTIONSTATE_SEARCH_STRING_VALUE,                &key_ha)},
        {"k",    KeyStateData(ACTIONSTATE_SEARCH_STRING_VALUE,                &key_k)},
        {"fa",   KeyStateData(ACTIONSTATE_SEARCH_STRING_VALUE,                &key_fa)},
        {"msg",  KeyStateData(ACTIONSTATE_SEARCH_STRING_VALUE,                &key_msg)},
        {"h",    KeyStateData(ACTIONSTATE_SEARCH_STRING_VALUE,                &key_h)},
        {"ph",   KeyStateData(ACTIONSTATE_SEARCH_STRING_VALUE,                &key_ph)},
        {"w",    KeyStateData(ACTIONSTATE_SEARCH_STRING_VALUE,                &key_w)},
        {"id",   KeyStateData(ACTIONSTATE_SEARCH_STRING_VALUE,                &key_id)},
        {"cid",  KeyStateData(ACTIONSTATE_SEARCH_STRING_VALUE,                &key_cid)},
        {"t",    KeyStateData(ACTIONSTATE_SEARCH_STRING_VALUE,                &key_t)},
        {"st",   KeyStateData(ACTIONSTATE_SEARCH_ST_VALUE,                    &key_st)},
        {"it",   KeyStateData(ACTIONSTATE_SEARCH_INT64_VALUE,                 &key_it)},
        {"ts",   KeyStateData(ACTIONSTATE_SEARCH_INT64_VALUE,                 &key_ts)},
        {"ok",   KeyStateData(ACTIONSTATE_SEARCH_INT64_VALUE,                 &key_ok)},
        {"uts",  KeyStateData(ACTIONSTATE_SEARCH_INT64_VALUE,                 &key_uts)},
        {"rts",  KeyStateData(ACTIONSTATE_SEARCH_INT64_VALUE,                 &key_rts)},
        {"dts",  KeyStateData(ACTIONSTATE_SEARCH_INT64_VALUE,                 &key_dts)},
        {"clv",  KeyStateData(ACTIONSTATE_SEARCH_INT64_VALUE,                 &key_clv)},
        {"down", KeyStateData(ACTIONSTATE_SEARCH_INT64_VALUE,                 &key_down)},
        {"ets",  KeyStateData(ACTIONSTATE_SEARCH_INT64_VALUE,                 &key_ets)},
        {"cs",   KeyStateData(ACTIONSTATE_SEARCH_INT64_VALUE,                 &key_cs)},
        {"f",    KeyStateData(ACTIONSTATE_SEARCH_INT64_VALUE,                 &key_f)},
        {"c",    KeyStateData(ACTIONSTATE_SEARCH_INT64_VALUE,                 &key_c)},
        {"gd",   KeyStateData(ACTIONSTATE_SEARCH_INT64_VALUE,                 &key_gd)},
        {"td",   KeyStateData(ACTIONSTATE_SEARCH_INT64_VALUE,                 &key_td)},
        {"n",    KeyStateData(ACTIONSTATE_SEARCH_STRING_OR_INT_VALUE,         &key_n)},
        {"p",    KeyStateData(ACTIONSTATE_SEARCH_STRING_OR_INT_VALUE,         &key_p)},
        {"o",    KeyStateData(ACTIONSTATE_SEARCH_STRING_OR_INT_VALUE,         &key_o)},
        {"m",    KeyStateData(ACTIONSTATE_SEARCH_STRING_OR_INT_VALUE,         &key_m)},
        {"e",    KeyStateData(ACTIONSTATE_SEARCH_STRING_OR_INT_VALUE,         &key_e)},
        {"s",    KeyStateData(ACTIONSTATE_SEARCH_STRING_OR_INT_VALUE,         &key_s)},
        {"d",    KeyStateData(ACTIONSTATE_SEARCH_STRING_OR_INT_VALUE,         &key_d)},
        {"r",    KeyStateData(ACTIONSTATE_SEARCH_STRING_OR_INT_OR_BOOL_VALUE, &key_r)},
        {"ua",   KeyStateData(ACTIONSTATE_SEARCH_ARRAY_OF_STRING_VALUE,       &key_ua)},
        {"v",    KeyStateData(ACTIONSTATE_SEARCH_ARRAY_OF_STRING_VALUE,       &key_v)},
        {"cmd",  KeyStateData(ACTIONSTATE_SEARCH_ARRAY_OF_STRING_VALUE,       &key_cmd)},
        {"g",    KeyStateData(ACTIONSTATE_SEARCH_BOOL_VALUE,                  &key_g)},

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
