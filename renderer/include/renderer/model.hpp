#ifndef _h_textoolkit_renderer_model
#define _h_textoolkit_renderer_model

#include "common/result.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <vector>
#include <string>
#include <memory>
#include <fstream>

namespace textoolkit::renderer
{
	class Model;

	class ModelRenderData
	{
	public:
		virtual ~ModelRenderData() = default;
		virtual void update(
			const std::vector<glm::vec3>& vertices, 
			const std::vector<glm::vec2>& uvs, 
			const std::vector<glm::vec3>& normals, 
			const std::vector<unsigned int>& indices
		) = 0;
	};

	using ModelLoadResult = Result<Model>;

	class Model
	{
	public:
		Model();
		Model(Model&& b);
		~Model();

		static ModelLoadResult load(const std::string& path);
		static ModelLoadResult load(std::istream& stream);

		ModelRenderData* getData() const;
		void setData(std::unique_ptr<ModelRenderData>&& data);

		const std::vector<glm::vec3>& getVertices() const;
		const std::vector<glm::vec2>& getUVs() const;
		const std::vector<glm::vec3>& getNormals() const;
		const std::vector<unsigned int>& getIndices() const;

		const std::string& getName() const;

		void setPoints(const std::vector<glm::vec3>& points);
		void setPoints(const std::vector<glm::vec3>& points, const std::vector<unsigned int>& indices);

		bool isUploaded() const;
		bool updateRequired() const;
		void clearUpdateStatus();

		std::optional<unsigned int> findVertexNU(const glm::vec3& vert, const glm::vec3& norm, const glm::vec2& uv) const;
		std::optional<unsigned int> findVertexN(const glm::vec3& vert, const glm::vec3& norm) const;
		std::optional<unsigned int> findVertexU(const glm::vec3& vert, const glm::vec2& uv) const;
		std::optional<unsigned int> findVertex(const glm::vec3& vert) const;

	protected:
		void index(
			const std::vector<unsigned int>& facesVert, 
			const std::vector<unsigned int>& facesNorm, 
			const std::vector<unsigned int>& facesUV
		);
		void calculateDimensions();

		std::unique_ptr<ModelRenderData> data;

		std::string name;
		glm::vec3 extents;

		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;
		std::vector<unsigned int> indices;

		bool needsUpdate = true;
	};
}

#endif