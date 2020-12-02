#include "Game.h"
#include "Vertex.h"
#include "BufferStructs.h"
// Needed for a helper function to read compiled shader files from the hard drive
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>


// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.\n");
#endif

}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	// Note: Since we're using smart pointers (ComPtr),
	// we don't need to explicitly clean up those DirectX objects
	// - If we weren't using smart pointers, we'd need
	//   to call Release() on each DirectX object created in Game
	delete cubeMesh;
	delete triangle;
	delete topHat;
	delete chair;
	delete table;

	delete topHatOne;
	delete topHatTwo;
	delete cubeOne;
	delete cubeTwo;
	delete triaOne;

	delete tableOne;
	delete tableTwo;
	delete chairOne;

	delete camera;

	delete defaultMaterial;
	delete cushionMaterial;
	delete defaultMaterialNormal;
	delete tableMaterial;
	delete chairMaterial;

	delete vertexShader;
	delete pixelShader;
	delete vertexShaderNormal;
	delete pixelShaderNormal;
	delete pixelShaderPBR;
	delete stylizedPS;
	delete postVS;
	delete silhouettePS;

	delete skyBox;
	delete skyVS;
	delete skyPS;
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();
	CreateBasicGeometry();
	

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	camera = new Camera(DirectX::XMFLOAT3(0.0f, 0.0f, -5.0f), 90.0f, 0.1f, 300.0f);
	light = DirectionalLight();
	light.ambientColor = DirectX::XMFLOAT3(0.2f, 0.1f, 0.1f);
	light.diffuseColor = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	light.direction = DirectX::XMFLOAT3(-1.0f, 1.0f, 0);
	light.type = 0;

	lightTwo = DirectionalLight();
	lightTwo.ambientColor = DirectX::XMFLOAT3(0.2f, 0.1f, 0.1f);
	lightTwo.diffuseColor = DirectX::XMFLOAT3(0.5f, 0.25f, 0.5f);
	lightTwo.direction = DirectX::XMFLOAT3(1.0f, 0, 0);
	lightTwo.type = 0;

	lightThree = DirectionalLight();
	lightThree.ambientColor = DirectX::XMFLOAT3(0.2f, 0.1f, 0.1f);
	lightThree.diffuseColor = DirectX::XMFLOAT3(0.5f, 0.5f, 1.0f);
	lightThree.direction = DirectX::XMFLOAT3(0, 1, 0);
	lightThree.type = 0;

	pointLight = DirectionalLight();
	pointLight.ambientColor = DirectX::XMFLOAT3(0.2f, 0.1f, 0.1f);
	pointLight.diffuseColor = DirectX::XMFLOAT3(0.0f, 1.0f, 0.1f);
	pointLight.direction = DirectX::XMFLOAT3(5, 1, 0);
	pointLight.type = 1;

	// assign the palettes
	colorPalette[0] = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	colorPalette[1] = DirectX::XMFLOAT4(0.0941f, 0.0941f, 0.0941f, 0.0941f);
	colorPalette[2] = DirectX::XMFLOAT4(0.1882f, 0.1882f, 0.1882f, 0.1882f);
	colorPalette[3] = DirectX::XMFLOAT4(0.2823f, 0.2823f, 0.2823f, 0.2823f);
	colorPalette[4] = DirectX::XMFLOAT4(0.3764f, 0.3764f, 0.3764f, 0.3764f);
	colorPalette[5] = DirectX::XMFLOAT4(0.4706f, 0.4706f, 0.4706f, 0.4706f);
	colorPalette[6] = DirectX::XMFLOAT4(0.5647f, 0.5647f, 0.5647f, 0.5647f);
	colorPalette[7] = DirectX::XMFLOAT4(0.6588f, 0.6588f, 0.6588f, 0.6588f);
	colorPalette[8] = DirectX::XMFLOAT4(0.7529f, 0.7529f, 0.7529f, 0.7529f);
	colorPalette[9] = DirectX::XMFLOAT4(0.8470f, 0.8470f, 0.8470f, 0.8470f);
	colorPalette[10] = DirectX::XMFLOAT4(0.9412f, 0.9412f, 0.9412f, 0.9412f);
	colorPalette[11] = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);


	newPalette[0] = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	newPalette[1] = DirectX::XMFLOAT4(0.1294f, 0.2509f, 0.6039f, 0.0941f);
	newPalette[2] = DirectX::XMFLOAT4(0.2862f, 0.2666f, 0.2314f, 0.1882f);
	newPalette[3] = DirectX::XMFLOAT4(0.4549f, 0.6431f, 0.4f, 0.2823f);
	newPalette[4] = DirectX::XMFLOAT4(0.5372f, 0.1569f, 0.8588f, 0.3764f);
	newPalette[5] = DirectX::XMFLOAT4(0.6588f, 0.8941f, 0.8588f, 0.4706f);
	newPalette[6] = DirectX::XMFLOAT4(0.6980f, 0.3569f, 0.0470f, 0.5647f);
	newPalette[7] = DirectX::XMFLOAT4(0.7451f, 0.1176f, 0.1765f, 0.6588f);
	newPalette[8] = DirectX::XMFLOAT4(0.8118f, 0.6784f, 0.0549f, 0.7529f);
	newPalette[9] = DirectX::XMFLOAT4(0.8627f, 0.2667f, 0.1412f, 0.8470f);
	newPalette[10] = DirectX::XMFLOAT4(0.9607f, 0.8509f, 0.0863f, 0.9412f);
	newPalette[11] = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	bauhausPalette[0] = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	bauhausPalette[1] = DirectX::XMFLOAT4(0.0431f, 0.1529f, 0.4274f, 0.0941f);
	bauhausPalette[2] = DirectX::XMFLOAT4(0.1058f, 0.5569f, 0.3569f, 0.1882f);
	bauhausPalette[3] = DirectX::XMFLOAT4(0.2f, 0.6824f, 0.0509f, 0.2823f);
	bauhausPalette[4] = DirectX::XMFLOAT4(0.2588f, 0.2784f, 0.3098f, 0.3764f);
	bauhausPalette[5] = DirectX::XMFLOAT4(0.3059f, 0.4509f, 0.5098f, 0.4706f);
	bauhausPalette[6] = DirectX::XMFLOAT4(0.5058f, 0.3059f, 0.2118f, 0.5647f);
	bauhausPalette[7] = DirectX::XMFLOAT4(0.7372f, 0.1804f, 0.1059f, 0.6588f);
	bauhausPalette[8] = DirectX::XMFLOAT4(0.7529f, 0.5804f, 0.5569f, 0.7529f);
	bauhausPalette[9] = DirectX::XMFLOAT4(0.8627f, 0.8235f, 0.3412f, 0.8470f);
	bauhausPalette[10] = DirectX::XMFLOAT4(0.9294f, 0.8980f, 0.6078f, 0.9412f);
	bauhausPalette[11] = DirectX::XMFLOAT4(1.0f, 0.533f, 0.0745f, 1.0f);


	// Set up post processing texture, RTV, and SRV.
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = width / portion;
	textureDesc.Height = height / portion;
	textureDesc.ArraySize = 1;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; // Will render to it and sample from it!
	textureDesc.CPUAccessFlags = 0;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.MipLevels = 1;
	textureDesc.MiscFlags = 0;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;

	// Create the color and normals textures
	ID3D11Texture2D* ppTexture;

	device->CreateTexture2D(&textureDesc, 0, &ppTexture);

	if (ppTexture != 0)
	{
		device->CreateRenderTargetView(ppTexture, 0, postRTV.ReleaseAndGetAddressOf());
		device->CreateShaderResourceView(ppTexture, 0, postSRV.ReleaseAndGetAddressOf());
		
	}
	ppTexture->Release();
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files
// and also created the Input Layout that describes our 
// vertex data to the rendering pipeline. 
// - Input Layout creation is done here because it must 
//    be verified against vertex shader byte code
// - We'll have that byte code already loaded below
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device.Get(), context.Get(), GetFullPathTo_Wide(L"VertexShader.cso").c_str()); 
	pixelShader = new SimplePixelShader(device.Get(), context.Get(), GetFullPathTo_Wide(L"PixelShader.cso").c_str());
	vertexShaderNormal = new SimpleVertexShader(device.Get(), context.Get(), GetFullPathTo_Wide(L"NormalMapVS.cso").c_str());
	pixelShaderNormal = new SimplePixelShader(device.Get(), context.Get(), GetFullPathTo_Wide(L"NormalMapPS.cso").c_str());
	pixelShaderPBR = new SimplePixelShader(device.Get(), context.Get(), GetFullPathTo_Wide(L"PBRPixelShader.cso").c_str());
	skyVS = new SimpleVertexShader(device.Get(), context.Get(), GetFullPathTo_Wide(L"CubemapVS.cso").c_str());
	skyPS = new SimplePixelShader(device.Get(), context.Get(), GetFullPathTo_Wide(L"CubemapPS.cso").c_str());
	stylizedPS = new SimplePixelShader(device.Get(), context.Get(), GetFullPathTo_Wide(L"StyleShader.cso").c_str());
	silhouettePS = new SimplePixelShader(device.Get(), context.Get(), GetFullPathTo_Wide(L"SilhouettePS.cso").c_str());
	postVS = new SimpleVertexShader(device.Get(), context.Get(), GetFullPathTo_Wide(L"SilhouetteVS.cso").c_str());
}



// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT3 red = XMFLOAT3(0.0f, 0.0f, -1.0f);
	XMFLOAT3 black = red;
	XMFLOAT3 white = red;

	XMFLOAT2 UV = XMFLOAT2(0, 0);


	// Load Skybox
	CreateDDSTextureFromFile( device.Get(), GetFullPathTo_Wide(L"../../Assets/Skybox/SunnyCubeMap.dds").c_str(), nullptr, srvSky.GetAddressOf()); 

	//create Mesh objects
	triangle = new Mesh(GetFullPathTo("../../Assets/Models/cone.obj").c_str(), device);
	topHat = new Mesh(GetFullPathTo("../../Assets/Models/sphere.obj").c_str(), device);
	cubeMesh = new Mesh(GetFullPathTo("../../Assets/Models/cube.obj").c_str(), device);
	chair = new Mesh(GetFullPathTo("../../Assets/Models/chair.obj").c_str(), device);
	table = new Mesh(GetFullPathTo("../../Assets/Models/table.obj").c_str(), device);


	//Load Textures
	HRESULT fire = CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/hotnspicy.png").c_str(), nullptr, srvFire.GetAddressOf() );
	HRESULT cursio = CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/cushion.png").c_str(), nullptr, srvCurse.GetAddressOf());
	HRESULT wood = CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/wood.png").c_str(), nullptr, srvChair.GetAddressOf());
	HRESULT fabric = CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/table.jpg").c_str(), nullptr, srvTable.GetAddressOf());

	//Load Normal Maps
	HRESULT normfire = CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/rock_normals.png").c_str(), nullptr, normalFire.GetAddressOf());
	HRESULT normcursio = CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/cushion_normals.png").c_str(), nullptr, normalCushion.GetAddressOf());
	HRESULT normtable = CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/tableN.jpg").c_str(), nullptr, srvNormalTable.GetAddressOf());

	//Load PBR
	HRESULT alcobble = CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/cobblestone_albedo.png").c_str(), nullptr, albedoCobble.GetAddressOf());
	HRESULT roucobble = CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/cobblestone_roughness.png").c_str(), nullptr, roughCobble.GetAddressOf());
	HRESULT metcobble = CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/cobblestone_metal.png").c_str(), nullptr, metalCobble.GetAddressOf());
	HRESULT normcobble = CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/cobblestone_normals.png").c_str(), nullptr, normalCobble.GetAddressOf());

	D3D11_SAMPLER_DESC sampleDesc = { };
	sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampleDesc.MaxLOD = 16;

	device->CreateSamplerState(&sampleDesc, sampleState.GetAddressOf());

	// create Material
	defaultMaterial = new Material(XMFLOAT4(1, 1, 1, 0), pixelShader, vertexShader, 5.0f, 64.0f, srvCurse, sampleState);
	defaultMaterialNormal = new Material(XMFLOAT4(1, 1, 1, 0), pixelShaderPBR, vertexShaderNormal, 5.0f, 64.0f, albedoCobble, normalCobble, roughCobble, metalCobble, sampleState);
	cushionMaterial = new Material(XMFLOAT4(1, 0, 0, 0), pixelShaderNormal, vertexShaderNormal, 100.0f, 64.0f, srvCurse, normalCushion, sampleState);
	tableMaterial = new Material(XMFLOAT4(1, 1, 1, 0), pixelShaderNormal, vertexShaderNormal, 100.0f, 64.0f, srvTable, srvNormalTable, sampleState);
	chairMaterial = new Material(XMFLOAT4(1, 1, 1, 0), pixelShader, vertexShader, 10.0f, 32.0f, srvChair, sampleState);


	topHatOne = new GameEntity(topHat, defaultMaterial);
	topHatTwo = new GameEntity(topHat, cushionMaterial);
	cubeOne = new GameEntity(cubeMesh, defaultMaterial);
	cubeTwo = new GameEntity(cubeMesh, cushionMaterial);
	triaOne = new GameEntity(triangle, defaultMaterialNormal);
	tableOne = new GameEntity(table, tableMaterial);
	tableTwo = new GameEntity(table, tableMaterial);
	chairOne = new GameEntity(chair, chairMaterial);


	skyBox = new SkyBox(sampleState, srvSky, device, cubeMesh, skyPS, skyVS);

	topHatOne->GetTransform()->SetPosition(4, 0, 0);
	topHatTwo->GetTransform()->SetPosition(-4, 0, 0);
	cubeOne->GetTransform()->SetPosition(1, 2, 0);
	cubeTwo->GetTransform()->SetPosition(-1, -2, 0);
	tableOne->GetTransform()->SetPosition(2, 0, 0);
	tableTwo->GetTransform()->SetPosition(-2, 0, 0);
	chairOne->GetTransform()->SetPosition(1, 0, 0);
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	if (camera != nullptr)
	{
		camera->UpdateProjectionMatrix((float)this->width / this->height);
	}
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	// Set up post processing texture, RTV, and SRV.
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = width / portion;
	textureDesc.Height = height / portion;
	textureDesc.ArraySize = 1;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; // Will render to it and sample from it!
	textureDesc.CPUAccessFlags = 0;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.MipLevels = 1;
	textureDesc.MiscFlags = 0;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;

	// Create the color and normals textures
	ID3D11Texture2D* ppTexture;

	device->CreateTexture2D(&textureDesc, 0, &ppTexture);

	if (ppTexture != 0)
	{
		device->CreateRenderTargetView(ppTexture, 0, postRTV.ReleaseAndGetAddressOf());
		device->CreateShaderResourceView(ppTexture, 0, postSRV.ReleaseAndGetAddressOf());
	}
	
	ppTexture->Release();
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();

	// Overall input bools
	bool currentTab = (GetAsyncKeyState(VK_TAB) & 0x8000) != 0;

	// Check for change and wrap if necessary
	if (currentTab && !prevTab) paletteNumber++;
	if (paletteNumber > 2) paletteNumber = 0;

	// Save state for next frame
	prevTab = currentTab;

	//Update the camera
	camera->Update(deltaTime, hWnd);


	// Update the objects

	// cube one rotates about the x-axis
	cubeOne->GetTransform()->Rotate(0, 0.1f * deltaTime, 0);
	cubeOne->GetTransform()->MoveAbsolute(-0.03f * deltaTime, 0, 0);

	// cube two rotates the other way and moves right
	cubeTwo->GetTransform()->Rotate(-0.1f * deltaTime, 0, 0);
	cubeTwo->GetTransform()->MoveAbsolute( 0.03f * deltaTime, 0, 0);

	// top Hat one rotates about the z
	topHatOne->GetTransform()->Rotate(0, 0, 0.1f * deltaTime);
	if (topHatOne->GetTransform()->GetScale().x < 2)
	{
		topHatOne->GetTransform()->Scale(0.1f * deltaTime, 0.1f * deltaTime, 0.1f * deltaTime);
	}
	else
	{
		topHatOne->GetTransform()->SetScale(1, 1, 1);
	}
	// top hat two moves left and away
	topHatTwo->GetTransform()->MoveAbsolute( -0.05f * deltaTime, 0, 0.05f * deltaTime);
	

	// triangle just kinda schmoves
	triaOne->GetTransform()->Rotate(0, 0.1f * deltaTime, 0);
	triaOne->GetTransform()->MoveAbsolute( 0.05f * deltaTime, 0, 0);


}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	

	

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV.Get(), color);
	context->ClearDepthStencilView(
		depthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	// Ensure we're clearing the post process target too
	context->ClearRenderTargetView(postRTV.Get(), color);

	// Set the render target
	context->OMSetRenderTargets(1, postRTV.GetAddressOf(), depthStencilView.Get());
	


	// Ensure the pipeline knows how to interpret the data (numbers)
	// from the vertex buffer.  
	// - If all of your 3D models use the exact same vertex layout,
	//    this could simply be done once in Init()
	// - However, this isn't always the case (but might be for this course)
	context->IASetInputLayout(inputLayout.Get());

	

	// Set the pixel shader with the lights
	pixelShader->SetData(
		"directionalLight",
		&light,
		sizeof(DirectionalLight)
	);
	pixelShader->SetData(
		"lightTwo",
		&lightTwo,
		sizeof(DirectionalLight)
	);
	pixelShader->SetData(
		"lightThree",
		&lightThree,
		sizeof(DirectionalLight)
	);
	pixelShader->SetData(
		"pointLight",
		&pointLight,
		sizeof(DirectionalLight)
	);
	pixelShader->SetData(
		"cameraPosition",
		&camera->GetPosition(),
		sizeof(DirectX::XMFLOAT3)
	);
	pixelShader->CopyAllBufferData();

	// Set the pixel shader normal with the lights
	pixelShaderNormal->SetData(
		"directionalLight",
		&light,
		sizeof(DirectionalLight)
	);
	pixelShaderNormal->SetData(
		"lightTwo",
		&lightTwo,
		sizeof(DirectionalLight)
	);
	pixelShaderNormal->SetData(
		"lightThree",
		&lightThree,
		sizeof(DirectionalLight)
	);
	pixelShaderNormal->SetData(
		"pointLight",
		&pointLight,
		sizeof(DirectionalLight)
	);
	pixelShaderNormal->SetData(
		"cameraPosition",
		&camera->GetPosition(),
		sizeof(DirectX::XMFLOAT3)
	);
	pixelShaderNormal->CopyAllBufferData();

	//Pixel shader PBR
	pixelShaderPBR->SetData(
		"directionalLight",
		&light,
		sizeof(DirectionalLight)
	);
	pixelShaderPBR->SetData(
		"lightTwo",
		&lightTwo,
		sizeof(DirectionalLight)
	);
	pixelShaderPBR->SetData(
		"lightThree",
		&lightThree,
		sizeof(DirectionalLight)
	);
	pixelShaderPBR->SetData(
		"pointLight",
		&pointLight,
		sizeof(DirectionalLight)
	);
	pixelShaderPBR->SetData(
		"cameraPosition",
		&camera->GetPosition(),
		sizeof(DirectX::XMFLOAT3)
	);
	pixelShaderPBR->CopyAllBufferData();


	//Pixel shader style
	stylizedPS->SetData(
		"directionalLight",
		&light,
		sizeof(DirectionalLight)
	);
	stylizedPS->SetData(
		"lightTwo",
		&lightTwo,
		sizeof(DirectionalLight)
	);
	stylizedPS->SetData(
		"lightThree",
		&lightThree,
		sizeof(DirectionalLight)
	);
	stylizedPS->SetData(
		"pointLight",
		&pointLight,
		sizeof(DirectionalLight)
	);
	stylizedPS->SetData(
		"cameraPosition",
		&camera->GetPosition(),
		sizeof(DirectX::XMFLOAT3)
	);
	
	

	

	stylizedPS->CopyAllBufferData();

	// Set buffers in the input assembler
	//  - Do this ONCE PER OBJECT you're drawing, since each object might
	//    have different geometry.
	//  - for this demo, this step *could* simply be done once during Init(),
	//    but I'm doing it here because it's often done multiple times per frame
	//    in a larger application/game
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	// Finally do the actual drawing
	//  - Do this ONCE PER OBJECT you intend to draw

	//Reset silhouette ID
	silhouetteID = 0;
	
	// increment because skybox uses ID 0
	silhouetteID++;
	topHatOne->GetMaterial()->GetPixelShader()->SetInt("silhouetteID", silhouetteID);
	topHatOne->Draw(context, stride, offset, camera);

	silhouetteID++;
	topHatTwo->GetMaterial()->GetPixelShader()->SetInt("silhouetteID", silhouetteID);
	topHatTwo->Draw(context, stride, offset, camera);
	
	silhouetteID++;
	cubeOne->GetMaterial()->GetPixelShader()->SetInt("silhouetteID", silhouetteID);
	cubeOne->Draw(context, stride, offset, camera);
	
	silhouetteID++;
	cubeTwo->GetMaterial()->GetPixelShader()->SetInt("silhouetteID", silhouetteID);
	cubeTwo->Draw(context, stride, offset, camera);
	
	silhouetteID++;
	triaOne->GetMaterial()->GetPixelShader()->SetInt("silhouetteID", silhouetteID);
	triaOne->Draw(context, stride, offset, camera);
	
	// Thomas Models
	silhouetteID++;
	tableOne->GetMaterial()->GetPixelShader()->SetInt("silhouetteID", silhouetteID);
	tableOne->Draw(context, stride, offset, camera);
	
	silhouetteID++;
	tableTwo->GetMaterial()->GetPixelShader()->SetInt("silhouetteID", silhouetteID);
	tableTwo->Draw(context, stride, offset, camera);
	
	silhouetteID++;
	chairOne->GetMaterial()->GetPixelShader()->SetInt("silhouetteID", silhouetteID);
	chairOne->Draw(context, stride, offset, camera);

	skyBox->Draw(context, camera);
	
	//Go back to rendering on the actual frame
	context->OMSetRenderTargets(1, backBufferRTV.GetAddressOf(), depthStencilView.Get());

	// Set up post process shaders
	postVS->SetShader();

	silhouettePS->SetShaderResourceView("pixels", postSRV.Get());
	silhouettePS->SetSamplerState("samplerOptions", sampleState.Get());
	silhouettePS->SetShader();

	silhouettePS->SetFloat("pixelWidth", 1.0f / (width / portion) ); // / portion
	silhouettePS->SetFloat("pixelHeight", 1.0f / (height / portion) );

	// Set Palette
	// are we allowed to use auto?
	auto PaletteUsed = &colorPalette;
	switch (paletteNumber)
	{
	case 0:
		PaletteUsed = &colorPalette;
		break;

	case 1:
		PaletteUsed = &newPalette;
		break;
	case 2:
		PaletteUsed = &bauhausPalette;
		break;
	default:
		PaletteUsed = &newPalette;
	}

	silhouettePS->SetData(
		"palette",
		PaletteUsed,
		sizeof(DirectX::XMFLOAT4) * 12
	);
	silhouettePS->CopyAllBufferData();

	context->IASetIndexBuffer(0, DXGI_FORMAT_R32_UINT, 0);
	ID3D11Buffer* nothing = 0;
	context->IASetVertexBuffers(0, 1, &nothing, &stride, &offset);

	// Draw exactly 3 vertices, which the special post-process vertex shader will
	// "figure out" on the fly (resulting in our "full screen triangle")
	context->Draw(3, 0);

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);

	// Due to the usage of a more sophisticated swap chain,
	// the render target must be re-bound after every call to Present()
	context->OMSetRenderTargets(1, backBufferRTV.GetAddressOf(), depthStencilView.Get());
	
}