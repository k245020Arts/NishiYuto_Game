#pragma once
#include "../../../Library/GameObject.h"

class Boss;

class BossCreater : public GameObject
{
public:

	
	BossCreater();
	~BossCreater();

	/// <summary>
	/// ボスの生成
	/// </summary>
	void CreateBoss();

private:

	std::string bossID;
	std::string	animID;
	std::vector<std::string> bossAnimFileName;
	bool LoadBossParam(std::string _fileName);

	Boss* bossComponentPtr;//関数ポインタ実行用

	/*std::vector<std::string> GetFilesStartsWith(const std::string& folderPath, const std::string& prefix);
	std::string FindAfterAndMatch(const std::vector<std::string> str, const std::string& after, const std::string& keyword);*/
	
};
