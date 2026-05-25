#ifndef OMC_PARAMS_H
#define OMC_PARAMS_H

#include <vector>
#include <string>
#include <utility>
#include <functional>
#include <cctype>
#include <stdexcept>

#define JSON_DIAGNOSTIC_POSITIONS 1
#include <nlohmann/json.hpp>

#include <omc/error.h>

using json = nlohmann::json;

namespace omc
{
	class param // A simple struct to hold a parameter name and its value as JSON
	{
	public:
		std::string m_name;
		json m_value;

		inline static std::vector<param> parse_params(std::string& p_cmd_line)
		{
			try // Parses a command line string into a vector of params, ignores tokens not starting with "--" and allows for quoted strings and nested brackets
			{
				auto skip_spaces = [](std::string& str, std::string::iterator it) -> std::string::iterator
					{
						while (it != str.end() && std::isspace(static_cast<unsigned char>(*it)))
							++it;
						return it;
					};

				std::vector<param> params;

				std::string::iterator it = skip_spaces(p_cmd_line, p_cmd_line.begin());

				while (it != p_cmd_line.end())
				{
					std::string::iterator param_start = it;
					if (std::distance(it, p_cmd_line.end()) >= 2 && *it == '-' && *(it + 1) == '-')
					{
						param p;
						it += 2; // Skip the "--"
						if (it == p_cmd_line.end() || std::isspace(static_cast<unsigned char>(*it)))
							throw error(error::code::command_line_syntax_error, "Parameter name expected after '--':\n" + p_cmd_line + "\n" + std::string(it - p_cmd_line.begin(), ' ') + "^");
						if (*it == '"')
						{
							++it; // Skip opening quote
							auto end_quote = std::find(it, p_cmd_line.end(), '"');
							if(end_quote == p_cmd_line.end())
								throw error(error::code::command_line_syntax_error, "Unmatched quote in parameter name starting with \"--" + std::string(it, end_quote) + "\":\n" + p_cmd_line + "\n" + std::string(it - p_cmd_line.begin(), ' ') + "^");
							p.m_name.assign(it, end_quote);
							it = end_quote + 1; // Move past the closing quote
						}
						else
						{

							auto name_end = std::find_if(it, p_cmd_line.end(), [](char c) { return std::isspace(static_cast<unsigned char>(c)) || c == '='; });
							p.m_name.assign(it, name_end);
							it = name_end;
						}
						it = skip_spaces(p_cmd_line, it);
						if (it == p_cmd_line.end())
						{
							p.m_value = json::object();
						}
						else if (*it == '=')
						{
							it++; // Skip the '='
							it = skip_spaces(p_cmd_line, it);
							try
							{
								p.m_value = json::parse(p_cmd_line.substr(it - p_cmd_line.begin())); // Parse the value as JSON
							}
							catch (const json::parse_error& e)
							{
								throw error(error::code::command_line_json_error, "Invalid JSON value for parameter \"--" + p.m_name + "\": " + e.what());
							}
							it += p.m_value.end_pos();
						}
						else
						{
							p.m_value = json::object();
						}
						params.push_back(std::move(p));
						it = skip_spaces(p_cmd_line, it);
					}
					else
					{
						int depth = 0;
						bool in_quote = false, in_double_quote = false, escaped = false;
						while (it != p_cmd_line.end() && (in_quote || in_double_quote || depth > 0 || !std::isspace(static_cast<unsigned char>(*it))))
						{
							if (*it == '\\' && !escaped)
								escaped = true;
							else if (*it == '\'' && !in_double_quote && !escaped)
								in_quote = !in_quote;
							else if (*it == '"' && !in_quote && !escaped)
								in_double_quote = !in_double_quote;
							else if ((*it == '(' || *it == '{' || *it == '[') && !in_quote && !in_double_quote)
								depth++;
							else if ((*it == ')' || *it == '}' || *it == ']') && !in_quote && !in_double_quote)
								depth--;
							else
								escaped = false;
							it++;
						}
						if (depth < 0)
							throw error(error::code::command_line_syntax_error, "Unmatched closing bracket in command line:\n" + p_cmd_line + "\n" + std::string(it - p_cmd_line.begin(), ' ') + "^");
					}
					it = p_cmd_line.erase(param_start, it); // Remove the non-parameter token from the command line
					it = skip_spaces(p_cmd_line, it);
				}
				return params;
			}
			catch(const error&)
			{
				throw; // Rethrow our custom error
			}
			catch(const std::exception& e)
			{
				throw error(error::code::command_line_generic_error, "Error parsing command line: " + std::string(e.what()));
			}
		}
	};
}

#endif // OMC_PARAMS_H