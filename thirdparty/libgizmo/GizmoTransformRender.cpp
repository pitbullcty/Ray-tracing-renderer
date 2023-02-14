///////////////////////////////////////////////////////////////////////////////////////////////////
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


#include "LibBase.h"
#include "GizmoTransformRender.h"
#include <QVector>


CGizmoTransformRender::CGizmoTransformRender(QOpenGLExtraFunctions* _functions, QOpenGLShaderProgram* _program):VBO(QOpenGLBuffer::VertexBuffer)
	,functions(_functions),program(_program)
{
	VBO.create();
}

CGizmoTransformRender::~CGizmoTransformRender() {
	VBO.destroy();
}

void CGizmoTransformRender::DrawCircle(const tvector3& orig,float r,float g,float b,const tvector3 &vtx,const tvector3 &vty)
{

	QVector<float> data;
	for (int i = 0; i <= 50; i++)
	{
		tvector3 vt;
		vt = vtx * cos((2 * ZPI / 50) * i);
		vt += vty * sin((2 * ZPI / 50) * i);
		vt += orig;
		addVector3(data, vt);
		addColor(data, r, g, b);
	}
	Draw(data, GL_LINE_LOOP, 50);
}


void CGizmoTransformRender::DrawCircleHalf(const tvector3 &orig,float r,float g,float b,const tvector3 &vtx,const tvector3 &vty,tplane &camPlan)
{
	
	QVector<float> data;
	for (int i = 0; i < 30; i++)
	{
		tvector3 vt;
		vt = vtx * cos((ZPI / 30) * i);
		vt += vty * sin((ZPI / 30) * i);
		vt += orig;
		if (camPlan.DotNormal(vt)) {
			addVector3(data, vt);
			addColor(data, r, g, b);
		}
	}
	Draw(data, GL_LINE_LOOP, 29);
}

void CGizmoTransformRender::DrawAxis(const tvector3 &orig, const tvector3 &axis, const tvector3 &vtx,const tvector3 &vty, float fct,float fct2,const tvector4 &col)
{
	
	QVector<float> data;
	addVector3(data, orig);
	addColor(data, col);
	addVector3(data, orig + axis);
	addColor(data, col);
	Draw(data, GL_LINES, 2);
	data.clear();
	for (int i = 0; i <= 30; i++)
	{
		tvector3 pt;
		pt = vtx * cos(((2 * ZPI) / 30.0f) * i) * fct;
		pt += vty * sin(((2 * ZPI) / 30.0f) * i) * fct;
		pt += axis * fct2;
		pt += orig;
		addVector3(data, pt);
		addColor(data, col);
		pt = vtx * cos(((2 * ZPI) / 30.0f) * (i + 1)) * fct;
		pt += vty * sin(((2 * ZPI) / 30.0f) * (i + 1)) * fct;
		pt += axis * fct2;
		pt += orig;
		addVector3(data, pt);
		addColor(data, col);
		addVector3(data, orig + axis);
		addColor(data, col);
	}
	Draw(data, GL_TRIANGLE_FAN, 31*3);
}

void CGizmoTransformRender::DrawCamem(const tvector3& orig,const tvector3& vtx,const tvector3& vty,float ng)
{
	QVector<float> data;
	addVector3(data, orig);
	addColor(data, { 1,1,0,0.5f });
	for (int i = 0; i <= 50; i++)
	{
		tvector3 vt;
		vt = vtx * cos(((ng) / 50) * i);
		vt += vty * sin(((ng) / 50) * i);
		vt += orig;
		addVector3(data, vt);
		addColor(data, { 1,1,0,0.5f });
	}
	Draw(data, GL_TRIANGLE_FAN, 51, true);
	data.clear();
	addVector3(data, orig);
	addColor(data, { 1,1,0.2f,1 });
	for (int i = 0; i <= 50; i++)
	{
		tvector3 vt;
		vt = vtx * cos(((ng) / 50) * i);
		vt += vty * sin(((ng) / 50) * i);
		vt += orig;
		addVector3(data, vt);
		addColor(data, { 1,1,0.2f,1 });
	}
	Draw(data, GL_LINE_LOOP, 51);
}

