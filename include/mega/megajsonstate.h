//
//  megastate.h
//  sdk
//
//  Created by marco on 2025/10/1.
//


#ifndef MEGASTATE_H
#define MEGASTATE_H 1


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
private:
    PacketStateMgr();
    std::vector<PacketStateMgrFuncPtr> func_map;
public:
    JsonString* jsongString;
    MegaClient* client;
    char cur_c = 0;
    char pre_c = 0;
    string key;
    PACKETSTAT p_state;
};
}
#endif
