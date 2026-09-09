#ifndef MAIN_H
#define MAIN_H

#define MAX_DISK_COUNT 44
#include "stdlib.h"

typedef struct	disk_info_s{
	char	disk_name[128];
	char	disk_vendor[128];
	int		is_ssd;		// 1 = SSD | 0 = HDD
} disk_info_t;

typedef	struct	disk_list_s{
	disk_info_t disk_info[MAX_DISK_COUNT];
	int	disk_count; // Gerçekte kaç disk bulundu
} disk_list_t;

typedef struct	hardware_static_info_s{
	char				pc_name[128]; //Done
	char				motherboard_name[128]; //Done
	char				motherboard_label[128]; //Done
	//char				ram_name[128];
	disk_list_t			storage_info; // Done
	unsigned long long	storage_max_value_mb;
	char				cpu_label_name[128];
	char				os_version[128];
	int					is_64bit;
	unsigned long long	memory_capacity_max_mb;
	unsigned long long	storage_capacity_max_mb;
} hardware_static_info_t;

typedef struct hardware_dynamic_info_s {
	unsigned long long	memory_usage_mb;
	unsigned long long	cpu_usage_percent;
} hardware_dynamic_info_t;

typedef struct	hardware_info_s {
	hardware_dynamic_info_t	dynamic_info;
	hardware_static_info_t	static_info;
}	hardware_info_t;

typedef struct	user_static_info_s {
	char	current_user_name[64];
	int		is_admin;
} user_static_info_t;

typedef struct	user_info_s {
	user_static_info_t user_info;
} user_info_t;


typedef struct device_info_s {
	hardware_info_t	hardware_info;
	user_info_t		user_info;
} device_info_t;



#endif