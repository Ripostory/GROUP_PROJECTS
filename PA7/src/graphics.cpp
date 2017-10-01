#include "graphics.h"

Graphics::Graphics()
{
}

Graphics::~Graphics()
{
}

bool Graphics::InitShader(Shader *&shader, string vertex, string fragment)
{
	 // Set up the shaders
	  shader = new Shader();
	  if(!shader->Initialize())
	  {
	    printf("Shader Failed to Initialize\n");
	    return false;
	  }

	  // Add the vertex shader
	  if(!shader->AddShader(GL_VERTEX_SHADER, vertex))
	  {
	    printf("Vertex Shader failed to Initialize\n");
	    return false;
	  }

	  // Add the fragment shader
	  if(!shader->AddShader(GL_FRAGMENT_SHADER, fragment))
	  {
	    printf("Fragment Shader failed to Initialize\n");
	    return false;
	  }

	  // Connect the program
	  if(!shader->Finalize())
	  {
	    printf("Program to Finalize\n");
	    return false;
	  }
	  return true;
}

bool Graphics::Initialize(int width, int height)
{
	this->width = width;
	this->height = height;
  // Used for the linux OS
  #if !defined(__APPLE__) && !defined(MACOSX)
    // cout << glewGetString(GLEW_VERSION) << endl;
    glewExperimental = GL_TRUE;

    auto status = glewInit();

    // This is here to grab the error that comes from glew init.
    // This error is an GL_INVALID_ENUM that has no effects on the performance
    glGetError();

    //Check for error
    if (status != GLEW_OK)
    {
      std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
      return false;
    }
  #endif

  // For OpenGL 3
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  //initialize fbo
  generateFrameBuffer(fbo, fbTex, width, height);

  //allocate depth and stencil buffers for base target
  GLuint rboDepthStencil;
  glGenRenderbuffers(1, &rboDepthStencil);
  glBindRenderbuffer(GL_RENDERBUFFER, rboDepthStencil);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

  glFramebufferRenderbuffer(
      GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepthStencil
  );

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
	  auto error = glGetError();
	  string val = ErrorString( error );
	  std::cerr << "Framebuffer creation was not successful:"
			  << endl << error << " "<< val << endl;
  }

  //create lower resolution framebuffer for bloom
  generateFrameBuffer(fboBloom, fbTexBloom, width>>5, height>>5);
  generateFrameBuffer(fboBloom2, fbTexBloom2, width>>6, height>>6);
  generateFrameBuffer(fboBloom3, fbTexBloom3, width>>7, height>>7);

  //reset to default buffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //initialize quad
  glGenBuffers(1, &screen);
  glBindBuffer(GL_ARRAY_BUFFER, screen);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Init Camera
  m_camera = new camera();
  if(!m_camera->Initialize(width, height))
  {
    printf("Camera Failed to Initialize\n");
    return false;
  }

  // Create the object
  m_cube = new SolarSystem(1.0f, 5.0f);
  m_cube->LoadSolSystem("solarSystem.JSON");
  m_camera->SetWorld(m_cube);
  renderTarget = m_cube;

  //create shader
  if (!InitShader(m_shader, "assets/shaders/sunShader.vsh", "assets/shaders/sunShader.fsh"))
	  return false;
  if (!InitShader(m_planetShader, "assets/shaders/vertexShader.vsh", "assets/shaders/fragmentShader.fsh"))
	  return false;
  if (!InitShader(m_earthShader, "assets/shaders/earthShader.vsh", "assets/shaders/earthShader.fsh"))
	  return false;
  if (!InitShader(m_gasGiantShader, "assets/shaders/gasGiantShader.vsh", "assets/shaders/gasGiantShader.fsh"))
	  return false;
  if (!InitShader(m_screenShader, "assets/shaders/screenShader.vsh", "assets/shaders/screenShader.fsh"))
	  return false;
  if (!InitShader(m_bloomShader, "assets/shaders/bloomShader.vsh", "assets/shaders/bloomShader.fsh"))
	  return false;
  if (!InitShader(m_bloomPreShader, "assets/shaders/bloomPreShader.vsh", "assets/shaders/bloomPreShader.fsh"))
	  return false;

  // Locate the projection matrix in the shader
  m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
  if (m_projectionMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  m_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
  if (m_viewMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_viewMatrix not found\n");
    return false;
  }

  // Locate the model matrix in the shader
  m_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
  if (m_modelMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_modelMatrix not found\n");
    return false;
  }

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  return true;
}

void Graphics::generateFrameBuffer(GLuint &fbo, GLuint &fbTarget, int width, int height)
{
	  glGenFramebuffers(1, &fbo);
	  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	  //allocate texture
	  glGenTextures(1, &fbTarget);
	  glBindTexture(GL_TEXTURE_2D, fbTarget);

	  glTexImage2D(
	      GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL
	  );

	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	  glFramebufferTexture2D(
	      GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbTarget, 0
	  );
}

