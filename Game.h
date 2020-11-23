#pragma once

#include "DXCore.h"
#include <DirectXMath.h>
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include "GameEntity.h"
#include "Camera.h"
#include "SimpleShader.h"
#include "Lights.h"
#include "SkyBox.h"
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"

class Game	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

private:

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders(); 
	void CreateBasicGeometry();

	
	// Note the usage of ComPtr below
	//  - This is a smart pointer for objects that abide by the
	//    Component Object Model, which DirectX objects do
	//  - More info here: https://github.com/Microsoft/DirectXTK/wiki/ComPtr

	//Meshes to hold geometery
	Mesh * triangle = nullptr;
	Mesh * topHat = nullptr;
	Mesh * cubeMesh = nullptr;
	Mesh* table = nullptr;
	Mesh* chair = nullptr;
	
	GameEntity* topHatOne = nullptr;
	GameEntity* topHatTwo = nullptr;
	GameEntity* cubeOne = nullptr;
	GameEntity* cubeTwo = nullptr;
	GameEntity* triaOne  = nullptr;
	GameEntity* tableOne = nullptr;
	GameEntity* tableTwo = nullptr;
	GameEntity* chairOne = nullptr;

	Camera* camera = nullptr;

	Material* defaultMaterial = nullptr;
	Material* defaultMaterialNormal = nullptr;
	Material* cushionMaterial = nullptr;
	Material* chairMaterial = nullptr;
	Material* tableMaterial = nullptr;


	SimpleVertexShader* vertexShader = nullptr;
	SimplePixelShader* pixelShader = nullptr;
	SimpleVertexShader* vertexShaderNormal = nullptr;
	SimplePixelShader* pixelShaderNormal = nullptr;
	SimplePixelShader* pixelShaderPBR = nullptr;
	SimplePixelShader* stylizedPS = nullptr;

	DirectionalLight light = {};
	DirectionalLight lightTwo = {};
	DirectionalLight lightThree = {};

	DirectionalLight pointLight = {};
	DirectionalLight pointLightTwo = {};

	// Skybo related shaders and resources
	SkyBox* skyBox = nullptr;
	SimpleVertexShader* skyVS = nullptr;
	SimplePixelShader* skyPS = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srvSky;

	// Shaders and shader-related constructs
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srvFire;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srvCurse;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normalFire;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normalCushion;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampleState;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srvTable;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srvNormalTable;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srvChair;

	// PBR Assets
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> albedoCobble;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> roughCobble;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> metalCobble;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normalCobble;

	// Stylized Color Array
	DirectX::XMFLOAT4 colorPalette[12];

	//silhouette related assets
	int silhouetteID = 0;
	SimplePixelShader* silhouettePS;
	SimpleVertexShader* postVS;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> postRTV;		
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> postSRV;		

};