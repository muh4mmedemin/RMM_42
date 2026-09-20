/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 10:14:36 by muayna            #+#    #+#             */
/*   Updated: 2026/09/13 10:14:36 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FUNCHTIONS_WINDOWS_H
#define FUNCHTIONS_WINDOWS_H

void get_os_name(device_info_t *source);
void get_cpu_name(device_info_t *source);
void get_pc_name(device_info_t *source);
void get_mother_board_static_value(device_info_t *source);
void get_storage_static_info(disk_list_t *source);
void get_volume_names(device_info_t *source);
void get_volume_space(device_info_t *source);
static unsigned int find_volume_index(char letter);
void get_disk_partitioned_space(device_info_t *source);
void get_cpu_arch(device_info_t *source);
void get_total_memory_mb(device_info_t *source);
void get_dns_name(device_info_t *source);
void get_time_zone(device_info_t *source);

#endif