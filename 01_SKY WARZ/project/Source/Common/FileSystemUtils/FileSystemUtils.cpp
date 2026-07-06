#include "FileSystemUtils.h"
#include <filesystem>
#include <fstream>
#include <assert.h>
#include <list>

int FileSystemUtils::GetDirectoryAndFileCount(const std::string_view& _filePath)
{
	return GetEntryCount(_filePath, ENTRY_TYPE::ALL);
}

int FileSystemUtils::GetDirectoryCount(const std::string_view& _filePath)
{
	return GetEntryCount(_filePath, ENTRY_TYPE::DIRECTORY);
}

int FileSystemUtils::GetFileCount(const std::string_view& _filePath)
{
	return GetEntryCount(_filePath, ENTRY_TYPE::FILE);
}

int FileSystemUtils::GetEntryCount(const std::string_view& _filePath, const ENTRY_TYPE _type)
{
	if (!std::filesystem::exists(_filePath))
		assert(false && "ディレクトリが見つかりませんでした");

	int entryCount = 0;    // エントリのカウント

	std::error_code errorCode;												// 下記のディレクトリパスの先頭要素を指すイテレータ構築時に、エラー判定をする際に使用 valueが0でエラーなし:0以外でエラーあり
	std::filesystem::directory_iterator directoryItr(_filePath, errorCode);	// ディレクトリパスの先頭要素を指すイテレータを構築

	assert(!errorCode && "ディレクトリパスの先頭要素を指すイテレータ構築に失敗しました。不正なディレクトリパスの可能性があります。");

	// _filePathディレクトリ内のエントリを列挙する
	for (const std::filesystem::directory_entry& directoryEntry : directoryItr)
	{
		// 全てのエントリをカウントするタイプだったら
		if (_type == ENTRY_TYPE::ALL)
		{
			// エントリ数をカウント
			entryCount++;
			continue;	// エントリ数をカウントしたのでcontinue
		}

		// ディレクトリをカウントするタイプだったら
		if (_type == ENTRY_TYPE::DIRECTORY)
		{
			// ディレクトリだったら
			if (directoryEntry.is_directory())
			{
				// エントリ数をカウント
				entryCount++;
				continue;	// エントリ数をカウントしたのでcontinue
			}
		}
		else
		{
			// ディレクトリでなく、ファイルだったら
			if (!directoryEntry.is_directory())
			{
				// エントリ数をカウント
				entryCount++;
				continue;	// エントリ数をカウントしたのでcontinue
			}
		}
	}
	return entryCount;
}

bool FileSystemUtils::CreateDirectoryAndFile(std::string_view _DirectoryPath, std::string_view _fileName)
{
	return CreateEntry(_DirectoryPath, _fileName, ENTRY_TYPE::ALL);
}

bool FileSystemUtils::CreateDirectoryAndFile(std::string_view _filePath)
{
	std::string directoryPath = std::string(_filePath);	// ディレクトリパス
	std::list<char> reverseFileName;					// 逆順のまま、ファイルパスの後ろから/までの文字列を保存するコンテナ	std::stringにpush_front()がないので、処理負荷軽減のため一度逆順でいいので、保存する
	std::string fileName;								// reverseFileNameの逆順のファイル名を元の順に戻した文字列　

	// ファイルパス文字列を後ろから回す
	for (auto pathItr = _filePath.rbegin();pathItr != _filePath.rend();pathItr++)
	{
		if (*pathItr == '/')
			break;

		std::string::iterator directoryPathItr = directoryPath.end() - 1;	// 最後の文字のイテレータ

		// ディレクトリパスからファイル名を削除するために、文字列の最後を削除
		directoryPathItr = directoryPath.erase(directoryPathItr);

		// 逆順のまま、ファイルパスの後ろから「 / 」までを代入
		reverseFileName.emplace_back(*pathItr);
	}
	
	// reverseFileNameで保存した逆ファイル名文字列を後ろから回す
	for (auto rFileNameItr = reverseFileName.rbegin();rFileNameItr != reverseFileName.rend();rFileNameItr++)
	{
		// 逆順から戻した、ファイルパスの後ろから「 / 」までのファイル名を代入
		fileName += *rFileNameItr;
	}

	return CreateDirectoryAndFile(directoryPath, fileName);
}

bool FileSystemUtils::CreateEntryDirectory(std::string_view _DirectoryPath)
{
	return CreateEntry(_DirectoryPath, "", ENTRY_TYPE::DIRECTORY);
}

bool FileSystemUtils::CreateEntryFile(std::string_view _filePath)
{
	return CreateEntry("", _filePath, ENTRY_TYPE::FILE);
}

// 以下、自身で追加した関数 -----------------------------------------------------------------------

std::vector<std::string> FileSystemUtils::GetFilesStartsWith(const std::string& folderPath, const std::string& prefix)
{
	std::vector<std::string> result;

	for (const auto& entry : std::filesystem::directory_iterator(folderPath))
	{
		if (entry.is_directory()) continue;

		std::string fileName = entry.path().stem().string();

		// 先頭一致チェック
		if (fileName.rfind(prefix, 0) == 0)
		{
			result.push_back(fileName);
		}
	}

	return result;
}

std::string FileSystemUtils::FindAfterAndMatch(const std::vector<std::string> str, const std::string& after, const std::string& keyword)
{
	return FindAfterAndMatch(str, after, keyword, after.length());
}

std::string FileSystemUtils::FindAfterAndMatch(const std::vector<std::string> str, const std::string& after, const std::string& keyword, size_t _cutNum)
{
	// 「after」の位置を探す
	for (auto& s : str) {
		size_t pos = s.find(after);
		if (pos == std::string::npos)
			return "";

		// 「after」の後ろを切り出す
		std::string afterStr = s.substr(pos);

		// keywordが含まれているか
		if (afterStr.find(keyword) != std::string::npos)
		{
			return afterStr; // マッチしたので返す
		}
	}

	return "";
}

std::string FileSystemUtils::FindAfterAndMatchFileName(const std::vector<std::string> str, const std::string& after, const std::string& keyword)
{
	// 「after」の位置を探す
	for (auto& s : str) {
		size_t pos = s.find(after);
		if (pos == std::string::npos)//文字列が含まれてなかったら
			continue;

		// 「after」の後ろを切り出す
		std::string afterStr = s.substr(pos - 1);

		// keywordが含まれているか
		if (afterStr.find(keyword) != std::string::npos)
		{
			return s; // マッチしたので返す
		}
	}

	return "";
}

// -------------------------------------------------------------------------------------------

bool FileSystemUtils::CreateEntry(std::string_view _DirectoryPath, std::string_view _fileName, const ENTRY_TYPE _type)
{
	// ファイルを作成するタイプでなかったら
	if (_type != ENTRY_TYPE::FILE)
	{
		try
		{
			// ディレクトリの作成
			std::filesystem::create_directories(_DirectoryPath);
		}
		catch (std::filesystem::filesystem_error)
		{
			assert(false && "ディレクトリの作成に失敗しました。");
		}
		
		// ディレクトリを作成するタイプだったら
		if (_type == ENTRY_TYPE::DIRECTORY)
		{
			return true;
		}
	}
	std::string filePathAndFileName = std::string(_DirectoryPath) + std::string(_fileName);	// ファイル作成のためのファイルパス + ファイル名

	// ファイルの作成
	std::ofstream ofs(filePathAndFileName);

	return (bool)ofs;
}

