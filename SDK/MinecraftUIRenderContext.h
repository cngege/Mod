#pragma once
#include <string>
#include "..\Mod\Utils\HMath.h"
#include "..\Mod\Utils\Game.h"
#include "ClientInstance.h"
#include "TextHolder.h"


/* 如何虚函数出错,就是去ida跑安卓的MC去对新的虚函数 */
/* 按照视频作者的说法，这个虚函数从一开始就没有变过 */

struct RectangleArea {
public:
	float w, x, y, z;

	RectangleArea(float w, float x, float y, float z) {
		this->w = w;
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

struct UIColor {
public:
	float r, g, b, a;
	UIColor(float r = 255, float g = 255, float b = 255, float a = 255) {
		this->r = r / 255.f;
		this->g = g / 255.f;
		this->b = b / 255.f;
		this->a = a / 255.f;
	}
};

struct TextMeasureData {
	float textSize;
	int idk;
	bool displayShadow;
	bool bool2;
};

class BitmapFont;
class CaretMeasureData;

using MUICDrawText = void(__fastcall*)(class MinecraftUIRenderContext*, BitmapFont*, RectangleArea const&, TextHolder* , UIColor const&, float, float, TextMeasureData*, uintptr_t*);
//MUICDrawText drawtextCall;

class MinecraftUIRenderContext {
private:
	virtual void Destructor();
public:
	virtual float getLinelength(BitmapFont&, std::string const&, float, bool);
private:
	virtual int getTextAlpha();
	virtual void setTextAlpha();
public:
	virtual void drawDebugText(RectangleArea const&, std::string const&, UIColor const&, float, float, TextMeasureData const&, CaretMeasureData const&);
	virtual void drawText(BitmapFont&, RectangleArea const&, std::string const&, UIColor const&, float, float, TextMeasureData const&, CaretMeasureData const&);
	virtual void flushText(float);
private:
	virtual void drawImage();
	virtual void drawNineslice();
	virtual void flushImage();
	virtual void beginSharedMeshBatch();
	virtual void endSharedMeshBatch();
public:
	virtual void drawRectangle(RectangleArea const&, UIColor const&, float, int);
	virtual void fillRectangle(RectangleArea const&, UIColor const&, float);


	static uintptr_t* drawtextCall;

	//非官方 自定义函数
public:

	/// <summary>
	/// 填充矩形
	/// </summary>
	/// <param name="pos">位置</param>
	/// <param name="size">大小</param>
	/// <param name="color">颜色</param>
	void Fillshape(struct vec2_t, struct vec2_t, UIColor);
	/// <summary>
	/// 画空心矩形
	/// </summary>
	/// <param name="pos">位置(左上)</param>
	/// <param name="size">大小</param>
	/// <param name="color">颜色</param>
	/// <param name="w">边框宽度</param>
	void Drawshape(struct vec2_t pos, struct vec2_t size, UIColor, float w);

	void Drawtext(const vec2_t& pos, std::string* textStr, const UIColor& color, float textSize, float alpha) {
		
		static uintptr_t caretMeasureData = 0xFFFFFFFF;
		if (!Game::mcfont)
		{
			return;
		}
		TextHolder text(*textStr);
		RectangleArea rect(pos.x, pos.x + 1000, pos.y - 1, pos.y + 1000);

		TextMeasureData textMeasure{};
		memset(&textMeasure, 0, sizeof(TextMeasureData));
		textMeasure.textSize = textSize;

		reinterpret_cast<MUICDrawText>(drawtextCall)(this, Game::mcfont, rect, &text, color, alpha, 0, &textMeasure, &caretMeasureData);
	}
};