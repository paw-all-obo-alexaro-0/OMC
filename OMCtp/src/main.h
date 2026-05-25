#ifndef OMCTP_MAIN_H
#define OMCTP_MAIN_H

#include <string>
#include <vector>
#include <omc/params.h>
#include <omc/error.h>

namespace omc
{
	namespace omctp
	{
		class real_omctp_instance
		{
			public:
				error main(std::string params);
		};
	}
}

#endif // OMCTP_MAIN_H