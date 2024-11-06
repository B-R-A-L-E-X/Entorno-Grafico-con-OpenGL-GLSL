#include "Scene.h"

using namespace std;

Scene::Scene(OpenGLClass* OpenGLObject, HWND hwnd) {
	handlerWindow = hwnd;
	OpenGL = OpenGLObject;
	Camera = 0;
	LightShader = 0;
	ColorShader = 0;
	ShaderBounding = 0;
	Triangulo = 0;
	Light = 0;
	speedAxisX = 0.0f;
	speedAxisY = 0.0f;
	speed = 0.0f;
	angulo = 0.0f;
	Object3d = 0;
	LoaderTexture = new TextureClass(OpenGL);
}

Scene::~Scene() {

}

bool Scene::Initialize() {
	bool result = true;
	string resultDetail = "";

	angulo = 0.0f;
	speedAxisX = SPEED_AXIS_X;
	speedAxisY = SPEED_AXIS_Y;
	speed = SPEED_CAMERA;

	// Crea un objeto camara.
	Camera = new CameraClass;
	if (!Camera) {
		result = false;
		_RPT1(0, "Alert! Camera has not been initialized. \n", 0);
		return result;
	}
	else {
		// Asigna las coordenadas de posicion y rotacion por defecto a los arreglos delta.
		DeltaPosition->X = InitialPosition[0]; DeltaPosition->Y = InitialPosition[1]; DeltaPosition->Z = InitialPosition[2];
		DeltaRotation->X = InitialRotation[0]; DeltaRotation->Y = InitialRotation[1]; DeltaRotation->Z = InitialRotation[2];
		// Luego se los pasamos a la camara.
		Camera->SetPosition(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z);
		Camera->SetRotation(DeltaRotation->X, DeltaRotation->Y, DeltaRotation->Z);
		// Con esto podriamos aislar siempre el punto de partida inicial de la escena para evitar teletrasportarse sin querer.
	}

	LightShader = new LightShaderClass(OpenGL, handlerWindow, "shaders/terrain.vs", "shaders/terrain.ps");
	if (!LightShader) {
		result = false;
		MessageBoxA(handlerWindow, "Could not declared and initialized the light shader object.", "Error", MB_OK);
		_RPT1(0, "Alert! LightShader has an error on declare and not been initialized. \n", 0);
		return result;
	}

	ColorShader = new ColorShaderClass(OpenGL, handlerWindow, "shaders/color.vs", "shaders/color.ps");
	if (!ColorShader) {
		result = false;
		MessageBoxA(handlerWindow, "Could not declared and initialized the color shader object.", "Error", MB_OK);
		_RPT1(0, "Alert! ColorShader has an error on declare and not been initialized. \n", 0);
		return result;
	}

	//Terreno = new Land(handlerWindow, OpenGL, LoaderTexture, L"recursos/heightmap.jpg", "recursos/test.tga", "recursos/Piedras_normal.jpg",(float)400, (float)400, 0, 1);
	Terreno = new Land(handlerWindow, OpenGL, LoaderTexture, L"recursos/Terreno/terreno4.jpg", "recursos/Terreno/textura2.jpg", "recursos/Terreno/textura1.jpg", "recursos/Terreno/terreno3.jpg", (float)150, (float)150);
	if (!Terreno){
		result = false;
		_RPT1(0, "Alert! Land has not been initialized. \n", 0);
		return result;
	}
	else {
		Terreno->SetLandShader(LightShader);
	}

	agua = new Water(handlerWindow, OpenGL, LoaderTexture, L"recursos/Terreno/agua1.jpg", "recursos/Terreno/agua.jpg", "recursos/Terreno/agua.jpg", "recursos/Terreno/agua1.jpg", (float)150, (float)150);
	if (!agua) {
		result = false;
		_RPT1(0, "Alert! Land has not been initialized. \n", 0);
		return result;
	}
	else {
		agua->SetLandShader(LightShader);
	}

	//

	Triangulo = new TriangleClass(OpenGL);
	if (!Triangulo) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the triangle model object.", "Error", MB_OK);
		_RPT1(0, "Alert! Triangle has not been initialized. \n", 0);
		return result;
	}
	else {
		Triangulo->SetShaderTriangle(ColorShader);
	}

	ShaderModel = new ShaderTexture(OpenGL, handlerWindow, "shaders/light.vs", "shaders/light.ps");
	if (!ShaderModel) {
		result = false;
		MessageBoxA(handlerWindow, "Could not declared and initialized the shader model object.", "Error", MB_OK);
		_RPT1(0, "Alert! ShaderModel has an error on declare and not been initialized. \n", 0);
		return result;
	}

	ShaderBounding = new ShaderBoundingBox(OpenGL, handlerWindow, "shaders/vertexcolor.vs", "shaders/vertexcolor.ps");
	if (!ShaderBounding) {
		result = false;
		MessageBoxA(handlerWindow, "Could not declared and initialized the shader model object.", "Error", MB_OK);
		_RPT1(0, "Alert! ShaderModel has an error on declare and not been initialized. \n", 0);
		return result;
	}

	//OBJETOS
	/*
	Object3d = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/WarGreymon/WarGreymon.obj",
		"recursos/WarGreymon/WarGreymon.png");
	if (!Object3d) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Object3d->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Object3d->SetShaders(ShaderModel, ShaderBounding);
	}

	Object3d1 = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/Official Models/Tree1.obj",
		"recursos/Official Models/Tree1T1.jpg");
	if (!Object3d1) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Object3d1->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Object3d1->SetShaders(ShaderModel, ShaderBounding);
		Object3d1->AddTexture("recursos/Official Models/Tree1T2.jpg");
	}

	Object3d2 = new GameObject(OpenGL, handlerWindow, LoaderTexture, 
		"recursos/WarGreymon/WarGreymon.obj",
		"recursos/WarGreymon/WarGreymon.png");
	if (!Object3d2) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Object3d2->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Object3d2->SetShaders(ShaderModel, ShaderBounding);
	}
	*/
	//OBJETOS MIOS

	Casa1 = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/Modelos/CASA1/CASA1.obj",
		"recursos/Modelos/CASA1/casa1.png");
	if (!Casa1) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Casa1->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Casa1->SetShaders(ShaderModel, ShaderBounding);
	}

	Casa3 = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/Modelos/CASA1/CASA1.obj",
		"recursos/Modelos/CASA1/casa1.png");
	if (!Casa3) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Casa3->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Casa3->SetShaders(ShaderModel, ShaderBounding);
	}

	Casa4 = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/Modelos/CASA1/CASA1.obj",
		"recursos/Modelos/CASA1/casa1.png");
	if (!Casa4) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Casa4->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Casa4->SetShaders(ShaderModel, ShaderBounding);
	}

	Casa2 = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/Modelos/CASA2/CASA2.obj",
		"recursos/Modelos/CASA2/casa2.png");
	if (!Casa2) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Casa2->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Casa2->SetShaders(ShaderModel, ShaderBounding);
	}

	Banca1 = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/Modelos/Banca1/Banca1.obj",
		"recursos/Modelos/Banca1/Banca1.png");
	if (!Banca1) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Banca1->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Banca1->SetShaders(ShaderModel, ShaderBounding);
	}

	Banca3 = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/Modelos/Banca1/Banca1.obj",
		"recursos/Modelos/Banca1/Banca1.png");
	if (!Banca3) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Banca3->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Banca3->SetShaders(ShaderModel, ShaderBounding);
	}

	Banca2 = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/Modelos/Banca2/Banca2.obj",
		"recursos/Modelos/Banca2/Banca2.png");
	if (!Banca2) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Banca2->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Banca2->SetShaders(ShaderModel, ShaderBounding);
	}

	Banca4 = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/Modelos/Banca2/Banca2.obj",
		"recursos/Modelos/Banca2/Banca2.png");
	if (!Banca4) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Banca4->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Banca4->SetShaders(ShaderModel, ShaderBounding);
	}

	Fogata1 = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/Modelos/FOGATA/FOGATA.obj",
		"recursos/Modelos/FOGATA/fogata.jpg");
	if (!Fogata1) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Fogata1->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Fogata1->SetShaders(ShaderModel, ShaderBounding);
	}

	Fogata2 = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/Modelos/FOGATA/FOGATA.obj",
		"recursos/Modelos/FOGATA/fogata.jpg");
	if (!Fogata2) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Fogata2->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Fogata2->SetShaders(ShaderModel, ShaderBounding);
	}

	Moto = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/Modelos/Moto/Moto2.obj",
		"recursos/Modelos/Moto/Moto1.jpg");
	if (!Moto) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Moto->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Moto->SetShaders(ShaderModel, ShaderBounding);
	}


	// Skydome
	ShaderSky = new SkydomeShaderClass(OpenGL, handlerWindow, "shaders/SkydomeShader.vs", "shaders/SkydomeShader.ps");
	if (!ShaderSky) {
		result = false;
		MessageBoxA(handlerWindow, "Could not declared and initialized the shader skydome.", "Error", MB_OK);
		_RPT1(0, "Alert! ShaderModel has an error on declare and not been initialized. \n", 0);
		return result;
	}

	Skydome = new Dome("recursos/cielo.jpg", OpenGL, LoaderTexture, 500);
	if (!Skydome) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the Skydome.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Skydome->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the Skydome.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Skydome->SetIterpolationDay(false, true);
		Skydome->SetShader(ShaderSky);
	}

	// Collision Boxes
	box = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 7.0f, 5.0f, 65.0f }, BoundingBox::GLFLOAT3{ -7.0f, -5.0f, -65.0f });
	if (!box) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the box.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		box->SetShader(ShaderBounding);
	}

	box2 = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 51.5f, 10.0f, 1.0f }, BoundingBox::GLFLOAT3{ -51.5f, -10.0f, -1.0f });
	if (!box) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the box.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		box2->SetShader(ShaderBounding);
	}


	box3 = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 1.0f, 10.0f, 65.0f }, BoundingBox::GLFLOAT3{ -1.0f, -10.0f, -65.0f });
	if (!box3) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the box.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		box3->SetShader(ShaderBounding);
	}

	box4 = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 51.5f, 10.0f, 1.0f }, BoundingBox::GLFLOAT3{ -51.5f, -10.0f, -1.0f });
	if (!box4) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the box.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		box4->SetShader(ShaderBounding);
	}

	Pared1 = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 7.0f, 6.0f, 0.5f }, BoundingBox::GLFLOAT3{ -7.0f, -6.0f, -0.5f });
	if (!box4) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the box.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		Pared1->SetShader(ShaderBounding);
	}

	Pared2 = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 0.5f, 6.0f, 4.5f }, BoundingBox::GLFLOAT3{ -0.5f, -6.0f, -4.5f });
	if (!box4) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the box.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		Pared2->SetShader(ShaderBounding);
	}

	Pared3 = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 7.0f, 6.0f, 0.5f }, BoundingBox::GLFLOAT3{ -7.0f, -6.0f, -0.5f });
	if (!box4) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the box.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		Pared3->SetShader(ShaderBounding);
	}

	Pared4 = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 0.5f, 6.0f, 6.0f }, BoundingBox::GLFLOAT3{ -0.5f, -6.0f, -6.0f });
	if (!box4) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the box.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		Pared4->SetShader(ShaderBounding);
	}
	// Billboards
	ShaderBill = new BillboardShaderClass(OpenGL, handlerWindow, "shaders/billboard.vs", "shaders/billboard.ps");
	if (!ShaderBill) {
		result = false;
		MessageBoxA(handlerWindow, "Could not declared and initialized the shader billboard.", "Error", MB_OK);
		_RPT1(0, "Alert! ShaderModel has an error on declare and not been initialized. \n", 0);
		return result;
	}

	Arbol2 = new Billboard(OpenGL, handlerWindow, LoaderTexture, "recursos/Billboard/Arbol16.png");
	if (!Arbol2) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the billboard.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		Arbol2->Initialize(5.0f);
		Arbol2->SetShader(ShaderBill);
	}

	Arbol3 = new Billboard(OpenGL, handlerWindow, LoaderTexture, "recursos/Billboard/Arbol14.png");
	if (!Arbol3) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the billboard.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		Arbol3->Initialize(5.0f);
		Arbol3->SetShader(ShaderBill);
	}

	Arbol5 = new Billboard(OpenGL, handlerWindow, LoaderTexture, "recursos/Billboard/Arbol13.png");
	if (!Arbol5) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the billboard.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		Arbol5->Initialize(5.0f);
		Arbol5->SetShader(ShaderBill);
	}

	Arbol6 = new Billboard(OpenGL, handlerWindow, LoaderTexture, "recursos/Billboard/Arbol11.png");
	if (!Arbol6) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the billboard.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		Arbol6->Initialize(5.0f);
		Arbol6->SetShader(ShaderBill);
	}

	Arbol7 = new Billboard(OpenGL, handlerWindow, LoaderTexture, "recursos/Billboard/Arbol15.png");
	if (!Arbol7) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the billboard.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		Arbol7->Initialize(5.0f);
		Arbol7->SetShader(ShaderBill);
	}

	return result;
}

