#pragma once
#include <string>
//#include <WinUser.h>
#include "../Utils/Utils.h"

enum KeyMode {
	Trigger = 0,						//按下 触发 onTrigger();
	Switch = 1,							//按下一次  isenable 的值就切换一次 同时也会触发对应的 onEnable()  onDisable()
	Hold = 2							//按下 触发 onEnable()   松开触发  onDisable()
};

struct IntUIValue
{
	std::string name;
	int* value;
	int min;
	int max;
	float speed;
};

struct FloatUIValue
{
	std::string name;
	float* value;
	float min;
	float max;
	float speed;
};

struct BoolUIValue
{
	std::string name;
	bool* value;
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
	std::vector<int> controlkeys = std::vector<int>();		//是否要求功能键也要按下
	std::string modulename;
	std::string moduleinfo;
	KeyMode km = KeyMode::Switch;

	std::vector<IntUIValue> intUIValue = std::vector<IntUIValue>();
	std::vector<FloatUIValue> floatUIValue = std::vector<FloatUIValue>();
	std::vector<BoolUIValue> boolUIValue = std::vector<BoolUIValue>();
public:
	void SetKeyMode(KeyMode km);
	KeyMode GetKeyMode();
protected:
	void AddIntUIValue(std::string name, int* defautvalue_ptr, int minvalue, int maxvalue, float speed);
	void AddFloatUIValue(std::string name, float* defautvalue_ptr, float minvalue, float maxvalue, float speed);
	void AddBoolUIValue(std::string name, bool* defautvalue_ptr);
public:
	std::vector<IntUIValue> GetIntUIValue();
	std::vector<FloatUIValue> GetFloatUIValue();
	std::vector<BoolUIValue> GetBoolUIValue();
public:
	virtual int getKeybind();
	virtual void setKeybind(int key);
	virtual std::vector<int> getcontrolkeysbind();
	virtual bool checkcontrolkeys();
	virtual void setcontrolkeysbind(std::vector<int> key);
	virtual std::string getModuleName();
	virtual std::string getModuleInfo();
	virtual std::string getBindKeyName();
	virtual void onTick(class GameMode*);
	virtual void onKeyUpdate(int key, bool isDown);
	virtual void onMouseUpdate(char mousebutton, char isdown, __int16 mouseX, __int16 mouseY, __int16 relativeMovementX, __int16 relativeMovementY);
	virtual void onTrigger();
	virtual void onEnable();
	virtual void onDisable();
	virtual bool onAttack(class Actor*);				// 返回值可以拦截该事件
	virtual bool useItem(class GameMode*,class ItemStack*);
	virtual bool useItemOn(class GameMode*, class ItemStack*, vec3_ti*, uint8_t*, struct vec3_t*, class Block*);
	virtual bool onKnockback(class LocalPlayer*, struct vec3_t*);
	virtual void onActorTick(class Actor*);
	virtual void onLocalPlayerTick(class LocalPlayer*);
	virtual void onRenderDetour(class MinecraftUIRenderContext*);
	virtual void onImGUIRender();
	virtual bool onSendMessage(class TextHolder*);
	virtual void setEnabled(bool enabled);
	virtual void toggle();
	virtual bool isEnabled();
};