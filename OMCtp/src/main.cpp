#define PROJECT_OMCTP
#define OMCTP_MAIN_CPP

#include <omc/omctp/config.h>

#include "main.h"

namespace omc
{
	namespace omctp
	{
		error real_omctp_instance::main(std::string p_cmd_line)
		{
			std::vector<param> params;
			try
			{
				params = param::parse_params(p_cmd_line);
			}
			catch (const error& e)
			{
				return e;
			}
			return error(error::code::feature_not_implemented, "OMCtp is not yet implemented");
		}
	}
}