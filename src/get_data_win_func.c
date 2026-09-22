/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_data_windows_func.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/20 05:05:49 by muayna            #+#    #+#             */
/*   Updated: 2026/09/20 05:05:49 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <windows.h>
#include "../include/struct.h"
#include <stdio.h>

void get_os_name(device_info_t *source)
{
	OSVERSIONINFOEXA os_info_st;
	DWORD product_type;
	ZeroMemory(&os_info_st, sizeof(os_info_st));
	os_info_st.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXA);
	GetVersionExA((LPOSVERSIONINFO)&os_info_st);
	GetProductInfo(os_info_st.dwMajorVersion, os_info_st.dwMinorVersion, 0, 0, &product_type);

	if (os_info_st.dwMajorVersion == 10 && os_info_st.dwMinorVersion == 0) 
	{
		if(os_info_st.dwBuildNumber >= 22000)
			strncpy((*source).hardware_info.static_info.os_name, "Windows 11", (sizeof((*source).hardware_info.static_info.os_name) - 1));
		else
			strncpy((*source).hardware_info.static_info.os_name, "Windows 10", (sizeof((*source).hardware_info.static_info.os_name) - 1));
	}
	else if (os_info_st.dwMajorVersion == 6 && os_info_st.dwMinorVersion == 3) 
		strncpy((*source).hardware_info.static_info.os_name, "Windows 8.1", (sizeof((*source).hardware_info.static_info.os_name) - 1));
	else if (os_info_st.dwMajorVersion == 6 && os_info_st.dwMinorVersion == 2) 
		strncpy((*source).hardware_info.static_info.os_name, "Windows 8", (sizeof((*source).hardware_info.static_info.os_name) - 1));
	else if (os_info_st.dwMajorVersion == 6 && os_info_st.dwMinorVersion == 1) 
		strncpy((*source).hardware_info.static_info.os_name, "Windows 7", (sizeof((*source).hardware_info.static_info.os_name) - 1));
	 else if (os_info_st.dwMajorVersion == 6 && os_info_st.dwMinorVersion == 0) 
		strncpy((*source).hardware_info.static_info.os_name, "Windows Vista", (sizeof((*source).hardware_info.static_info.os_name) - 1));
	else if (os_info_st.dwMajorVersion == 5 && os_info_st.dwMinorVersion == 2) 
		strncpy((*source).hardware_info.static_info.os_name, "Windows XP x64 / Server 2003", (sizeof((*source).hardware_info.static_info.os_name) - 1));
	 else if (os_info_st.dwMajorVersion == 5 && os_info_st.dwMinorVersion == 1) 
		strncpy((*source).hardware_info.static_info.os_name, "Windows XP", (sizeof((*source).hardware_info.static_info.os_name) - 1));
	 else 
		strncpy((*source).hardware_info.static_info.os_name, "Bilinmeyen Windows", (sizeof((*source).hardware_info.static_info.os_name) - 1));
	(*source).hardware_info.static_info.os_name[(sizeof((*source).hardware_info.static_info.os_name) - 1)] = '\0';
}

void get_cpu_name(device_info_t *source)
{
	char	name[128];
	HKEY	key;
	DWORD	buffer_size;

	buffer_size = sizeof(name);

	RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &key);
	RegQueryValueExA(key, "ProcessorNameString", NULL, NULL, (LPBYTE)name, &buffer_size);
	strncpy((*source).hardware_info.static_info.cpu_label_name, name, ((sizeof((*source).hardware_info.static_info.cpu_label_name)) - 1));
	(*source).hardware_info.static_info.cpu_label_name[(sizeof((*source).hardware_info.static_info.cpu_label_name) - 1)] = '\0';
	RegCloseKey(key);
}

void get_pc_name(device_info_t *source)
{
	char	name[128];
	DWORD	name_size = 126;
	if(GetComputerNameA(name, &name_size) == FALSE)
		return ;
	strncpy((*source).hardware_info.static_info.pc_name, name, (sizeof((*source).hardware_info.static_info.pc_name) - 1));
	(*source).hardware_info.static_info.pc_name[(sizeof((*source).hardware_info.static_info.pc_name) - 1)] = '\0';
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
	strncpy((*source).hardware_info.static_info.motherboard_label, label_name, (sizeof((*source).hardware_info.static_info.motherboard_label) - 1));
	strncpy((*source).hardware_info.static_info.motherboard_name, model_name, (sizeof((*source).hardware_info.static_info.motherboard_name) - 1));
	(*source).hardware_info.static_info.motherboard_name[(sizeof((*source).hardware_info.static_info.motherboard_name) - 1)] = '\0';
	(*source).hardware_info.static_info.motherboard_name[(sizeof((*source).hardware_info.static_info.motherboard_label) - 1)] = '\0';
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

static unsigned int find_volume_index(char letter)
{
    return 1 << (letter - 'A');
}

void get_disk_partitioned_space(device_info_t *source)
{
	HANDLE key;
	PVOLUME_DISK_EXTENTS volume_info;
	char	info[600];
	char	path[7];
	DWORD	returned_bytes;
	int i;
	int volume_index;

	volume_index = 1;
	i = 0;
	while(i < (*source).hardware_info.static_info.volume_info.volume_count)
	{
		sprintf(path, "\\\\.\\%c:", (*source).hardware_info.static_info.volume_info.volumes[i].volume_letter[0]);
		key = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (key == INVALID_HANDLE_VALUE)
			break ;
		if (DeviceIoControl(key, IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS, NULL, 0, info, (DWORD)sizeof(info), &returned_bytes, NULL) == FALSE)
			break ;
		volume_info = (PVOLUME_DISK_EXTENTS)info;
		(*source).hardware_info.static_info.storage_info.disk_info[volume_info->Extents->DiskNumber].total_volume_capacity_mb += (*source).hardware_info.static_info.volume_info.volumes[i].total_mb;
		(*source).hardware_info.static_info.storage_info.disk_info[volume_info->Extents->DiskNumber].volume_bitmap_list |= find_volume_index((*source).hardware_info.static_info.volume_info.volumes[i].volume_letter[0]);
		(*source).hardware_info.static_info.storage_info.disk_info[volume_info->Extents->DiskNumber].used_volume_capacity_mb += (*source).hardware_info.static_info.volume_info.volumes[i].used_mb;
		(*source).hardware_info.static_info.storage_info.disk_info[volume_info->Extents->DiskNumber].free_volume_capacity_mb += (*source).hardware_info.static_info.volume_info.volumes[i].free_mb;
		volume_index += volume_index;
		i++;
		CloseHandle(key);
	}
}

void get_cpu_arch(device_info_t *source)
{
	SYSTEM_INFO  system_st;
	GetNativeSystemInfo(&system_st);
	if (system_st.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
		(*source).hardware_info.static_info.is_64bit = 1;
	else if (system_st.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_UNKNOWN)
		(*source).hardware_info.static_info.is_64bit = -1;
	else
		(*source).hardware_info.static_info.is_64bit = 0;
}
void get_total_memory_mb(device_info_t *source)
{
	MEMORYSTATUSEX memory_info; 
	memory_info.dwLength = sizeof(MEMORYSTATUSEX);
	if (GlobalMemoryStatusEx(&memory_info) == FALSE)
		return ;
	(*source).hardware_info.static_info.memory_capacity_max_mb = (memory_info.ullTotalPhys / (1024ULL * 1024ULL));
}

void get_dns_name(device_info_t *source)
{
	char info[64];
	DWORD info_size;
	info_size = sizeof(info);
	if (GetComputerNameExA(ComputerNameDnsDomain, info, &info_size) == FALSE)
		return ;
	if(strlen(info) > 0)
		(*source).user_info.static_info.is_domain_joined = 1;
	else
		(*source).user_info.static_info.is_domain_joined = 0;
	strncpy((*source).user_info.static_info.domain_or_workgroup_name, info, (sizeof((*source).user_info.static_info.domain_or_workgroup_name) - 1));
}

void get_time_zone(device_info_t *source)
{
	TIME_ZONE_INFORMATION timezone_info;
	char info[64];
	char conv[64];

	ZeroMemory(&timezone_info, sizeof(timezone_info));
	GetTimeZoneInformation(&timezone_info);
	wcstombs(info, timezone_info.StandardName, sizeof(info));
	CharToOem(info, conv);
	strncpy((*source).user_info.static_info.timezone_name, conv, (sizeof((*source).user_info.static_info.timezone_name) - 1));
}

void get_cpu_workload_percent(device_info_t *source)
{
	FILETIME idle1;
	FILETIME kernel1;
	FILETIME user1;

	FILETIME idle2;
	FILETIME kernel2;
	FILETIME user2;

	ULONGLONG idlediff;
	ULONGLONG userdif;
	ULONGLONG kerneldif;

	ULONGLONG totaldif;

	ULARGE_INTEGER u_idle1;
	ULARGE_INTEGER u_idle2;
	ULARGE_INTEGER u_kernel1;
	ULARGE_INTEGER u_kernel2;
	ULARGE_INTEGER u_user1;
	ULARGE_INTEGER u_user2;

	double cpu_workload;

	if (GetSystemTimes(&idle1, &kernel1, &user1) == FALSE)
	{
		(*source).hardware_info.dynamic_info.cpu_usage_percent = -1.00;
		return ;
	}
	Sleep(1000);
	if (GetSystemTimes(&idle2, &kernel2, &user2) == FALSE)
	{
		(*source).hardware_info.dynamic_info.cpu_usage_percent = -1.00;
		return ;
	}
	u_idle1.HighPart = idle1.dwHighDateTime;
	u_idle1.LowPart = idle1.dwLowDateTime;
	u_user1.HighPart = user1.dwHighDateTime;
	u_user1.LowPart = user1.dwLowDateTime;
	u_kernel1.HighPart = kernel1.dwHighDateTime;
	u_kernel1.LowPart = kernel1.dwLowDateTime;

	u_idle2.HighPart = idle2.dwHighDateTime;
	u_idle2.LowPart = idle2.dwLowDateTime;
	u_kernel2.HighPart = kernel2.dwHighDateTime;
	u_kernel2.LowPart = kernel2.dwLowDateTime;
	u_user2.HighPart = user2.dwHighDateTime;
	u_user2.LowPart = user2.dwLowDateTime;

	idlediff = u_idle2.QuadPart - u_idle1.QuadPart;
	userdif = u_user2.QuadPart - u_user1.QuadPart;
	kerneldif = u_kernel2.QuadPart - u_kernel1.QuadPart;

	totaldif = kerneldif + userdif;
	cpu_workload = (1.0 - ((double)idlediff/(double)totaldif)) * 100.0;
	(*source).hardware_info.dynamic_info.cpu_usage_percent = cpu_workload;
}