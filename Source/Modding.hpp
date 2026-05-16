#pragma once

// #include "../Libraries/hostfxr/hostfxr.h"
// #include "../Libraries/hostfxr/coreclr_delegates.h"
// #include "../Libraries/hostfxr/nethost.h"

#include <SDL3/SDL_loadso.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_filesystem.h>

#include <fstream>
#include <ios>
#include <iostream>
#include <sstream>
#include <vector>

#include "../Libraries/Wasmtime/include/wasmtime.hh"

namespace Modding {
	// For now, C# modding support is not available; too complex to implement X(
	// namespace CSharp {
	// 	inline SDL_SharedObject* dotnetSO;
	// 	inline hostfxr_initialize_for_runtime_config_fn initFunction;

	// 	inline hostfxr_initialize_for_runtime_config_fn init_fptr;
	// 	inline hostfxr_get_runtime_delegate_fn get_delegate_fptr;
	// 	inline hostfxr_close_fn close_fptr;
	// 	inline load_assembly_and_get_function_pointer_fn load_assembly_and_get_function_pointer;
	// 	inline char_t dotNetSOPath[256];
	// 	// Load and initialize .NET Core and get desired function pointer for scenario
	// 	inline load_assembly_and_get_function_pointer_fn get_dotnet_load_assembly(const char_t *config_path)
	// 	{
	// 		// Load .NET Core
	// 		void *load_assembly_and_get_function_pointer = nullptr;
	// 		hostfxr_handle cxt = nullptr;
	// 		int rc = init_fptr(config_path, nullptr, &cxt);
	// 		if (rc != 0 || cxt == nullptr)
	// 		{
	// 			SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Loading .NET modding failed with error code %i.", rc);
	// 			close_fptr(cxt);
	// 			return nullptr;
	// 		}

	// 		// Get the load assembly function pointer
	// 		rc = get_delegate_fptr(
	// 			cxt,
	// 			hdt_load_assembly_and_get_function_pointer,
	// 			&load_assembly_and_get_function_pointer);
	// 		if (rc != 0 || load_assembly_and_get_function_pointer == nullptr)
	// 			SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Loading .NET modding (Get delegate) failed with error code %i.", rc);

	// 		close_fptr(cxt);
	// 		return (load_assembly_and_get_function_pointer_fn)load_assembly_and_get_function_pointer;
	// 	}
	// 	inline void initModding() {
	// 		size_t buffer_size = sizeof(dotNetSOPath) / sizeof(char_t);
	// 		int rc = get_hostfxr_path(dotNetSOPath, &buffer_size, nullptr);
	// 		if (rc != 0) SDL_LogWarn(SDL_LOG_CATEGORY_CUSTOM, "Starting C# mods support has failed.");
	// 		dotnetSO = SDL_LoadObject(dotNetSOPath);

	// 		initFunction = (hostfxr_initialize_for_runtime_config_fn)SDL_LoadFunction(dotnetSO, "hostfxr_initialize_for_runtime_config");
	// 		get_delegate_fptr =     (hostfxr_get_runtime_delegate_fn)SDL_LoadFunction(dotnetSO, "hostfxr_get_runtime_delegate");
	// 		close_fptr        =                    (hostfxr_close_fn)SDL_LoadFunction(dotnetSO, "hostfxr_close");

	// 		load_assembly_and_get_function_pointer = get_dotnet_load_assembly("./DotNetConfigOrSmth/");
	// 	}
	// 	inline void executeFunction(const char * functionName, const char* modName) {
	// 		using mod_init_fn = void(*)();
	// 		mod_init_fn init = nullptr;
	// 		// Function pointer to managed delegate
	// 		component_entry_point_fn function = nullptr;
	// 		int rc = load_assembly_and_get_function_pointer(
	// 			std::format("Mods/{}.dll", modName).c_str(),
	// 			std::format("clModClass, {}", modName).c_str(),
	// 			functionName,
	// 			nullptr,
	// 			nullptr,
	// 			(void**)&init);
	// 		}
	// 		init();

	// }
	namespace C {
		struct CMod {
			const char* name;
			const char* description;
			const char* version = "1.0";
			SDL_FunctionPointer initFunction;
			SDL_FunctionPointer updateFunction;
			SDL_FunctionPointer stopFunction;
			CMod(const char* name, const char* description, const char* version,
				SDL_FunctionPointer initialization, SDL_FunctionPointer update, SDL_FunctionPointer uninitialization) {
				this->name = name; this->description = description; this->version = version;
				this->initFunction = initialization; this->updateFunction = update; this->stopFunction = uninitialization;
			}
		};
		inline std::vector<CMod> mods;
		inline void initModding() {
			
		}
	}
	namespace Python {
		// Also not implemented yet.
	}
	namespace WASM {
		using namespace wasmtime;

		struct WASMod {
			const char* name;
			const char* description;
			const char* version = "1.0";
			Func initFunction;
			Func updateFunction;
			Func stopFunction;
			WASMod(const char* name, const char* description, const char* version,
				std::optional<Extern> initialization, std::optional<Extern> update, std::optional<Extern> uninitialization) {
				this->name = name; this->description = description; this->version = version;
				this->initFunction 		= std::get<Func>(*initialization);
				this->updateFunction 	= std::get<Func>(*update);
				this->stopFunction 		= std::get<Func>(*uninitialization);
			}
		};
		inline std::string readFile(const char *name) {
			std::ifstream watFile(name);
			watFile.open(name);
			std::stringstream strStream(std::ios_base::in);
			strStream << watFile.rdbuf();
			return strStream.str();
		}
		inline void initModding() {
			Engine engine;
            int match_count;
			char** mods = SDL_GlobDirectory("Mods", "*.wat", SDL_GLOB_CASEINSENSITIVE, &match_count);
			for (int i = 0; i < match_count; i++) {
				const Module module = Module::compile(engine, readFile(mods[i])).unwrap();
				Store store(engine);
				Func host_func =
      				Func::wrap(store, []() { std::cout << "Test2\n"; });
				auto instance = Instance::create(store, module, {host_func}).unwrap();
				std::get<Func>(*instance.get(store, "run"));
			}
		}
	}
}