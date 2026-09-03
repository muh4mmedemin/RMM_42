#include <windows.h>
#include <stdio.h>
#include <winnt.h>
#include "main.h"
#include <psapi.h>
#include <winbase.h>

// To ensure correct resolution of symbols, add Psapi.lib to TARGETLIBS
// and compile with -DPSAPI_VERSION=1

void get_pc_name(device_info_t *source)
{
	char	name[128];
	DWORD	name_size = 126;
	GetComputerNameA(name, &name_size);
	strcpy((*source).hardware_info.static_info.pc_name, name);
}

int main( void )
{
	device_info_t device_info;
	get_pc_name(&device_info);
	printf("%s\n", device_info.hardware_info.static_info.pc_name);
    // Get the list of process identifiers.
	//pc_hardware_info_static_t static_hardware_info;
	//static_hardware_info = sizeof(pc_hardware_info_static_t);

	//get_static_value(&static_hardware_info);
	//SYSTEM_LOGICAL_PROCESSOR_INFORMATION W;
}