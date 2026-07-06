#pragma once
#include <iostream>
#include <algorithm>
#include <cmath>
#include "../Component.h"
#include "../../../Library/myDxLib.h"

//colorの設定を行えるクラス。

class Color : public Component{
public:

    struct Rgb
    {
        Rgb() {
            Rgb(255.0f, 255.0f, 255.0f,255.0f);
        }
        Rgb(float _r, float _g, float _b,float _a) {
            r = _r;
            g = _g;
            b = _b;
            a = _a;
        }
        float r, g, b,a;
    };

    struct Hsv
    {
        Hsv() {
            Hsv(360.0f, 1.0f, 1.0f);
        }
        Hsv(float _h, float _s, float _v) {
            h = _h;
            s = _s;
            v = _v;
        }
        float h, s, v;
    };

    Color() {
        rgb.r = 255.0f;
        rgb.b = 255.0f;
        rgb.g = 255.0f;
        rgb.a = 255.0f;

        hsv.h = 360.0f;
        hsv.s = 1.0f;
        hsv.v = 1.0f;
        changeMode = 0;
        debugId = 4;
        colorfRgb = ChangeColorF();
        tag = Function::GetClassNameC<Color>();
    }

    Color(float r, float g, float b,float a, float h, float s, float v) {
        setRGB(Rgb(r, g, b, a));
        setHSV(Hsv(h,s,v));
        changeMode = 0;
        debugId = 4;
        tag = Function::GetClassNameC<Color>();
    }

    ~Color() {

    }

    void Update()override {

    }
    /// <summary>
    /// RGB情報をセットする
    /// </summary>
    /// <param name="_rgb">RGBのセット</param>
    void setRGB(Rgb _rgb) {
        rgb.r = std::clamp(_rgb.r, 0.0f, 255.0f);
        rgb.g = std::clamp(_rgb.g, 0.0f, 255.0f);
        rgb.b = std::clamp(_rgb.b, 0.0f, 255.0f);
        rgb.a = std::clamp(_rgb.a, 0.0f, 255.0f);
        colorfRgb = ChangeColorF();
    }

    // HSVセット
    // Hは0-360、S,Vは0-1で指定
    void setHSV(Hsv _hsv) {
        hsv.h = fmod(_hsv.h, 360.0f);
        if (hsv.h < 0) hsv.h += 360;
        hsv.s = std::clamp(_hsv.s, 0.0f, 1.0f);
        hsv.v = std::clamp(_hsv.v, 0.0f, 1.0f);
    }

    Rgb GetRgb() const { return rgb; }

    COLOR_F GetColorF() {
        return colorfRgb;
    }

    //rgbをCOLOR_F型に変換できる。
    COLOR_F ChangeColorF() {
        COLOR_F color;
        color.r = (rgb.r) / 255.0f;
        color.g = (rgb.g) / 255.0f;
        color.b = (rgb.b) / 255.0f;
        color.a = (rgb.a) / 255.0f;
        return color;
    }

    Hsv GetHsv()const { return hsv; }

    void ImguiDraw()override;

private:

    Rgb rgb;
    COLOR_F colorfRgb;
    Hsv hsv;
    int changeMode;

    void ColorFToRGB() {
        Rgb rgb;
        rgb.a = colorfRgb.a * 255.0f;
        rgb.g = colorfRgb.g * 255.0f;
        rgb.b = colorfRgb.b * 255.0f;
        rgb.r = colorfRgb.r * 255.0f;
    }
};
//-----------------------色の定数----------------------

const Color::Rgb WHITE = Color::Rgb(255, 255, 255, 255);
const Color::Rgb RED = Color::Rgb(255, 0, 0, 255);
const Color::Rgb GREEN = Color::Rgb(0, 255, 0, 255);
const Color::Rgb BLUE = Color::Rgb(0, 0, 255, 255);
const Color::Rgb YELLOW = Color::Rgb(255, 255, 0, 255);
const Color::Rgb MAGENTA = Color::Rgb(255, 0, 255, 255);
const Color::Rgb CYAN = Color::Rgb(0, 255, 255, 255);
const Color::Rgb BLACK = Color::Rgb(0, 0, 0, 255);
const Color::Rgb SAKURA = Color::Rgb(253, 238, 239, 255);
const Color::Rgb ORANGE = Color::Rgb(255, 165, 0, 255);
const Color::Rgb PURPLE = Color::Rgb(128, 0, 128, 255);
const Color::Rgb PINK = Color::Rgb(255, 192, 203, 255);
const Color::Rgb BROWN = Color::Rgb(165, 42, 42, 255);
const Color::Rgb GOLD = Color::Rgb(255, 215, 0, 255);
const Color::Rgb SILVER = Color::Rgb(192, 192, 192, 255);
const Color::Rgb SKY_BLUE = Color::Rgb(135, 206, 235, 255);
const Color::Rgb DARK_BLUE = Color::Rgb(0, 0, 139, 255);
const Color::Rgb LIGHT_BLUE = Color::Rgb(173, 216, 230, 255);
const Color::Rgb DARK_RED = Color::Rgb(139, 0, 0, 255);
const Color::Rgb LIGHT_GREEN = Color::Rgb(144, 238, 144, 255);
const Color::Rgb DARK_GREEN = Color::Rgb(0, 100, 0, 255);
const Color::Rgb GRAY = Color::Rgb(128, 128, 128, 255);
const Color::Rgb DARK_GRAY = Color::Rgb(64, 64, 64, 255);
const Color::Rgb LIGHT_GRAY = Color::Rgb(192, 192, 192, 255);

//-----------------------------------------------------------

//透明と不透明で使う定数(DxLibのColorF型verと通常で使うverの2つ用意)

static const int OPAQUE_COLOR = 255; //不透明
static const int TRANSPARENT_COLOR = 0; //透明

static const float OPAQUE_COLOR_F = 1.0f; //不透明
static const float TRANSPARENT_COLOR_F = 0.0f; //透明