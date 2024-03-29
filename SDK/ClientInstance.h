﻿#pragma once
#include "Level.h"
#include "Player.h"

#include <cstdint>
#include <string>
#include "..\Mod\Utils\HMath.h"
#include "Utils.h"
#include "Logger.h"

class LevelRenderer {
public:
	// 从InGamePlayScreen::_renderItemInHand中获取 从LevelRenderer 到 LevelRendererPlayer的转换
	// 搜索 "variable.bob_animation" 快速定位InGamePlayScreen::_renderItemInHand函数
	class LevelRenderPlayer* getLevelRendererPlayer() {
		// + 776 InGamePlayScreen::_renderItemInHand
		static SignCode sign("getLevelRendererPlayer");
		sign.AddSign("48 8B 88 ? ? ? ? F3 44 0F 10 81", [](uintptr_t v) { return v + 3; });
		sign.AddSign("48 8B 88 ? ? ? ? E8 ? ? ? ? 0F 28 C8", [](uintptr_t v) { return v + 3; });
		sign.AddSign("48 8B 88 ? ? ? ? E8 ? ? ? ? 90 48 85 DB 74 ? 48", [](uintptr_t v) { return v + 3; });
		sign.AddSign("48 8B 90 ? ? ? ? F2 0F 10 82", [](uintptr_t v) { return v + 3; });
		_ASSERT(sign);
		auto offset = *reinterpret_cast<int*>(*sign);
		return *reinterpret_cast<LevelRenderPlayer**>((uintptr_t)this + offset);
	}


};


class LevelRenderPlayer {
public:
	//先拿到 LevelRendererPlayer 地址，然后看内存，人称视角移动相机，找相机位置，最后确定指针 确定偏移，然后找访问写入看偏移，找特征码
	vec3_t& getCamPos() {
		static SignCode sign("getCamPos");
		sign.AddSign("F3 41 0F 11 87 ? ? ? ? F3 41 0F 11 8F ? ? ? ? F3", [](uintptr_t v) { return v + 5; });
		sign.AddSign("F3 0F 11 87 ? ? ? ? F3 0F 11 8F ? ? ? ? F3 0F 11 97 ? ? ? ? F3", [](uintptr_t v) { return v + 4; });
		
		_ASSERT(sign);
		//logF("*reinterpret_cast<int*>(*sign) %d", (uintptr_t)this + *reinterpret_cast<int*>(*sign));
		return *(vec3_t*)((uintptr_t)this + *reinterpret_cast<int*>(*sign));//0x5E4
	}
};

