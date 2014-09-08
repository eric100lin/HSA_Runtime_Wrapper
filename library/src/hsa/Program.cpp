#include <hsa/Program.h>
#include <hsa/Context.h>
#include <hsa/Kernel.h>
#include <hsa/Exception.h>
#include <hsa/elf_utils.h>
#include <cstring>

using namespace std;
using namespace hsa;

/*
 * Define required BRIG data structures.
 */
 
typedef uint32_t BrigCodeOffset32_t;

typedef uint32_t BrigDataOffset32_t;

typedef uint16_t BrigKinds16_t;

typedef uint8_t BrigLinkage8_t;

typedef uint8_t BrigExecutableModifier8_t;

typedef BrigDataOffset32_t BrigDataOffsetString32_t;

enum BrigKinds {
    BRIG_KIND_NONE = 0x0000,
    BRIG_KIND_DIRECTIVE_BEGIN = 0x1000,
    BRIG_KIND_DIRECTIVE_KERNEL = 0x1008,
};

typedef struct BrigBase BrigBase;
struct BrigBase {
    uint16_t byteCount;
    BrigKinds16_t kind;
};

typedef struct BrigExecutableModifier BrigExecutableModifier;
struct BrigExecutableModifier {
    BrigExecutableModifier8_t allBits;
};

typedef struct BrigDirectiveExecutable BrigDirectiveExecutable;
struct BrigDirectiveExecutable {
    uint16_t byteCount;
    BrigKinds16_t kind;
    BrigDataOffsetString32_t name;
    uint16_t outArgCount;
    uint16_t inArgCount;
    BrigCodeOffset32_t firstInArg;
    BrigCodeOffset32_t firstCodeBlockEntry;
    BrigCodeOffset32_t nextModuleEntry;
    uint32_t codeBlockEntryCount;
    BrigExecutableModifier modifier;
    BrigLinkage8_t linkage;
    uint16_t reserved;
};

typedef struct BrigData BrigData;
struct BrigData {
    uint32_t byteCount;
    uint8_t bytes[1];
};

static hsa_status_t find_symbol_offset(hsa_ext_brig_module_t* brig_module,
    const char* symbol_name,
    hsa_ext_brig_code_section_offset32_t* offset)
{

    /*
     * Get the data section
     */
    hsa_ext_brig_section_header_t* data_section_header =
                brig_module->section[HSA_EXT_BRIG_SECTION_DATA];
    /*
     * Get the code section
     */
    hsa_ext_brig_section_header_t* code_section_header =
             brig_module->section[HSA_EXT_BRIG_SECTION_CODE];

    /*
     * First entry into the BRIG code section
     */
    BrigCodeOffset32_t code_offset = code_section_header->header_byte_count;
    BrigBase* code_entry = (BrigBase*) ((char*)code_section_header + code_offset);
    while (code_offset != code_section_header->byte_count) {
        if (code_entry->kind == BRIG_KIND_DIRECTIVE_KERNEL) {
            /*
             * Now find the data in the data section
             */
            BrigDirectiveExecutable* directive_kernel = (BrigDirectiveExecutable*) (code_entry);
            BrigDataOffsetString32_t data_name_offset = directive_kernel->name;
            BrigData* data_entry = (BrigData*)((char*) data_section_header + data_name_offset);
            if (!strncmp(symbol_name, (char*)data_entry->bytes, sizeof(symbol_name))){
                *offset = code_offset;
                return HSA_STATUS_SUCCESS;
            }
        }
        code_offset += code_entry->byteCount;
        code_entry = (BrigBase*) ((char*)code_section_header + code_offset);
    }
    return HSA_STATUS_ERROR;
}

Program::Program(Context *context, const char *fileName)
	: _context(context), _brigModule(nullptr)
{
    /*
     * Load BRIG, encapsulated in an ELF container, into a BRIG module.
     */
	status_t brigErr = create_brig_module_from_brig_file(fileName, &_brigModule);
    CHECK_HSA(Creating the brig module, brigErr);
	
    /*
     * Create hsa program.
     */
	hsa_agent_t hsaAgent = context->getHsaAgent();
	hsa_status_t err = hsa_ext_program_create(&hsaAgent, 1, HSA_EXT_BRIG_MACHINE_LARGE, HSA_EXT_BRIG_PROFILE_FULL, &_hsaProgram);
    CHECK_HSA(Creating the hsa program, err);
	
    /*
     * Add the BRIG module to hsa program.
     */
    err = hsa_ext_add_module(_hsaProgram, _brigModule, &_moduleHandle);
    CHECK_HSA(Adding the brig module to the program, err);
}

Program::~Program()
{
	for(size_t i=0; i<kernels.size();i++)
		delete kernels[i];
	hsa_ext_program_destroy(_hsaProgram);
	destroy_brig_module(_brigModule);
}

Kernel *Program::operator[] (const char *kernelName)
{
    /* 
     * Construct finalization request list.
     */
    hsa_ext_finalization_request_t finalization_request_list;
    finalization_request_list.module = _moduleHandle;
    finalization_request_list.program_call_convention = 0;
    hsa_status_t err = find_symbol_offset(_brigModule, kernelName, &finalization_request_list.symbol);
    CHECK_HSA(Finding the symbol offset for the kernel, err);
	
    /*
     * Finalize the hsa program.
     */
    err = hsa_ext_finalize_program(_hsaProgram, _context->getHsaAgent(), 1, 
		&finalization_request_list, NULL, NULL, 0, NULL, 0);
    CHECK_HSA(Finalizing the program, err);
	
    /*
     * Get the hsa code descriptor address.
     */
    hsa_ext_code_descriptor_t *hsaCodeDescriptor;
    err = hsa_ext_query_kernel_descriptor_address(_hsaProgram, _moduleHandle, 
		finalization_request_list.symbol, &hsaCodeDescriptor);
    CHECK_HSA(Querying the kernel descriptor address, err);
	
	Kernel *kernel = new Kernel(_context, hsaCodeDescriptor);
	kernels.push_back(kernel);
	return kernel;
}
