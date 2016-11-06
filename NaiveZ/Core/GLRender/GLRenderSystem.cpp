#include <iostream>
using namespace std;

#include "Include\GLRenderSystem.h"
#include "../../UniTest/UniTest.h"
#include "../../Application/Include/Application.h"

bool NaiveZ3D::GLRenderSystem::Init(Application* app)
{
	mApplication_ = app;

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return false;
	}

	auto width = app->GetAppWidth();
	auto height = app->GetAppHeight();
	glfwGetFramebufferSize(app->GetGlfwWindow(), &width, &height);
	SetViewPort(0, 0, width, height);
	mViewPort_.Use();
	return true;
}

void NaiveZ3D::GLRenderSystem::Run(GLfloat delta)
{
	glClear(GL_COLOR_BUFFER_BIT);

}

void NaiveZ3D::GLRenderSystem::SetClearColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	glClearColor(r, g, b, a);
}

void NaiveZ3D::GLRenderSystem::SetClearColor(GLColor c)
{
	glClearColor(c.red, c.green, c.blue, c.alpha);
}

void NaiveZ3D::GLRenderSystem::SetViewPort(GLint x, GLint y, GLint w, GLint h)
{
	mViewPort_ = ViewPort({ x, y }, w, h);
}