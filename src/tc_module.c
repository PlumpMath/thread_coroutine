#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "tc_module.h"

typedef struct tc_module_node tc_module_node;
struct tc_module_node {
    lua_State *L;
    int id;
    tc_module_node *next;
};

struct tc_module {
    tc_module_node *head;
    tc_module_node *tail;
    pthread_mutex_t lock;
};

static tc_module *module = NULL;

tc_module *
tc_module_create() {
    int res;

    if (module == NULL) {
        module = (tc_module *)malloc(sizeof(tc_module));
        module->head = NULL;
        module->tail = NULL;
        res = pthread_mutex_init(&(module->lock), NULL);
        if (res != 0) {
            printf("pthread_mutex_init failed\n");
            exit(0);
        }
    }

    return module;
}

void
tc_module_release(tc_module *module) {
    pthread_mutex_destroy(&(module->lock));
    free(module);
    module = NULL;
}

static void
_tc_module_load(lua_State *L, const char *mod_name) {
    luaL_loadfile(L, mod_name);
    printf("load module: %s\n", mod_name);
}

static void
_tc_module_call(lua_State *L) {
    int res;
    res = lua_pcall(L, 0, LUA_MULTRET, 0);
    if (res == 2) {
        printf("call res: %d\n", res);
        printf("run error: %s\n", lua_tostring(L, -1));
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
    tc_module_node *node = (tc_module_node *)malloc(sizeof(tc_module_node));
    node->L = luaL_newstate();
    luaL_openlibs(node->L);
    _tc_module_load(node->L, "./lualib/tc.lua");
    _tc_module_call(node->L);
    _tc_module_load(node->L, "./lualib/normal.lua");
    _tc_module_call(node->L);
    node->id = id;
    node->next = NULL;
    if (module->head == NULL) {
        module->head = node;
        module->tail = node;
    } else {
        module->tail->next = node;
        module->tail = node;
    }
    _init(node->L, id);
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

tc_module_node *
_find(tc_module *module, int id) {
    tc_module_node *head = module->head;

    while (head) {
        if (head->id == id) break;
        head = head->next;
    }

    return head;
}

void
tc_module_recv(tc_module *module, int id, char *data, int len, char *type) {
    tc_module_node *node = _find(module, id);
    if (node == NULL) return;
    _recv(node->L, id, data, len, type);
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
