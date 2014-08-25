#ifndef _HSA_COMMANDQUEUE_H_
#define _HSA_COMMANDQUEUE_H_

#include <hsa/Base.h>
#include <hsa/Context.h>

namespace hsa
{
	class Context;

	class CommandQueue
	{
		public:
			hsa_queue_t *_hsaCommandQueue;
			Context *_context;
			
			CommandQueue(Context *context, hsa_queue_type_t queueType=HSA_QUEUE_TYPE_MULTI, size_t queueSize=0);
			~CommandQueue();
	};
}

#endif
