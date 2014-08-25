#include <hsa/Exception.h>
#include <sstream>

using namespace hsa;
using namespace std;

Exception::Exception(const hsa_status_t &status, const char *_file_, int _line_, const char* message)
{
	stringstream ss;
	switch (status)
	{
		case HSA_STATUS_INFO_BREAK:
			status_str = "HSA_STATUS_INFO_BREAK";
			break;
		case HSA_EXT_STATUS_INFO_ALREADY_INITIALIZED:
			status_str = "HSA_EXT_STATUS_INFO_ALREADY_INITIALIZED";
			break;
		case HSA_EXT_STATUS_INFO_UNRECOGNIZED_OPTIONS:
			status_str = "HSA_EXT_STATUS_INFO_UNRECOGNIZED_OPTIONS";
			break;
		case HSA_STATUS_ERROR:
			status_str = "HSA_STATUS_ERROR";
			break;
		case HSA_STATUS_ERROR_INVALID_ARGUMENT:
			status_str = "HSA_STATUS_ERROR_INVALID_ARGUMENT";
			break;
		case HSA_STATUS_ERROR_INVALID_QUEUE_CREATION:
			status_str = "HSA_STATUS_ERROR_INVALID_QUEUE_CREATION";
			break;
		case HSA_STATUS_ERROR_INVALID_ALLOCATION:
			status_str = "HSA_STATUS_ERROR_INVALID_ALLOCATION";
			break;
		case HSA_STATUS_ERROR_INVALID_AGENT:
			status_str = "HSA_STATUS_ERROR_INVALID_AGENT";
			break;
		case HSA_STATUS_ERROR_INVALID_REGION:
			status_str = "HSA_STATUS_ERROR_INVALID_REGION";
			break;
		case HSA_STATUS_ERROR_INVALID_SIGNAL:
			status_str = "HSA_STATUS_ERROR_INVALID_SIGNAL";
			break;
		case HSA_STATUS_ERROR_INVALID_QUEUE:
			status_str = "HSA_STATUS_ERROR_INVALID_QUEUE";
			break;
		case HSA_STATUS_ERROR_OUT_OF_RESOURCES:
			status_str = "HSA_STATUS_ERROR_OUT_OF_RESOURCES";
			break;
		case HSA_STATUS_ERROR_INVALID_PACKET_FORMAT:
			status_str = "HSA_STATUS_ERROR_INVALID_PACKET_FORMAT";
			break;
		case HSA_STATUS_ERROR_RESOURCE_FREE:
			status_str = "HSA_STATUS_ERROR_RESOURCE_FREE";
			break;
		case HSA_STATUS_ERROR_NOT_INITIALIZED:
			status_str = "HSA_STATUS_ERROR_NOT_INITIALIZED";
			break;
		case HSA_STATUS_ERROR_REFCOUNT_OVERFLOW:
			status_str = "HSA_STATUS_ERROR_REFCOUNT_OVERFLOW";
			break;
		case HSA_EXT_STATUS_ERROR_DIRECTIVE_MISMATCH:
			status_str = "HSA_EXT_STATUS_ERROR_DIRECTIVE_MISMATCH";
			break;
		case HSA_EXT_STATUS_ERROR_IMAGE_FORMAT_UNSUPPORTED:
			status_str = "HSA_EXT_STATUS_ERROR_IMAGE_FORMAT_UNSUPPORTED";
			break;
		case HSA_EXT_STATUS_ERROR_IMAGE_SIZE_UNSUPPORTED:
			status_str = "HSA_EXT_STATUS_ERROR_IMAGE_SIZE_UNSUPPORTED";
			break;
		default: status_str = "HSA_UNKNOWN_ERROR";
	}
	ss << "HSA error " << status_str << " [" << _file_ << "]\nfile: " << _file_ << ", line: " << _line_;
	if(message!=nullptr)
		ss << ", message: " << message;
	status_str = ss.str().c_str();
}

Exception::Exception(const status_t &status, const char *_file_, int _line_, const char* message)
{
	stringstream ss;
	switch (status)
	{
		case STATUS_KERNEL_INVALID_SECTION_HEADER:
			status_str = "STATUS_KERNEL_INVALID_SECTION_HEADER";
			break;
		case STATUS_KERNEL_ELF_INITIALIZATION_FAILED:
			status_str = "STATUS_KERNEL_ELF_INITIALIZATION_FAILED";
			break;
		case STATUS_KERNEL_INVALID_ELF_CONTAINER:
			status_str = "STATUS_KERNEL_INVALID_ELF_CONTAINER";
			break;
		case STATUS_KERNEL_MISSING_DATA_SECTION:
			status_str = "STATUS_KERNEL_MISSING_DATA_SECTION";
			break;
		case STATUS_KERNEL_MISSING_CODE_SECTION:
			status_str = "STATUS_KERNEL_MISSING_CODE_SECTION";
			break;
		case STATUS_KERNEL_MISSING_OPERAND_SECTION:
			status_str = "STATUS_KERNEL_MISSING_OPERAND_SECTION";
			break;
		case STATUS_UNKNOWN:
			status_str = "STATUS_UNKNOWN";
			break;
		default: status_str = "HSA_UNKNOWN_ERROR";
	}
	ss << "HSA error " << status_str << " [" << _file_ << "]\nfile: " << _file_ << ", line: " << _line_;
	if(message!=nullptr)
		ss << ", message: " << message;
	status_str = ss.str().c_str();
}


void Exception::Check(const char* message, const hsa_status_t &status, const char *_file_, int _line_)
{
	if (HSA_STATUS_SUCCESS != status)
		throw Exception(status, _file_, _line_, message);
}

void Exception::Check(const char* message, const status_t &status, const char *_file_, int _line_)
{
	if (STATUS_SUCCESS != status)
		throw Exception(status, _file_, _line_, message);
}
