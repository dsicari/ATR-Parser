/*******************************************************************************
 * main.c calculate current local time using CDMA 1X Sync message IEs
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

#include <stdio.h>
#include <stdlib.h>

#include "atr.h"

#define MAX_BUFFER_SIZE 101
/* 33 byte data * 2 + max 33 blank space + \n + \0 */

int main (int argc, char *argv[]){
	char buffer[MAX_BUFFER_SIZE];
	atr_str_type atr;
	printf("Input ATR string(white space seperated) and press ENTER:\n");
	fgets(buffer, MAX_BUFFER_SIZE - 1, stdin);
	get_hex_atr_string(buffer, &atr);
	decode_and_show_atr(&atr);
}