//From	https://github.com/NRGJobro/Horion-Open-SRC/blob/master/SDK/CClientInstance.h
class ClientInstance {
protected:
	static uintptr_t** vfTables;
public:
	template <typename TRet, typename... TArgs>
	static auto GetVFtableFun(int) -> auto*;
	static auto GetVFtableFun(int) -> uintptr_t*;
	static auto GetVFtables() -> uintptr_t**;
	static auto SetVFtables(uintptr_t** vTables) -> void;
public:
/*
private:
	virtual __int64 destructorClientInstance();
	// Duplicate destructor
	virtual __int64 onInitMinecraftGame(void);
	virtual __int64 onDestroyMinecraftGame(void);
	virtual __int64 init(__int64*, __int64&, __int64&, __int64&, __int64&, int);
	virtual __int64 initSceneFactory(__int64*);
	virtual __int64 initGraphics(void);
	virtual __int64 stop(void);
	virtual __int64 teardown(void);
	virtual __int64 preShutDown(void);
	virtual __int64 requestLeaveGame(bool, bool);
	virtual __int64 stopPlayScreen(void);

public:
	virtual bool isLeaveGameDone(void) const;
	virtual void setupPlayScreenForLeaveGame(void);

private:
	virtual __int64 resetPrimaryClient(void);
	virtual __int64 resetGameSession(void);
	virtual __int64 tick(void);									// 1.20 - 19(+4)
	virtual __int64 frameUpdate(__int64&);
	virtual __int64 update(bool);
	virtual __int64 endFrame(void);
	virtual __int64 startSubClientLateJoin(bool);

public:
	virtual void setupClientGame(__int64&&, bool);

private:
	virtual __int64 sub_1400C5D00(void) const;
	virtual __int64 getLocalC_Player(void);						// 1.20 - 27
	virtual __int64 getLocalC_Player(void) const;				// 1.20 - 28 28 return 0000
	virtual __int64 getCameraEntity(void) const;

public:
	virtual void setCameraEntity(Actor*);					//virtual void setCameraEntity(C_Entity*);

private:
	virtual __int64 getCameraTargetEntity(void) const;

public:
	virtual void setCameraTargetEntity(Actor*);
	virtual bool isLeavingGame(void) const;
	virtual bool isDestroyingGame(void) const;

private:
	virtual __int64 sub_1400C98E0(void) const;
	virtual __int64 useLowFrequencyUIRender(void) const;

public:
	virtual bool isSplitScreenActive(void) const;
	virtual bool isVRClient(void) const;
	virtual bool isARClient(void) const;
	virtual bool isARVRClient(void) const;
	virtual bool isHoloviewerMode(void) const;
	virtual bool isHoloscreenMode(void) const;
	virtual bool isLivingroomMode(void) const;
	virtual bool isExitingLevel(void) const;
	virtual bool isNotVLRMode(void) const;
	virtual bool isHoloRealityMode(void) const;
	virtual bool isRealityFullVRMode(void) const;
	virtual bool isInBedScreen(void) const;
	virtual bool isKeyboardEnabled(void) const;
	virtual bool isVRTransitioning(void) const;
	virtual bool hasCommands(void) const;
	virtual bool isScriptingEnabled(void) const;

private:
	virtual __int64 getHoloviewerScale(void) const;
	virtual __int64 getSplitScreenCount(void) const;

public:
	virtual bool isStereoRendering(void) const;
	virtual bool isPlatformNX(void) const;
	virtual bool isLocalSplitscreenWith(__int64 const&) const;
	virtual bool isValidCrossPlatformSkin(void) const;
	virtual __int64 sub_1400C9B70(void);

private:
	virtual __int64 getSplitScreenInfo(void) const;
	virtual __int64 getCurrentMaxGUIScaleIndex(void) const;
	virtual __int64 getRealityModeFrameFactor(void) const;

public:
	virtual void setRealityModeFrameFactor(float const&);

private:
	virtual __int64 getRealityModeToggleTriggered(void) const;

public:
	virtual void setRealityModeToggleTriggered(bool);

private:
	virtual bool sub_1400C9C50(void);

public:
	virtual void setOpenControllerDisconnectScreen(bool);

private:
	virtual __int64 getClientPlayMode(void) const;

public:
	virtual void setClientPlayMode(__int64 const&);

private:
	virtual __int64 getLivingRoomForCredits(void) const;

public:
	virtual void setLivingRoomForCredits(bool);

private:
	virtual __int64 getCreditsCallback(void);

public:
	virtual void setCreditsCallback(__int64);
	virtual void setupTransitionForCredits(__int64);

private:
	virtual __int64 refreshScreenSizeData(void);
	virtual __int64 onScreenSizeChanged(int, int, float);
	virtual __int64 onGuiScaleOffsetChanged(void);
	virtual __int64 onSafeZoneChanged(void);
	virtual __int64 quit(std::string const&, std::string const&);

public:
	virtual __int64 getMinecraftGame(void) const;

private:
	virtual __int64 getAutomationClient(void) const;
	virtual __int64 getScreenshotStreamer(void) const;
	virtual __int64 getEventing(void) const;
	virtual __int64 sub_1400CA2E0(__int64 a1);
	virtual __int64 sub_1400CA2E8(__int64 a1);
	virtual __int64 sub_1400CA2F0(__int64 a1);
public:
	virtual class BitmapFont* getFont(void) const;
	virtual class BitmapFont* getRuneFont(void) const;
	virtual class BitmapFont* getUnicodeFont(void) const;
private:
	virtual __int64 getGeometryGroup(void) const;
	virtual __int64 getMultiplayerServiceManager(void) const;
	virtual __int64 getLocalServerLevel(void);
	virtual __int64 getResourcePackRepository(void) const;
	virtual __int64 getResourcePackManager(void) const;
	virtual __int64 getSkinRepository(void) const;
	virtual __int64 getSkinRepositoryClientInterface(void) const;
	virtual __int64 sub_140630650(void) const;
	virtual void test0();
	virtual void test1();
	virtual __int64 sub_1400CA400(void) const;
	virtual __int64 getTextures(void) const;
	virtual __int64 getStoreCacheTextures(void) const;
	virtual __int64 getMinecraftGraphics(void) const;
	virtual __int64 getUIRepository(void) const;
	virtual __int64 getUserManager(void) const;
	virtual __int64 wantToQuit(void) const;

public:
	virtual bool isPrimaryLevelCrossPlatformMultiplayer(void) const;
	virtual bool isPrimaryLevelMultiplayer(void) const;
	virtual bool isAdhocEnabled(void) const;

private:
	virtual char sub_1406A1620(void) const;
	virtual __int64 linkToOffer(std::string const&, bool, std::string const&, __int64);
	virtual __int64 linkToAllOffers(std::string const&, std::string const&, bool, std::string const&, bool);
	virtual __int64 navigateToBrazeScreen(std::string const&, std::string const&, std::string const&, std::string const&, std::string const&, std::string const&, std::string const&);
	virtual __int64 navigateToStoreHomeScreen(void);
	virtual __int64 navigateToCoinPurchaseScreen(int, __int64);
	virtual __int64 navigateToPurchaseOfferScreen(__int64, bool);
	virtual __int64 navigateToOfferCollectionScreen(__int64&);
	virtual __int64 navigateToStoreSeeAllByCreatorScreen(std::string const&, bool);
	virtual __int64 navigateToServersScreen(bool);
	virtual __int64 navigateToHowToPlayScreen(std::string const&);
	virtual __int64 tryPushLeaveGameScreen(void);
	virtual char sub_1400D16A0(void) const;

public:
	virtual bool isReadyToRender(void) const;

private:
	virtual __int64 onDimensionChanged(void);
	virtual __int64 onGameEventNotification(__int64);

public:
	virtual class TextHolder getTopScreenName(void) const;
	virtual void setLeaveGameInProgressAsReadyToContinue(void);

private:
	virtual __int64 stopDestroying(void);
	virtual __int64 onClientCreatedLevel(__int64);
	virtual __int64 getClientRandomId(void) const;
	virtual __int64 getUserAuthentication(void);
	virtual __int64 sub_1400CAC50(__int64* a2);
	virtual __int64 registerToUserManager(__int64&, int);
	virtual __int64 resumeWithUserManager(__int64&, int);
	virtual __int64 createUserAuthentication(std::string);
	virtual __int64 createUserAuthentication(unsigned long long, std::string const&);
	virtual __int64 getPlatformId(void) const;
	virtual __int64 getPlatformOnlineId(void) const;

public:
	virtual bool isHoloCursorNeeded(void) const;

private:
	virtual __int64 useController(void) const;
	virtual __int64 useTouchscreen(void) const;
	virtual void subFunction1(void) const;
	virtual void subFunction2(void) const;
	virtual void subFunction3(void) const;
	virtual void subFunction4(void) const;
	virtual void subFunction5(void) const;
	virtual void subFunction6(void) const;
	virtual void subFunction7(void) const;

public:
	virtual bool getMouseGrabbed(void) const;

private:
	virtual __int64 currentInputModeIsGamePadOrMotionController(void) const;
	virtual __int64 currentInputModeIsMouseAndKeyboard(void) const;
	virtual __int64 getForceMonoscopic(void) const;
	virtual __int64 allowPicking(void) const;

public:
	virtual bool isShowingMenu(void) const;
	virtual bool isShowingProgressScreen(void) const;
	virtual bool isShowingWorldProgressScreen(void) const;
	virtual bool isShowingDeathScreen(void) const;
	virtual bool isScreenReplaceable(void) const;
	virtual bool isInGame(void) const;
	virtual bool sub_14070D6F0(void) const;
	virtual bool isInRealm(void);

private:
	virtual __int64 readyForShutdown(void) const;

public:
	virtual bool isPrimaryClient(void) const;
	virtual bool isEduMode(void) const;
	virtual bool isGamepadCursorEnabled(void) const;

private:
	virtual void* getServerData(void);						//virtual Minecraft* getServerData(void);
	virtual void* getServerData(void) const;				//virtual Minecraft* getServerData(void) const;

public:
	virtual Level* getLevel(void);
	virtual Level* getLevel(void) const;
	virtual bool isPreGame(void) const;
	virtual bool isInMultiplayerGame(void) const;
	virtual bool isMultiC_PlayerClient(void) const;
	virtual __int64 sub_14012F590() const;
	virtual __int64 sub_14012F4D0() const;
	virtual __int64 getOptions(void);
	virtual __int64 getOptions(void) const;
	//virtual __int64 getOptionsPtr(void);
	virtual __int64 getUser(void);
	virtual __int64 getUser(void) const;
	virtual __int64 getGameRenderer(void) const;
	virtual __int64 getHolosceneRenderer(void) const;
	virtual __int64 getLevelRenderer(void) const;
	virtual __int64 getLevelRendererCameraProxy(void) const;

public:
	virtual void* getCameraManager(void) const;					//virtual C_CameraManager* getCameraManager(void) const;

private:
	virtual __int64 sub_1400CCC08(void) const;

public:
	virtual __int64 getLightTexture(void);

public:
	virtual void setupLevelRendering(__int64&, Actor&);

private:
	virtual __int64 getViewportInfo(void) const;

public:
	virtual void setViewportInfo(__int64 const&);

private:
	virtual __int64 getNormalizedViewportSize(void) const;
	virtual __int64 updateChunkRadius(void);

public:
	virtual void setUITexture(__int64*);

private:
	virtual __int64 getUITexture(void);

public:
	virtual void setLevelTexture(__int64*);

private:
	virtual __int64 getLevelTexture(void);

public:
	virtual void setUICursorTexture(__int64);

	virtual __int64 getUICursorTexture(void) const;
	virtual __int64 getCamera(void);
	virtual __int64 getShaderColor(void);
	virtual __int64 getDarkShaderColor(void);
	virtual __int64 clearGraphicsCache(void);
	virtual __int64 getNormalizedUICursorTransform(__int64&, float);
	virtual __int64 shouldRenderUICursor(void) const;
	virtual bool sub_1400CD3B0(void);
	virtual bool sub_1400CD3B1(void);
	virtual __int64 getGuiScale(void) const;
	virtual __int64 getGuiScaleOption(void) const;
	virtual __int64 getGuiScaleOffset(void) const;

public:
	virtual void setGuiScaleOffset(int);

private:
	virtual __int64 renderEditorGui(__int64&, bool);

public:
	virtual void* getGuiDataDOESNTWORK(void);						//virtual C_GuiData* getGuiDataDOESNTWORK(void);
	virtual void* getGuiDataDOESNTWORK(void) const;				//virtual C_GuiData* getGuiDataDOESNTWORK(void) const;

private:
	virtual __int64 getGuidedFlowManager(void);
	virtual __int64 getDpadScale(void) const;
	virtual __int64 getDateManager(void) const;
	virtual __int64 addOverrideHoursToDateTime(unsigned int);
	virtual __int64 getToastManager(void);
	virtual __int64 getTopScene(void);
	virtual __int64 getTopScene(void) const;
	virtual __int64 getActiveScene(void);
	virtual __int64 getActiveScene(void) const;
	virtual __int64 getSceneFactory(void) const;
	virtual __int64 getClientSceneStack(void) const;
	virtual __int64 getClientSceneStack(void);
	virtual __int64 sub_140634B30(void) const;
	virtual __int64 getMainSceneStack(void);
	virtual __int64 getMainSceneStack(void) const;
	virtual __int64 getCurrentSceneStack(void) const;
	virtual __int64 getCurrentSceneStack(void);
	virtual __int64 getCurrentUIRouter(void);
	virtual __int64 getCurrentUIRouter(void) const;
	virtual __int64 sub_14012BAB0(void) const;
	virtual __int64 prepareSceneFor(__int64);
	virtual __int64 getCachedScenes(void);

public:
	virtual TextHolder getScreenName(void) const;

private:
	virtual __int64 getScreenTelemetry(void) const;
	virtual __int64 getTopSceneType(void) const;
	virtual __int64 getMobEffectsLayout(void);
	virtual __int64 newFunc2(__int64 a2, __int64 a3, char a4);
	virtual __int64 newFunc3(__int64 a2, __int64 a3);
	virtual __int64 onMobEffectsChange(void);

public:
	virtual void setUISizeAndScale(int, int, float);

private:
	virtual __int64 forEachVisibleScreen(__int64, bool);
	virtual __int64 forEachScreen(__int64, bool);
	virtual __int64 forEachScreenConst(__int64, bool) const;
	virtual __int64 updateSceneStack(void);
	virtual __int64 forEachAlwaysAcceptInputScreen(__int64);
	virtual __int64 forEachAlwaysAcceptInputScreenWithTop(__int64);
	virtual __int64 showC_PlayerProfile(std::string const&, __int64);
	virtual __int64 getCurrentInputMode(void) const;

public:
	virtual bool isTouchGameplayAllowed(void) const;

private:
	virtual __int64 getMinecraftInput(void) const;

public:
	virtual void setHoloInput(__int64);

private:
	virtual __int64 getHoloInput(void) const;
	virtual __int64 getVoiceSystem(void) const;
	virtual __int64 getKeyboardManager(void);

public:
	virtual void setVoiceSystem(__int64);
	virtual void setLastPointerLocation(float, float, float);

private:
	virtual __int64 getLastPointerLocation(void);
	virtual __int64 shouldUseLastPointerLocationOnFocusChange(void);
	virtual __int64 adjustGazeCursorByMouse(float, float);
	virtual __int64 currentScreenShouldStealMouse(void);
	virtual __int64 getInProgressBAI(void) const;
	virtual __int64 newDictationDataAvailable(void) const;
	virtual __int64 clearDictationDataAvailable(void);
	virtual __int64 getDictationText(void) const;
	virtual __int64 getPacketSender(void);
	virtual __int64 getClientNetworkSystem(void);
	virtual __int64 getClientNetworkSystem(void) const;
	virtual void fake_function_to_make_grabmouse_work(void) const;

public:
	virtual void setMoveTurnInput(__int64);
	virtual void* getMoveTurnInput(void);						//virtual C_MoveInputHandler* getMoveTurnInput(void);

public:
	virtual void setupPersistentControls(__int64);

private:
	virtual __int64 resetC_PlayerMovement(void);
	virtual __int64 onClientInputInitComplete(void);

public:
	virtual void setClientInputHandler(__int64);

private:
	virtual __int64 getInput(void) const;
	virtual __int64 getControllerId(void) const;

public:
	virtual bool hasConnectedController(void) const;

private:
	virtual __int64 getClientSubId(void) const;

public:
	virtual void setSuspendInput_VT(bool);												// 304
	virtual void setDisableInput_VT(bool);												// 305

private:
	//virtual void subFunction(void) const; // void* xx(bool)

public:
	virtual void grabMouse_VT(void);													//this 306
	virtual void releaseMouse_VT(void);	
	virtual void refocusMouse_VT(void);													// 308

private:
	virtual __int64 resetBai(int);
	virtual __int64 clearInProgressBAI(void);
	virtual __int64 tickBuildAction(void);
	virtual __int64 getSoundEngine(void) const;
	virtual __int64 play(std::string const&, vec3_t const&, float, float);
	virtual __int64 playUI(std::string const&, float, float);
	virtual __int64 muteAudio(void);
	virtual __int64 unMuteAudio(void);
	virtual __int64 fadeOutMusic(void) const;
	virtual __int64 getTaskGroup(void);
	virtual __int64 onFullVanillaPackOnStack(void);

public:
	virtual bool isFullVanillaPackOnStack(void) const;

private:
	virtual __int64 onPlayerLoaded(Player&);

public:
	virtual void setClientGameMode(__int64);//322?

private:
	virtual __int64 resetToDefaultGameMode(void);
	virtual __int64 connectToThirdParyServer(std::string const&, std::string const&, int);
	virtual __int64 startExternalNetworkWorld(__int64, std::string const&, bool);
	virtual __int64 checkForPiracy(void);
	virtual __int64 updateChatFilterStatus(void);
	virtual void sub_1400D2A20(void);

public:
	virtual void setBehaviorCommandCallback(__int64);
	virtual void setBehaviorCommandStatus(std::string, __int64);

private:
	virtual __int64 requestScreenshot(__int64&);
	virtual void sub_1400BE0A0(char a2);
	virtual __int64 getDevConsoleLogger(void) const;
	virtual __int64 requestImageFromUrl(std::string const&, __int64);
	virtual __int64 initializeRenderResources(void);
	virtual __int64 postInitRenderResources(void);
	virtual __int64 onAppSuspended(void);
	virtual __int64 onActiveResourcePacksChanged(__int64 const&);
	virtual __int64 reloadEntityRenderers(__int64 const&);
	virtual __int64 getBlockTessellator(void);
	virtual __int64 getBlockEntityRenderDispatcher(void);
	virtual __int64 getEntityRenderDispatcher(void);
	virtual __int64 getEntityBlockRenderer(void);
	virtual __int64 getItemInHandRenderer(void);
	virtual __int64 getItemRenderer(void);
	virtual __int64 getSentMessageHistory(void);
	virtual __int64 getUIProfanityContext(void) const;
	virtual __int64 initTTSClient(__int64&);
	virtual __int64 getTTSClient(void);
	virtual __int64 getTTSClient(void) const;
	virtual __int64 getTTSEventManager(void);
	virtual __int64 addTTSMessage(std::string const&, __int64, bool, bool, bool);
	virtual __int64 initCommands(void);
	virtual __int64 getUserId(void) const;
	virtual __int64 getServerConnectionTime(void) const;
	virtual __int64 getHMDState(void);
	virtual __int64 getHMDState(void) const;

public:
	virtual void setServerPingTime(unsigned int);

private:
	virtual __int64 getServerPingTime(void) const;

public:
	virtual void setDefaultPlayscreenTab(__int64);
	virtual void setClientInstanceState(__int64 const&);
	virtual void setUIEventCoordinator(__int64&&);

private:
	virtual __int64 getUIEventCoordinator(void);
	virtual __int64 getEventCoordinator(void);
	virtual __int64 computeScreenCoordsFromScreenNormCoords(float, float, short&, short&);
	virtual __int64 getNoBlockBreakUntil(void);

public:
	virtual void setNoBlockBreakUntil(__int64);
	virtual void setDictation(std::string const&);
	virtual void setNewDictationString(bool);
	virtual void setGameModule(__int64);

private:
	virtual __int64 getGameModule(void);
	virtual __int64 getGameCallbacks(void);
	virtual __int64 getStorageAreaStateListener(void);
	virtual __int64 getScriptEngine(void);
	virtual __int64 sendClientEnteredLevel(void);
	virtual __int64 getHitDetectSystem(void);

public:
	virtual bool isPlaying(void) const;

private:
	virtual __int64 getLatencyGraphDisplay(void) const;
	virtual __int64 createSkin(void);
	virtual __int64 onExtendDiskSpace(bool, __int64&, unsigned long long const&, __int64);
	virtual __int64 onLowDiskSpace(bool);
	virtual __int64 onOutOfDiskSpace(bool);
	virtual __int64 onCriticalDiskError(bool, __int64 const&);
	virtual __int64 onLevelCorrupt(void);
	virtual __int64 onGameModeChanged(void);
	virtual __int64 onTick(int, int);
	virtual __int64 onInternetUpdate(void);
	virtual __int64 onGameSessionReset(void);
	virtual __int64 onLevelExit(void);
	virtual __int64 updateScreens(void);
*/

public:

