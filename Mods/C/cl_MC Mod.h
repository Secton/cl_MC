#pragma once

#ifdef __cplusplus
extern "C" {
#endif

enum LogLevel {Debug, Info, Warning, Error, FatalError};

__attribute__((import_module("clMC")))
__attribute__((import_name("log")))
void clMC_log(int level, const char* msg_ptr);

// Exported by mod (host will call these)
__attribute__((export_name("Mod_Init")))
void Mod_Init(void);

__attribute__((export_name("Mod_Update")))
void Mod_Update(float dt);

__attribute__((export_name("Mod_Shutdown")))
void Mod_Shutdown();

#ifdef __cplusplus
}
#endif