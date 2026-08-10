#include "common/image.hpp"

#include <fstream>

namespace textoolkit
{
	bool Image::save(const std::string& path) const
	{
		std::ofstream ofs(path, std::ios_base::out | std::ios_base::binary);

		if (ofs.is_open())
		{
			return this->save(ofs);
		}

		return false;
	}
}