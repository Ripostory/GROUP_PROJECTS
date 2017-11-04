#include "graphics.h"

Graphics::Graphics()
{
	delay = 1;
	frameCount = 1;
	fps = 1;
	resScale = 1;
}

Graphics::~Graphics()
{
	Object::end();
	Billboard::end();
}

bool Graphics::InitShader(Shader *&shader, string vertex, string fragment)
{
	 // Set up the shaders
	  shader = new Shader();
	  return shader->InitShader(vertex, fragment);
}

bool Graphics::Initialize(int width, int height, SDL_Window *window)
{
  //init UI
  ui.initGUI(window);

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

  //initialize quad
  glGenBuffers(1, &screen);
  glBindBuffer(GL_ARRAY_BUFFER, screen);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  //Generate Frame buffer
  m_deferredFBO.initFB(width*resScale, height*resScale);
  m_deferredFBO.addRBOTex(GL_RGBA, GL_COLOR_ATTACHMENT0);			//albedo
  m_deferredFBO.addRBOTex(GL_RGB16F, GL_COLOR_ATTACHMENT1);			//normal
  m_deferredFBO.addRBOTex(GL_RG, GL_COLOR_ATTACHMENT2);				//material
  m_deferredFBO.addRBOTex(GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT);	//depth

  m_lightingFBO.initFB(width, height);
  m_lightingFBO.addRBOTex(GL_RGBA16F, GL_COLOR_ATTACHMENT0);

  //reset to default buffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, 0);

  //initialize object default directory
  Object::init();
  Billboard::init();

  // Init Camera
  m_camera = new camera();
  if(!m_camera->Initialize(width, height))
  {
    printf("Camera Failed to Initialize\n");
    return false;
  }

  //create shader
  bool success = true;

  //build all shaders
  success &= InitShader(m_shader, "shader.vert", "shader.frag");
  success &= InitShader(m_deferredShader, "deferredPass.vert", "deferredPass.frag");
  success &= InitShader(m_screenShader, "screenShader.vert", "screenShader.frag");
  success &= InitShader(m_HDRShader, "screenShader.vert", "screenHDR.frag");
  success &= InitShader(m_pointShader, "shader.vert", "screenDeferredPoint.frag");
  success &= InitShader(m_directionShader, "screenShader.vert", "screenDeferredDir.frag");
  success &= InitShader(m_ambientShader, "screenShader.vert", "cheapAmbient.frag");
  success &= InitShader(m_skyboxShader, "skyboxShader.vert", "skyboxShader.frag");
  success &= InitShader(m_billboard, "billboard.vert", "billboard.frag");

  //stop program build if there was an error
  if (!success)
	  return false;

  // Create the object
  world = new World();
  world->Begin();

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
  beginFBODraw(width*resScale, height*resScale);
  //enable correct shader
  m_deferredShader->Enable();
  glUniform1i(m_deferredShader->GetUniformLocation("texture"), 0);
  glUniform1i(m_deferredShader->GetUniformLocation("normalMap"), 1);
  glUniform1i(m_deferredShader->GetUniformLocation("specMap"), 2);

  //render world
  RenderList(world->getChildren());

  //render result of FBO to default buffer
  m_lightingFBO.bindFB();

  //set frame buffer parameters
  glViewport(0,0, width, height);
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  glDepthFunc(GL_EQUAL);

  //render lightless world
  renderDeferred(m_ambientShader, NULL);

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

  // draw to default FBO
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  //render cubemap
  renderSkybox(m_skyboxShader);
  m_HDRShader->Enable();
  m_lightingFBO.bindAllTex();
  drawQuad();

  // Get any errors from OpenGL
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    string val = ErrorString( error );
    ImGui::TextColored(ImVec4(1,0,0,1), "OpenGL ERROR: %i, %s",error, val.c_str());
    //std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
  }

  //render a test quad
  //TODO create a billboard world
  m_billboard->Enable();
  Billboard test;
  test.Update(0);
  passMatrices(test.getMatrix());
  test.setImage("ERROR_TEXTURE.jpg");
  test.Render();

  //render user interface
  glViewport(0,0, width, height);
  ui.Render();
}

