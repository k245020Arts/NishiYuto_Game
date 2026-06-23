#pragma once
#include  <vector>
#include <string>


class CsvReader
{
public:
	CsvReader();
	~CsvReader();
	/// <summary>
	/// データを読み込む
	/// </summary>
	/// <param name="_filename"></param>
	/// <returns></returns>
	bool Load(std::string _filename);
	/// <summary>
	/// 全データを一括で書き込む（上書き）
	/// </summary>
	/// <param name="_filename"></param>
	/// <param name="all"></param>
	void SaveAll(std::string _filename,const std::vector<std::vector<std::string>>& all);
	/// <summary>
	/// 一ラインを書き換える
	/// </summary>
	/// <param name="_filename"></param>
	/// <param name="targetLine"></param>
	/// <param name="newLine"></param>
	void SaveLine(std::string _filename, int targetLine, const std::vector<std::string>& newLine);
	/// <summary>
	/// セル一箇所だけ書き換える
	/// </summary>
	/// <param name="_filename"></param>
	/// <param name="line"></param>
	/// <param name="column"></param>
	/// <param name="newValue"></param>
	void SaveCell(std::string _filename, int line, int column, const std::string& newValue);
	
	/// <summary>
	/// 全てのセルを1種類の文字で埋める
	/// </summary>
	/// <param name="_filename"></param>
	/// <param name="_all"></param>
	void ResetAll(std::string _filename, const std::string& _allText);
	/// <summary>
	/// 行の数を取る
	/// </summary>
	/// <returns>csvで読み込んだ行の数</returns>
	int GetLines();
	/// <summary>
	/// 指定した行の列数を取得
	/// </summary>
	/// <param name="line">列数を取得する対象の行番号（整数）</param>
	/// <returns>指定した行の列数を表す整数</returns>
	int GetColums(int line);
	/// <summary>
	/// 指定した行と列に対応するintの値を取得
	/// </summary>
	/// <param name="line">対象の行番号</param>
	/// <param name="column">対象の行番号</param>
	/// <returns>指定した行と列にあるintの値</returns>
	int GetInt(int line, int column);
	/// <summary>
	/// 指定した行と列に対応するfloatの値を取得
	/// </summary>
	/// <param name="line">対象の行番号</param>
	/// <param name="column">対象の行番号</param>
	/// <returns>指定した行と列にあるfloatの値</returns>
	float GetFloat(int line, int column);
	/// <summary>
	/// 指定した行番号と列番号に対応する文字列の取得
	/// </summary>
	/// <param name="line">取得対象の行番号</param>
	/// <param name="colum">取得対象の列番号</param>
	/// <returns>指定した位置にある文字列を取得</returns>
	std::string GetString(int line, int colum);
private:
	std::vector<std::vector<std::string>> record;
	
};