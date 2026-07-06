#include "Animator.h"
#include <string>
#include <assert.h>
#include "../../../Library/time.h"
#include "../../Common/ResourceLoader.h"
#include "../../../ImGui/imgui.h"
#include "../Transform/Transform.h"
#include <algorithm> 
#include "../../Common/JsonReader.h"
#include "../../Common/FileSystemUtils/FileSystemUtils.h"

Animator::Animator()
{
    fileInfos.clear();
    playSpeed       = 1.0f;
    blendTime       = 0.0f;
    blendTimeMax    = 0.0f;
    finished        = true;
    debugId         = 0;
    tag = Function::GetClassNameC<Animator>();
    animEventCan    = false;
    baseModel       = -1;
    rootNum         = -1;
}

Animator::~Animator()
{
    //AnimDataSaveAll();
    if (before.attachID >= 0)
    {
        MV1DetachAnim(baseModel, before.attachID);
    }
    if (current.attachID >= 0)
    {
        MV1DetachAnim(baseModel, current.attachID);
    }
    for (auto& inf : fileInfos)
    {
        if (inf.second.hModel > 0)
        {
            //MV1DeleteModel(inf.second.hModel);
            inf.second.hModel = -1;
        }
    }
    fileInfos.clear();
    
    //MV1DeleteModel(baseModel);
}

void Animator::BaseModelSet(int _model, int _root)
{

    baseModel =  _model;
    rootNum = _root;

}

void Animator::BaseModelSet(int _model, std::string _rootName)
{
    baseModel = _model;
    rootNum = MV1SearchFrame(_model, _rootName.c_str());
}

void Animator::Update()
{
    //一回リセットしないと位置が戻らないので戻す。
    MV1ResetFrameUserLocalMatrix(baseModel, rootNum);
    MATRIX animMatrix = MGetIdent();
    MATRIX matrix = MGetIdent();
    MATRIX beforeMatrix = MGetIdent();

    /*if (obj->GetTag() == "PLAYER") {
        int a = 0;
    }*/

    VECTOR3 beforePos = currentPosition;
    if (current.attachID >= 0)
    { // current
        const AnimFileInfo& f = fileInfos[current.fileID];
        current.beforeFrame = current.frame;
        current.frame += obj->GetObjectTimeRate() * playSpeed * f.playSpeed * 30.0f;
        if (current.frame >= f.maxFrame)
        {
            if (f.loop)
            {
                current.frame -= f.maxFrame;
            }
            else
            {
                current.frame = f.maxFrame;
                finished = true;
            }
        }
        if (fileInfos[current.fileID].eventFinishTime >= current.frame && fileInfos[current.fileID].eventStartTime <= current.frame) {
            animEventCan = true;
        }
        else {
            animEventCan = false;
        }
        if (current.boneIndex != -1) {
            currentPosition = MV1GetAttachAnimFrameLocalPosition(baseModel, current.attachID, rootNum);
            MV1SetAttachAnimTime(baseModel, current.attachID, current.frame);
            nowPosition = MV1GetAttachAnimFrameLocalPosition(baseModel, current.attachID, rootNum);
            subPosition = nowPosition - currentPosition;
        }
        else {
           MV1SetAttachAnimTime(baseModel, current.attachID, current.frame);
        }

        animMatrix = MV1GetFrameLocalMatrix(baseModel, rootNum);

        // 現在の行列を取得
        matrix = MV1GetFrameLocalMatrix(baseModel, rootNum);
        // 無補正時の座標を取得
        const VECTOR3 framePos = MV1GetAttachAnimFrameLocalPosition(baseModel, current.attachID, rootNum);

        // 座標移動を打ち消す
        matrix *= MGetTranslate(framePos * -1.0f);

            // Yだけ維持、XZを原点
        matrix *= MGetTranslate(VECTOR3(0.0f, framePos.y, 0.0f));

    }

    if (before.attachID>=0)
    { // before
        blendTime += obj->GetObjectTimeRate();
        if (blendTime >= blendTimeMax)
        {
            MV1SetAttachAnimBlendRate(baseModel, current.attachID, 1.0f);
            MV1SetAttachAnimBlendRate(baseModel, before.attachID, 0.0f);
            MV1DetachAnim(baseModel, before.attachID);
            before.attachID = -1;
            before.fileID = -1;
        }
        else {
            const AnimFileInfo& f = fileInfos[before.fileID];
            before.frame += obj->GetObjectTimeRate() * playSpeed * f.playSpeed * 30.0f;
            if (before.frame >= f.maxFrame)
            {
                if (f.loop) {
                    before.frame -= f.maxFrame;
                }
                else {
                    before.frame = f.maxFrame;
                }
            }
            MV1SetAttachAnimTime(baseModel, before.attachID, before.frame);

            float rate = blendTime / blendTimeMax;
            MV1SetAttachAnimBlendRate(baseModel, current.attachID, rate);
            MV1SetAttachAnimBlendRate(baseModel, before.attachID, 1.0f - rate);
        }
        

       // 前回の行列を設定
       beforeMatrix = MV1GetFrameLocalMatrix(baseModel, rootNum);

       // 無補正時の座標を取得
       const VECTOR3 framePos = MV1GetAttachAnimFrameLocalPosition(baseModel, before.attachID, rootNum);

       // 座標移動を打ち消す
       beforeMatrix *= MGetTranslate(framePos * -1.0f);

       // Yだけ維持、XZを原点
       beforeMatrix *= MGetTranslate(VECTOR3(0.0f, framePos.y, 0.0f));
    }

    //前回のアニメーションが再生中なら、ブレンドする
   if (before.attachID >= 0)
   {
       float rate = blendTime / blendTimeMax;

       //ブレンド前とブレンド後を合成
       matrix = MAdd(beforeMatrix, MAdd(matrix, beforeMatrix * MGetScale(VOne * -1.0f)) * MGetScale(VOne* rate));
   }
   obj;

   // 最後に行列セット（1回だけ）
   MV1SetFrameUserLocalMatrix(baseModel, rootNum, matrix);
}

