#include "Framework.h"
#include "MainGame.h"

#include "Scene/Class/TutorialScene.h"
#include "Scene/Class/TextureScene.h"
#include "Scene/Class/GridScene.h"
#include "Scene/Class/TerrainScene.h"
#include "Scene/Class/TerrainEditorScene.h"
#include "Scene/Class/ModelExportScene.h"
#include "Scene/Class/ModelAnimationScene.h"
#include "Scene/Class/CollisionScene.h"
#include "Scene/Class/CharacterScene.h"
#include "Scene/Class/InstancingScene.h"
#include "Scene/Class/ModelInstancingScene.h"

#include "Scene/Revolt/GameScene.h"

MainGame::MainGame()
{
	Initialize();

    //scene = new TutorialScene;
    //scene = new TextureScene;

    //SCENE->Create("Tutorial", new TutorialScene);

    SCENE->Create("Grid", new GridScene);

    //SCENE->Create("Start", new TextureScene);
    //SCENE->Create("Start", new TerrainScene);
    //SCENE->Create("Start", new TerrainEditorScene);
    //SCENE->Create("Start", new ModelExportScene);
    //SCENE->Create("Start", new ModelAnimationScene);
    //SCENE->Create("Start", new CollisionScene);
    //SCENE->Create("Start", new CharacterScene);
    //SCENE->Create("Start", new InstancingScene);
    SCENE->Create("Start", new ModelInstancingScene);


    //SCENE->Add("Tutorial");
    SCENE->Add("Grid");
    SCENE->Add("Start");

    char path[128];

    GetCurrentDirectoryA(128, path);

    projectDir = path;
}

MainGame::~MainGame()
{
    //delete scene;

    SceneManager::Delete();

	Release();
}

void MainGame::Update()
{
    ENVIRONMENT->Update();
    KEYBOARD->Update();
    TIME->Update();

    SCENE->Update();
}

void MainGame::Render()
{
    ENVIRONMENT->Set();
    
    SCENE->Render();

}

void MainGame::PreRender()
{
    SCENE->PreRender();
}

void MainGame::PostRender()
{
    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    TIME->Render();

    static bool isWireFrame = false;

    ImGui::Checkbox("WireFrame", &isWireFrame);

    if (isWireFrame)
        STATE->EnableWireFrame();
    else
        STATE->DisableWireFrame();

    static bool isGrid = true;

    if (ImGui::Checkbox("Grid", &isGrid))
    {
        if (isGrid)
            SCENE->Add("Grid");
        else
            SCENE->Remove("Grid");
    }

    SCENE->PostRender();

    ENVIRONMENT->Debug();

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    ImGui::EndFrame();
}

void MainGame::Initialize()
{
    Device::GetInstance();
    Environment::GetInstance();
    StateManager::GetInstance();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(DEVICE, DC);
}

void MainGame::Release()
{
    Device::Delete();
    Environment::Delete();
    Shader::Delete();

    Keyboard::Delete();
    Time::Delete();
    Texture::Delete();

    StateManager::Delete();

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}
