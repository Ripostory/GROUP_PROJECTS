#include "graphics.h"

Graphics::Graphics()
{
	delay = 1;
	frameCount = 1;
	fps = 1;
}

Graphics::~Graphics()
{
	Object::end();
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

bool Graphics::Initialize(int width, int height, SDL_Window *window)
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

  //init UI
  ui.initGUI(window);

  // For OpenGL 3
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  //initialize quad
  glGenBuffers(1, &screen);
  glBindBuffer(GL_ARRAY_BUFFER, screen);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  //Generate Frame buffer
  generateFBO(FBO);
  //setup render buffers
  generateRBOTex(RB_albedo, GL_RGBA, GL_COLOR_ATTACHMENT0, width, height);
  generateRBOTex(RB_normal, GL_RGBA16F, GL_COLOR_ATTACHMENT1, width, height);
  generateRBOTex(RB_worldPos, GL_RGBA16F, GL_COLOR_ATTACHMENT2, width, height);
  generateRBO(RB_depth, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, width, height);

  generateFBO(FB_buffer);
  generateRBOTex(T_buffer, GL_RGBA, GL_COLOR_ATTACHMENT0, width, height);


  //reset to default buffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // Init Camera
  m_camera = new camera();
  if(!m_camera->Initialize(width, height))
  {
    printf("Camera Failed to Initialize\n");
    return false;
  }

  //create shader
  if (!InitShader(m_shader, "shaders/shader.vsh", "shaders/shader.fsh"))
	  return false;
  if (!InitShader(m_deferredShader, "shaders/deferredPass.vsh", "shaders/deferredPass.fsh"))
	  return false;
  if (!InitShader(m_screenShader, "shaders/screenShader.vsh", "shaders/screenShader.fsh"))
	  return false;
  if (!InitShader(m_pointShader, "shaders/shader.vsh", "shaders/screenDeferredPoint.fsh"))
	  return false; //Validation skipped for point shader
  if (!InitShader(m_directionShader, "shaders/screenShader.vsh", "shaders/screenDeferredDir.fsh"))
	  return false;
  if (!InitShader(m_ambientShader, "shaders/screenShader.vsh", "shaders/deferredAmbient.fsh"))
	  return false;
  if (!InitShader(m_skyboxShader, "shaders/skyboxShader.vsh", "shaders/skyboxShader.fsh"))
	  return false;

  //initialize object default directory
  Object::init();
  Shader::init();

  // Create the object
  world = new World();
  world->Begin();
  world->setLightPointer(
		  m_shader->GetUniformLocation("lPos"),
		  m_shader->GetUniformLocation("lRad"),
		  m_shader->GetUniformLocation("lSize"));
  //set world for camera
  m_camera->SetWorld(world);

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
  glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
  return true;
}

void Graphics::generateFBO(GLuint &fbo)
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void Graphics::generateFBOTex(GLuint &fbo, GLuint &fbTarget, int width, int height)
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

void Graphics::generateRBO(GLuint &target, GLenum type, GLenum attach, int width, int height)
{
	glGenRenderbuffers(1, &target);
	glBindRenderbuffer(GL_RENDERBUFFER, target);
	glRenderbufferStorage(GL_RENDERBUFFER, type, width, height);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, attach, GL_RENDERBUFFER, target);


	  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	  {
		  auto error = glGetError();
		  string val = ErrorString( error );
		  std::cerr << "Framebuffer creation was not successful:"
				  << endl << error << " "<< val << endl;
	  }
}

