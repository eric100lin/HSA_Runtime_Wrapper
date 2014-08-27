#ifndef _HSA_COMMANDQUEUE_H_
#define _HSA_COMMANDQUEUE_H_

#include <hsa/Base.h>
#include <hsa/Context.h>

namespace hsa
{
	class Context;
	class Kernel;
	class WorkSize;

	class CommandQueue
	{
		public:
			hsa_queue_t *_hsaCommandQueue;
			Context *_context;
			
			CommandQueue(Context *context, hsa_queue_type_t queueType=HSA_QUEUE_TYPE_MULTI, uint32_t queueSize=0);
			~CommandQueue();

			hsa_dispatch_packet_t makeAqlPacket(const Kernel *kernel,
					const WorkSize &globalWorkSize, const WorkSize &localWorkSize) const;
			void enqueueNDRangeKernel(hsa_dispatch_packet_t aqlPacket);
	};
}

#endif