	static int getMinecraftGameOffset;

	// 这里的 0x330 有点特殊 Wiki 待补充
	struct glmatrixf* getGlmatrixf() {
		return (glmatrixf*)(((uintptr_t)this) + 0x330);
	}

	class LocalPlayer* getCILocalPlayer() {
		//这个特征码用于定位虚表偏移+3->int
		//48 8B 80 ? ? ? ? FF 15 ? ? ? ? 48 8B F8 48 85 ? 0F 84 ? ? ? ? 8B
		return Utils::CallVFunc<27, class LocalPlayer*>(this);
	}

	// 这个方法的作用就是获取FOV所在的结构地址
	// 怎么定位这个方法的特征码：首先找到下面fov的X, 然后找写入(或者找访问，写入只有一条，访问有两条, 其中有一条是和写入相同的)
	// 进入汇编 发现 mov [r9+00000148],eax , 这里r9+148的值就是 FOV X的指针, 这里的r9就是我们要获取的地址,也就是这个函数的返回值
	// 往上找 我们看r9是怎么来的, mov r9, rax  , 这里的r9是rax复制过来的 , 而再上面有个call， rax就是这个call的返回值
	// 再往上面我们可以看到 mov rax,[rax+00000B40] ,可以判断这个 call就是 CI的虚表函数, 且是第(B40 / 8)个虚表函数
	// 为什么笃定是CI的虚表函数呢, 通过断点可以知道 这里赋值前的rax就是CI的虚表由此判断, 所以我们要通过特征码获取这个 B40
	void* getFovSubStructure() {
		//48 8B 80 ? ? ? ? FF 15 ? ? ? ? 4C 8B C8 48 8D 8E
		static int offset = 0;
		if (!offset) {
			auto memcodePos = FindSignature("48 8B 80 ? ? ? ? FF 15 ? ? ? ? 4C 8B C8 48 8B 8F");
			if (memcodePos) {
				offset = *reinterpret_cast<int*>(memcodePos + 3);//BC0
			}
			else {
				logF("[%s::%s] 特征码查找失败", "ClientInstance", "getFovSubStructure");
				throw "ClientInstance::getFovSubStructure 特征码查找失败1";
			}
		}
		using Fn = void*(__thiscall*)(ClientInstance*);
		return (*reinterpret_cast<Fn*>(*(uintptr_t*)this + offset))(this);
	}


