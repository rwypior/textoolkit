#include "renderer/model.hpp"
#include "common/util.hpp"

#include <glm/gtx/component_wise.hpp>

#include <functional>
#include <array>
#include <string>
#include <limits>

namespace
{
	struct FaceInserter
	{
		using InserterFn = std::function<void(std::array<unsigned int, 3>)>;
		using ErrInserterFn = std::function<void(const std::string&)>;

		FaceInserter(InserterFn vertexInserter, InserterFn uvInserter, InserterFn normalInserter, ErrInserterFn errInserter)
			: vertexInserter(vertexInserter)
			, uvInserter(uvInserter)
			, normalInserter(normalInserter)
			, errInserter(errInserter)
		{
		}

		template<typename T>
		void operator()(T&) {}

		template<>
		void operator()(textoolkit::model::obj::FaceVUN& vun)
		{
			this->vertexInserter({ vun.at(0).vertex, vun.at(1).vertex, vun.at(2).vertex });
			this->uvInserter({ vun.at(0).uv, vun.at(1).uv, vun.at(2).uv });
			this->normalInserter({ vun.at(0).normal, vun.at(1).normal, vun.at(2).normal });
		}

		template<>
		void operator()(textoolkit::model::obj::FaceVU& vun)
		{
			this->vertexInserter({ vun.at(0).vertex, vun.at(1).vertex, vun.at(2).vertex });
			this->uvInserter({ vun.at(0).uv, vun.at(1).uv, vun.at(2).uv });
		}

		template<>
		void operator()(textoolkit::model::obj::FaceVN& vun)
		{
			this->vertexInserter({ vun.at(0).vertex, vun.at(1).vertex, vun.at(2).vertex });
			this->normalInserter({ vun.at(0).normal, vun.at(1).normal, vun.at(2).normal });
		}

		template<>
		void operator()(textoolkit::model::obj::FaceV& vun)
		{
			this->vertexInserter({ vun.at(0).vertex, vun.at(1).vertex, vun.at(2).vertex });
		}

		template<>
		void operator()(textoolkit::model::obj::FaceErr& err)
		{
			this->errInserter(err);
		}

		InserterFn vertexInserter;
		InserterFn uvInserter;
		InserterFn normalInserter;
		ErrInserterFn errInserter;
	};
}

namespace textoolkit::renderer
{
	Model::Model() = default;
	Model::~Model() = default;

	Model::Model(Model&& b)
		: data(std::move(b.data))
		, name(std::move(b.name))
		, extents(std::move(b.extents))
		, vertices(std::move(b.vertices))
		, normals(std::move(b.normals))
		, uvs(std::move(b.uvs))
		, indices(std::move(b.indices))
		, needsUpdate(std::move(b.needsUpdate))
	{
	}

	ModelLoadResult Model::load(const std::string& path)
	{
		std::ifstream stream(path, std::ios_base::in | std::ios_base::binary);
		if (!stream.is_open())
			return ModelLoadResult::Code::FileInaccessible;
		return Model::load(stream);
	}

	ModelLoadResult Model::load(std::istream& stream)
	{
		ModelLoadResult::Warnings warnings;
		Model model;

		std::vector<unsigned int> facesVert;
		std::vector<unsigned int> facesNorm;
		std::vector<unsigned int> facesUV;

		unsigned int lineNumber = 0;
		for (std::string line; std::getline(stream, line); lineNumber++)
		{
			ltrim(line);

			if (startsWith(line, "#"))
				continue;

			if (startsWith(line, "o "))
			{
				model.name = line.substr(2);
				if (!model.name.empty())
					warnings.emplace_back("Object specified more than once", lineNumber);
				continue;
			}

			if (startsWith(line, "mtllib"))
				continue;

			if (startsWith(line, "usemtl"))
				continue;

			if (startsWith(line, "vt"))
			{
				if (auto vec = model::obj::extractVec2(line))
					model.uvs.push_back(*vec);
				else
					warnings.emplace_back("Invalid UV specification", lineNumber);
				continue;
			}

			if (startsWith(line, "vn"))
			{
				if (auto vec = model::obj::extractVec3(line))
					model.normals.push_back(*vec);
				else
					warnings.emplace_back("Invalid normal specification", lineNumber);
				continue;
			}

			if (startsWith(line, "v"))
			{
				if (auto vec = model::obj::extractVec3(line))
					model.vertices.push_back(*vec);
				else
					warnings.emplace_back("Invalid vertex specification", lineNumber);
				continue;
			}

			if (startsWith(line, "f "))
			{
				auto face = model::obj::extractFace(line);

				std::string error;
				FaceInserter inserter(
					[&facesVert](auto& f) { facesVert.insert(facesVert.end(), f.begin(), f.end()); },
					[&facesUV](auto& f) { facesUV.insert(facesUV.end(), f.begin(), f.end()); },
					[&facesNorm](auto& f) { facesNorm.insert(facesNorm.end(), f.begin(), f.end()); },
					[&error](auto& f) { error = f; }
				);

				std::visit(inserter, face);

				if (!error.empty())
					warnings.emplace_back(error, lineNumber);

				continue;
			}
		}

		model.index(facesVert, facesNorm, facesUV);
		model.calculateDimensions();

		return model;
	}