void Graphics::Update(unsigned int dt)
{
  // Update the object
  m_cube->Update(dt);
  m_camera->Update(dt);
}

void Graphics::Render()
{

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glViewport(0,0, width, height);
  //set renderTarget
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  //clear the screen
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Render all objects
  TreeRender(m_cube);

  glDisable(GL_DEPTH_TEST);
  //render to low resolution buffer
  glViewport(0,0, width>>5, height>>5);
  glBindFramebuffer(GL_FRAMEBUFFER, fboBloom);
  addRenderTarget(m_bloomPreShader, fbTex);
  glViewport(0,0, width>>6, height>>6);
  glBindFramebuffer(GL_FRAMEBUFFER, fboBloom2);
  addRenderTarget(m_bloomPreShader, fbTex);
  glViewport(0,0, width>>7, height>>7);
  glBindFramebuffer(GL_FRAMEBUFFER, fboBloom3);
  addRenderTarget(m_bloomPreShader, fbTex);


  glViewport(0,0, width, height);
  //render frameBuffer to defaultBuffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //enable passthrough screen shader
  m_bloomShader->Enable();
  glBindVertexArray(screen);
  GLint posAttrib = glGetAttribLocation(m_bloomShader->getShader(), "position");
  GLint colAttrib = glGetAttribLocation(m_bloomShader->getShader(), "texcoord");

  //draw quad onto the screen
  glEnableVertexAttribArray(posAttrib);
  glEnableVertexAttribArray(colAttrib);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE,
                         4*sizeof(float), 0);
  glVertexAttribPointer(colAttrib, 2, GL_FLOAT, GL_FALSE,
                         4*sizeof(float), (void*)(2*sizeof(float)));

  glUniform1i(m_bloomShader->GetUniformLocation("texFramebuffer"), 0);
  glUniform1i(m_bloomShader->GetUniformLocation("bloom1"), 1);
  glUniform1i(m_bloomShader->GetUniformLocation("bloom2"), 2);
  glUniform1i(m_bloomShader->GetUniformLocation("bloom3"), 3);
  //pass in output texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, fbTex);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, fbTexBloom);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, fbTexBloom2);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, fbTexBloom3);
  glDrawArrays(GL_TRIANGLES, 0 ,6);
  glDisableVertexAttribArray(posAttrib);
  glDisableVertexAttribArray(colAttrib);

  // Get any errors from OpenGL
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    string val = ErrorString( error );
    std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
  }
}

void Graphics::addRenderTarget(Shader *shader, GLuint texTarget)
{
	  shader->Enable();
	  glBindVertexArray(screen);
	  GLint posAttrib = glGetAttribLocation(shader->getShader(), "position");
	  GLint colAttrib = glGetAttribLocation(shader->getShader(), "texcoord");

	  //draw quad onto the screen
	  glEnableVertexAttribArray(posAttrib);
	  glEnableVertexAttribArray(colAttrib);
	  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE,
	                         4*sizeof(float), 0);
	  glVertexAttribPointer(colAttrib, 2, GL_FLOAT, GL_FALSE,
	                         4*sizeof(float), (void*)(2*sizeof(float)));

	  //pass in output texture
	  glActiveTexture(GL_TEXTURE0);
	  glBindTexture(GL_TEXTURE_2D, texTarget);
	  glDrawArrays(GL_TRIANGLES, 0 ,6);
	  glDisableVertexAttribArray(posAttrib);
	  glDisableVertexAttribArray(colAttrib);
}

void Graphics::RenderList(vector<Object*> list)
{
	  for (int i = 0; i < list.size(); i++)
	  {
		  renderTarget = list[i];
		  TreeRender(renderTarget);
	  }
}

void Graphics::TreeRender(Object* object)
{
	  //enable correct shader
	if (object->isEarth())
		m_earthShader->Enable();
	else if (object->isaGasGiant())
		m_gasGiantShader->Enable();
	else if (object->isaPlanet())
		m_planetShader->Enable();
	else
		m_shader->Enable();

	  // Send in the projection and view to the shader
	  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
	  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));

	  //render this model
	  renderTarget = object;
	  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(renderTarget->GetModel()));
	  renderTarget->Render();

	  //render children if there are children
	  if (object->getChildren().size() != 0)
		  RenderList(object->getChildren());
}

std::string Graphics::ErrorString(GLenum error)
{
  if(error == GL_INVALID_ENUM)
  {
    return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
  }

  else if(error == GL_INVALID_VALUE)
  {
    return "GL_INVALID_VALUE: A numeric argument is out of range.";
  }

  else if(error == GL_INVALID_OPERATION)
  {
    return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
  }

  else if(error == GL_INVALID_FRAMEBUFFER_OPERATION)
  {
    return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
  }

  else if(error == GL_OUT_OF_MEMORY)
  {
    return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
  }
  else
  {
    return "None";
  }
}

