#pragma once

#include <vector>

#include "GL/glew.h"

#include "GLShader.h"
#include "../../../Math/Include/Vector.h"

namespace NaiveZ3D
{
	class Model;
	using Buffer = std::vector<GLuint>;

	struct GLVertexData
	{
		GLVertexData(const Vector3& v)
		{
			x_ = v.x_;
			y_ = v.y_;
			z_ = v.z_;
		}
		GLVertexData(const Vector3& v, const Vector2& t, const Vector3& n)
		{
			x_ = v.x_;
			y_ = v.y_;
			z_ = v.z_;

			u_ = t.x_;
			v_ = t.y_;

			nx_ = n.x_;
			ny_ = n.y_;
			nz_ = n.z_;
		}

		GLfloat x_, y_, z_;
		GLfloat u_, v_;
		GLfloat nx_, ny_, nz_;
	};
	using GLVertexDataBufferBuffer = std::vector<std::vector<GLVertexData>>;

	class GLModel
	{
	public:
		GLModel(const Model&);
		
		void Destroy();
		void Draw()const;

	private:
		void DrawArrays()const;
		void DrawElements()const;

	private:
		Buffer mVAOBuffer_;
		Buffer mVBOBuffer_;
		Buffer mEBOBuffer_;
		std::vector<size_t> mEBOSizeBuffer_;
		GLVertexDataBufferBuffer mGLVertexDataBufferBuffer_;
		std::string mMtlName_;
		std::vector<std::string> mUseMtlBuffer_;

		bool mUseTex_;
	};
}