	// 这个fov找法和鼠标坐标什么的差不多,它的位置大概是鼠标坐标地址后面一点, 特点是只有在窗口大小变化, 
	// 从 CI到 FOV确实有一个固定偏移 0x6D0
	// 但无法从特征码中获取这个偏移, 游戏中Fov获取的原理是, 首先由CI调用一个虚表函数(也就是上面获取的函数),
	// 然后这个函数的返回值是一个结构, 从这个结构到fov可以在特征码中定位, 这个偏移就是我们先要获取的
	// 这个结构我们可以通过上面的函数获取, 而返回值到fov的偏移就是这里要获取的
	vec2_t getFov() {
		//float x = *(float*)(((uintptr_t)this) + 0x6D0);
		//float y = *(float*)(((uintptr_t)this) + 0x6E4);
		//return vec2_t(x, y);

		//4C 8D 80 ? ? ? ? 49 8B D0 48 8D 85 ? ? ? ? 48 2B D0 0F 1F
		static int offset = 0;
		if (!offset) {
			auto memcodePos = FindSignature("89 88 ? ? ? ? 41 8B 40 04 41 89 81 ? ? ? ? 41 8B 40 08 41");	//1.20.31
			if (memcodePos) {
				offset = *reinterpret_cast<int*>(memcodePos + 2);
			}
			else {
				logF("[%s::%s] 特征码查找失败", "ClientInstance", "getFov");
				throw "ClientInstance::getFovSubStructure 特征码查找失败2";
			}
		}
		uintptr_t fovstruct = (uintptr_t)getFovSubStructure();
		float x = *(float*)(fovstruct + offset);
		float y = *(float*)(fovstruct + offset + 0x14);
		return vec2_t(x, y);
	}

