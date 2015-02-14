#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "tc_module.h"

struct tc_module {
    lua_State *L;
};

static tc_module *module = NULL;

tc_module *
tc_module_create() {
    if (module == NULL) {
        module = (tc_module *)malloc(sizeof(tc_module));
        module->L = luaL_newstate();
        luaL_openlibs(module->L);
    }

    return module;
}

void
tc_module_release(tc_module *module) {
    lua_close(module->L);
    free(module);
    module = NULL;
}

void
tc_module_load(tc_module *module, const char *mod_name) {
    luaL_loadfile(module->L, mod_name);
    printf("load module: %s\n", mod_name);
}

void
tc_module_call(tc_module *module) {
    int res;
    res = lua_pcall(module->L, 0, LUA_MULTRET, 0);
    if (res == 2) {
        printf("call res: %d\n", res);
        printf("run error: %s\n", lua_tostring(module->L, -1));
    } else {
        printf("call res: %d\n", res);
    }
}

static void
_init(lua_State *L, int id) {
    lua_getglobal(L, "init");
    lua_pushinteger(L, id);
    lua_call(L, 1, 0);
}

void
tc_module_init(tc_module *module, int id) {
    printf("tc_module_init: %d\n", id);
    _init(module->L, id);
}

static void
_recv(lua_State *L, int id, char *data, int len, char *type) {
    //lua_Debug *debug = (lua_Debug *)malloc(sizeof(lua_Debug));
    //lua_getstack(L, 0, debug);
    //printf("lua_Debug event: %d\n", debug->event);
    //printf("lua_Debug name: %s\n", debug->name);
    //printf("lua_Debug namewhat: %s\n", debug->namewhat);
    //printf("lua_Debug what: %s\n", debug->what);
    //printf("lua_Debug source: %s\n", debug->source);
    //printf("lua_Debug currentline: %d\n", debug->currentline);
    //printf("lua_Debug linedefined: %d\n", debug->linedefined);
    //printf("lua_Debug lastlinedefined: %d\n", debug->lastlinedefined);
    //printf("lua_Debug nups: %d\n", debug->nups);
    //printf("lua_Debug nparams: %d\n", debug->nparams);
    //printf("lua_Debug isvararg: %d\n", debug->isvararg);
    //printf("lua_Debug istailcall: %d\n", debug->istailcall);
    //free(debug);

    lua_getglobal(L, "recv");
    lua_pushinteger(L, id);
    lua_pushlstring(L, data, len);
    lua_pushinteger(L, len);
    lua_pushstring(L, type);

    lua_call(L, 4, 0);
}

//tc_module_node *
//_find(tc_module *module, int id) {
//    tc_module_node *head = module->head;
//
//    while (head) {
//        if (head->id == id) break;
//        head = head->next;
//    }
//
//    return head;
//}

void
tc_module_recv(tc_module *module, int id, char *data, int len, char *type) {
    _recv(module->L, id, data, len, type);
}

//static void
//_set_module(lua_State *L, tc_module *module) {
//    lua_getglobal(L, "set_module");
//    lua_pushlightuserdata(L, (void *)module);
//    lua_call(L, 1, 0);
//}
//
//void
//tc_module_set_module(tc_module *module) {
//    _set_module(module->L, module);
//}
