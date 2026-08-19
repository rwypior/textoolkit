#ifndef _h_textoolkit_renderer_modeldatabase
#define _h_textoolkit_renderer_modeldatabase

#include "common/result.hpp"

#include <wx/arrstr.h>

#include <unordered_map>
#include <map>
#include <string>
#include <memory>
#include <vector>

namespace textoolkit::renderer
{
	class Model;

	struct DatabaseModelLoadSubresult
	{
		struct Message
		{
			std::vector<std::string> errors;
			std::vector<std::string> warnings;
		};
		std::map<std::string, Message> messages;
	};

	using DatabaseModelLoadResult = Result<DatabaseModelLoadSubresult>;

	class ModelDatabase
	{
	public:
		using ModelContainer = std::unordered_map<std::string, std::shared_ptr<Model>>;

	public:
		DatabaseModelLoadResult loadModels(const wxArrayString& paths);
		Model* addModel(Model&& model);

		std::shared_ptr<Model> findModel(const std::string& name) const;

		ModelContainer::iterator begin();
		ModelContainer::iterator end();

	private:
		ModelContainer models;
	};
}

#endif