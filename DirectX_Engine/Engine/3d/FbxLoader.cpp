#include "FbxLoader.h"
#include<cassert>

const std::string FbxLoader::baseDirectory = "Resources/";
using namespace DirectX;
const std::string FbxLoader::defaultTextureFileName =
"white1x1.png";

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

Model*FbxLoader::LoadModelFromFile(const string& modelName)
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

    //
    int nodeCount = fbxScene->GetNodeCount();

    model->nodes.reserve(nodeCount);



    //√ノードから準二階席する
    ParseNodeRecursive(model, fbxScene->GetRootNode());
    //シーン解放
   // fbxScene->Destroy();
    model->fbxScene = fbxScene;

    model->CreateBuffers2(device);

   
    return model;

}

Model* FbxLoader::RenLoadModelFromFile(const string& modelName)
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

    //
    int nodeCount = fbxScene->GetNodeCount();

    model->nodes.reserve(nodeCount);



    //√ノードから準二階席する
    ParseNodeRecursive(model, fbxScene->GetRootNode());
    //シーン解放
   // fbxScene->Destroy();
    model->fbxScene = fbxScene;

    model->RenCreateBuffers2(device);

    model->RenderInitialize(device);
    return model;
}

Model* FbxLoader::RenLoadModelFromFile2(const string& modelName)
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

    //
    int nodeCount = fbxScene->GetNodeCount();

    model->nodes.reserve(nodeCount);



    //√ノードから準二階席する
    ParseNodeRecursive(model, fbxScene->GetRootNode());
    //シーン解放
   // fbxScene->Destroy();
    model->fbxScene = fbxScene;

    model->RenCreateBuffers22(device);

    model->RenderInitialize2(device);
    return model;
}

void FbxLoader::ParseNodeRecursive(Model* model, FbxNode* fbxNode,Node*parent)
{
    //ノード名を取得
    string name = fbxNode->GetName();



    //ノード追加
    model->nodes.emplace_back();
    Node& node = model->nodes.back();
    //名を取得
    node.name = fbxNode->GetName();

    //ローカル移動情報
    FbxDouble3 rotation = fbxNode->LclRotation.Get();
    FbxDouble3 scaling = fbxNode->LclScaling.Get();
    FbxDouble3 translation = fbxNode->LclTranslation.Get();


    //形式変換する
    node.rotation = { (float)rotation[0],(float)rotation[1],(float)rotation[2],0.0f };
    node.scaling = { (float)scaling[0],(float)scaling[1],(float)scaling[2],0.0f };
    node.translation = { (float)translation[0],(float)translation[1],(float)translation[2],1.0f };

    //回転角をラジアンに変更
    node.rotation.m128_f32[0] = XMConvertToRadians(node.rotation.m128_f32[0]);
    node.rotation.m128_f32[1] = XMConvertToRadians(node.rotation.m128_f32[1]);
    node.rotation.m128_f32[2] = XMConvertToRadians(node.rotation.m128_f32[2]);

    //スケール、回転、平行移動行列の計算
    XMMATRIX matScaling, matRotation, matTranslation;
    matScaling = XMMatrixScalingFromVector(node.scaling);
    matRotation = XMMatrixRotationRollPitchYawFromVector(node.rotation);
    matTranslation = XMMatrixTranslationFromVector(node.translation);

    //ローカル変形行列の計算
    node.transform = XMMatrixIdentity();
    node.transform *= matScaling;
    node.transform *= matRotation;
    node.transform *= matTranslation;

    //グローバル変形行列の計算
    node.globalTransform = node.transform;
    if (parent) {
        node.parent = parent;
        //親の変形を乗算
        node.globalTransform *= parent->globalTransform;
    }


    FbxNodeAttribute* fbxNodeAttribute = fbxNode->GetNodeAttribute();

    if (fbxNodeAttribute)
    {
        if (fbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh) {
            model->meshNode = &node;
            ParseMesh(model, fbxNode);
        }
    }



    //子ノードに対して再帰呼び出し
    for (int i = 0; i < fbxNode->GetChildCount(); i++)
    {
        ParseNodeRecursive(model, fbxNode->GetChild(i),&node);
    }

    
}

void FbxLoader::ParseMesh(Model* model, FbxNode* fbxNode)
{

    //メッシュ取得
    FbxMesh* fbxMesh = fbxNode->GetMesh();

    //座標読み取り
    ParseMeshVectices(model, fbxMesh);
    //データ読み取り
    ParseMeshFaces(model, fbxMesh);
    //マテリアル読み取り
    ParseMaterial(model, fbxNode);
    //スキニング情報の読み取り
    ParseSkin(model, fbxMesh);
}