void Graphics::generateRBOTex(GLuint &target, GLenum type, GLenum attach, int width, int height)
{
	  //allocate texture
	  glGenTextures(1, &target);
	  glBindTexture(GL_TEXTURE_2D, target);

	  if (type == GL_RGBA || type == GL_RGB8)
		  glTexImage2D(GL_TEXTURE_2D, 0, type,
				  width, height, 0, type,
				  GL_UNSIGNED_BYTE, NULL);
	  else if (type == GL_RGBA16F || type == GL_RGBA32F || type == GL_RGB10_A2)
		  glTexImage2D(GL_TEXTURE_2D, 0, type,
				  width, height, 0, GL_RGBA,
				  GL_FLOAT, NULL);
	  else if (type == GL_RGB16F || type == GL_RGB32F || type == GL_RGB10_A2)
		  glTexImage2D(GL_TEXTURE_2D, 0, type,
				  width, height, 0, GL_RGB,
				  GL_FLOAT, NULL);

	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	  glFramebufferTexture2D(
	      GL_FRAMEBUFFER, attach, GL_TEXTURE_2D, target, 0
	  );

	  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	  {
		  auto error = glGetError();
		  string val = ErrorString( error );
		  std::cerr << "Framebuffer creation was not successful:"
				  << endl << error << " "<< val << endl;
	  }
}

void Graphics::Update(unsigned int dt)
{
  // Update everything
  updateFPS(dt);
  ui.Update(dt);
  ImGui::Text("FPS: %.2f", fps);
  world->Update(dt);
  m_camera->Update(dt);
}

void Graphics::updateFPS(unsigned int dt)
{
	frameCount++;
	delay += dt;
	if (delay >= 1000)
	{
		fps = frameCount;
		delay = 0;
		frameCount = 0;
	}
}

void Graphics::Render()
{
  //set renderTarget
  beginFBODraw(FBO, width, height);
  //enable correct shader
  m_deferredShader->Enable();

  //render world
  RenderList(world->getChildren());

  //render result of FBO to default buffer
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  glViewport(0,0, width, height);
  glClearColor(0.0, 0.3, 0.5, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  glDepthFunc(GL_EQUAL);
  //render cubemap
  renderSkybox(m_skyboxShader);

  //render lightless world
  Light *tempLight = new Light(LIGHT_DIR);
  renderDeferred(m_ambientShader, tempLight);
  delete tempLight;

  //render all lights
  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_ONE, GL_ONE);
  glEnable(GL_STENCIL_TEST);
  for (int i = 0; i < world->getLightCount(); i++)
  {
	  renderDeferred(m_pointShader, world->getLightData(i));
  }

  glDisable(GL_BLEND);
  // Get any errors from OpenGL
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    string val = ErrorString( error );
    ImGui::TextColored(ImVec4(1,0,0,1), "OpenGL ERROR: %i, %s",error, val.c_str());
    //std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
  }

  //render user interface
  ui.Render();
}

void Graphics::renderSkybox(Shader *shader)
{
	shader->Enable();
	// Send in the projection and view to the shader
	glm::vec4 camPos = glm::inverse(m_camera->GetView()) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	world->translate(glm::vec3(camPos));
	glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
	glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(world->GetModel()));
	ImGui::Text("camera Position: <%.1f,%.1f,%.1f>", camPos.x, camPos.y, camPos.z);
	//render world
	world->Render();
}

