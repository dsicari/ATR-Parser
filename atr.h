/*******************************************************************************
 * atr.h header file of atr.c
 * Copyright (C) 2017  Wenhe Qi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/
#ifndef _ATR_H_
#define _ATR_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>


/* Answer-to-Reset (a string of at most 32 bytes) + initial character TS*/
#define ATR_MAX_CHARS_LEN 33

#define ATR_DIRECT_CONV 0x3B
#define ATR_INVERSE_CONV 0x3F

#define ATR_T0_K_NUM_MASK 0x0F
#define ATR_TA_EXIST_MASK 0x10
#define ATR_TB_EXIST_MASK 0x20
#define ATR_TC_EXIST_MASK 0x40
#define ATR_TD_EXIST_MASK 0x80


#define BYTE_TO_BINARY_PATTERN "%2c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

typedef enum {
	ATR_BYTE_TYPE_TS,
	ATR_BYTE_TYPE_T0,
	ATR_BYTE_TYPE_TA,
	ATR_BYTE_TYPE_TB,
	ATR_BYTE_TYPE_TC,
	ATR_BYTE_TYPE_TD,
	ATR_BYTE_TYPE_T_HIS,
	ATR_BYTE_TYPE_TCK,
	ATR_BYTE_TYPE_MAX
} atr_byte_class_e_type;

typedef struct {
	atr_byte_class_e_type type;
	unsigned int i;
	int value;
} atr_byte_type;

typedef struct {
	atr_byte_type atr_str[ATR_MAX_CHARS_LEN];
	unsigned int atr_k_num;
	unsigned int atr_len;
} atr_str_type;

void get_hex_atr_string(const char *str_buffer, atr_str_type *atr_hex_str);

void decode_and_show_atr(atr_str_type *atr_hex_str);
#endif