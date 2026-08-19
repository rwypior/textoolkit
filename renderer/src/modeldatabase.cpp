#include "renderer/modeldatabase.hpp"
#include "renderer/model.hpp"

#include <sstream>

namespace textoolkit::renderer
{
	DatabaseModelLoadResult ModelDatabase::loadModels(const wxArrayString& paths)
	{
		DatabaseModelLoadSubresult subresult;

		this->models.clear();
		for (const auto& path : paths)
		{
			auto subresultit = subresult.messages.find(path.ToStdString());
			if (subresultit == subresult.messages.end())
				subresult.messages[path.ToStdString()] = {};
			auto& messages = subresult.messages.at(path.ToStdString());

			auto result = Model::load(path.ToStdString());
			if (!result)
			{
				messages.errors.push_back((std::stringstream() << "Model \"" << path << "\" could not be loaded").str());
				continue;
			}

			Model model = std::move(*result);
			std::string modelName = model.getName();

			std::transform(result.warnings.begin(), result.warnings.end(), messages.warnings.begin(), [](ModelLoadResult::Warning& warning) { 
				return warning.toString();
			});

			this->models[modelName] = std::make_unique<Model>(std::move(model));
		}

		if (this->models.empty())
			return DatabaseModelLoadResult::Code::NothingLoaded;

		return subresult;
	}

	Model* ModelDatabase::addModel(Model&& model)
	{
		std::string modelName = model.getName();
		this->models[modelName] = std::make_unique<Model>(std::move(model));
		return this->models.at(modelName).get();
	}

	std::shared_ptr<Model> ModelDatabase::findModel(const std::string& name) const
	{
		auto it = this->models.find(name);
		if (it == this->models.end())
			return nullptr;
		return it->second;
	}

	ModelDatabase::ModelContainer::iterator ModelDatabase::begin()
	{
		return this->models.begin();
	}

	ModelDatabase::ModelContainer::iterator ModelDatabase::end()
	{
		return this->models.end();
	}
}