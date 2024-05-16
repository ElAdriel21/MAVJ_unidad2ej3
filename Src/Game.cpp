#include "Game.h"
#include "Box2DHelper.h"
#include <iostream>

// Constructor de la clase Game
Game::Game(int ancho, int alto, std::string titulo)
{
    // Inicialización de la ventana y configuración de propiedades
    wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
    wnd->setVisible(true);
    fps = 60;
    wnd->setFramerateLimit(fps);
    frameTime = 1.0f / fps;
    SetZoom(); // Configuración de la vista del juego
    InitPhysics(); // Inicialización del motor de física
}

// Bucle principal del juego
void Game::Loop()
{
    while (wnd->isOpen())
    {
        wnd->clear(clearColor); // Limpiar la ventana
        DoEvents(); // Procesar eventos de entrada
        CheckCollitions(); // Comprobar colisiones
        UpdatePhysics(); // Actualizar la simulación física
        DrawGame(); // Dibujar el juego
        wnd->display(); // Mostrar la ventana
    }
}

// Actualización de la simulación física
void Game::UpdatePhysics()
{
    phyWorld->Step(frameTime, 8, 8); // Simular el mundo físico
    phyWorld->ClearForces(); // Limpiar las fuerzas aplicadas a los cuerpos
    phyWorld->DebugDraw(); // Dibujar el mundo físico para depuración
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
            wnd->close(); // Cerrar la ventana si se presiona el botón de cerrar
            break;
        }
    }
}

// Comprobación de colisiones (a implementar más adelante)
void Game::CheckCollitions()
{
    // Implementación de la comprobación de colisiones
}

// Configuración de la vista del juego
void Game::SetZoom()
{
    View camara;
    // Posicionamiento y tamaño de la vista
    camara.setSize(100.0f, 100.0f);
    camara.setCenter(50.0f, 50.0f);
    wnd->setView(camara); // Asignar la vista a la ventana
}

// Inicialización del motor de física y los cuerpos del mundo físico
void Game::InitPhysics()
{
    // Inicializar el mundo físico con la gravedad por defecto
    phyWorld = new b2World(b2Vec2(5.3f, 9.8f));

    // Crear un renderer de debug para visualizar el mundo físico
    debugRender = new SFMLRenderer(wnd);
    debugRender->SetFlags(UINT_MAX);
    phyWorld->SetDebugDraw(debugRender);

    // Crear el suelo y las paredes estáticas del mundo físico
    b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
    groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

    b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
    leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

    b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
    rightWallBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);

    b2BodyDef boundaryDef;
    b2Body* boundaryBody;

    // Límite superior
    boundaryDef.position.Set(50.0f, 0.0f);
    boundaryBody = phyWorld->CreateBody(&boundaryDef);
    b2PolygonShape topBoundary;
    topBoundary.SetAsBox(50.0f, 0.5f); // Ancho de la ventana
    boundaryBody->CreateFixture(&topBoundary, 0.0f);

    // Límite inferior
    boundaryDef.position.Set(50.0f, 100.0f);
    boundaryBody = phyWorld->CreateBody(&boundaryDef);
    b2PolygonShape bottomBoundary;
    bottomBoundary.SetAsBox(50.0f, 0.5f); // Ancho de la ventana
    boundaryBody->CreateFixture(&bottomBoundary, 0.0f);

    // Límite izquierdo
    boundaryDef.position.Set(0.0f, 50.0f);
    boundaryBody = phyWorld->CreateBody(&boundaryDef);
    b2PolygonShape leftBoundary;
    leftBoundary.SetAsBox(0.5f, 50.0f); // Alto de la ventana
    boundaryBody->CreateFixture(&leftBoundary, 0.0f);

    // Límite derecho
    boundaryDef.position.Set(100.0f, 50.0f);
    boundaryBody = phyWorld->CreateBody(&boundaryDef);
    b2PolygonShape rightBoundary;
    rightBoundary.SetAsBox(0.5f, 50.0f); // Alto de la ventana
    boundaryBody->CreateFixture(&rightBoundary, 0.0f);

    // Crear la pelota dinámica
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
    ballFixtureDef.restitution = 4.8f; // Restitución (coeficiente de restitución)
    controlBody->CreateFixture(&ballFixtureDef);

    // Obstáculo 1
    b2BodyDef obstacleBodyDef1;
    obstacleBodyDef1.type = b2_staticBody; // Cuerpo estático
    obstacleBodyDef1.position.Set(70.0f, 50.0f); // Posición del cuadrado
    b2Body* obstacleBody1 = phyWorld->CreateBody(&obstacleBodyDef1);
    b2PolygonShape obstacleShape1;
    obstacleShape1.SetAsBox(10.0f, 10.0f); // Tamaño del cuadrado (ancho, alto)
    obstacleBody1->CreateFixture(&obstacleShape1, 0.0f); // Densidad cero para un cuerpo estático

    // Obstáculo 2
    b2BodyDef obstacleBodyDef2;
    obstacleBodyDef2.type = b2_staticBody; // Cuerpo estático
    obstacleBodyDef2.position.Set(10.0f, 80.0f); // Posición del cuadrado
    b2Body* obstacleBody2 = phyWorld->CreateBody(&obstacleBodyDef2);
    b2PolygonShape obstacleShape2;
    obstacleShape2.SetAsBox(10.0f, 10.0f); // Tamaño del cuadrado (ancho, alto)
    obstacleBody2->CreateFixture(&obstacleShape2, 0.0f); // Densidad cero para un cuerpo estático
}

// Destructor de la clase

Game::~Game(void)
{ }