void Graphics::renderSkybox(Shader *shader)
{
	shader->Enable();
	// Send in the projection and view to the shader
	glm::vec4 camPos = glm::inverse(m_camera->GetView()) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	world->translate(glm::vec3(camPos));
	passMatrices(world->GetModel());
	ImGui::Text("camera Position: <%.1f,%.1f,%.1f>", camPos.x, camPos.y, camPos.z);
	//render world
	world->Render();
}

void Graphics::beginFBODraw(int width, int height)
{
	  m_deferredFBO.bindFB();

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

void Graphics::renderDeferred(Shader *shader, Light *light)
{
	  shader->Enable();

	  //pass in buffers
	  glUniform1i(shader->GetUniformLocation("albedo"), 0);
	  glUniform1i(shader->GetUniformLocation("normal"), 1);
	  //glUniform1i(shader->GetUniformLocation("worldPos"), 2);
	  glUniform1i(shader->GetUniformLocation("material"), 2);
	  glUniform1i(shader->GetUniformLocation("depth"), 3);

	  //pass in screen size
	  glm::vec2 screenSize = glm::vec2(width, height);
	  glUniform2fv(shader->GetUniformLocation("gScreenSize"), 1, glm::value_ptr(screenSize));

	  //pass in view matrix
	  glm::mat4 worldPosMatrix = glm::inverse(m_camera->GetProjection() * m_camera->GetView());
	  glUniformMatrix4fv(shader->GetUniformLocation("viewInverse"),
			  1, GL_FALSE, glm::value_ptr(worldPosMatrix));

	  //pass in output texture
	  m_deferredFBO.bindAllTex();
	  m_deferredFBO.bindDepth(3);

	  //choose render type
	  if (light != NULL)
	  {
		  //pass in light data
		  glm::vec4 camPos = glm::inverse(m_camera->GetView()) * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);

		  glUniform3fv(
				  shader->GetUniformLocation("lightPos"), 1,
				  glm::value_ptr(light->getLight()->pos));

		  glUniform3fv(
				  shader->GetUniformLocation("cameraPos"), 1,
				  glm::value_ptr(glm::vec3(camPos)));

		  glUniform1f(
				  shader->GetUniformLocation("radius"),
				  light->getLight()->radius);

		  glUniform3fv(
				  shader->GetUniformLocation("color"), 1,
				  glm::value_ptr(light->getLight()->color));


		  if (light->getLight()->type == LIGHT_POINT)
		  {
			  // Send in the projection and view to the shader
			  passMatrices(light->GetModel());
			  light->Render();
		  }
		  else if (light->getLight()->type == LIGHT_DIR)
		  {
			  // Send in the projection and view to the shader
			  glm::mat4 identity = glm::mat4(1.0f);
			  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(identity));
			  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(identity));
			  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(identity));

			  drawQuad();
		  }
	  }
	  else
	  {
		  //no light, render ambient light
		  //TODO worldpos not used in ambient
		  drawQuad();
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
	  passMatrices(object->GetModel());

	  //render this model
	  renderTarget = object;
	  object->Render();

	  //render children if there are children
	  if (object->getChildren().size() != 0)
		  RenderList(object->getChildren());
}

void Graphics::passMatrices(glm::mat4 modelMatrix)
{
	  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
	  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));
	  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}

void Graphics::drawQuad()
{
	  //draw quad for screen
	  glBindBuffer(GL_ARRAY_BUFFER, screen);
	  glEnableVertexAttribArray(0);
	  glEnableVertexAttribArray(1);
	  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
	                         4*sizeof(float), 0);
	  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
	                         4*sizeof(float), (void*)(2*sizeof(float)));

	  glDrawArrays(GL_TRIANGLES, 0 ,6);
	  glDisableVertexAttribArray(0);
	  glDisableVertexAttribArray(1);
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

