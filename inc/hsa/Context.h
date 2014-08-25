#ifndef _HSA_CONTEXT_H_
#define _HSA_CONTEXT_H_

#include <hsa/Base.h>

namespace hsa
{
	class Context
	{
		private:
			hsa_agent_t hsaAgent;
		public:		
			Context(hsa_device_type_t deviceType=HSA_DEVICE_TYPE_GPU);
			~Context();
			hsa_agent_t getHsaAgent() const;
			void getHsaAgentInfo(hsa_agent_info_t attribute, void *value) const;
	};
} // namespace hsa

#endif