void FbxLoader::ParseMeshVectices(Model* model, FbxMesh* fbxMesh)
{
    auto& vertices = model->vertices;

    //
    const int controlPointsCount =
        fbxMesh->GetControlPointsCount();
    //
    Model::VertexPosNormalUv vert{};
    model->vertices.resize(controlPointsCount, vert);

    //
    FbxVector4* pCoord =
        fbxMesh->GetControlPoints();

    //
    for (int i = 0; i < controlPointsCount; i++)
    {
        Model::VertexPosNormalUv& vertex =
            vertices[i];
        
        vertex.pos.x = (float)pCoord[i][0];
        vertex.pos.y = (float)pCoord[i][1];
        vertex.pos.z = (float)pCoord[i][2];

    }
}

void FbxLoader::ParseMeshFaces(Model* model, FbxMesh* fbxMesh)
{
    auto& vertices = model->vertices;
    auto& indices = model->indices;

    //複数は未対応
    assert(indices.size() == 0);
    //面の数
    const int polygonCount = fbxMesh->GetPolygonCount();
    //UVデータ
    const int textureUVCount = fbxMesh->GetTextureUVCount();
    //UV名
    FbxStringList uvNames;
    fbxMesh->GetUVSetNames(uvNames);

    for (int i = 0; i < polygonCount; i++)
    {
        const int polygonSize = fbxMesh->GetPolygonSize(i);
        //assert(polygonSize <= 4);

        //1頂点ずつ処理
        for (int j = 0; j < polygonSize; j++)
        {
            int index = fbxMesh->GetPolygonVertex(i, j);
            assert(index >= 0);

            Model::VertexPosNormalUv& vertex = vertices[index];
            FbxVector4 normal;
            if (fbxMesh->GetPolygonVertexNormal(i, j, normal))
            {
                vertex.normal.x = (float)normal[0];
                vertex.normal.y = (float)normal[1];
                vertex.normal.z = (float)normal[2];


                if (textureUVCount > 0) {
                    FbxVector2 uvs;
                    bool lUnmappedUV;

                    if (fbxMesh->GetPolygonVertexUV(i, j, uvNames[0], uvs, lUnmappedUV))
                    {
                        vertex.uv.x = (float)uvs[0];
                        vertex.uv.y = (float)uvs[1];
                   
             
                    }
                }


                //3
                if (j < 3)
                {
                    indices.push_back(index);
                }

                //4
                else
                {
                    int index2 = indices[indices.size() - 1];
                    int index3 = index;
                    int index0 = indices[indices.size() - 3];
                    indices.push_back(index2);
                    indices.push_back(index3);
                    indices.push_back(index0);


                }
            }
        }
    }

  
}

void FbxLoader::ParseMaterial(Model* model, FbxNode* fbxNode)
{
    const int materialCount = fbxNode->GetMaterialCount();
    if (materialCount > 0) {
        //戦闘マテリアル取得
        FbxSurfaceMaterial* material = fbxNode->GetMaterial(0);
        //読み込んだかどうか
        bool textureLoaded = false;

        if (material){

            if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
            {
                FbxSurfaceLambert* lambert =
                    static_cast<FbxSurfaceLambert*>(material);

                //環境攻
                FbxPropertyT<FbxDouble3>ambient = lambert->Ambient;
                model->ambient.x = (float)ambient.Get()[0];
                model->ambient.y = (float)ambient.Get()[1];
                model->ambient.z = (float)ambient.Get()[2];

                //拡散反射光
                FbxPropertyT<FbxDouble3>diffuse = lambert->Diffuse;
                model->diffuse.x = (float)diffuse.Get()[0];
                model->diffuse.y = (float)diffuse.Get()[1];
                model->diffuse.z = (float)diffuse.Get()[2];
            }
            const FbxProperty diffuseProperty =
                material->FindProperty(FbxSurfaceMaterial::sDiffuse);
            if (diffuseProperty.IsValid())
            {
                const FbxFileTexture* texture = diffuseProperty.GetSrcObject<FbxFileTexture>();
                if (texture) {
                    const char* filepath = texture->GetFileName();
                    //ファイルパスから名前
                    string path_str(filepath);
                    string name = ExtractFileName(path_str);
                    //テクスチャ読み込み
                    LoadTexture(model, baseDirectory + model->name + "/" + name);
                    textureLoaded = true;
                }
            }


            }
        //テクスチャがない場合は白にする
        if (!textureLoaded) {
            LoadTexture(model, baseDirectory + defaultTextureFileName);
        }
    }
}

