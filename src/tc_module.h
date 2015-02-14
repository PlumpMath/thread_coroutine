#ifndef _TC_MODULE_H_
#define _TC_MODULE_H_

typedef struct tc_module tc_module;

tc_module * tc_module_create();
void        tc_module_release(tc_module *module);

//void        tc_module_load(tc_module *module, const char *mod_name);
//void        tc_module_call(tc_module *module);
void        tc_module_init(tc_module *module, int id);
void        tc_module_recv(tc_module *module, int id, char *data, int len, char *type);

//void        tc_module_set_module(tc_module *module);
#endif
