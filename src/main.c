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
#include "../include/struct.h"
#include "../include/functions_windows.h"
#include "unistd.h"

// To ensure correct resolution of symbols, add Psapi.lib to TARGETLIBS
// and compile with -DPSAPI_VERSION=1

void test_func(device_info_t device_info)
{
	int i;

	i = 0;
	printf("%s\n", device_info.hardware_info.static_info.pc_name);
	printf("%s\n", device_info.hardware_info.static_info.cpu_label_name);
	printf("%s\n", device_info.hardware_info.static_info.motherboard_label);
	printf("%s\n", device_info.hardware_info.static_info.motherboard_name);
	printf("%s\n", device_info.hardware_info.static_info.os_name);
	printf("TOTAL MEMORY : %llu\n", device_info.hardware_info.static_info.memory_capacity_max_mb);
	printf("OS ARCH is 64 : %d\n", device_info.hardware_info.static_info.is_64bit);
	while(i < device_info.hardware_info.static_info.storage_info.disk_count)
	{
		printf("Disk%d : %s MAX CAPACITY MB : %llu\n", i, device_info.hardware_info.static_info.storage_info.disk_info[i].disk_name, device_info.hardware_info.static_info.storage_info.disk_info[i].total_mb);
		printf("Disk%d : TOTAL_VOLUMED_CAPACITY : %llu\n", i, device_info.hardware_info.static_info.storage_info.disk_info[i].total_volume_capacity_mb);
		printf("Disk%d : USED_VOLUMED_CAPACITY : %llu\n", i, device_info.hardware_info.static_info.storage_info.disk_info[i].used_volume_capacity_mb);
		printf("Disk%d : FREE_VOLUMED_CAPACITY : %llu\n", i, device_info.hardware_info.static_info.storage_info.disk_info[i].free_volume_capacity_mb);
		printf("Disk%d %lu \n", i, device_info.hardware_info.static_info.storage_info.disk_info[i].volume_bitmap_list);
		i++;
	}
	i = 0;
	while(i < device_info.hardware_info.static_info.volume_info.volume_count)
	{
		printf("VOLUME : %s\nVOLUME TOTAL SPACE : %llu\nVOLUME USED SPACE : %llu\nVOLUME FREE SPACE : %llu\n", device_info.hardware_info.static_info.volume_info.volumes[i].volume_letter, device_info.hardware_info.static_info.volume_info.volumes[i].total_mb, device_info.hardware_info.static_info.volume_info.volumes[i].used_mb, device_info.hardware_info.static_info.volume_info.volumes[i].free_mb);
		i++;
	}
	printf("DNS IS JOINED %d\nDNS NAME : %s\n",device_info.user_info.static_info.is_domain_joined, device_info.user_info.static_info.domain_or_workgroup_name);
	printf("USER TIME ZONE : %s\n", device_info.user_info.static_info.timezone_name);
}

void get_dynamic_memory(device_info_t *source)
{
	MEMORYSTATUSEX mem_st;
	ZeroMemory(&mem_st, sizeof(mem_st));
	mem_st.dwLength = sizeof(mem_st);
	if(GlobalMemoryStatusEx(&mem_st) == FALSE)
		return ;
	(*source).hardware_info.dynamic_info.memory_usage_mb = mem_st.dwMemoryLoad;
}
#include "psapi.h"
int main( void )
{
	device_info_t device_info;
	get_pc_name(&device_info);
	get_cpu_name(&device_info);
	get_volume_names(&device_info);
	get_mother_board_static_value(&device_info);
	get_storage_static_info(&device_info.hardware_info.static_info.storage_info);
	get_volume_space(&device_info);
	get_disk_partitioned_space(&device_info);
	get_os_name(&device_info);
	get_cpu_arch(&device_info);
	get_total_memory_mb(&device_info);
	get_dns_name(&device_info);
	get_time_zone(&device_info);
	test_func(device_info);

	while(1)
	{
		get_dynamic_memory(&device_info);
		printf("MEMORY USAGE %llu%%\n", device_info.hardware_info.dynamic_info.memory_usage_mb);
		usleep(5000000);
	}
}