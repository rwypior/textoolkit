#ifndef _h_textoolkit_renderer_model
#define _h_textoolkit_renderer_model

#include <glm/vec3.hpp>

#include <vector>
#include <string>
#include <memory>

namespace textoolkit::renderer
{
	class ModelRenderData
	{
	public:
		virtual ~ModelRenderData() = default;
		virtual void update(const std::vector<glm::vec3>& points, const std::vector<unsigned int>& indices) = 0;
	};

	class Model
	{
	public:
		Model();
		~Model();

		ModelRenderData* getData() const;
		void setData(std::unique_ptr<ModelRenderData>&& data);

		virtual const std::vector<glm::vec3>& getVertices() const;
		virtual const std::vector<glm::vec3>& getNormals() const;
		virtual const std::vector<unsigned int>& getIndices() const;

		virtual std::string getName() const;

		void setPoints(const std::vector<glm::vec3>& points);
		void setPoints(const std::vector<glm::vec3>& points, const std::vector<unsigned int>& indices);

		bool isUploaded() const;
		bool updateRequired() const;

	protected:
		std::unique_ptr<ModelRenderData> data;

		std::string name;

		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> indices;

		bool needsUpdate = false;
	};
}

#endif