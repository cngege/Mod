#pragma once

class Loader {
public:
	static void* dllHMODULE;
	static bool Eject_Signal;
	static bool RemoteFreeLib;
	static bool EnableEjectKey;
public:
	static auto init(void*)->void;
	static auto exit(void*)->void;
};