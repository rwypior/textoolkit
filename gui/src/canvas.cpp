#include "gui/canvas.hpp"

#include "wx/wxprec.h"
#include "wx/time.h"
#include "wx/arrstr.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace
{
	wxGLAttributes getDefaultAttribs()
	{
		wxGLAttributes dispAttrs;
		dispAttrs
			.PlatformDefaults()
			.Depth(24)
			.Stencil(8)
			.RGBA()
			.DoubleBuffer()
			.EndList();
		return dispAttrs;
	}
}

namespace textoolkit
{
	Canvas::Canvas(wxWindow* parent, wxWindowID id)
		: wxGLCanvas(parent, getDefaultAttribs(), id)
	{
		wxGLContextAttrs ctxAttrs;
		ctxAttrs.PlatformDefaults().OGLVersion(4, 0).EndList();

		m_context = std::make_unique<wxGLContext>(this, nullptr, &ctxAttrs);

		if (!m_context->IsOK())
			wxMessageBox("Unable to initialize drawing context", "Error - textoolkit", wxOK | wxCENTRE | wxICON_ERROR, this);

		this->Bind(wxEVT_PAINT, &Canvas::onPaint, this);
		this->Bind(wxEVT_SIZE, &Canvas::onSize, this);

		this->Bind(wxEVT_MIDDLE_DOWN, &Canvas::onMiddleButtonDown, this);
		this->Bind(wxEVT_MIDDLE_UP, &Canvas::onMiddleButtonUp, this);
		this->Bind(wxEVT_MOTION, &Canvas::onMouseMove, this);
		this->Bind(wxEVT_MOUSEWHEEL, &Canvas::onMouseWheel, this);
	}

	Canvas::~Canvas()
	{
		SetCurrent(*m_context);
	}

	void Canvas::update()
	{

		this->Refresh();

		wxLongLong currentMillis = wxGetUTCTimeMillis();
		if (currentMillis - this->lastUpdateMillis >= (1.0 / 60.0 * 1000.0))
		{
			this->Update();
			this->lastUpdateMillis = currentMillis;
		}
	}

	void Canvas::cameraArcball(wxMouseEvent& event)
	{
		glm::ivec2 mousePos;
		event.GetPosition(&mousePos.x, &mousePos.y);

		glm::uvec2 viewport = getViewportSize();
				
		glm::vec4 position(this->renderer.getCameraPosition(), 1);
		glm::vec4 pivot(this->renderer.getCameraLookTarget(), 1);
		glm::vec3 upVector = this->renderer.getCameraUpVector();

		float deltaAngleX = (2 * M_PI / viewport.x); // a movement from left to right = 2*PI = 360 deg
		float deltaAngleY = (M_PI / viewport.y);  // a movement from top to bottom = PI = 180 deg
		float xAngle = (this->lastMousePos.x - mousePos.x) * deltaAngleX;
		float yAngle = (this->lastMousePos.y - mousePos.y) * deltaAngleY;

		glm::vec3 lookDirection = glm::normalize(pivot - position);

		float cosAngle = glm::dot(lookDirection, upVector);
		if (cosAngle * glm::sin(deltaAngleY) > 0.99f)
			deltaAngleY = 0;

		// X axis
		glm::mat4x4 rotationMatrixX(1.0f);
		rotationMatrixX = glm::rotate(rotationMatrixX, xAngle, upVector);
		position = (rotationMatrixX * (position - pivot)) + pivot;

		// Y axis
		glm::vec3 rightVector = glm::cross(lookDirection, upVector);
		glm::mat4x4 rotationMatrixY(1.0f);
		rotationMatrixY = glm::rotate(rotationMatrixY, yAngle, rightVector);
		glm::vec3 finalPosition = (rotationMatrixY * (position - pivot)) + pivot;

		this->renderer.setCameraPosition(finalPosition);
		this->lastMousePos = mousePos;
	}

	void Canvas::cameraStrafe(wxMouseEvent& event)
	{
		glm::ivec2 mousePos;
		event.GetPosition(&mousePos.x, &mousePos.y);

		float xDelta = (this->lastMousePos.x - mousePos.x) * 0.01;
		float yDelta = (this->lastMousePos.y - mousePos.y) * 0.01;

		glm::vec3 position = this->renderer.getCameraPosition();
		glm::vec3 looktarget = this->renderer.getCameraLookTarget();
		glm::vec3 upVector = this->renderer.getCameraUpVector();

		glm::vec3 lookDirection = glm::normalize(looktarget - position);
		glm::vec3 rightVector = glm::normalize(glm::cross(lookDirection, upVector));

		glm::vec3 offset = rightVector * xDelta - upVector * yDelta;

		glm::vec3 newLookTarget = looktarget + offset;
		this->renderer.setCameraLookTarget(newLookTarget);

		glm::vec3 newPosition = position + offset;
		this->renderer.setCameraPosition(newPosition);

		this->update();

		this->lastMousePos = mousePos;
	}

	void Canvas::onPaint(wxPaintEvent& event)
	{
		bool success = SetCurrent(*m_context);
		assert(success && "Failed to set context");

		this->renderer.render();

		SwapBuffers();
	}

	void Canvas::onSize(wxSizeEvent& event)
	{
		glm::uvec2 size = this->getViewportSize();
		this->renderer.setViewportSize(size);
	}

	void Canvas::onMiddleButtonDown(wxMouseEvent& event)
	{
		this->middleButtonDown = true;
		event.GetPosition(&this->lastMousePos.x, &this->lastMousePos.y);
	}

	void Canvas::onMiddleButtonUp(wxMouseEvent& event)
	{
		this->middleButtonDown = false;
	}

	void Canvas::onMouseMove(wxMouseEvent& event)
	{
		if (this->middleButtonDown)
		{
			if (event.ShiftDown())
			{
				this->cameraStrafe(event);
			}
			else
			{
				this->cameraArcball(event);
			}
		}
	}

	void Canvas::onMouseWheel(wxMouseEvent& event)
	{
		float delta = static_cast<float>(event.GetWheelRotation()) / (120.0f * 5.0f);
		glm::vec3 position = this->renderer.getCameraPosition();
		glm::vec3 looktarget = this->renderer.getCameraLookTarget();
		glm::vec3 lookDirection = glm::normalize(looktarget - position);
		glm::vec3 newPosition = position + lookDirection * delta;
		this->renderer.setCameraPosition(newPosition);
	}

	void Canvas::addObject(renderer::Object* object)
	{
		this->objects.push_back(object);
		this->Refresh();
	}

	renderer::Object* Canvas::getObject(unsigned int index)
	{
		return this->objects[index];
	}

	const std::vector<renderer::Object*>& Canvas::getObjects() const
	{
		return this->objects;
	}

	void Canvas::removeObject(renderer::Object* object)
	{
		for (auto it = this->objects.begin(); it != this->objects.end(); it++)
		{
			if (*it == object)
			{
				this->objects.erase(it);
				this->Refresh();
				return;
			}
		}
	}

	void Canvas::clearObjects()
	{
		this->objects.clear();
		this->Refresh();
	}

	glm::uvec2 Canvas::getViewportSize() const
	{
		wxSize size = this->GetClientSize() * this->GetContentScaleFactor();
		return glm::uvec2(size.x, size.y);
	}
}