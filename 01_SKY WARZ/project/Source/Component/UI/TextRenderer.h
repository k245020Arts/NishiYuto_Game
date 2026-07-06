#pragma once
#include "UIBase.h"
#include "../Color/Color.h"

class Shaker;

class TextRenderer : public Component
{
public:
	TextRenderer();
	~TextRenderer();

	void Update()override;
	void Draw()override;

	/// <summary>
	/// 描画するテキストのセッティング
	/// </summary>
	/// <param name="_text">描画するテキスト</param>
	/// <param name="_pos">座標</param>
	/// <param name="_fileName">ファイルの名前</param>
	/// <param name="_rgb">色</param>
	/// <param name="_scale">拡大率</param>
	/// <param name="_size">フォントの基礎の大きさ</param>
	void TextSetting(std::string _text, std::string _fileName, std::string _exten, const Color::Rgb& _rgb, int _size, Font_ID::Font_ID _id);

	/// <summary>
	/// 色のセット
	/// </summary>
	/// <param name="_rgba"></param>
	void SetColor(Color::Rgb _rgba) { rgb = _rgba; }
	/// <summary>
	/// αのセット
	/// </summary>
	/// <param name="_alpha"></param>
	void SetAlpha(float _alpha) { rgb.a = _alpha; }
	/// <summary>
	/// テキストのセット
	/// </summary>
	/// <param name="_text"></param>
	void SetText(std::string _text) { text = _text; }

	/// <summary>
	/// テキストの長さを取得
	/// </summary>
	/// <returns></returns>
	int GetTextWidth();

	/// <summary>
	/// このテキストをUIとして扱う
	/// UIとして扱うメリット
	/// ①カットシーンの時などに描画をしないセッティングがすぐに出来る
	/// ②UIManagerで最後にまとめて描画をするため、描画順を意識しなくても良くなる
	/// 意図としてカットシーン中にポーズ画面のテキストの表示がされないのが困るため作成
	/// </summary>
	void UseDrawUI();
	/// <summary>
	/// テキストの大きさを変える
	/// </summary>
	/// <param name="_scale"></param>
	void SetScale(const VECTOR3& _scale);
	void SetPos(const VECTOR3& _pos);

private:
	int fontHandle;
	std::string text;
	Color::Rgb rgb;
	Shaker* shaker;
	UIManager* uiManager;
};
