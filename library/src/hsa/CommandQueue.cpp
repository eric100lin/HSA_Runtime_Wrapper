#include <hsa/Exception.h>
#include <hsa/CommandQueue.h>
#include <hsa/WorkSize.h>
#include <hsa/Context.h>
#include <hsa/Kernel.h>
#include <cstring>

using namespace hsa;

CommandQueue::CommandQueue(Context *context, hsa_queue_type_t queueType, uint32_t queueSize)
	: _context(context)
{
	hsa_agent_t hsaAgent = context->getHsaAgent();
	
	uint32_t maxQueueSize;
	context->getHsaAgentInfo(HSA_AGENT_INFO_QUEUE_MAX_SIZE, &maxQueueSize);

	size_t dstQueueSize = queueSize==0?maxQueueSize:queueSize;
    hsa_status_t err = hsa_queue_create(hsaAgent, dstQueueSize,
    		queueType, NULL, NULL, &_hsaCommandQueue);
    CHECK_HSA(Creating the queue, err);
}

CommandQueue::~CommandQueue()
{
    hsa_status_t err=hsa_queue_destroy(_hsaCommandQueue);
    CHECK_HSA(Destroying the queue, err);
}

hsa_dispatch_packet_t CommandQueue::makeAqlPacket(const Kernel *kernel,
		const WorkSize &globalWorkSize, const WorkSize &localWorkSize) const
{
    /*
     * Create a signal to wait for the dispatch to finish.
     */
    hsa_signal_t signal;
    hsa_status_t err=hsa_signal_create(1, 0, NULL, &signal);
    CHECK_HSA(Creating a HSA signal, err);

    /*
     * Initialize the dispatch packet.
     * And setup the dispatch information.
     */
    hsa_dispatch_packet_t aql;
    memset(&aql, 0, sizeof(aql));
    aql.completion_signal=signal;
    aql.dimensions=globalWorkSize.Dimensions();
    aql.grid_size_x=globalWorkSize.Size0();
    aql.grid_size_y=globalWorkSize.Size1();
    aql.grid_size_z=globalWorkSize.Size2();
    aql.workgroup_size_x=localWorkSize.Size0();
    aql.workgroup_size_y=localWorkSize.Size1();
    aql.workgroup_size_z=localWorkSize.Size2();
    aql.header.type=HSA_PACKET_TYPE_DISPATCH;
    aql.header.acquire_fence_scope=2;
    aql.header.release_fence_scope=2;
    aql.header.barrier=1;
    aql.group_segment_size=kernel->getGroupSegmentSize();
    aql.private_segment_size=kernel->getPrivateSegmentSize();
    aql.kernel_object_address=kernel->getExtCodeHandle();
    aql.kernarg_address=kernel->getKernArgAddress();

    return aql;
}

void CommandQueue::enqueueNDRangeKernel(hsa_dispatch_packet_t aqlPacket)
{
    /*
     * Obtain the current queue write index.
     */
    uint64_t index = hsa_queue_load_write_index_relaxed(_hsaCommandQueue);
    const uint32_t queueMask = _hsaCommandQueue->size - 1;
    ((hsa_dispatch_packet_t*)(_hsaCommandQueue->base_address))[index&queueMask]=aqlPacket;
    hsa_queue_store_write_index_relaxed(_hsaCommandQueue, index+1);
    hsa_signal_store_relaxed(_hsaCommandQueue->doorbell_signal, index);

    hsa_signal_value_t signalValue =
    		hsa_signal_wait_acquire(aqlPacket.completion_signal, HSA_LT, 1,
    				(uint64_t) -1, HSA_WAIT_EXPECTANCY_UNKNOWN);
    if(signalValue!=0)
    	THROW(HSA signal wait error);

    hsa_status_t err=hsa_signal_destroy(aqlPacket.completion_signal);
    CHECK_HSA(Creating a HSA signal, err);
}
