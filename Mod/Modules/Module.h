#pragma once
#include <string>
//#include <WinUser.h>
#include "../Mod/Utils/Utils.h"

enum KeyMode {
	Trigger = 0,						//按下 触发 onTrigger();
	Switch = 1,							//按下一次  isenable 的值就切换一次 同时也会触发对应的 onEnable()  onDisable()
	Hold = 2							//按下 触发 onEnable()   松开触发  onDisable()
};


class Module
{
protected:
	Module(int key,std::string name,std::string info);
public:
	virtual ~Module();
private:
	bool enabled = false;
	int keybind = 0x00;
	std::string modulename;
	std::string moduleinfo;
	KeyMode km = KeyMode::Trigger;
public:
	void SetKeyMode(KeyMode km);
	KeyMode GetKeyMode();
public:
	virtual int getKeybind();
	virtual void setKeybind(int key);
	virtual std::string getModuleName();
	virtual void onTick(class GameMode*);
	virtual void onKeyUpdate(int key, bool isDown);
	virtual void onTrigger();
	virtual void onEnable();
	virtual void onDisable();
	virtual bool onAttack(class Actor*);				// 返回值可以拦截该事件
	virtual bool onKnockback(class LocalPlayer*, struct vec3_t*);
	virtual void onActorTick(class Actor*);
	virtual void onLocalPlayerTick(class LocalPlayer*);
	virtual void setEnabled(bool enabled);
	virtual void toggle();
	virtual bool isEnabled();
};