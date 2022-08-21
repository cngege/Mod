#pragma once

class Loader {
public:
	static void* dllHMODULE;
public:
	static auto init(void*)->void;
	static auto exit(void*)->void;
};