	void Model::index(
		const std::vector<unsigned int>& facesVert,
		const std::vector<unsigned int>& facesNorm,
		const std::vector<unsigned int>& facesUV
	)
	{
		bool hasNormals = !this->normals.empty();
		bool hasUVs = !this->uvs.empty();

		std::vector<glm::vec3> vertices;
		vertices.swap(this->vertices);

		std::vector<glm::vec3> normals;
		normals.swap(this->normals);

		std::vector<glm::vec2> uvs;
		uvs.swap(this->uvs);

		this->indices.clear();

		for (unsigned int j = 0; j < facesVert.size(); ++j)
		{
			bool result;
			std::optional<unsigned int> index;

			if (hasNormals && hasUVs)
				index = this->findVertexNU(vertices[facesVert[j]], normals[facesNorm[j]], uvs[facesUV[j]]);
			else if (hasNormals)
				index = this->findVertexN(vertices[facesVert[j]], normals[facesNorm[j]]);
			else if (hasUVs)
				index = this->findVertexU(vertices[facesVert[j]], uvs[facesUV[j]]);
			else
				index = this->findVertex(vertices[facesVert[j]]);

			if (!index)
			{
				index = static_cast<unsigned int>(this->vertices.size());

				this->vertices.push_back(vertices[facesVert[j]]);

				if (hasNormals)
					this->normals.push_back(normals[facesNorm[j]]);

				if (hasUVs)
					this->uvs.push_back(uvs[facesUV[j]]);
			}

			this->indices.push_back(*index);
		}
	}

	void Model::calculateDimensions()
	{
		static constexpr float fltmin = std::numeric_limits<float>::lowest();
		static constexpr float fltmax = std::numeric_limits<float>::max();
		float temp[6] = { fltmin, fltmax, fltmin, fltmax, fltmin, fltmax };

		for (unsigned int idx : this->indices)
		{
			if (this->vertices[idx].x > temp[0])
				temp[0] = this->vertices[idx].x;
			if (this->vertices[idx].x < temp[1])
				temp[1] = this->vertices[idx].x;
			if (this->vertices[idx].y > temp[2])
				temp[2] = this->vertices[idx].y;
			if (this->vertices[idx].y < temp[3])
				temp[3] = this->vertices[idx].y;
			if (this->vertices[idx].z > temp[4])
				temp[4] = this->vertices[idx].z;
			if (this->vertices[idx].z < temp[5])
				temp[5] = this->vertices[idx].z;
		}

		this->extents = glm::abs(glm::vec3(temp[0] - temp[1], temp[2] - temp[3], temp[4] - temp[5]) * 0.5f);
	}

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

	const std::vector<glm::vec2>& Model::getUVs() const
	{
		return this->uvs;
	}

	const std::vector<glm::vec3>& Model::getNormals() const
	{
		return this->normals;
	}

	const std::vector<unsigned int>& Model::getIndices() const
	{
		return this->indices;
	}

	const std::string& Model::getName() const
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
		return this->needsUpdate || !this->data;
	}

	void Model::clearUpdateStatus()
	{
		this->needsUpdate = false;
	}

	std::optional<unsigned int> Model::findVertexNU(const glm::vec3& vert, const glm::vec3& norm, const glm::vec2& uv) const
	{
		for (unsigned i = 0; i < this->vertices.size(); ++i)
		{
			if (this->vertices[i] == vert && this->normals[i] == norm && this->uvs[i] == uv)
				return i;
		}
		return {};
	}

	std::optional<unsigned int> Model::findVertexN(const glm::vec3& vert, const glm::vec3& norm) const
	{
		for (unsigned int i = 0; i < this->vertices.size(); ++i)
		{
			if (this->vertices[i] == vert && this->normals[i] == norm)
				return i;
		}
		return {};
	}

	std::optional<unsigned int> Model::findVertexU(const glm::vec3& vert, const glm::vec2& uv) const
	{
		for (unsigned int i = 0; i < this->vertices.size(); ++i)
		{
			if (this->vertices[i] == vert && this->uvs[i] == uv)
				return i;
		}
		return {};
	}

	std::optional<unsigned int> Model::findVertex(const glm::vec3& vert) const
	{
		for (unsigned int i = 0; i < this->vertices.size(); ++i)
		{
			if (this->vertices[i] == vert)
				return i;
		}
		return {};
	}
}