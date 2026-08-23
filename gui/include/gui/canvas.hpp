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
    class Image;

    namespace renderer
    {
        class DisplayMode;
    }

    class Canvas : public wxGLCanvas
    {
        using ObjectsContainer = std::vector<std::unique_ptr<renderer::Object>>;

    public:
        Canvas(wxWindow* parent, wxWindowID id = wxID_ANY);
        Canvas(const Canvas& tc) = delete;
        Canvas(Canvas&& tc) = delete;

        virtual ~Canvas();

        void setImage(const Image& image);
        void update();

        Canvas& operator=(const Canvas& tc) = delete;
        Canvas& operator=(Canvas&& tc) = delete;

        renderer::Object* addObject(std::unique_ptr<renderer::Object> object);
        renderer::Object* getObject(unsigned int index);
        const ObjectsContainer& getObjects() const;
        void removeObject(renderer::Object* object);
        void clearObjects();

        glm::uvec2 getViewportSize() const;

        void setDisplayMode(const renderer::DisplayMode& mode);
        void setWrappingS(renderer::Wrapping wrap);
        void setWrappingT(renderer::Wrapping wrap);
        void setFilterMin(renderer::FilteringMin filter);
        void setFilterMag(renderer::FilteringMag filter);
        void setShowWireframe(bool show);
        void setCubeAlignment(const renderer::CubemapAlignment& alignment);

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

        ObjectsContainer objects;

        renderer::Compass compass;

        bool middleButtonDown = false;
        glm::ivec2 lastMousePos{0};
        wxLongLong lastUpdateMillis = 0;
    };
}

#endif