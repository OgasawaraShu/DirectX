#include "FbxLoader.h"
#include<cassert>

const std::string FbxLoader::baseDirectory = "Resources/";

FbxLoader* FbxLoader::GetInstance()
{
    static FbxLoader instance;
    return &instance;
}

void FbxLoader::Initialize(ID3D12Device* device)
{
    //チェック
    assert(fbxManager == nullptr);
    //引数からメンバ変数に代入
    this->device = device;
    //マネージャ生成
    fbxManager = FbxManager::Create();
    //入出力生成
    FbxIOSettings* ios = FbxIOSettings::Create(fbxManager, IOSROOT);
    fbxManager->SetIOSettings(ios);
    //インポータ生成
    fbxImporter = FbxImporter::Create(fbxManager, "");

}

void FbxLoader::Finalize()
{

    //FBXインスタンスの破棄
    fbxImporter->Destroy();
    fbxManager->Destroy();
}

void FbxLoader::LoadModelFromFile(const string& modelName)
{
    //同じ名前から読み込む
    const string directoryPath = baseDirectory + modelName + "/";
    //拡張子追加
    const string fileName = modelName + ".fbx";
    //連結してフルパスを得る
    const string fullpath = directoryPath + fileName;

    //ファイル名を指定して読み込む
    if (!fbxImporter->Initialize(fullpath.c_str(), -1, fbxManager->GetIOSettings()))
    {
        assert(0);
    }

    //シーン生成
    FbxScene* fbxScene =
        FbxScene::Create(fbxManager, "fbxScene");

    //ファイルから得た情報をシーンにインポート
    fbxImporter->Import(fbxScene);

    //モデル生成
    Model* model = new Model();
    model->name = modelName;
    //√ノードから準二階席する
    ParseNodeRecursive(model, fbxScene->GetRootNode());
    //シーン解
    fbxScene->Destroy();

}

void FbxLoader::ParseNodeRecursive(Model* model, FbxNode* fbxNode)
{
    //ノード名を取得
    string name = fbxNode->GetName();

    //子ノードに対して再帰呼び出し
    for (int i = 0; i < fbxNode->GetChildCount(); i++)
    {
        ParseNodeRecursive(model, fbxNode->GetChild(i));
    }
}
