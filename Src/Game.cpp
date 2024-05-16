#include "Game.h"
#include "Box2DHelper.h"
#include <iostream>

// Constructor de la clase Game
Game::Game(int ancho, int alto, std::string titulo)
{
    // Inicializaci�n de la ventana y configuraci�n de propiedades
    wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
    wnd->setVisible(true);
    fps = 60;
    wnd->setFramerateLimit(fps);
    frameTime = 1.0f / fps;
    SetZoom(); // Configuraci�n de la vista del juego
    InitPhysics(); // Inicializaci�n del motor de f�sica
}

// Bucle principal del juego
void Game::Loop()
{
    while (wnd->isOpen())
    {
        wnd->clear(clearColor); // Limpiar la ventana
        DoEvents(); // Procesar eventos de entrada
        CheckCollitions(); // Comprobar colisiones
        UpdatePhysics(); // Actualizar la simulaci�n f�sica
        DrawGame(); // Dibujar el juego
        wnd->display(); // Mostrar la ventana
    }
}

// Actualizaci�n de la simulaci�n f�sica
void Game::UpdatePhysics()
{
    phyWorld->Step(frameTime, 8, 8); // Simular el mundo f�sico
    phyWorld->ClearForces(); // Limpiar las fuerzas aplicadas a los cuerpos
    phyWorld->DebugDraw(); // Dibujar el mundo f�sico para depuraci�n
}

// Dibujo de los elementos del juego
void Game::DrawGame()
{
}

// Procesamiento de eventos de entrada
void Game::DoEvents()
{
    Event evt;
    while (wnd->pollEvent(evt))
    {
        switch (evt.type)
        {
        case Event::Closed:
            wnd->close(); // Cerrar la ventana si se presiona el bot�n de cerrar
            break;
        }
    }
}

// Comprobaci�n de colisiones (a implementar m�s adelante)
void Game::CheckCollitions()
{
    // Implementaci�n de la comprobaci�n de colisiones
}

// Configuraci�n de la vista del juego
void Game::SetZoom()
{
    View camara;
    // Posicionamiento y tama�o de la vista
    camara.setSize(100.0f, 100.0f);
    camara.setCenter(50.0f, 50.0f);
    wnd->setView(camara); // Asignar la vista a la ventana
}

// Inicializaci�n del motor de f�sica y los cuerpos del mundo f�sico
void Game::InitPhysics()
{
    // Inicializar el mundo f�sico con la gravedad por defecto
    phyWorld = new b2World(b2Vec2(5.3f, 9.8f));

    // Crear un renderer de debug para visualizar el mundo f�sico
    debugRender = new SFMLRenderer(wnd);
    debugRender->SetFlags(UINT_MAX);
    phyWorld->SetDebugDraw(debugRender);

    // Crear el suelo y las paredes est�ticas del mundo f�sico
    b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
    groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

    b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
    leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

    b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
    rightWallBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);

    b2BodyDef boundaryDef;
    b2Body* boundaryBody;

    // L�mite superior
    boundaryDef.position.Set(50.0f, 0.0f);
    boundaryBody = phyWorld->CreateBody(&boundaryDef);
    b2PolygonShape topBoundary;
    topBoundary.SetAsBox(50.0f, 0.5f); // Ancho de la ventana
    boundaryBody->CreateFixture(&topBoundary, 0.0f);

    // L�mite inferior
    boundaryDef.position.Set(50.0f, 100.0f);
    boundaryBody = phyWorld->CreateBody(&boundaryDef);
    b2PolygonShape bottomBoundary;
    bottomBoundary.SetAsBox(50.0f, 0.5f); // Ancho de la ventana
    boundaryBody->CreateFixture(&bottomBoundary, 0.0f);

    // L�mite izquierdo
    boundaryDef.position.Set(0.0f, 50.0f);
    boundaryBody = phyWorld->CreateBody(&boundaryDef);
    b2PolygonShape leftBoundary;
    leftBoundary.SetAsBox(0.5f, 50.0f); // Alto de la ventana
    boundaryBody->CreateFixture(&leftBoundary, 0.0f);

    // L�mite derecho
    boundaryDef.position.Set(100.0f, 50.0f);
    boundaryBody = phyWorld->CreateBody(&boundaryDef);
    b2PolygonShape rightBoundary;
    rightBoundary.SetAsBox(0.5f, 50.0f); // Alto de la ventana
    boundaryBody->CreateFixture(&rightBoundary, 0.0f);

    // Crear la pelota din�mica
    b2BodyDef ballBodyDef;
    ballBodyDef.type = b2_dynamicBody;
    ballBodyDef.position.Set(50.0f, 50.0f); 
    controlBody = phyWorld->CreateBody(&ballBodyDef);
    b2CircleShape ballShape;
    ballShape.m_radius = 5.0f;
    b2FixtureDef ballFixtureDef;
    ballFixtureDef.shape = &ballShape;
    ballFixtureDef.density = 1.0f; 
    ballFixtureDef.friction = 0.3f; 
    ballFixtureDef.restitution = 4.8f; // Restituci�n (coeficiente de restituci�n)
    controlBody->CreateFixture(&ballFixtureDef);

    // Obst�culo 1
    b2BodyDef obstacleBodyDef1;
    obstacleBodyDef1.type = b2_staticBody; // Cuerpo est�tico
    obstacleBodyDef1.position.Set(70.0f, 50.0f); // Posici�n del cuadrado
    b2Body* obstacleBody1 = phyWorld->CreateBody(&obstacleBodyDef1);
    b2PolygonShape obstacleShape1;
    obstacleShape1.SetAsBox(10.0f, 10.0f); // Tama�o del cuadrado (ancho, alto)
    obstacleBody1->CreateFixture(&obstacleShape1, 0.0f); // Densidad cero para un cuerpo est�tico

    // Obst�culo 2
    b2BodyDef obstacleBodyDef2;
    obstacleBodyDef2.type = b2_staticBody; // Cuerpo est�tico
    obstacleBodyDef2.position.Set(10.0f, 80.0f); // Posici�n del cuadrado
    b2Body* obstacleBody2 = phyWorld->CreateBody(&obstacleBodyDef2);
    b2PolygonShape obstacleShape2;
    obstacleShape2.SetAsBox(10.0f, 10.0f); // Tama�o del cuadrado (ancho, alto)
    obstacleBody2->CreateFixture(&obstacleShape2, 0.0f); // Densidad cero para un cuerpo est�tico
}

// Destructor de la clase

Game::~Game(void)
{ }