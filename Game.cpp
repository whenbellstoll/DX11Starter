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

	delete topHatOne;
	delete topHatTwo;
	delete cubeOne;
	delete cubeTwo;
	delete triaOne;

	delete camera;

	delete defaultMaterial;
	delete redMaterial;

	delete vertexShader;
	delete pixelShader;
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

	// Set up the vertices of the triangle we would like to draw
	// - We're going to copy this array, exactly as it exists in memory
	//    over to a DirectX-controlled data structure (the vertex buffer)
	// - Note: Since we don't have a camera or really any concept of
	//    a "3d world" yet, we're simply describing positions within the
	//    bounds of how the rasterizer sees our screen: [-1 to +1] on X and Y
	// - This means (0,0) is at the very center of the screen.
	// - These are known as "Normalized Device Coordinates" or "Homogeneous 
	//    Screen Coords", which are ways to describe a position without
	//    knowing the exact size (in pixels) of the image/window/etc.  
	// - Long story short: Resizing the window also resizes the triangle,
	//    since we're describing the triangle in terms of the window itself
	Vertex vertices[] =
	{
		{ XMFLOAT3(+0.8f, +0.25f, +0.0f), red, UV },
		{ XMFLOAT3(+0.95f, -0.15f, +0.0f), red, UV },
		{ XMFLOAT3(+0.75f, -0.05f, +0.0f), red, UV },
	};

	// Set up the indices, which tell us which vertices to use and in which order
	// - This is somewhat redundant for just 3 vertices (it's a simple example)
	// - Indices are technically not required if the vertices are in the buffer 
	//    in the correct order and each one will be used exactly once

	unsigned int indices[] = { 0, 1, 2 };

	Vertex hat[] =
	{
		{ XMFLOAT3(-0.15f, +0.35f, +0.0f), red, UV },
		{ XMFLOAT3(+0.15f, -0.35f, +0.0f), red, UV },
		{ XMFLOAT3(-0.15f, -0.35f, +0.0f), black, UV },
		{ XMFLOAT3(+0.15f, +0.35f, +0.0f), black, UV },
		{ XMFLOAT3(+0.30f, -0.50f, +0.0f), black, UV },
		{ XMFLOAT3(-0.30f, -0.50f, +0.0f), black, UV },
	};
	unsigned int hatI[] = { 0, 1, 2, 3, 1, 0, 2, 1, 5, 4, 5, 1 };


	// Getting ambitious, making a 3D object
	Vertex cube[] =
	{
		{ XMFLOAT3(-0.75f, +0.35f, +0.24f), white, UV },
		{ XMFLOAT3(-0.75f, +0.10f, +0.24f), white, UV },
		{ XMFLOAT3(-0.62f, +0.45f, +0.36f), red, UV },
		{ XMFLOAT3(-0.62f, +0.10f, +0.36f), white, UV },
		{ XMFLOAT3(-0.62f, +0.35f, +0.12f), white, UV },
		{ XMFLOAT3(-0.62f, +0.00f, +0.12f), red, UV },
		{ XMFLOAT3(-0.50f, +0.35f, +0.24f), white, UV },
		{ XMFLOAT3(-0.50f, +0.10f, +0.24f), white, UV },
	};
	unsigned int cubeI[] = { 0, 1, 2, 2, 1, 3, 2, 3, 7, 6, 2, 7, 4, 6, 7, 4, 7, 5, 0, 4, 5, 0, 5, 1, 2, 6, 4, 2, 4, 0, 3, 7, 5, 3, 5, 1 };

	//create Mesh objects
	triangle = new Mesh(vertices, 3, indices, 3, device);
	topHat = new Mesh(GetFullPathTo("../../Assets/Models/sphere.obj").c_str(), device);
	cubeMesh = new Mesh(cube, 8, cubeI, 36, device);

	// create Material
	defaultMaterial = new Material(XMFLOAT4(1, 1, 1, 0), pixelShader, vertexShader);
	redMaterial = new Material(XMFLOAT4(1, 0, 0, 0), pixelShader, vertexShader);

	topHatOne = new GameEntity(topHat, defaultMaterial);
	topHatTwo = new GameEntity(topHat, redMaterial);
	cubeOne = new GameEntity(cubeMesh, defaultMaterial);
	cubeTwo = new GameEntity(cubeMesh, redMaterial);
	triaOne = new GameEntity(triangle, defaultMaterial);
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
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();

	//Update the camera
	camera->Update(deltaTime, hWnd);


	// Update the objects

	// cube one rotates about the x-axis
	cubeOne->GetTransform()->Rotate(0.1f * deltaTime, 0, 0);

	// cube two rotates the other way and moves right
	cubeTwo->GetTransform()->Rotate(-0.1f * deltaTime, 0, 0);
	cubeTwo->GetTransform()->MoveAbsolute( 0.01f * deltaTime, 0, 0);

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
	topHatTwo->GetTransform()->MoveAbsolute( -0.01f * deltaTime, 0, 0.01f * deltaTime);
	

	// triangle just kinda schmoves
	triaOne->GetTransform()->Rotate(0, 0.1f * deltaTime, 0);



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

	

	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	


	// Ensure the pipeline knows how to interpret the data (numbers)
	// from the vertex buffer.  
	// - If all of your 3D models use the exact same vertex layout,
	//    this could simply be done once in Init()
	// - However, this isn't always the case (but might be for this course)
	context->IASetInputLayout(inputLayout.Get());


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
	topHatOne->Draw(context, stride, offset, camera);
	topHatTwo->Draw(context, stride, offset, camera);
	cubeOne->Draw(context, stride, offset, camera);
	cubeTwo->Draw(context, stride, offset, camera);
	triaOne->Draw(context, stride, offset, camera);


	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);

	// Due to the usage of a more sophisticated swap chain,
	// the render target must be re-bound after every call to Present()
	context->OMSetRenderTargets(1, backBufferRTV.GetAddressOf(), depthStencilView.Get());
}