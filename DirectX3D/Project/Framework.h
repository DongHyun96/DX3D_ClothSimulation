// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "__Others/targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <functional>

#include <shlwapi.h>

#pragma comment(lib, "Shlwapi.lib")

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "DirectXTex.h"
#include "DirectXTex.inl"

#include <DirectXCollision.h>

#pragma comment(lib, "DirectXTex.lib")

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#include "ImGuiFileDialog.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#pragma comment(lib, "assimp.lib")

using namespace DirectX;
using namespace std;

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#define WIN_HEIGHT		720.f
#define WIN_WIDTH		1280.f

#define MAX_FRAME		512
#define MAX_BONE		256

#define MAX_INSTANCE	128

#define DEVICE			Device::GetInstance()->GetDevice()
#define DC				Device::GetInstance()->GetDC()

#define ENVIRONMENT		Environment::GetInstance()
#define KEYBOARD		Keyboard::GetInstance()
#define TIME			Time::GetInstance()
#define SCENE			SceneManager::GetInstance()
#define STATE			StateManager::GetInstance()
#define CAMERA			Environment::GetInstance()->GetMainCamera()

#define DIALOG			ImGuiFileDialog::Instance()


#define DELTA_TIME		Time::Delta()

#define KEY_PRESS(p)	Keyboard::GetInstance()->Press(p)
#define KEY_DOWN(p)		Keyboard::GetInstance()->Down(p)
#define KEY_UP(p)		Keyboard::GetInstance()->Up(p)

#define LERP(s, e, t)	((e - s) * t + s);

typedef XMMATRIX Matrix;
typedef XMFLOAT2 Vector2;
typedef XMFLOAT4 Vector4;

#include "Utility/Singleton.h"
#include "Utility/Vector3.h"
#include "Utility/Time.h"
#include "Utility/Keyboard.h"
#include "Utility/Utility.h"
#include "Utility/GameMath.h"

using namespace Utility;
using namespace GameMath;

#include "Utility/BinaryReader.h"
#include "Utility/BinaryWriter.h"

#include "System/Device.h"

#include "Render/Buffer/VertexLayout.h"

#include "Render/Buffer/VertexBuffer.h"
#include "Render/Buffer/IndexBuffer.h"
#include "Render/Buffer/ConstBuffer.h"
#include "Render/Buffer/GlobalBuffer.h"
#include "Render/Buffer/StructuredBuffer.h"


#include "Object/Transform.h"

#include "Render/Shader/Shader.h"
#include "Render/Shader/VertexShader.h"
#include "Render/Shader/PixelShader.h"
#include "Render/Shader/ComputeShader.h"

#include "System/Camera.h"
#include "System/Environment.h"

#include "Render/Texture/Texture.h"

#include "Render/Mesh.h"
#include "Render/Material.h"

#include "Render/State/RasterizerState.h"
#include "Render/State/SamplerState.h"
#include "Render/State/BlendState.h"
#include "Render/State/DepthStencilState.h"

#include "Render/State/StateManager.h"

#include "Object/GameObject.h"

#include "Object/Basic/Cube.h"
#include "Object/Basic/Quad.h"
#include "Object/Basic/Grid.h"
#include "Object/Basic/Sphere.h"

#include "Object/Basic/TextureCube.h"

#include "Object/LandScape/Terrain.h"
#include "Object/LandScape/TerrainEditor.h"
#include "Object/LandScape/SkyBox.h"

#include "Object/Collider/Collider.h"
#include "Object/Collider/ColliderBox.h"
#include "Object/Collider/ColliderSphere.h"
#include "Object/Collider/ColliderCapsule.h"

#include "Object/UI/ProgressBar.h"


#include "Model/ModelData.h"
#include "Model/ModelExporter.h"
#include "Model/ModelMesh.h"
#include "Model/Model.h"
#include "Model/ModelClip.h"
#include "Model/ModelAnimator.h"
#include "Model/ModelInstancing.h"
#include "Model/ModelAnimatorInstancing.h"

#include "Object/Game/Character.h"
#include "Object/Game/Knight.h"

///////////////////////////////////////////////////

#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

#include "MainGame.h"


extern HWND hWnd;
extern Vector3 mousePos;
extern string projectDir;