void FbxLoader::LoadTexture(Model* model, const std::string& fullpath)
{
    HRESULT result = S_FALSE;
    //WICテクスチャロード
    TexMetadata& metadata = model->metadata;
    ScratchImage& scratchImg = model->scratchImg;
    //ユニコード文字列に変換
    wchar_t wfilepath[128];
    MultiByteToWideChar(CP_ACP, 0, fullpath.c_str(), -1, wfilepath, _countof(wfilepath));
    result = LoadFromWICFile(
        wfilepath, WIC_FLAGS_NONE,
        &metadata, scratchImg);
    if (FAILED(result)) {
        assert(0);
    }
}

void FbxLoader::ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src)
{


    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            dst->r[i].m128_f32[j] = (float)src.Get(i, j);
        }
    }

}

std::string FbxLoader::ExtractFileName(const std::string& path)
{
    size_t pos1;

    pos1 = path.rfind('\\');
    if (pos1 != string::npos) {
        return path.substr(pos1 + 1, path.size() - pos1 - 1);
    }

    pos1 = path.rfind('/');
    if (pos1 != string::npos)
    {
        return path.substr(pos1 + 1, path.size() - pos1 - 1);
    }

    return path;
}

void FbxLoader::ParseSkin(Model* model, FbxMesh* fbxMesh)
{
    //情報
    FbxSkin* fbxSkin =
        static_cast<FbxSkin*>(fbxMesh->GetDeformer(0, FbxDeformer::eSkin));

    if (fbxSkin == nullptr) {

        /**/
        for (int i = 0; i < model->vertices.size(); i++)
        {
            model->vertices[i].boneIndex[0] = 0;
            model->vertices[i].boneWeight[0] = 1.0f;
        }
        return;
    }

    //ボーンの参照
    std::vector<Model::Bone>& bones = model->bones;

    //ボーンの数
    int clusterCount = fbxSkin->GetClusterCount();
    bones.reserve(clusterCount);

    //全てのボーン
    for (int i = 0; i < clusterCount; i++)
    {
        //FBXボーン情報
        FbxCluster* fbxCluster = fbxSkin->GetCluster(i);

        //名前取得
        const char* boneName = fbxCluster->GetLink()->GetName();

        //新しく追加し、参照を得る
        bones.emplace_back(Model::Bone(boneName));
        Model::Bone& bone = bones.back();
        //自作とFBxのボーンを紐づける
        bone.fbxCluster = fbxCluster;

        FbxAMatrix fbxMat;
        fbxCluster->GetTransformLinkMatrix(fbxMat);

        //XMMatrix型に変換する
        XMMATRIX intialPose;
        ConvertMatrixFromFbx(&intialPose, fbxMat);

        //初期姿勢行列の行列を得る
        bone.invInitialPose = XMMatrixInverse(nullptr, intialPose);
    }

    struct WeightSet {
        UINT index;
        float weight;
    };

    std::vector<std::list<WeightSet>>
        weightLists(model->vertices.size());

    for (int i = 0; i < clusterCount; i++)
    {
        FbxCluster* fbxCluster = fbxSkin->GetCluster(i);

        int controlPointIndicesCount = fbxCluster->GetControlPointIndicesCount();

        int* controlPointIndices = fbxCluster->GetControlPointIndices();

        double* controlPointWeights = fbxCluster->GetControlPointWeights();

        for (int j = 0; j < controlPointIndicesCount; j++)
        {
            int vertIndex = controlPointIndices[j];

            float weight = (float)controlPointWeights[j];

            weightLists[vertIndex].emplace_back(WeightSet{ (UINT)i,weight });
        }
    }

    auto& vertices = model->vertices;


    for (int i = 0; i < vertices.size(); i++)
    {
        auto& weightList = weightLists[i];

        weightList.sort(
            [](auto const& lhs, auto const& rhs) {
                return lhs.weight > rhs.weight;
            }
        );

        int weightArrayIndex = 0;

        for (auto& weightSet : weightList)
        {
            vertices[i].boneIndex[weightArrayIndex] = weightSet.index;
            vertices[i].boneWeight[weightArrayIndex] = weightSet.weight;

            if (++weightArrayIndex >= Model::MAX_BONE_INDICES) {
                float weight = 0.0f;

                for (int j = 1; j < Model::MAX_BONE_INDICES; j++) {
                    weight += vertices[i].boneWeight[j];
                }

                vertices[i].boneWeight[0] = 1.0f - weight;
                break;
            }
        }
    }
   
}
