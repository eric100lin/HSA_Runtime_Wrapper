#include <hsa/Exception.h>
#include <hsa/CommandQueue.h>

using namespace hsa;

CommandQueue::CommandQueue(Context *context, hsa_queue_type_t queueType, size_t queueSize)
	: _context(context)
{
	hsa_agent_t hsaAgent = context->getHsaAgent();
    if(queueSize==0)
		context->getHsaAgentInfo(HSA_AGENT_INFO_QUEUE_MAX_SIZE, &queueSize);
	
    hsa_status_t err = hsa_queue_create(hsaAgent, queueSize, queueType, NULL, NULL, &_hsaCommandQueue);
    CHECK_HSA(Creating the queue, err);
}

CommandQueue::~CommandQueue()
{
    hsa_status_t err=hsa_queue_destroy(_hsaCommandQueue);
    CHECK_HSA(Destroying the queue, err);
}
