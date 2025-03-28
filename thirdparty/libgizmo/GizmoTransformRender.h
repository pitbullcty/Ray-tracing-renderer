﻿///////////////////////////////////////////////////////////////////////////////////////////////////
// LibGizmo
// File Name : 
// Creation : 10/01/2012
// Author : Cedric Guillemet
// Description : LibGizmo
//
///Copyright (C) 2012 Cedric Guillemet
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
//of the Software, and to permit persons to whom the Software is furnished to do
///so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
///FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// 



#ifndef GIZMOTRANSFORMRENDER_H__
#define GIZMOTRANSFORMRENDER_H__

#include "ZBaseMaths.h"

typedef tvector4 tplane;
#include <QOpenGLExtraFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include "src/Data/Model.h"

class CGizmoTransformRender  
{
public:
	
	CGizmoTransformRender(QOpenGLExtraFunctions* _functions, QOpenGLShaderProgram* _program);
	virtual ~CGizmoTransformRender();
	void DrawCircle(const tvector3 &orig,float r,float g,float b,const tvector3 &vtx,const tvector3 &vty);
	void DrawCircleHalf(const tvector3 &orig,float r,float g,float b,const tvector3 &vtx,const tvector3 &vty,tplane &camPlan);
	void DrawAxis(const tvector3 &orig, const tvector3 &axis, const tvector3 &vtx,const tvector3 &vty, float fct,float fct2,const tvector4 &col);
	void DrawCamem(const tvector3& orig,const tvector3& vtx,const tvector3& vty,float ng);
	void DrawQuad(const tvector3& orig, float size, bool bSelected, const tvector3& axisU, const tvector3 &axisV);
	void DrawTri(const tvector3& orig, float size, bool bSelected, const tvector3& axisU, const tvector3& axisV);
	

protected:
	QOpenGLExtraFunctions* functions;
	QOpenGLShaderProgram* program;
	QOpenGLBuffer VBO;
	QOpenGLVertexArrayObject VAO;
	
	void Draw(const QVector<float>& data, GLenum mode, int count, bool isBlend = false);
	void addColor(QVector<float>& data, const tvector4& col);
	void addColor(QVector<float>& data, int r, int g, int b);
	void addVector3(QVector<float>& data, const tvector3& v);
	
};

#endif // !defined(AFX_GIZMOTRANSFORMRENDER_H__549F6E7A_D46D_4B18_9E74_76B7E43A3841__INCLUDED_)
