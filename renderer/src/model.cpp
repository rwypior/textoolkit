#include "renderer/model.hpp"

namespace textoolkit::renderer
{
	Model::Model() = default;
	Model::~Model() = default;

	ModelRenderData* Model::getData() const
	{
		return this->data.get();
	}

	void Model::setData(std::unique_ptr<ModelRenderData>&& data)
	{
		this->data = std::move(data);
	}

	const std::vector<glm::vec3>& Model::getVertices() const
	{
		return this->vertices;
	}

	const std::vector<glm::vec3>& Model::getNormals() const
	{
		return this->normals;
	}

	const std::vector<unsigned int>& Model::getIndices() const
	{
		return this->indices;
	}

	std::string Model::getName() const
	{
		return this->name;
	}

	void Model::setPoints(const std::vector<glm::vec3>& points)
	{
		std::vector<unsigned int> indices;
		for (unsigned int i = 0; i < points.size(); i++)
		{
			indices.push_back(i);
		}

		this->setPoints(points, indices);
	}

	void Model::setPoints(const std::vector<glm::vec3>& points, const std::vector<unsigned int>& indices)
	{
		this->vertices = points;
		this->indices = indices;
		this->needsUpdate = true;
	}

	bool Model::isUploaded() const
	{
		return !!this->data;
	}

	bool Model::updateRequired() const
	{
		return this->needsUpdate;
	}
}