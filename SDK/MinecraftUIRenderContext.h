#pragma once
#include <string>

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
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
};

class TextMeasureData;
class CaretMeasureData;


class MinecraftUIRenderContext {
private:
	virtual void Destructor();
public:
	virtual float getLinelength(class BitmapFont&, std::string const&, float, bool);
private:
	virtual int getTextAlpha();
	virtual void setTextAlpha();
public:
	virtual void drawDebugText(RectangleArea const&, std::string const&, UIColor const&, float, float, TextMeasureData const&, CaretMeasureData const&);
	virtual void drawText(class BitmapFont&, RectangleArea const&, std::string const&, UIColor const&, float, float, TextMeasureData const&, CaretMeasureData const&);
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
};