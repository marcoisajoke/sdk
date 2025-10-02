//
//  megastate.h
//  sdk
//
//  Created by marco on 2025/10/1.
//


#ifndef PACKET_STATE_MGR_H
#define PACKET_STATE_MGR_H 1


namespace mega {

class JsonString {
public:
    JsonString();
    void init(const char* s, uint64_t size);
    int getCurChar(char& c);
    void inc();
private:
    char* str;
    uint64_t pos;
    uint64_t size;
};
typedef int (*FuncPtr)(int, int);
typedef bool (*PacketStateMgrFuncPtr)(PacketStateMgr* mgr)
class PacketStateMgr {
public:
    PacketStateMgr& getInstance();
    void init(JsonString* jStr, MegaClient* cli);
    bool exec();
    bool bypassEmpty();
    bool decodeString(std::string& s);
    bool bypassChar(char c);
    bool checkEnd();
    static bool begin_func(PacketStateMgr* mgr);
    static bool search_key_func(PacketStateMgr* mgr);
    static bool saving_key_func(PacketStateMgr* mgr);
    static bool find_comma_func(PacketStateMgr* mgr);
    static bool save_w_value_pre_func(PacketStateMgr* mgr);
    static bool save_w_value_func(PacketStateMgr* mgr);
    static bool save_nolast_pre_fun(PacketStateMgr* mgr);
    static bool save_nolast_func(PacketStateMgr* mgr);
    static bool save_nolast_after_func(PacketStateMgr* mgr);
    static bool save_sn_pre_func(PacketStateMgr* mgr);
    static bool save_sn_func(PacketStateMgr* mgr);
    
private:
    PacketStateMgr();
    std::vector<PacketStateMgrFuncPtr> func_map;
public:
    JsonString* jsongString;
    MegaClient* client;
    char cur_c = 0;
    char pre_c = 0;
    string key;
    string value;
    PACKETSTAT p_state;
};
}
#endif
