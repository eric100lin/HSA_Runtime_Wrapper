#include <stdio.h>
#include <stdlib.h>
#include <hsa/Context.h>
#include <hsa/Exception.h>

using namespace hsa;

static hsa_device_type_t _deviceType=HSA_DEVICE_TYPE_GPU;

static hsa_status_t find_device(hsa_agent_t agent, void *data) {
    if (data == NULL) {
        return HSA_STATUS_ERROR_INVALID_ARGUMENT;
    }
    hsa_device_type_t device_type;
    hsa_status_t stat =
    hsa_agent_get_info(agent, HSA_AGENT_INFO_DEVICE, &device_type);
    if (stat != HSA_STATUS_SUCCESS) {
        return stat;
    }
    if (device_type == _deviceType) {
        *((hsa_agent_t *)data) = agent;
    }
    return HSA_STATUS_SUCCESS;
}

Context::Context(hsa_device_type_t deviceType)
{
	hsa_status_t err = hsa_init();
	CHECK_HSA(Initializing the hsa runtime, err);
	
	/* 
     * Iterate over the agents and pick the _deviceType agent 
     * using the find_device callback.
     */
	 _deviceType = deviceType;
    err = hsa_iterate_agents(find_device, &hsaAgent);
    CHECK_HSA(Calling hsa_iterate_agents, err);
	
	err = (hsaAgent == 0) ? HSA_STATUS_ERROR : HSA_STATUS_SUCCESS;
    CHECK_HSA(CHECK_HSAing if the GPU hsaAgent is non-zero, err);
}

Context::~Context()
{
	hsa_status_t err=hsa_shut_down();
    CHECK_HSA(Shutting down the runtime, err);
}

hsa_agent_t Context::getHsaAgent() const
{
	return hsaAgent;
}

void Context::getHsaAgentInfo(hsa_agent_info_t attribute, void *value) const
{
	hsa_status_t err = hsa_agent_get_info(hsaAgent, attribute, value);
	CHECK_HSA(Querying the hsa agent info, err);
}