void Animator::AddFile(ID::IDType id, std::string filename, bool loop, float speed, float _eventStart, float _eventFinish)
{
    AnimFileInfo inf;
    inf.hModel = ResourceLoad::LoadAnim(filename, id);
    std::string str = ID::GetID(id);
    inf.fileName = filename;
    assert(inf.hModel > 0);
    inf.loop = loop;
    inf.playSpeed = speed;
    inf.eventStartTime = _eventStart;
    inf.eventFinishTime = _eventFinish;
    inf.id = str;
    if (inf.hModel > 0)
    {
        inf.maxFrame = MV1GetAnimTotalTime(inf.hModel, 0);
    }
    if (fileInfos.count(str) > 0)
    {
        MessageBox(nullptr,
            (std::string("ID=") + (str)+"は使われています").c_str(),
            "Animator", MB_OK);
        assert(false);
    }
    fileInfos.emplace(str, inf); // 登録
}

void Animator::SetMaxFrame(ID::IDType id, float _maxTime)
{
    fileInfos[ID::GetID(id)].maxFrame = _maxTime;
}

void Animator::Play(ID::IDType id, float margeTime)
{
    std::string str = ID::GetID(id);
    //文字列とファイルのIDが一致してなかったらリターン
    if (current.fileID == str)
    {  
        return;
    }
    //前に再生されたアニメーションがあったならでデタッチ
    if (before.attachID >= 0)
    {
        MV1DetachAnim(baseModel, before.attachID);
        before.attachID = -1;
        before.fileID = -1;
    }
    if (current.attachID >= 0)
    {
        before          = current;
        blendTimeMax    = margeTime;
        blendTime       = 0.0f;
    }
    else
    {
        blendTimeMax    = 0.0f;
    }
    current.fileID      = str;
    //ファイル内にデータが入っていたらアニメーションのアタッチ
    if (fileInfos.count(str) > 0)
    {
        current.attachID    = MV1AttachAnim(baseModel, 0, fileInfos[str].hModel);
        current.frame       = 0.0f;
        current.beforeFrame = 0.0f;
        finished = false;
        MV1SetAttachAnimTime(baseModel, current.attachID, 0.0f);
        //前に再生されたデータがあるならブレンド
        if (before.attachID >= 0)
        {
            MV1SetAttachAnimBlendRate(baseModel, current.attachID, 0.0f);
            MV1SetAttachAnimBlendRate(baseModel, before.attachID, 1.0f);
        }
        else 
        {
            MV1SetAttachAnimBlendRate(baseModel, current.attachID, 1.0f);
        }
    }
}

void Animator::SetPlaySpeed(float speed)
{
    playSpeed = speed;
}

float Animator::GetPlaySpeed()
{
    return playSpeed;
}

bool Animator::IsFinish()
{
    return finished;
}

