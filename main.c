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

static void fill_disk_struct(disk_list_t *source)
{
	int		i;
	char	disk_name[64];
	char	disk_info[600];
	HANDLE key;
	DWORD storage_info_size;
	DWORD returned_bytes;
	STORAGE_PROPERTY_QUERY storage_init;
	PSTORAGE_DEVICE_DESCRIPTOR disk_info_structer;

	storage_info_size = sizeof(disk_info);
	ZeroMemory(&storage_init, sizeof(storage_init));
	storage_init.PropertyId = StorageDeviceProperty;
	storage_init.QueryType = PropertyStandardQuery;
	i = 0;
	(*source).disk_count = i;
	while(i < MAX_DISK_COUNT)
	{
		sprintf(disk_name, "\\\\.\\PhysicalDrive%d", i);
		key = CreateFileA(disk_name, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (key == INVALID_HANDLE_VALUE)
			break;
		if (DeviceIoControl(key, IOCTL_STORAGE_QUERY_PROPERTY, &storage_init, sizeof(storage_init), disk_info, storage_info_size, &returned_bytes, NULL) == FALSE)
			break ;
		disk_info_structer = (PSTORAGE_DEVICE_DESCRIPTOR)disk_info;
		strncpy((*source).disk_info[i].disk_name, (disk_info + disk_info_structer->ProductIdOffset), sizeof((*source).disk_info->disk_name[i]) - 1);
		(*source).disk_info->disk_name[sizeof((*source).disk_info->disk_name) - 1] = '\0';
		(*source).disk_count = i;
		CloseHandle(key);		
		i++;
	}
}

void get_storage_static_info(disk_list_t *source)
{
	int		i;
	char	disk_name[64];
	char	disk_info[600];
	HANDLE key;
	DWORD storage_info_size;
	DWORD returned_bytes;
	STORAGE_PROPERTY_QUERY storage_init;
	PSTORAGE_DEVICE_DESCRIPTOR disk_info_structer;

	storage_info_size = sizeof(disk_info);
	ZeroMemory(&storage_init, sizeof(storage_init));
	storage_init.PropertyId = StorageDeviceProperty;
	storage_init.QueryType = PropertyStandardQuery;
	i = 0;
	(*source).disk_count = i;
	while(i < MAX_DISK_COUNT)
	{
		sprintf(disk_name, "\\\\.\\PhysicalDrive%d", i);
		key = CreateFileA(disk_name, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (key == INVALID_HANDLE_VALUE)
			break;
		if (DeviceIoControl(key, IOCTL_STORAGE_QUERY_PROPERTY, &storage_init, sizeof(storage_init), disk_info, storage_info_size, &returned_bytes, NULL) == FALSE)
			break ;
		disk_info_structer = (PSTORAGE_DEVICE_DESCRIPTOR)disk_info;
		strncpy((*source).disk_info[i].disk_name, (disk_info + disk_info_structer->ProductIdOffset), sizeof((*source).disk_info[i].disk_name) - 1);
		(*source).disk_info->disk_name[sizeof((*source).disk_info->disk_name) - 1] = '\0';
		(*source).disk_count = i;
		CloseHandle(key);		
		i++;
	}
	
}

int main( void )
{
	device_info_t device_info;
	get_pc_name(&device_info);
	get_mother_board_static_value(&device_info);
	get_storage_static_info(&device_info.hardware_info.static_info.storage_info);
	printf("%s\n", device_info.hardware_info.static_info.pc_name);
	printf("%s\n", device_info.hardware_info.static_info.motherboard_label);
	printf("%s\n", device_info.hardware_info.static_info.motherboard_name);
	printf("%s\n", device_info.hardware_info.static_info.storage_info.disk_info[3].disk_name);
	//DeviceIoControl(test, fdwCreate, )
	;
    // Get the list of process identifiers.
	//pc_hardware_info_static_t static_hardware_info;
	//static_hardware_info = sizeof(pc_hardware_info_static_t);

	//get_static_value(&static_hardware_info);
	//SYSTEM_LOGICAL_PROCESSOR_INFORMATION W;
}