	// 检查版本 1.20.12
	// 0x498 // 这个偏移好找,先找到CI,然后看内存，显示单浮点，打开背包，动动鼠标，看看哪个数变了
	vec2_t* getMousepos() {
		static int offset = 0;
		if (!offset) {
			auto memcodePos = FindSignature("F3 0F 11 89 ? ? ? ? F3 0F 11 91 ? ? ? ? F3 0F 11 99 ? ? ? ? C3");
			if (memcodePos) {
				offset = *reinterpret_cast<int*>(memcodePos + 4);
			}
			else {
				logF("[%s::%s] 特征码查找失败", "ClientInstance", "getMousepos");
				throw "ClientInstance::getMousepos 特征码查找失败";
			}
		}
		return (vec2_t*)(((uintptr_t)this) + offset);
	}

	//MinecraftGame 是 Clientinstance::Clientinstance 的第二个参数 所以在构造函数里可以看到偏移 //在 1.20.41 中偏移是200byte
	//或者在 Clientinstance::onDimensionChanged 函数中内部调用的函数就是MinecraftGame的虚表，在其中可以找到Clientinstance 到MinecraftGame 的偏移
	//Minecraft.Windows.exe + 38CF90 - 48 8B 89 C8000000 - mov rcx, [rcx + 000000C8]
	//Minecraft.Windows.exe + 38CF97 - 48 8B 01 - mov rax, [rcx]
	//Minecraft.Windows.exe + 38CF9A - 48 8B 80 28030000 - mov rax, [rax + 00000328]
	//Minecraft.Windows.exe + 38CFA1 - 48 FF 25 C05C3904 - jmp qword ptr[Minecraft.Windows.exe + 4722C68]
	// 当前此偏移 有Hook 在HookonDimensionChanged 之前获取
	class MinecraftGame* getMinecraftGame() {
		return *(MinecraftGame**)((uintptr_t)this + getMinecraftGameOffset);
	}