std::string Animator::GetCurrentID()
{
    return current.fileID;
}

float Animator::GetCurrentFrame()const
{
    return current.frame;
}

float Animator::GetCurrentBeforeFrame()const
{
    return current.beforeFrame;
}

float Animator::GetMaxFrame()
{
    if (current.attachID >= 0)
    {
        return fileInfos[current.fileID].maxFrame;
    }
    return 0.0f;
}

void Animator::SetBoneFrame(int _index)
{
    current.boneIndex = _index;
}

VECTOR3 Animator::BoneMovePositionAdd(VECTOR3 _position)
{
    return _position + subPosition;
}

VECTOR3 Animator::BoneNowPosition()
{
    MATRIX matrix = MV1GetFrameLocalWorldMatrix(baseModel, current.boneIndex);

   /* MV1SetMatrix(baseModel, matrix);

    VECTOR3 Vect = VGet(0.0f, 0.0f, 0.0f);
    Vect = VTransform(Vect, matrix);*/
    return nowPosition * matrix;
}

void Animator::SetFrame(float _frame)
{
    current.frame = _frame;
    current.beforeFrame = current.frame;
}

int Animator::GetBaseModel()const
{
    return baseModel;
}

int Animator::GetCurrentAttachID()const
{
    return current.attachID;
}

bool Animator::BlendFinish()const
{
    return blendTime >= blendTimeMax;
}

void Animator::ImguiDraw()
{
    ImGui::Text("frame : %.1f", current.frame);
    if (ImGui::TreeNode("AnimInfo")) {
        for (auto file : fileInfos) {
            ImGui::Text("%s : %f", file.first.c_str(), file.second.maxFrame);
        }
        ImGui::TreePop();
    }
}

void Animator::AnimDataSaveAll()
{
    for (auto& [key, info] : fileInfos)
    {
        nlohmann::json root;
        root["Animator"] = info;

        JsonReader json;

        std::string path = "data/model/animation/" + info.id + ".json";

        json.Save(path, root);
    }
}

void Animator::AnimDataReSave(std::string _fileName)
{
    nlohmann::json root;

    root["Animator"] = fileInfos[_fileName];

    JsonReader json;

    std::string path = "data/model/animation/" + fileInfos[_fileName].id + ".json";

    json.Save(path, root);
}

void Animator::AnimDataLoad(const std::string& _charaID, const std::string _typeID)
{

    std::string charaID = _charaID;
    std::string charaTypeID = charaID + _typeID;

    std::string animFolderPath = "data/model/animation";
    std::vector <std::string> allAnimFolderFileName;

    allAnimFolderFileName = FileSystemUtils::GetFilesStartsWith(animFolderPath, charaTypeID);

    
    for (auto& fileInfo : allAnimFolderFileName) {
        JsonReader json;

        std::string fileName = fileInfo;
        std::string path = "data/model/animation/" + fileName + ".json";

        if (!json.Load(path))
        {
            //ロードできなかったらコンティニュー
            continue;
        }

        auto& data = json.Data();

        if (!data.contains("Animator"))
        {
            //Animatorが存在しなかったらコンティニュー
            continue;
        }

        AnimFileInfo info;
        data["Animator"].get_to(info);

        // モデル取得
        info.hModel = ResourceLoad::GetHandle(ID::StringToID(info.id));

        //jsonデータでmaxFrameを-1に設定すればアニメーションの最大フレームを取得
        if (info.maxFrame <= -1.0f) {
            info.maxFrame = MV1GetAnimTotalTime(info.hModel, 0);
        }
       

        // 登録
        fileInfos[info.id] = info;

    }
}

//
//void Animator::AnimDataLoad(const std::string& _path)
//{
//    JsonReader json;
//
//    std::string path = "data/json/Animation/" + _path + ".json";
//
//    json.Load(path);
//
//    for (auto& j: json.Data()["Animator"])
//    {
//
//        AnimFileInfo info ;
//        j.get_to(info);
//        
//        info.hModel = ResourceLoad::GetHandle(ID::StringToID(info.id));
//        fileInfos[info.id] = info;
//    }
//}

Animator::AnimFileInfo Animator::GetSelectFileInfo(std::string _fileName)
{
    return fileInfos[_fileName];
}

void Animator::SetSelectFileInfo(AnimFileInfo _animFileInfo)
{
    fileInfos[_animFileInfo.fileName] = _animFileInfo;
}

VECTOR3 Animator::BoneMovePositionAdd()
{
    return subPosition;
}