void Graphics::beginFBODraw(GLuint fbo, int width, int height)
{
	  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	  GLenum buffers[] = {
			  GL_COLOR_ATTACHMENT0_EXT,
			  GL_COLOR_ATTACHMENT1_EXT,
			  GL_COLOR_ATTACHMENT2_EXT };
	  glDrawBuffers(3, buffers);

	  //enable depth testing
	  glEnable(GL_DEPTH_TEST);
	  glEnable(GL_STENCIL_TEST);
	  glDisable(GL_BLEND);
	  glEnable(GL_CULL_FACE);
	  glDepthFunc(GL_LESS);
	  glCullFace(GL_BACK);
	  glViewport(0,0, width, height);

	  glClearColor(0.0, 0.0, 0.0, 0.0);
	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void Graphics::addRenderTarget(Shader *shader, GLuint texTarget)
{
	  glDisable(GL_DEPTH_TEST);
	  glDisable(GL_STENCIL_TEST);
	  glDisable(GL_CULL_FACE);
	  shader->Enable();
	  glBindBuffer(GL_ARRAY_BUFFER, screen);
	  GLint posAttrib = glGetAttribLocation(shader->getShader(), "position");
	  GLint colAttrib = glGetAttribLocation(shader->getShader(), "texcoord");

	  //draw quad onto the screen
	  glEnableVertexAttribArray(posAttrib);
	  glEnableVertexAttribArray(colAttrib);
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

void Graphics::renderDeferred(Shader *shader, Light *light)
{
	  glActiveTexture(0);
	  shader->Enable();

	  //pass in light data
	  glm::vec4 camPos = glm::inverse(m_camera->GetView()) * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
	  glUniform3fv(shader->GetUniformLocation("lightPos"), 1,
			  glm::value_ptr(light->getLight()->pos));
	  glUniform3fv(shader->GetUniformLocation("cameraPos"), 1,
			  glm::value_ptr(glm::vec3(camPos)));
	  glUniform1f(shader->GetUniformLocation("radius"), light->getLight()->radius);
	  glUniform3fv(shader->GetUniformLocation("color"), 1,
			  glm::value_ptr(light->getLight()->color));

	  //pass in screen size
	  glm::vec2 screenSize = glm::vec2(width, height);
	  glUniform2fv(shader->GetUniformLocation("gScreenSize"), 1, glm::value_ptr(screenSize));

	  //pass in buffers
	  glUniform1i(shader->GetUniformLocation("albedo"), 0);
	  glUniform1i(shader->GetUniformLocation("normal"), 1);
	  glUniform1i(shader->GetUniformLocation("worldPos"), 2);
	  glUniform1i(shader->GetUniformLocation("skybox"), 3);

	  //pass in output texture
	  glActiveTexture(GL_TEXTURE0);
	  glBindTexture(GL_TEXTURE_2D, RB_albedo);
	  glActiveTexture(GL_TEXTURE1);
	  glBindTexture(GL_TEXTURE_2D, RB_normal);
	  glActiveTexture(GL_TEXTURE2);
	  glBindTexture(GL_TEXTURE_2D, RB_worldPos);
	  //pass in world cube map
	  glActiveTexture(GL_TEXTURE3);
	  glBindTexture(GL_TEXTURE_CUBE_MAP, world->getSkybox());

	  //choose render type
	  if (light->getLight()->type == LIGHT_POINT)
	  {
		  // Send in the projection and view to the shader
		  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
		  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));
		  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(light->GetModel()));
		  light->Render();
	  }
	  else if (light->getLight()->type == LIGHT_DIR)
	  {
		  // Send in the projection and view to the shader
		  //TODO fix directional light error
		  glm::mat4 identity = glm::mat4(1.0f);
		  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(identity));
		  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(identity));
		  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(identity));

		  //draw quad for screen
		  glBindBuffer(GL_ARRAY_BUFFER, screen);
		  GLint posAttrib = glGetAttribLocation(shader->getShader(), "position");
		  GLint colAttrib = glGetAttribLocation(shader->getShader(), "texcoord");

		  glEnableVertexAttribArray(posAttrib);
		  glEnableVertexAttribArray(colAttrib);
		  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE,
		                         4*sizeof(float), 0);
		  glVertexAttribPointer(colAttrib, 2, GL_FLOAT, GL_FALSE,
		                         4*sizeof(float), (void*)(2*sizeof(float)));

		  glDrawArrays(GL_TRIANGLES, 0 ,6);

		  glDisableVertexAttribArray(posAttrib);
		  glDisableVertexAttribArray(colAttrib);

	  }
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
	  // Send in the projection and view to the shader
	  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
	  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));

	  //render this model
	  renderTarget = object;
	  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(object->GetModel()));
	  object->Render();

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

