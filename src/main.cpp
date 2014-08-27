#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <hsa/Context.h>
#include <hsa/CommandQueue.h>
#include <hsa/Program.h>
#include <hsa/Kernel.h>
#include <hsa/RegisterMemory.h>
#include <hsa/WorkSize.h>
#include <hsa/Exception.h>

using namespace std;
using namespace hsa;

void printHSAContextInfo(Context hsaContext)
{
	char name[64] = {0};
	hsaContext.getHsaAgentInfo(HSA_AGENT_INFO_NAME, name);
	cout << "HSA device name : " << name << endl;

	uint32_t queue_size = 0;
	hsaContext.getHsaAgentInfo(HSA_AGENT_INFO_QUEUE_MAX_SIZE, &queue_size);
	cout << "The maximum queue size is : " << queue_size << endl;
}

void checkResult(int *in, int *out)
{
    int valid=1;
    int failIndex=0;
    for(int i=0; i<1024*1024; i++)
    {
    	int inValue = in[i];
    	int outValue = out[i];
        if(outValue!=inValue)
        {
            failIndex=i;
            valid=0;
            break;
        }
    }

    if(valid) {
        printf("Passed validation.\n");
    } else {
        printf("VALIDATION FAILED!\nBad index: %d\n", failIndex);
    }
}

int main()
{
	cout << "HSA Vector copy example start!!" << endl;

	Context hsaContext(HSA_DEVICE_TYPE_GPU);
	CommandQueue hsaCommandQueue(&hsaContext, HSA_QUEUE_TYPE_MULTI);
	Program vectorCopyProgram(&hsaContext, "src_cl/vector_copy.brig");
	Kernel vectorCopyKernel = vectorCopyProgram["&__OpenCL_vector_copy_kernel"];

	//Prepare input/output memory.
	RegisterMemory input(1024*1024*sizeof(int));
	RegisterMemory output(1024*1024*sizeof(int));
	int *ptrInput = input.getAddress<int>();
	srand(time(NULL));
	for(unsigned int i=0; i<1024*1024; i++)
		ptrInput[i] = rand();

	KernelArgs(&vectorCopyKernel) << &input << &output;
	WorkSize global(1024*1024), local(256);
	hsa_dispatch_packet_t packet =
			hsaCommandQueue.makeAqlPacket(&vectorCopyKernel, global, local);
	hsaCommandQueue.enqueueNDRangeKernel(packet);

	printHSAContextInfo(hsaContext);
	checkResult(input.getAddress<int>(), output.getAddress<int>());

	return 0;
}
