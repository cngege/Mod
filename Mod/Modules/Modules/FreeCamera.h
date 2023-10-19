#pragma once
#include "../Module.h"

class FreeCamera : public Module {
public:
	FreeCamera();
	~FreeCamera();
private:
	static void* _LockCameraTick(void* a1, unsigned int a2);
	static void* _setCameraPosTick(void* a1, void* a2, void* a3);

public:

	static bool UnlockCamera;

public:

	//virtual void onKeyUpdate(int key, bool isDown) override;
};
