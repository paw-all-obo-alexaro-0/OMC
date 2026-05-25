#ifndef OMC_ERROR_H
#define OMC_ERROR_H

#include <string>
#include <utility>

namespace omc
{
	class error
	{
		public:
			enum class code : unsigned short
			{
				success = 0,
				feature_not_implemented = 1,
				command_line_generic_error = 101,
				command_line_syntax_error = 102,
				command_line_json_error = 103,
				file_generic_error = 201,
				file_mode_error = 202
			};
			code m_error_code;
			std::string m_message;
			error(code p_error_code, const std::string& p_message) : m_error_code(p_error_code), m_message(p_message) {}
	};
}

#endif // OMC_ERROR_H