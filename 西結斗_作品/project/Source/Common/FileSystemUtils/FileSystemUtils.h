#pragma once
#include <iostream>
#include <vector>

/// <summary>
/// ファイル、ディレクトリの操作をする
/// </summary>

//君嶋作
class FileSystemUtils
{
public:
	
	/// <summary>
	///								フォルダパス内のフォルダとファイルの総数を返す
	/// </summary>
	/// <param name="_filePath">	フォルダパス					</param>
	/// <returns>					フォルダとファイルの総数		</returns>
	static int GetDirectoryAndFileCount(const std::string_view& _filePath);

	/// <summary>
	///								フォルダパス内のフォルダの総数を返す
	/// </summary>
	/// <param name="_filePath">	フォルダパス		</param>
	/// <returns>					フォルダの総数		</returns>
	static int GetDirectoryCount(const std::string_view& _filePath);

	/// <summary>
	///								フォルダパス内のファイルの総数を返す
	/// </summary>
	/// <param name="_filePath">	フォルダパス		</param>
	/// <returns>					ファイルの総数		</returns>
	static int GetFileCount(const std::string_view& _filePath);

	/// <summary>
	///									フォルダパスとファイルを作成する
	///									すでにフォルダパスが存在した場合でも、フォルダ内容が上書きされることはない。
	/// </summary>
	/// <param name="_DirectoryPath">	作成するフォルダパス(ディレクトリパス)	｛例) data/stage/wave/｝ [※最後の / がないとstageと同じ階層にwave.txtが作られる]	</param>
	/// <param name="_fileName">		作成するファイル名						｛例) wave.txt｝																	</param>
	/// <returns>						true:成功 / false:失敗																										</returns>
	static bool CreateDirectoryAndFile(std::string_view _DirectoryPath, std::string_view _fileName);

	/// <summary>
	///								フォルダパスとファイルを作成する
	///								すでにフォルダパスが存在した場合でも、フォルダ内容が上書きされることはない。
	/// </summary>
	/// <param name="_filePath">	作成するフォルダパス(ディレクトリパス)	｛例) data/stage/wave/wave.txt｝	</param>
	/// <returns>					true:成功 / false:失敗														</returns>
	static bool CreateDirectoryAndFile(std::string_view _filePath);

	/// <summary>
	///									フォルダパスを作成する
	///									すでにフォルダパスが存在した場合でも、フォルダ内容が上書きされることはない。
	/// </summary>
	/// <param name="_DirectoryPath">	作成するフォルダパス(ディレクトリパス)	｛例) data/stage/wave｝	</param>
	/// <returns>						true:成功 / false:失敗											</returns>
	static bool CreateEntryDirectory(std::string_view _DirectoryPath);

	/// <summary>
	///								ファイルを作成する
	/// </summary>
	/// <param name="_filePath">	作成するファイルパス		｛例) data/stage/wave/wave.txt｝	</param>
	/// <returns>					true:成功 / false:失敗											</returns>
	static bool CreateEntryFile(std::string_view _filePath);

	// 以下、自身で追加した関数 -----------------------------------------------------------------------
	
	/// <summary>
	/// 指定したフォルダからすべてのファイルを検索し、
	/// 先頭の文字列がprefixと同じならそのフォルダ名をすべて返す(拡張子なし)
	/// </summary>
	/// <param name="folderPath">フォルダパス</param>
	/// <param name="prefix">先頭の文字列</param>
	/// <returns>フォルダ名(拡張子なし)</returns>
	static std::vector<std::string> GetFilesStartsWith(const std::string& folderPath, const std::string& prefix);

	/// <summary>
	/// 指定した文字列のリストからすべての文字列を検索し、Keywardの後から最後の文字列を検索し
	/// そのafterと同じならその名前を返す
	/// </summary>
	/// <param name="str">検索する文字列のvector型</param>
	/// <param name="after">指定ワード</param>
	/// <param name="keyword">ここから検索開始の文字列</param>
	/// <returns>後の文字列だけ取得</returns>
	static std::string FindAfterAndMatch(const std::vector<std::string> str, const std::string& after, const std::string& keyword);
	static std::string FindAfterAndMatch(const std::vector<std::string> str, const std::string& after, const std::string& keyword,size_t _cutNum);
	static std::string FindAfterAndMatchFileName(const std::vector<std::string> str, const std::string& after, const std::string& keyword);

	// -------------------------------------------------------------------------------------------

private:

	/// <summary>
	/// エントリのタイプ
	/// </summary>
	enum class ENTRY_TYPE
	{
		ALL			= 0,	// 全て
		DIRECTORY,			// ディレクトリ
		FILE,				// ファイル
	};

	/// <summary>
	///							指定されたタイプのフォルダパス内のエントリの総数を返す
	/// </summary>
	/// <param name="_filePath">フォルダパス												</param>
	/// <param name="_type">	指定エントリタイプ 											</param>
	/// <returns>				指定されたエントリタイプのフォルダパス内のエントリの総数	</returns>
	static int GetEntryCount(const std::string_view& _filePath, const ENTRY_TYPE _type);

	/// <summary>
	///									指定されたタイプのエントリの作成を行う
	/// </summary>
	/// <param name="_DirectoryPath">	フォルダパス								</param>
	/// <param name="_fileName">		ファイル名									</param>
	/// <param name="_type">			指定エントリタイプ							</param>
	/// <returns>						true:成功 / false:失敗						</returns>
	static bool CreateEntry(std::string_view _DirectoryPath, std::string_view _fileName, const ENTRY_TYPE _type);
};