bool Scene::Render() {
	bool result = true;

	// Limpia los buffers para iniciar la escena nueva.
	OpenGL->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Definimos nuestro espacio para las 3 matrices necesarias para renderizar.
	float worldMatrix[16];
	float viewMatrix[16];
	float projectionMatrix[16];

	// Actualiza la posicion y rotacion de la camara
	if (tercerapersona == false) {
		Camera->SetPosition(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z);
	}
	else
	{
		Camera->SetPosition(DeltaPosition->X, DeltaPosition->Y +1.0f, DeltaPosition->Z);
	}


	Camera->SetRotation(DeltaRotation->X, DeltaRotation->Y+4, DeltaRotation->Z);

	// Genera la matriz de vista a partir de la posicion de la camara.
	Camera->Render();

	// Obtiene las matrices de vista, mundo y proyeccion de opengl y el objeto de la camara.
	OpenGL->GetWorldMatrix(worldMatrix);
	Camera->GetViewMatrix(viewMatrix);
	OpenGL->GetProjectionMatrix(projectionMatrix);

	// Renderizamos el Skydome
	Skydome->CicloInterpolaciones();
	Skydome->Render(viewMatrix, projectionMatrix);

	// Renderizamos terreno
	Terreno->Render(OpenGL, worldMatrix, viewMatrix, projectionMatrix);
	agua->Render(OpenGL, viewMatrix, projectionMatrix);

	// Renderizamos Triangulo (ya se logra ver en escena)
	//Triangulo->Render(viewMatrix, projectionMatrix);

	//MIS OBJETOS
	
	Casa1 ->Render(viewMatrix, projectionMatrix, false);
	Casa2->Render(viewMatrix, projectionMatrix, false);
	Casa3->Render(viewMatrix, projectionMatrix, false);
	Casa4->Render(viewMatrix, projectionMatrix, false);
	Banca1->Render(viewMatrix, projectionMatrix, false);
	Banca2->Render(viewMatrix, projectionMatrix, false);
	Banca3->Render(viewMatrix, projectionMatrix, false);
	Banca4->Render(viewMatrix, projectionMatrix, false);

	Fogata1->Render(viewMatrix, projectionMatrix, false);
	Fogata2->Render(viewMatrix, projectionMatrix, false);

	Moto->Render(viewMatrix, projectionMatrix, false);

	// Renderizamos las cajas de colisión
	//box->Draw(viewMatrix, projectionMatrix);
	//box2->Draw(viewMatrix, projectionMatrix);
	//box3->Draw(viewMatrix, projectionMatrix);
	//box4->Draw(viewMatrix, projectionMatrix);

	//Pared1->Draw(viewMatrix, projectionMatrix);
	//Pared2->Draw(viewMatrix, projectionMatrix);
	//Pared3->Draw(viewMatrix, projectionMatrix);
	//Pared4->Draw(viewMatrix, projectionMatrix);

	// Renderizamos los billboards
	//arbol2D->Render(viewMatrix, projectionMatrix, 
		//0.0f, Terreno->Superficie(0.0f, 0.0f), 0.0f, 
		//DeltaPosition->X, DeltaPosition->Z);

	//esquina superior
	Arbol2->Render(viewMatrix, projectionMatrix,
		2.0f, Terreno->Superficie(0.0f, 0.0f), 52.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol2->Render(viewMatrix, projectionMatrix,
		10.0f, Terreno->Superficie(0.0f, 0.0f), 52.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol2->Render(viewMatrix, projectionMatrix,
		18.0f, Terreno->Superficie(0.0f, 0.0f), 52.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol2->Render(viewMatrix, projectionMatrix,
		26.0f, Terreno->Superficie(0.0f, 0.0f), 52.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol2->Render(viewMatrix, projectionMatrix,
		34.0f, Terreno->Superficie(0.0f, 0.0f), 52.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol2->Render(viewMatrix, projectionMatrix,
		42.0f, Terreno->Superficie(0.0f, 0.0f), 52.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol2->Render(viewMatrix, projectionMatrix,
		42.0f, Terreno->Superficie(0.0f, 0.0f), 44.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol2->Render(viewMatrix, projectionMatrix,
		42.0f, Terreno->Superficie(0.0f, 0.0f), 36.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol2->Render(viewMatrix, projectionMatrix,
		42.0f, Terreno->Superficie(0.0f, 0.0f), 28.0f,
		DeltaPosition->X, DeltaPosition->Z);

	Arbol3->Render(viewMatrix, projectionMatrix,
		-2.0f, Terreno->Superficie(0.0f, 0.0f), 58.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol3->Render(viewMatrix, projectionMatrix,
		6.0f, Terreno->Superficie(0.0f, 0.0f), 58.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol3->Render(viewMatrix, projectionMatrix,
		14.0f, Terreno->Superficie(0.0f, 0.0f), 58.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol3->Render(viewMatrix, projectionMatrix,
		22.0f, Terreno->Superficie(0.0f, 0.0f), 58.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol3->Render(viewMatrix, projectionMatrix,
		30.0f, Terreno->Superficie(0.0f, 0.0f), 58.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol3->Render(viewMatrix, projectionMatrix,
		38.0f, Terreno->Superficie(0.0f, 0.0f), 58.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol3->Render(viewMatrix, projectionMatrix,
		46.0f, Terreno->Superficie(0.0f, 0.0f), 58.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol3->Render(viewMatrix, projectionMatrix,
		54.0f, Terreno->Superficie(0.0f, 0.0f), 58.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol3->Render(viewMatrix, projectionMatrix,
		54.0f, Terreno->Superficie(0.0f, 0.0f), 50.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol3->Render(viewMatrix, projectionMatrix,
		54.0f, Terreno->Superficie(0.0f, 0.0f), 42.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol3->Render(viewMatrix, projectionMatrix,
		54.0f, Terreno->Superficie(0.0f, 0.0f), 34.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol3->Render(viewMatrix, projectionMatrix,
		54.0f, Terreno->Superficie(0.0f, 0.0f), 26.0f,
		DeltaPosition->X, DeltaPosition->Z);

	// del otro lado del rio

	Arbol5->Render(viewMatrix, projectionMatrix,
		-56.0f, Terreno->Superficie(0.0f, 0.0f), -56.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol5->Render(viewMatrix, projectionMatrix,
		-56.0f, Terreno->Superficie(0.0f, 0.0f), -48.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol5->Render(viewMatrix, projectionMatrix,
		-56.0f, Terreno->Superficie(0.0f, 0.0f), -40.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol5->Render(viewMatrix, projectionMatrix,
		-56.0f, Terreno->Superficie(0.0f, 0.0f), -32.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol5->Render(viewMatrix, projectionMatrix,
		-56.0f, Terreno->Superficie(0.0f, 0.0f), -24.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol5->Render(viewMatrix, projectionMatrix,
		-56.0f, Terreno->Superficie(0.0f, 0.0f), -16.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol5->Render(viewMatrix, projectionMatrix,
		-56.0f, Terreno->Superficie(0.0f, 0.0f), -8.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol5->Render(viewMatrix, projectionMatrix,
		-56.0f, Terreno->Superficie(0.0f, 0.0f), 0.0f,
		DeltaPosition->X, DeltaPosition->Z);

	Arbol5->Render(viewMatrix, projectionMatrix,
		-56.0f, Terreno->Superficie(0.0f, 0.0f), 56.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol5->Render(viewMatrix, projectionMatrix,
		-56.0f, Terreno->Superficie(0.0f, 0.0f), 48.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol5->Render(viewMatrix, projectionMatrix,
		-56.0f, Terreno->Superficie(0.0f, 0.0f), 40.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol5->Render(viewMatrix, projectionMatrix,
		-56.0f, Terreno->Superficie(0.0f, 0.0f), 32.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol5->Render(viewMatrix, projectionMatrix,
		-56.0f, Terreno->Superficie(0.0f, 0.0f), 24.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol5->Render(viewMatrix, projectionMatrix,
		-56.0f, Terreno->Superficie(0.0f, 0.0f), 16.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol5->Render(viewMatrix, projectionMatrix,
		-56.0f, Terreno->Superficie(0.0f, 0.0f), 8.0f,
		DeltaPosition->X, DeltaPosition->Z);

	//
	Arbol2->Render(viewMatrix, projectionMatrix,
		-36.0f, Terreno->Superficie(0.0f, 0.0f), -56.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol2->Render(viewMatrix, projectionMatrix,
		-28.0f, Terreno->Superficie(0.0f, 0.0f), -56.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol2->Render(viewMatrix, projectionMatrix,
		-20.0f, Terreno->Superficie(0.0f, 0.0f), -56.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol2->Render(viewMatrix, projectionMatrix,
		-12.0f, Terreno->Superficie(0.0f, 0.0f), -56.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol2->Render(viewMatrix, projectionMatrix,
		-4.0f, Terreno->Superficie(0.0f, 0.0f), -56.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol2->Render(viewMatrix, projectionMatrix,
		4.0f, Terreno->Superficie(0.0f, 0.0f), -56.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol2->Render(viewMatrix, projectionMatrix,
		12.0f, Terreno->Superficie(0.0f, 0.0f), -56.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol2->Render(viewMatrix, projectionMatrix,
		20.0f, Terreno->Superficie(0.0f, 0.0f), -56.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol2->Render(viewMatrix, projectionMatrix,
		28.0f, Terreno->Superficie(0.0f, 0.0f), -56.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol2->Render(viewMatrix, projectionMatrix,
		36.0f, Terreno->Superficie(0.0f, 0.0f), -56.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol2->Render(viewMatrix, projectionMatrix,
		44.0f, Terreno->Superficie(0.0f, 0.0f), -56.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol2->Render(viewMatrix, projectionMatrix,
		52.0f, Terreno->Superficie(0.0f, 0.0f), -56.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol2->Render(viewMatrix, projectionMatrix,
		52.0f, Terreno->Superficie(0.0f, 0.0f), -48.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol2->Render(viewMatrix, projectionMatrix,
		52.0f, Terreno->Superficie(0.0f, 0.0f), -40.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol2->Render(viewMatrix, projectionMatrix,
		52.0f, Terreno->Superficie(0.0f, 0.0f), -32.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol2->Render(viewMatrix, projectionMatrix,
		52.0f, Terreno->Superficie(0.0f, 0.0f), -24.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol2->Render(viewMatrix, projectionMatrix,
		52.0f, Terreno->Superficie(0.0f, 0.0f), -16.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol2->Render(viewMatrix, projectionMatrix,
		52.0f, Terreno->Superficie(0.0f, 0.0f), -8.0f,
		DeltaPosition->X, DeltaPosition->Z);
	//

	Arbol6->Render(viewMatrix, projectionMatrix,
		7.0f, Terreno->Superficie(0.0f, 0.0f), -3.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol7->Render(viewMatrix, projectionMatrix,
		14.0f, Terreno->Superficie(0.0f, 0.0f), -13.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol6->Render(viewMatrix, projectionMatrix,
		26.0f, Terreno->Superficie(0.0f, 0.0f), -19.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol7->Render(viewMatrix, projectionMatrix,
		-15.0f, Terreno->Superficie(0.0f, 0.0f), -1.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol6->Render(viewMatrix, projectionMatrix,
		29.0f, Terreno->Superficie(0.0f, 0.0f), 35.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol7->Render(viewMatrix, projectionMatrix,
		-26.0f, Terreno->Superficie(0.0f, 0.0f), 35.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol6->Render(viewMatrix, projectionMatrix,
		-10.0f, Terreno->Superficie(0.0f, 0.0f), 27.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol7->Render(viewMatrix, projectionMatrix,
		9.0f, Terreno->Superficie(0.0f, 0.0f), 21.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol6->Render(viewMatrix, projectionMatrix,
		-9.0f, Terreno->Superficie(0.0f, 0.0f), 16.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol7->Render(viewMatrix, projectionMatrix,
		-29.0f, Terreno->Superficie(0.0f, 0.0f), 0.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol6->Render(viewMatrix, projectionMatrix,
		-26.0f, Terreno->Superficie(0.0f, 0.0f), -20.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol7->Render(viewMatrix, projectionMatrix,
		-11.0f, Terreno->Superficie(0.0f, 0.0f), -20.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol6->Render(viewMatrix, projectionMatrix,
		-7.0f, Terreno->Superficie(0.0f, 0.0f), -40.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol7->Render(viewMatrix, projectionMatrix,
		13.0f, Terreno->Superficie(0.0f, 0.0f), -40.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol6->Render(viewMatrix, projectionMatrix,
		7.0f, Terreno->Superficie(0.0f, 0.0f), 36.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol7->Render(viewMatrix, projectionMatrix,
		28.0f, Terreno->Superficie(0.0f, 0.0f), 18.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol6->Render(viewMatrix, projectionMatrix,
		-27.0f, Terreno->Superficie(0.0f, 0.0f), 17.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol7->Render(viewMatrix, projectionMatrix,
		17.0f, Terreno->Superficie(0.0f, 0.0f), 12.0f,
		DeltaPosition->X, DeltaPosition->Z);
	Arbol6->Render(viewMatrix, projectionMatrix,
		32.0f, Terreno->Superficie(0.0f, 0.0f), -5.0f,
		DeltaPosition->X, DeltaPosition->Z);

	
	// Damos la instruccion de que termino la escena para que nos muestre frame a frame.
	OpenGL->EndScene();

	return result;
}

bool Scene::Update(InputClass* input, float deltaTime) {
	bool result = true;

	this->input = input;
	this->deltaTime = deltaTime;
	result = ManageCommands();
	if (!result) {
		result = false;
		_RPT1(0, "Alert! Error on key. \n", 0);
		return result;
	}
	float* auxMatrix = new float[16] { 0.0f };
	OpenGL->BuildIdentityMatrix(auxMatrix);
	/*
	float* matrixTriangle = Triangulo->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixTriangle, -20.0f, 8.0f, 0.0f);
	
	float* matrixGameObject = Object3d->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject, -20.0f, 6.0f, -10.0f);

	float* matrixGameObject1 = Object3d1->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject1, -30.0f, 6.0f, -10.0f);

	float* matrixGameObject2 = Object3d2->GetWorldMatrix();
	OpenGL->MatrixScale(matrixGameObject2, 2.0f, 2.0f, 2.0f);
	OpenGL->MatrixTranslation(auxMatrix, -40.0f, 6.0f, -10.0f);
	OpenGL->MatrixMultiply(matrixGameObject2, matrixGameObject2, auxMatrix);
	*/
	//MIS OBJETOS
	
	float* matrixGameCasa1 = Casa1->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameCasa1, -17.0f, 10.6f, 50.0f);

	float* matrixGameCasa2 = Casa2->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameCasa2, 46.0f, 10.6f, 6.0f);

	float* matrixGameCasa3 = Casa3->GetWorldMatrix();
	OpenGL->MatrixRotationY(matrixGameCasa3, 180*(M_PI/180));
	OpenGL->MatrixTranslation(auxMatrix, -20.0f, 10.6f, -39.0f);
	OpenGL->MatrixMultiply(matrixGameCasa3, matrixGameCasa3, auxMatrix);

	float* matrixGameCasa4 = Casa4->GetWorldMatrix();
	OpenGL->MatrixRotationY(matrixGameCasa4, 180 * (M_PI / 180));
	OpenGL->MatrixTranslation(auxMatrix, 32.0f, 10.6f, -39.0f);
	OpenGL->MatrixMultiply(matrixGameCasa4, matrixGameCasa4, auxMatrix);

	float* matrixGameBanca1 = Banca1->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameBanca1, 41.0f, 10.6f, -8.0f);

	float* matrixGameBanca2 = Banca2->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameBanca2, 48.0f, 10.6f, 18.0f);

	float* matrixGameBanca3 = Banca3->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameBanca3, -31.0f, 10.6f, 52.0f);

	float* matrixGameBanca4 = Banca4->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameBanca4, -3.0f, 10.6f, 48.0f);

	float* matrixGameFogata1 = Fogata1->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameFogata1, 42.0f, 10.6f, 19.0f);

	float* matrixGameFogata2 = Fogata2->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameFogata2,-7.0f, 10.6f, 49.0f);

	

	if (tercerapersona == false) {
		float* matrixGameMoto = Moto->GetWorldMatrix();
		OpenGL->MatrixTranslation(matrixGameMoto, 0.0f, 12.6f, -39.0f);
	}
	else {
		float* matrixGameMoto_1 = Moto->GetWorldMatrix();
		OpenGL->MatrixRotationY(matrixGameMoto_1, DeltaRotation->Y*(M_PI/180));
		OpenGL->MatrixTranslation(auxMatrix, DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z);
		OpenGL->MatrixMultiply(matrixGameMoto_1, matrixGameMoto_1, auxMatrix);

	}

	//Tranformaciones de cajas de colisión
	

	float* matrixBox = box->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixBox, -47.0f, 12.0f, 1.0f);

	//OpenGL->MatrixScale(auxMatrix, 1.0f, 1.0f, 0.3f);
	//OpenGL->MatrixMultiply(matrixBox, matrixBox, auxMatrix);

	float* matrixBox2 = box2->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixBox2, 11.0f, 12.6f, 65.0f);

	float* matrixBox3 = box3->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixBox3, 63.0f, 12.6f, 1.0f);

	float* matrixBox4 = box4->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixBox4, 11.0f, 12.6f, -65.0f);

	float* matrixPared1 = Pared1->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixPared1, 49.0f, 12.6f, 12.25f);

	float* matrixPared2 = Pared2->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixPared2, 43.0f, 12.6f, 7.5f);

	float* matrixPared3 = Pared3->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixPared3, 49.0f, 12.6f, 0.25f);

	float* matrixPared4 = Pared4->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixPared4, 56.5f, 12.6f, 6.0f);

	//Colisión por esfera
	/*if (Object3d->GetSphericalCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z, 2)) {
		MessageBox(handlerWindow, L"Colisionando", L"Aviso", MB_OK);
	}*/

	//Colisión por caja
	/*
	if (Object3d->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (Object3d2->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}
	*/
	
	if (box->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (box2->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (box3->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (box4->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}
	
	if (Casa1->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (Pared1->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}
	if (Pared2->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}
	if (Pared3->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}
	if (Pared4->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	/*if (Casa2->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}*/

	if (Casa3->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (Casa4->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (Banca1->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (Banca2->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (Banca3->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (Banca4->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (Fogata1->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (Fogata2->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}
	if (tercerapersona == false) 
	{
		if (Moto->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
			DeltaPosition->X = LastDeltaPosition->X;
			DeltaPosition->Y = LastDeltaPosition->Y;
			DeltaPosition->Z = LastDeltaPosition->Z;
		}
	}
	

	return result;
}

bool Scene::ManageCommands() {
	bool result = true;
	int i = 2;
	//Guardar la posición de la Cámara antes de actualizarla
	LastDeltaPosition->X = DeltaPosition->X;
	LastDeltaPosition->Y = DeltaPosition->Y;
	LastDeltaPosition->Z = DeltaPosition->Z;

	float vr[2]{ 0,0 };
	float radians = DeltaRotation->Y * 0.0174532925f;

	//
	if (input->GetKey(KeyCode.F10)) {
		std::string info = "X: " + std::to_string(DeltaPosition->X)
			+ " Y: " + std::to_string(DeltaPosition->Y)
			+ " Z: " + std::to_string(DeltaPosition->Z);

		int bufferLength = MultiByteToWideChar(CP_UTF8, 0, info.c_str(), -1, NULL, 0);

		wchar_t* wideInfo = new wchar_t[bufferLength];

		MultiByteToWideChar(CP_UTF8, 0, info.c_str(), -1, wideInfo, bufferLength);

		MessageBox(NULL, wideInfo, L"Coordenadas", MB_OK);

		delete wideInfo;
	}
	//

	if((input->GetKey(KeyCode.W) || input->GetKey(KeyCode.Up))) {
		vr[0] += sinf(radians)*speed * deltaTime;
		vr[1] += cosf(radians) * speed * deltaTime;
	}
	if ((input->GetKey(KeyCode.S) || input->GetKey(KeyCode.Down))) {
		vr[0] -= sinf(radians) * speed * deltaTime;
		vr[1] -= cosf(radians) * speed * deltaTime;
	}
	if ((input->GetKey(KeyCode.A) || input->GetKey(KeyCode.Left))) {
		vr[0] -= sinf(radians + (M_PI * 0.5f)) * speed * deltaTime;
		vr[1] -= cosf(radians + (M_PI * 0.5f)) * speed * deltaTime;
	}
	if ((input->GetKey(KeyCode.D) || input->GetKey(KeyCode.Right))) {
		vr[0] += sinf(radians + (M_PI * 0.5f)) * speed * deltaTime;
		vr[1] += cosf(radians + (M_PI * 0.5f)) * speed * deltaTime;
	}

	if (input->GetRightAxisY() || input->GetRightAxisX()) {
		float angleX = input->GetRightAxisY() * speedAxisX * deltaTime;
		float angleY = input->GetRightAxisX() * speedAxisY * deltaTime;
		DeltaRotation->X -= angleX;
		DeltaRotation->Y += angleY;
	}

	if (input->GetKey(KeyCode.E)) {
		tercerapersona = !tercerapersona;  // Cambia entre verdadero y falso al presionar E
	}

	if (input->GetKey(KeyCode.Space)) {
		DeltaPosition->Y = DeltaPosition->Y + i;
		//DeltaRotation->X -= speedAxisX * deltaTime * 3;
		i = i + 1;
	}
	if (input->GetKey(KeyCode.Control)) {
		DeltaPosition->Y = DeltaPosition->Y - i;
	//	DeltaRotation->X += speedAxisX * deltaTime * 3;
		i = i - 1;
	}



	if (input->GetKey(KeyCode.I)) {
		DeltaRotation->X -= speedAxisX * deltaTime;
	}
	if (input->GetKey(KeyCode.K)) {
		DeltaRotation->X += speedAxisX * deltaTime;
	}
	if (input->GetKey(KeyCode.J)) {
		DeltaRotation->Y -= speedAxisY * deltaTime;
	}
	if (input->GetKey(KeyCode.L)) {
		DeltaRotation->Y += speedAxisY * deltaTime;
	}
	if (input->GetKey(KeyCode.One)) {
		Object3d1->ChangeTexture(0,0);
	}
	if (input->GetKey(KeyCode.Two)) {
		Object3d1->ChangeTexture(1, 1);
	}
	if (input->GetKey(KeyCode.Three)) {
		Object3d1->ChangeTexture(2, 2);
	}
	if (input->GetKey(KeyCode.Four)) {
		Object3d1->ChangeTexture(3, 3);
	}
	if (input->GetKey(KeyCode.Five)) {
		Object3d1->ChangeTexture(4, 4);
	}
	if (input->GetKey(KeyCode.Six)) {
		Object3d1->ChangeTexture(5, 5);
	}
	if (input->GetKey(KeyCode.Seven)) {
		Object3d1->ChangeTexture(6, 6);
	}
	if (input->GetKey(KeyCode.Eight)) {
		Object3d1->ChangeTexture(7, 7);
	}
	if (input->GetKey(KeyCode.Nine)) {
		Object3d1->ChangeTexture(8, 8);
	}
	if (input->GetKey(KeyCode.Zero)) {
		Object3d1->ChangeTexture(9, 9);
	}

	if (!tercerapersona) {
		DeltaPosition->Y = Terreno->Superficie(DeltaPosition->X, DeltaPosition->Z) + 2;
	}

	//DeltaPosition->Y = Terreno->Superficie(DeltaPosition->X, DeltaPosition->Z) + 2;

	DeltaPosition->X += vr[0];
	DeltaPosition->Z += vr[1];

	return result;
}

bool Scene::Shutdown() {
	bool result = true;

	if (Camera)
	{
		delete Camera;
		Camera = 0;
	}

	if (LightShader)
	{
		LightShader->Shutdown();
		delete LightShader;
		LightShader = 0;
	}

	if (Triangulo) {
		Triangulo->Shutdown();
		delete Triangulo;
		Triangulo = 0;
	}
	/*
	if (Object3d) {
		Object3d->Shutdown();
		delete Object3d;
		Object3d = 0;
	}
	*/
	if (Casa1) {
		Casa1->Shutdown();
		delete Casa1;
		Casa1 = 0;
	}

	if (Casa2) {
		Casa2->Shutdown();
		delete Casa2;
		Casa2 = 0;
	}

	if (Casa3) {
		Casa3->Shutdown();
		delete Casa3;
		Casa3 = 0;
	}

	if (Casa4) {
		Casa4->Shutdown();
		delete Casa4;
		Casa4 = 0;
	}

	if (Banca1) {
		Banca1->Shutdown();
		delete Banca1;
		Banca1 = 0;
	}

	if (Banca2) {
		Banca2->Shutdown();
		delete Banca2;
		Banca2 = 0;
	}

	if (Banca3) {
		Banca3->Shutdown();
		delete Banca3;
		Banca3 = 0;
	}

	if (Banca4) {
		Banca4->Shutdown();
		delete Banca4;
		Banca4 = 0;
	}

	if (Fogata1) {
		Fogata1->Shutdown();
		delete Fogata1;
		Fogata1 = 0;
	}

	if (Fogata2) {
		Fogata2->Shutdown();
		delete Fogata2;
		Fogata2 = 0;
	}

	if (Arbol2) {
		Arbol2->Shutdown();
		delete Arbol2;
		Arbol2 = 0;
	}

	if (Arbol3) {
		Arbol3->Shutdown();
		delete Arbol3;
		Arbol3 = 0;
	}

	if (Arbol5) {
		Arbol5->Shutdown();
		delete Arbol5;
		Arbol5 = 0;
	}

	if (Moto) {
		Moto->Shutdown();
		delete Moto;
		Moto = 0;
	}

	return result;
}