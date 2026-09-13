/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 10:14:39 by muayna            #+#    #+#             */
/*   Updated: 2026/09/13 10:14:39 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void get_storage_static_info(disk_list_t *source)
{
	int							i;
	char						disk_name[64];
	char						disk_info[600];
	char						disk_storage_info[600];

	HANDLE						key;
	DWORD 						returned_bytes;
	STORAGE_PROPERTY_QUERY		disk_info_init;
	PSTORAGE_DEVICE_DESCRIPTOR	disk_info_structer;
	PGET_LENGTH_INFORMATION		disk_storage_structer;

	ZeroMemory(&disk_info_init, sizeof(disk_info_init));
	disk_info_init.PropertyId = StorageDeviceProperty;
	disk_info_init.QueryType = PropertyStandardQuery;
	i = 0;
	(*source).disk_count = i;
	while(i < MAX_DISK_COUNT)
	{
		sprintf(disk_name, "\\\\.\\PhysicalDrive%d", i);
		key = CreateFileA(disk_name, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (key == INVALID_HANDLE_VALUE)
			break;
		if (DeviceIoControl(key, IOCTL_STORAGE_QUERY_PROPERTY, &disk_info_init, sizeof(disk_info_init), disk_info, (DWORD)sizeof(disk_info), &returned_bytes, NULL) == FALSE)
			break ;
		if (DeviceIoControl(key, IOCTL_DISK_GET_LENGTH_INFO, NULL, 0, disk_storage_info, (DWORD)sizeof(disk_storage_info), NULL, NULL) == FALSE)
			break ;
		disk_storage_structer = (PGET_LENGTH_INFORMATION)disk_storage_info;
		disk_info_structer = (PSTORAGE_DEVICE_DESCRIPTOR)disk_info;
		strncpy((*source).disk_info[i].disk_name, (disk_info + disk_info_structer->ProductIdOffset), sizeof((*source).disk_info[i].disk_name) - 1);
		strncpy((*source).disk_info[i].disk_vendor, (disk_info + disk_info_structer->VendorIdOffset), sizeof((*source).disk_info[i].disk_vendor) - 1);
		(*source).disk_info[i].total_mb = (unsigned long long)(disk_storage_structer->Length.QuadPart / (unsigned long long)(1024ULL * 1024ULL));
		(*source).disk_info[i].disk_name[sizeof((*source).disk_info->disk_name) - 1] = '\0';	
		i++;
		(*source).disk_count = i;
		CloseHandle(key);
	}
}

void get_volume_names(device_info_t *source)
{
	int 	i;
	int 	pos;
	int		vol_number;
	DWORD	volumes;

	volumes = GetLogicalDrives();
	i = 1;
	vol_number = 0;
	pos = 0;
	while(pos < MAX_VOLUME_COUNT)
	{
		if (volumes & i)
		{
			sprintf((*source).hardware_info.static_info.volume_info.volumes[vol_number].volume_letter, "%c:\\", ('A' + pos));
			vol_number++;
		}
		i += i;
		pos++;
	}
	(*source).hardware_info.static_info.volume_info.volume_count = vol_number;
}

void get_volume_space(device_info_t *source)
{
	DISK_SPACE_INFORMATION volume_struct;
	HRESULT test;
	unsigned long long total_space_volume;
	unsigned long long used_space_volume;
	int i;


	i = 0;
	ZeroMemory(&volume_struct, sizeof(volume_struct));
	while(i < (*source).hardware_info.static_info.volume_info.volume_count)
	{
		if (GetDiskSpaceInformationA((*source).hardware_info.static_info.volume_info.volumes[i].volume_letter, &volume_struct) == S_OK)
		{
			(*source).hardware_info.static_info.volume_info.volumes[i].total_mb = ((unsigned long long)volume_struct.SectorsPerAllocationUnit * volume_struct.BytesPerSector * volume_struct.ActualTotalAllocationUnits)  / (1024ULL * 1024ULL);
			(*source).hardware_info.static_info.volume_info.volumes[i].used_mb = ((unsigned long long)volume_struct.SectorsPerAllocationUnit * volume_struct.BytesPerSector * volume_struct.UsedAllocationUnits)  / (1024ULL * 1024ULL);
			(*source).hardware_info.static_info.volume_info.volumes[i].free_mb = (*source).hardware_info.static_info.volume_info.volumes[i].total_mb - (*source).hardware_info.static_info.volume_info.volumes[i].used_mb;
			ZeroMemory(&volume_struct, sizeof(volume_struct));
		}
		else
			break ;
		i++;
	}
}

void test_func(device_info_t device_info)
{
	int i;

	i = 0;
	printf("%s\n", device_info.hardware_info.static_info.pc_name);
	printf("%s\n", device_info.hardware_info.static_info.motherboard_label);
	printf("%s\n", device_info.hardware_info.static_info.motherboard_name);
	while(i < device_info.hardware_info.static_info.storage_info.disk_count)
	{
		printf("Disk%d : %s MAX CAPACITY MB : %llu\n", i, device_info.hardware_info.static_info.storage_info.disk_info[i].disk_name, device_info.hardware_info.static_info.storage_info.disk_info[i].total_mb);
		i++;
	}
	i = 0;
	while(i < device_info.hardware_info.static_info.volume_info.volume_count)
	{
		printf("VOLUME : %s\nVOLUME TOTAL SPACE : %llu\nVOLUME USED SPACE : %llu\nVOLUME FREE SPACE : %llu\n", device_info.hardware_info.static_info.volume_info.volumes[i].volume_letter, device_info.hardware_info.static_info.volume_info.volumes[i].total_mb, device_info.hardware_info.static_info.volume_info.volumes[i].used_mb, device_info.hardware_info.static_info.volume_info.volumes[i].free_mb);
		i++;
	}
}

int main( void )
{
	device_info_t device_info;
	char path[36];
	get_pc_name(&device_info);
	get_mother_board_static_value(&device_info);
	get_storage_static_info(&device_info.hardware_info.static_info.storage_info);
	get_volume_names(&device_info);
	get_volume_space(&device_info);
	test_func(device_info);
	//DeviceIoControl(test, fdwCreate, )
	;
    // Get the list of process identifiers.
	//pc_hardware_info_static_t static_hardware_info;
	//static_hardware_info = sizeof(pc_hardware_info_static_t);

	//get_static_value(&static_hardware_info);
	//SYSTEM_LOGICAL_PROCESSOR_INFORMATION W;
}