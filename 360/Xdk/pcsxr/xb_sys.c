#include <stdio.h>
#include <stdlib.h>
#include <xtl.h>
#include "psxcommon.h"
#include "r3000a.h"
#include "xbPlugins.h"


// Init mem and plugins
int  SysInit(){

	SysPrintf("SysInit\r\n");
	psxInit();

	SysPrintf("LoadPlugins()\r\n");
	if(LoadPlugins()==-1)
		SysPrintf("ErrorLoadingPlugins()\r\n");

	SysPrintf("LoadMcds\r\n");
	LoadMcds(Config.Mcd1, Config.Mcd2);

	SysPrintf("InitComplete\r\n");
	return 0;
}

// Resets mem
void SysReset(){
	SysPrintf("SysReset\r\n");
	psxReset();
}						

// Printf used by bios syscalls
void SysPrintf(char *fmt, ...){
	va_list list;
	char msg[512];

	va_start(list, fmt);
	vsprintf(msg, fmt, list);
	va_end(list);

	OutputDebugStringA(msg);
}

// Message used to print msg to users
void SysMessage(char *fmt, ...){
	va_list list;
	char msg[512];

	va_start(list, fmt);
	vsprintf(msg, fmt, list);
	va_end(list);

	OutputDebugStringA(msg);
}

#if 0
// Loads Library
void *SysLoadLibrary(char *lib){
}

// Loads Symbol from Library
void *SysLoadSym(void *lib, char *sym){
}
#endif

PluginTable plugins[] =
{ PLUGIN_SLOT_0,
PLUGIN_SLOT_1,
PLUGIN_SLOT_2,
PLUGIN_SLOT_3,
PLUGIN_SLOT_4,
PLUGIN_SLOT_5,
PLUGIN_SLOT_6,
PLUGIN_SLOT_7
};

void *SysLoadLibrary(char *lib) {
	int i;
	for(i=0; i<NUM_PLUGINS; i++)
		if((plugins[i].lib != NULL) && (!strcmp(lib, plugins[i].lib)))
			return (void*)i;
	return NULL;
}

void *SysLoadSym(void *lib, char *sym) {
	PluginTable* plugin = plugins + (int)lib;
	int i;
	for(i=0; i<plugin->numSyms; i++)
		if(plugin->syms[i].sym && !strcmp(sym, plugin->syms[i].sym))
			return plugin->syms[i].pntr;
	return NULL;
}


// Gets previous error loading sysbols
const char *SysLibError(){
	return NULL;
}

// Closes Library
void SysCloseLibrary(void *lib){
}		

// Called on VBlank (to update i.e. pads)
void SysUpdate(){

}

// Returns to the Gui
void SysRunGui(){
}						

// Close mem and plugins
void SysClose(){
	SysPrintf("psxShutdown\r\n");
	psxShutdown();
	ReleasePlugins();
}

void ClosePlugins() {
	int ret;

	PAD1_close();
	PAD2_close();

	ret = CDR_close();
	if (ret < 0) { SysMessage (_("Error Closing CDR Plugin")); return; }
	ret = GPU_close();
	if (ret < 0) { SysMessage (_("Error Closing GPU Plugin")); return; }
	ret = SPU_close();
	if (ret < 0) { SysMessage (_("Error Closing SPU Plugin")); return; }
}