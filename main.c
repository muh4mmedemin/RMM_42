#include <windows.h>
#include <stdio.h>
#include <winnt.h>
#include "main.h"
#include <psapi.h>
#include <winbase.h>
#include "sysinfoapi.h"
#include "winreg.h"
#include "portabledeviceapi.h"
#include "newdev.h"
#include "fileapi.h"
#include "ioapiset.h"

// To ensure correct resolution of symbols, add Psapi.lib to TARGETLIBS
// and compile with -DPSAPI_VERSION=1

void get_pc_name(device_info_t *source)
{
	char	name[128];
	DWORD	name_size = 126;
	GetComputerNameA(name, &name_size);
	strcpy((*source).hardware_info.static_info.pc_name, name);
}

void get_mother_board_static_value(device_info_t *source)
{
	HKEY	key;
	char	label_name[128];
	char	model_name[128];
	DWORD	buffer_size = sizeof(label_name);
	RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\BIOS", 0, KEY_READ, &key);
	RegQueryValueExA(key, "BaseBoardManufacturer", NULL, NULL, (LPBYTE)label_name, &buffer_size);
	RegQueryValueExA(key, "BaseBoardProduct", NULL, NULL, (LPBYTE)model_name, &buffer_size);
	strcpy((*source).hardware_info.static_info.motherboard_label, label_name);
	strcpy((*source).hardware_info.static_info.motherboard_name, model_name);
	RegCloseKey(key);

}

int main( void )
{
	device_info_t device_info;
	HANDLE test;
	char info[200];
	STORAGE_PROPERTY_QUERY new_st;
	DWORD a = 200;
	DWORD b;
	get_pc_name(&device_info);
	get_mother_board_static_value(&device_info);
	printf("%s\n", device_info.hardware_info.static_info.pc_name);
	printf("%s\n", device_info.hardware_info.static_info.motherboard_label);
	printf("%s\n", device_info.hardware_info.static_info.motherboard_name);
	test = CreateFileA("\\\\.\\PhysicalDrive1", GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (test == INVALID_HANDLE_VALUE)
    	printf("Disk acilamadi. Hata kodu: %lu\n", GetLastError());
	ZeroMemory(&new_st, sizeof(new_st));
	new_st.PropertyId = StorageDeviceProperty;
	new_st.QueryType = PropertyStandardQuery;
	DeviceIoControl(test, IOCTL_STORAGE_QUERY_PROPERTY, &new_st, sizeof(new_st), info, a, &b, NULL);
	PSTORAGE_DEVICE_DESCRIPTOR device_desc = (PSTORAGE_DEVICE_DESCRIPTOR)info;
	printf("Marka: %s\n", info + device_desc->VendorIdOffset);
	printf("Bytes returned: %lu\n", b);
	printf("VendorIdOffset: %lu\n", device_desc->VendorIdOffset);
	printf("ProductIdOffset: %lu\n", device_desc->ProductIdOffset);
	printf("Model: %s\n", info + 57);
	printf("%s\n", info);
	//DeviceIoControl(test, fdwCreate, )
	;
    // Get the list of process identifiers.
	//pc_hardware_info_static_t static_hardware_info;
	//static_hardware_info = sizeof(pc_hardware_info_static_t);

	//get_static_value(&static_hardware_info);
	//SYSTEM_LOGICAL_PROCESSOR_INFORMATION W;
}