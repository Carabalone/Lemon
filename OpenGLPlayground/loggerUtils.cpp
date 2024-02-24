#include "loggerUtils.h"
#include <boost/property_tree/json_parser.hpp>

namespace lemon {
	std::string formatUBOInfo(const std::string& UBOName, const UBOInfo& uboInfo) {
		boost::property_tree::ptree root;
		root.put("UBO.name", UBOName);
		root.put("UBO.index", uboInfo.index);
		root.put("UBO.bindingPoint", uboInfo.bindingPoint);

		std::ostringstream oss;
		boost::property_tree::write_json(oss, root);
		return oss.str();
	}

}
