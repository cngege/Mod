#pragma once
#include <string>
#include <functional>
#include "../Utils/Utils.h"
#include "../Utils/config.h"
#include "HMath.h"
#include "mcstring.h"

enum KeyMode {
	Trigger = 0,						//按下 触发 onTrigger();
	Switch = 1,							//按下一次  isenable 的值就切换一次 同时也会触发对应的 onEnable()  onDisable()
	Hold = 2							//按下 触发 onEnable()   松开触发  onDisable()
};

struct IntUIValue
{
	std::string name{};
	int* value = nullptr;
	int min = 0;
	int max = 0;
	bool slider = false;
	float speed = 0;
};

struct FloatUIValue
{
	std::string name{};
	float* value = nullptr;
	float min = 0;
	float max = 0;
	bool slider = false;
	float speed = 0;
};

struct BoolUIValue
{
	std::string name;
	bool* value = nullptr;
};

struct ButtonUIEvent
{
	std::string name;
	bool sameline;							//同一行 和前一个UI之间不换行
	std::function<void()> et;
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
	std::vector<ButtonUIEvent> buttonUIEvent = std::vector<ButtonUIEvent>();

public:
	void SetKeyMode(KeyMode km);
	KeyMode GetKeyMode();
protected:
	void AddIntUIValue(std::string name, int* defautvalue_ptr, int minvalue, int maxvalue, bool slider = true, float speed = 1.f);
	void AddFloatUIValue(std::string name, float* defautvalue_ptr, float minvalue, float maxvalue, bool slider = true, float speed = 0.05f);
	void AddBoolUIValue(std::string name, bool* defautvalue_ptr);
	void AddButtonUIEvent(std::string name, bool sameline, std::function<void()> et);
public:
	std::vector<IntUIValue> GetIntUIValue();
	std::vector<FloatUIValue> GetFloatUIValue();
	std::vector<BoolUIValue> GetBoolUIValue();
	std::vector<ButtonUIEvent> GetButtonUIEvent();
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
	virtual bool onAttackBefore(class GameMode* ,class Actor*);				// 返回值可以拦截该事件
	virtual void onAttackAfter(class GameMode* ,class Actor*);
	virtual bool useItem(class GameMode*,class ItemStack*);
	virtual bool useItemOn(class GameMode*, class ItemStack*, class ItemInstance*, vec3_ti* , uint8_t*, vec3_t* , class Block*);
	virtual bool onKnockback(class LocalPlayer*, vec3_t*);
	virtual void onActorTick(class Actor*);
	virtual void onActorSightTick(class Actor*);
	virtual void onPlayerTick(class Player*);					// 这个在远程服务器只有本地玩家会调用
	virtual void onPlayerSightTick(class Player*);
	virtual void onServerPlayerTick(class ServerPlayer*);
	virtual void onLocalPlayerTick(class LocalPlayer*);
	virtual void onRemotePlayerTick(class RemotePlayer*);
	virtual void onLevelTick(class Level*);
	virtual void onstartLeaveGame(class Level*);
	virtual void onRenderDetour(class MinecraftUIRenderContext*);
	virtual void onImGUIRender();
	virtual void onInternalImGUIRender();
	virtual bool onSendMessage(std::mcstring*);
	virtual void setEnabled(bool enabled);
	virtual void toggle();
	virtual void onloadConfigFile(json& data);			// 要求模块加载配置文件的时候将调用此方法 , 此时模块将data中的数据读取出来，应用到模块中
	virtual void onsaveConfigFile(json& data);			// 要求保存配置文件时将调用此方法, 此时，模块将变量等要存储的信息保存到data中,全部结束后统一保存到文件
	virtual bool isEnabled();
	virtual bool onSendPackToServer(class LoopbackPacketSender*, class Packet*);
	virtual void onDimensionChanged(class ClientInstance*);
};