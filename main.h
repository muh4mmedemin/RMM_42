#ifndef MAIN_H
#define MAIN_H

#include "stdlib.h"

typedef struct	hardware_static_info_s {
	char				pc_name[128]; //Done
	char				motherboard_name[128]; //Done
	char				motherboard_label[128]; //Done
	//char				ram_name[128];
	char				storage_name[128]; 
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