#include <iostream>

#include "engine.h"


int main(int argc, char **argv)
{

  for (int i = 1; i < argc; i++) {

    if (string(argv[i]) == "-l" && i < argc - 1) {

 	Object::defaultPath = argv [i + 1];
    }
  }


  // Start an engine and run it then cleanup after
  Engine *engine = new Engine("Tutorial Window Name", 800, 600);
  if(!engine->Initialize())
  {
    printf("The engine failed to start.\n");
    delete engine;
    engine = NULL;
    return 1;
  }
  engine->Run();
  delete engine;
  engine = NULL;
  return 0;
}
