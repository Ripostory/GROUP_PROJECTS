
#include "engine.h"

Engine::Engine(string name, int width, int height)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;
  m_FULLSCREEN = false;
}

Engine::Engine(string name)
{
  m_WINDOW_NAME = name;
  m_WINDOW_HEIGHT = 0;
  m_WINDOW_WIDTH = 0;
  m_FULLSCREEN = true;
}

Engine::~Engine()
{
  delete m_window;
  delete m_graphics;

  delete physWorld;
  delete solver;
  delete overlapPairCache;
  delete physDispatcher;
  delete collConfig;
  m_window = NULL;
  m_graphics = NULL;
}

bool Engine::Initialize()
{

  //initialize physics engine
  overlapPairCache = new btDbvtBroadphase();

  collConfig = new btDefaultCollisionConfiguration();
  physDispatcher = new btCollisionDispatcher(collConfig);
  solver = new btSequentialImpulseConstraintSolver;

  physWorld = new btDiscreteDynamicsWorld(physDispatcher,
		  overlapPairCache, solver, collConfig);
  physWorld->setGravity(btVector3(0, -10, 0));

  //initialize event handler
  m_event.init(physWorld);

  // Start a window
  m_window = new Window();
  if(!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT))
  {
    printf("The window failed to initialize.\n");
    return false;
  }

  // Start the graphics
  m_graphics = new Graphics();
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, m_window->getWindow()))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }

  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();

  // No errors
  return true;
}

void Engine::Run()
{
  m_running = true;

  while(m_running)
  {
    // Update the DT
    m_DT = getDT();

    //Update the physics
    physWorld->stepSimulation((float) m_DT/1000.0f, 30);

    // Check the keyboard input
    m_event.update();
    for (int i = 0; i < m_event.getSize(); i++)
    {
      Keyboard(m_event.getEvent(i));
    }

    // Update and render the graphics
    m_graphics->Update(m_DT);
    m_graphics->Render();

    // Swap to the Window
    m_window->Swap();
  }
}

void Engine::Keyboard(eventType event)
{


  if(event.eventVer == SDL_QUIT)
  {
    m_running = false;
  }
  else if (event.eventVer == SDL_KEYDOWN)
  {
    // handle key down events here
    if (event.key == SDLK_ESCAPE)
    {
      m_running = false;
    }

		if (event.key == SDLK_f)
			m_graphics -> SetShaderMode (true);

		if (event.key == SDLK_g)
			m_graphics -> SetShaderMode (false);

  }
}

unsigned int Engine::getDT()
{
  long long TimeNowMillis = GetCurrentTimeMillis();
  assert(TimeNowMillis >= m_currentTimeMillis);
  unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
  m_currentTimeMillis = TimeNowMillis;
  return DeltaTimeMillis;
}

long long Engine::GetCurrentTimeMillis()
{
  timeval t;
  gettimeofday(&t, NULL);
  long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
  return ret;
}
