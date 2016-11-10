#include <cassert>
#include <vector>
using namespace std;

#include "include/GLModel.h"
#include "../../../File/Include/Model.h"
#include "../Material/Include/MaterialMgr.h"

NaiveZ3D::GLModel::GLModel(const Model &model)
{
	const auto& meshBuffer = model.GetAllMesh();
	mUseTex_ = model.UseTex();
	mMtlName_ = model.GetMtl();
	mVAOBuffer_.resize(meshBuffer.size());
	mVBOBuffer_.resize(meshBuffer.size());
	mEBOBuffer_.resize(meshBuffer.size());
	mEBOSizeBuffer_.resize(meshBuffer.size());
	mUseMtlBuffer_.resize(meshBuffer.size());
	mGLVertexDataBufferBuffer_.resize(meshBuffer.size());
	glGenVertexArrays(mVAOBuffer_.size(), mVAOBuffer_.data());
	glGenBuffers(mVBOBuffer_.size(), mVBOBuffer_.data());
	glGenBuffers(mEBOBuffer_.size(), mEBOBuffer_.data());

	for (auto i = 0; i != meshBuffer.size(); ++i)
	{
		const auto& mesh = meshBuffer[i];
		mUseMtlBuffer_[i] = mesh.GetMtl();
		auto vao = mVAOBuffer_[i];//vertex array object
		auto vbo = mVBOBuffer_[i];//vertex buffer object
		auto ebo = mEBOBuffer_[i];//element buffer object

		auto& vdb = mGLVertexDataBufferBuffer_[i];	//GLVertexData buffer
		const auto& edb = mesh.GenIndiceBuffer(model);	//element data buffer for this mesh
		mEBOSizeBuffer_[i] = edb.size();
		const auto& vb = mesh.GenVertexBuffer(model);	//vertex data buffer for this mesh
		const auto& tex = mesh.GenTexCoordBuffer(model);	//texcoord data buffer for this mesh
		assert(tex.size() == vb.size());

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

		for (auto index = 0; index != vb.size(); ++index)
		{
			auto data = GLVertexData(vb[index], tex[index]);
			vdb.emplace_back(data);
		}
		// vertex buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLVertexData)*vdb.size(), (const GLvoid*)&vdb[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*5, (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*5, (GLvoid*)(3*sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		//element buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*edb.size(), edb.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}

void NaiveZ3D::GLModel::Destroy()
{
	glDeleteVertexArrays(mVAOBuffer_.size(), mVAOBuffer_.data());
	glDeleteBuffers(mVBOBuffer_.size(), mVBOBuffer_.data());
	glDeleteBuffers(mEBOBuffer_.size(), mEBOBuffer_.data());
}

void NaiveZ3D::GLModel::Draw()
{
	if (mEBOSizeBuffer_.empty())
	{
		DrawArrays();
	}
	else
	{
		DrawElements();
	}
}

void NaiveZ3D::GLModel::DrawArrays()
{
	assert(false, "do not use glDrawArrays");

	for (auto i = 0; i != mVAOBuffer_.size(); ++i)
	{
		auto vao = mVAOBuffer_[i];
		//auto ebo = mEBOBuffer_[i];
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBindVertexArray(vao);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

		glDrawArrays(GL_TRIANGLES, 0, mGLVertexDataBufferBuffer_[i].size());

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}

void NaiveZ3D::GLModel::DrawElements()
{
	assert(mMtlName_ != "");
	const auto& material = MaterialMgr::Instance().GetMaterial(mMtlName_);

	for (auto i = 0; i != mVAOBuffer_.size(); ++i)
	{
		auto vao = mVAOBuffer_[i];
		auto ebo = mEBOBuffer_[i];
		

		if (!mUseTex_)//model不使用纹理则渲染为线框模式
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//暂时设为线框模式
		}
		else
		{
			auto& usemtl = mUseMtlBuffer_[i];
			material.Use(usemtl);
		}
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//暂时设为线框模式
		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		
		glDrawElements(GL_TRIANGLES, mEBOSizeBuffer_[i], GL_UNSIGNED_INT, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}
