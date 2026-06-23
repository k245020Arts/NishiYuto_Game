#pragma once
#include <random>
#include <vector>

//std::randomを使用して乱数を生成するヘッダー
namespace Random {

	/// <summary>
	/// 任意でシードを指定する関数
	/// </summary>
	/// <param name="seed">指定するシード、指定しないならランダム</param>
	void Init(unsigned int seed = std::random_device{}());

    //基本分布

    /// <summary>
    /// 一様分布の乱数を生成...
    /// </summary>
    /// <returns>0.0から1.0の範囲で値をランダムに返す</returns>
    double GetReal();      

    /// <summary>
    /// // 整数の乱数を生成
    /// </summary>
    /// <param name="min">最小の値</param>
    /// <param name="max">最大の値</param>
    /// <returns>minからmaxの範囲で値をランダムに返す</returns>
    int GetInt(int min, int max); 
    /// <summary>
    /// // 浮動小数点型の乱数を生成.
    /// </summary>
    /// <param name="min">最小の値</param>
    /// <param name="max">最大の値</param>
    /// <returns>minからmaxの範囲で値をランダムに返す</returns>
    float GetFloat(float min, float max);

    /// <summary>
    /// // 重み付き選択
    /// </summary>
    /// <param name="weights">各要素の重みを書いてあるstd::vector</param>
    /// <returns>確率に応じてインデックスを返す。</returns>
    int GetWeightedIndex(const std::vector<double>& weights);

    // ===== 追加分布 =====▼
  
    /// <summary>
    /// 正規分布を生成
    /// </summary>
    /// <param name="mean">平均値</param>
    /// <param name="stddev">標準偏差</param>
    /// <returns>平均と標準偏差を、指定して乱数を生成</returns>
    double GetNormal(double mean, double stddev);

    /// <summary>
    /// ベルヌーイ分布
    /// </summary>
    /// <param name="p">0～1の割合</param>
    /// <returns>確率pでtrueを返す。0.3なら30%の確率でtrueを返す</returns>
    bool GetBernoulli(double p);       

    /// <summary>
    /// 指数分布
    /// </summary>
    /// <param name="lambda">平均値</param>
    /// <returns>乱数</returns>
    double GetExponential(double lambda);           

	/// <summary>
	/// 現在のシードを獲得
	/// </summary>
	/// <returns>現在のシード値</returns>
	unsigned int GetSeed();// 現在のシード値を取得
};