void CGizmoTransformRender::DrawQuad(const tvector3& orig, float size, bool bSelected, const tvector3& axisU, const tvector3 &axisV)
{
	QVector<float> data;
	tvector3 pts[4];
	pts[0] = orig;
	pts[1] = orig + (axisU * size);
	pts[2] = orig + (axisU + axisV) * size;
	pts[3] = orig + (axisV * size);
	tvector4 color;
	if (!bSelected)
		color = { 1,1,0,0.5f };
	else
		color = { 1,1,1,0.6f };
	for (int i = 0; i < 4; i++) {
		addVector3(data, pts[i]);
		addColor(data, color);
	}
	Draw(data, GL_QUADS, 4, true);
	if (!bSelected)
		color = { 1, 1, 0.2f, 1 };
	else
		color = { 1, 1, 1, 0.6f };
	data.clear();
	for (int i = 0; i < 4; i++) {
		addVector3(data, pts[i]);
		addColor(data, color);
	}
	Draw(data, GL_LINE_STRIP, 4, true);
}


void CGizmoTransformRender::DrawTri(const tvector3& orig, float size, bool bSelected, const tvector3& axisU, const tvector3& axisV)
{
	tvector3 pts[3];
	pts[0] = orig;
	pts[1] = axisU;
	pts[2] = axisV;
	pts[1] *= size;
	pts[2] *= size;
	pts[1] += orig;
	pts[2] += orig;
	tvector4 color;
	QVector<float> data;
	if (!bSelected)
		color = { 1, 1, 0, 0.5f };
	else
		color = { 1, 1, 1, 0.6f };
	for (int i = 0; i < 3; i++) {
		addVector3(data, pts[i]);
		addColor(data, color);
	}
	Draw(data, GL_TRIANGLES, 3, true);
	data.clear();
	if (!bSelected)
		color = { 1, 1, 0.2f, 1 };
	else
		color = { 1, 1, 1, 0.6f };
	for (int i = 0; i < 3; i++) {
		addVector3(data, pts[i]);
		addColor(data, color);
	}
	Draw(data, GL_LINE_STRIP, 3, true);
}

void CGizmoTransformRender::Draw(const QVector<float>& data, GLenum mode, int count, bool isBlend)
{
	program->bind();
	functions->glDisable(GL_DEPTH_TEST);
	functions->glDisable(GL_LIGHTING);
	if (isBlend) {
		functions->glEnable(GL_BLEND);
		functions->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		functions->glDisable(GL_CULL_FACE);
	}
	QOpenGLVertexArrayObject::Binder binder(&VAO);
	VBO.bind();
	VBO.allocate(data.data(), data.size() * sizeof(float));
	program->enableAttributeArray(0);
	program->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(float) * 7);
	program->enableAttributeArray(1);
	program->setAttributeBuffer(1, GL_FLOAT, sizeof(float) * 3, 4, sizeof(float) * 7);
	functions->glDrawArrays(mode, 0, count);
	functions->glEnable(GL_DEPTH_TEST);
	functions->glEnable(GL_LIGHTING);
	if (isBlend) {
		functions->glDisable(GL_BLEND);
	}
	VBO.release();
	program->release();
}


void CGizmoTransformRender::addColor(QVector<float>& data, const tvector4& col) {
	data.emplace_back(col.x);
	data.emplace_back(col.y);
	data.emplace_back(col.z);
	data.emplace_back(col.w);
	return;
}

void CGizmoTransformRender::addColor(QVector<float>& data, int r, int g, int b) {
	data.emplace_back(r);
	data.emplace_back(g);
	data.emplace_back(b);
	data.emplace_back(1.0f);
	return;
}

void CGizmoTransformRender::addVector3(QVector<float>& data, const tvector3& v)
{
	data.emplace_back(v.x);
	data.emplace_back(v.y);
	data.emplace_back(v.z);
	return;
}
