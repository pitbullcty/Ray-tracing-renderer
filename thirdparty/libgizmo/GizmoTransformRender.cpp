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
		color = { 1,1,0,0.16f };
	else
		color = { 1,1,1,0.42f };
	for (int i = 0; i < 4; i++) {
		addVector3(data, pts[i]);
		addColor(data, color);
	}
	Draw(data, GL_QUADS, 4, true);
	if (!bSelected)
		color = { 1, 1, 0.35f, 0.85f };
	else
		color = { 1, 1, 1, 0.42f };
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
		color = { 1, 1, 0, 0.16f };
	else
		color = { 1, 1, 1, 0.42f };
	for (int i = 0; i < 3; i++) {
		addVector3(data, pts[i]);
		addColor(data, color);
	}
	Draw(data, GL_TRIANGLES, 3, true);
	data.clear();
	if (!bSelected)
		color = { 1, 1, 0.35f, 0.85f };
	else
		color = { 1, 1, 1, 0.42f };
	for (int i = 0; i < 3; i++) {
		addVector3(data, pts[i]);
		addColor(data, color);
	}
	Draw(data, GL_LINE_STRIP, 3, true);
}

void CGizmoTransformRender::DrawAxisBox(const tvector3 &orig, const tvector3 &axis, const tvector3 &vtx, const tvector3 &vty, float fct, float fct2, const tvector4 &col)
{
	//缩放手柄: 轴线 + 轴端立方体。移动gizmo的箭头是圆锥，这里用立方体，形状上一眼能区分
	const float axisLen = axis.Length();
	if (axisLen <= 1e-6f)
		return;
	tvector3 dir = axis;
	dir *= (1.0f / axisLen); //轴单位方向

	const float halfAlong = axisLen * 0.06f; //沿轴方向半长
	const float halfCross = axisLen * 0.10f; //垂直轴方向半宽(略宽于沿轴长度, 保证任何视角下都是块状而不是细条)
	const float cubeCenterFrac = (fct2 > 0.0f) ? fct2 : 0.9f; //立方体中心在轴上的位置比例
	tvector3 cubeCenter = orig + axis * cubeCenterFrac;
	float lineEnd = axisLen * cubeCenterFrac - halfAlong * 0.9f; //轴线到立方体边缘为止
	if (lineEnd < 0.0f)
		lineEnd = 0.0f;

	QVector<float> data;
	addVector3(data, orig);
	addColor(data, col);
	addVector3(data, orig + dir * lineEnd);
	addColor(data, col);
	Draw(data, GL_LINES, 2);

	DrawBox(cubeCenter, dir * halfAlong, vtx * halfCross, vty * halfCross, col);
	(void)fct; //保留签名参数(与DrawAxis一致)，立方体不再使用圆锥半径
}

void CGizmoTransformRender::DrawBox(const tvector3 &center, const tvector3 &ax, const tvector3 &ay, const tvector3 &az, const tvector4 &col)
{
	tvector3 pts[8];
	for (int i = 0; i < 8; i++)
	{
		pts[i] = center;
		pts[i] += ax * ((i & 1) ? 1.0f : -1.0f);
		pts[i] += ay * ((i & 2) ? 1.0f : -1.0f);
		pts[i] += az * ((i & 4) ? 1.0f : -1.0f);
	}

	//6个面(半透明填充)
	static const int faces[6][4] = {
		{0, 1, 3, 2}, {4, 6, 7, 5}, {0, 4, 5, 1},
		{2, 3, 7, 6}, {0, 2, 6, 4}, {1, 5, 7, 3}
	};
	tvector4 fill = col;
	fill.w = 0.45f;
	QVector<float> data;
	for (int f = 0; f < 6; f++)
	{
		for (int k = 0; k < 4; k++)
		{
			addVector3(data, pts[faces[f][k]]);
			addColor(data, fill);
		}
	}
	Draw(data, GL_QUADS, 24, true);

	//边框
	static const int edges[12][2] = {
		{0, 1}, {2, 3}, {4, 5}, {6, 7}, {0, 2}, {1, 3}, {4, 6}, {5, 7},
		{0, 4}, {1, 5}, {2, 6}, {3, 7}
	};
	data.clear();
	for (int e = 0; e < 12; e++)
	{
		addVector3(data, pts[edges[e][0]]);
		addColor(data, col);
		addVector3(data, pts[edges[e][1]]);
		addColor(data, col);
	}
	Draw(data, GL_LINES, 24);
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
	//线框加粗: 1px的轴在亮背景上几乎不可见, 抓取也不直观
	const bool isLinePrimitive = (mode == GL_LINES || mode == GL_LINE_LOOP || mode == GL_LINE_STRIP);
	if (isLinePrimitive) {
		functions->glEnable(GL_LINE_SMOOTH);
		functions->glLineWidth(2.0f);
	}
	functions->glDrawArrays(mode, 0, count);
	if (isLinePrimitive) {
		functions->glLineWidth(1.0f);
		functions->glDisable(GL_LINE_SMOOTH);
	}
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
