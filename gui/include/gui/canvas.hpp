#ifndef _h_textoolkit_gui_canvas
#define _h_textoolkit_gui_canvas

#include "renderer/api.hpp"
#include "renderer/object.hpp"
#include "renderer/camera.hpp"
#include "renderer/compass.hpp"
#include "renderer/renderer.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <GL/glew.h>

#include <wxt/language.h>
#include <wx/glcanvas.h>

#include <memory> 

namespace textoolkit
{
    class Canvas : public wxGLCanvas
    {
    public:
        Canvas(wxWindow* parent, wxWindowID id = wxID_ANY);
        Canvas(const Canvas& tc) = delete;
        Canvas(Canvas&& tc) = delete;

        virtual ~Canvas();

        void update();

        Canvas& operator=(const Canvas& tc) = delete;
        Canvas& operator=(Canvas&& tc) = delete;

        void addObject(renderer::Object* object);
        renderer::Object* getObject(unsigned int index);
        const std::vector<renderer::Object*>& getObjects() const;
        void removeObject(renderer::Object* object);
        void clearObjects();

        glm::uvec2 getViewportSize() const;

    private:
        void cameraArcball(wxMouseEvent& event);
        void cameraStrafe(wxMouseEvent& event);

        void onPaint(wxPaintEvent& event);
        void onSize(wxSizeEvent& event);

        void onMiddleButtonDown(wxMouseEvent& event);
        void onMiddleButtonUp(wxMouseEvent& event);
        void onMouseMove(wxMouseEvent& event);
        void onMouseWheel(wxMouseEvent& event);

        renderer::Api api;
        renderer::Renderer renderer;

        std::vector<renderer::Object*> objects;

        renderer::Compass compass;

        bool middleButtonDown = false;
        glm::ivec2 lastMousePos{0};
        wxLongLong lastUpdateMillis = 0;
    };
}

#endif