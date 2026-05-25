#ifndef OMC_FILE_H
#define OMC_FILE_H

#ifdef PROJECT_OMCTP
#include <omc/omctp/config.h>
#endif

#include <bitset>
#include <fstream>
#include <memory>
#include <cstdlib>
#include <cstring>
#include <nlohmann/json.hpp>
#include <omc/error.h>

namespace omc
{
	class file
	{
		public:
			std::fstream m_file;

		private:

			std::bitset<4> m_mode; // read, write, binary, text

		public:
			virtual inline error open(std::shared_ptr<std::string> p_path, std::bitset<4> p_mode)
			{
				if(p_mode[2] ^ p_mode[3])
					m_mode = p_mode;
				else
					return error(error::code::file_mode_error, "Invalid file mode specified: " + p_mode.to_string());

				if(m_mode[0] && m_mode[1]) // read and write
				{
					m_file.open(*p_path, std::ios::in | std::ios::out | std::ios::binary);
				}
				else if (m_mode[0]) // read
				{
					m_file.open(*p_path, std::ios::in | std::ios::binary);
				}
				else if (m_mode[1]) // write
				{
					m_file.open(*p_path, std::ios::out | std::ios::binary);
				}
				else // invalid mode
				{
					return error(error::code::file_mode_error, "Invalid file mode specified: " + p_mode.to_string());
				}

				if (!m_file.is_open())
					return error(error::code::file_generic_error, "Failed to open file: \"" + *p_path + "\"\n" + std::strerror(errno));
				return error(error::code::success, "");
			}

			virtual inline void close()
			{
				m_file.close();
			}
	};
}

#endif // OMC_FILE_H