	class LevelRenderer* getLevelRender();


	auto getTopScreenName() -> std::mcstring {
		INT64 str[4]{};
		std::mcstring* out = (std::mcstring*) & str;
		using GetTopScreenName = void(__thiscall*)(ClientInstance*, std::mcstring*);
		auto _GetTopScreenName = (GetTopScreenName)((*(uintptr_t**)this)[142] /*->VTable[141]*/);
		_GetTopScreenName(this, out);
		return *out;
	}

	auto setSuspendInput(bool v) -> void* {
		return Utils::CallVFunc<323, void*, bool>(this, v);
	}

	auto setDisableInput(bool v) -> void* {
		return Utils::CallVFunc<324, void*,bool>(this, v);
	}

	auto grabMouse(void) -> void {
		// 特征码定位虚表偏移 +3 int
		//48 8B 80 ? ? ? ? FF 15 ? ? ? ? 90 48 85 ? 74 ? 48 8B ? E8 ? ? ? ? 48 8B 8F
		Utils::CallVFunc<325, void>(this);	/*1.20.41*/
	};
	auto releaseMouse(void) -> void {
		//48 8B 80 ? ? ? ? 48 8B CE FF 15 ? ? ? ? 84
		Utils::CallVFunc<326, void>(this);	/*1.20.41*/
	};
	auto refocusMouse(void) -> void {
		Utils::CallVFunc<327, void>(this);	/*1.20.41*/
	};
};