/*******************************************************************************
 * atr.c functions needed to parse ATR message
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

#include "atr.h"

/******************************************************************************
 * FUNCTION
 * get_hex_atr_string
 * DESCRIPTION
 * This function convert user input ATR char string into int type and save them. 
 * PARAMETERS
 * str_buffer	[IN]	ATR char string get from user input
 * atr_hex_str	[IN & OUT]	converted ATR structer
 * RETURNS
 * void
******************************************************************************/
void get_hex_atr_string(const char *str_buffer, atr_str_type *atr_hex_str){
	int i,j;
	for (i = 0, j = 0; i< strlen(str_buffer); i+=3, j++){
		if (str_buffer[i] == '\n' || str_buffer[i] == '\0'){
			break;
		}else{
			atr_hex_str->atr_str[j].value =(int) strtol(&str_buffer[i], NULL, 16);
		}
	}
	atr_hex_str->atr_len = j;
}

/******************************************************************************
 * FUNCTION
 * decode_and_show_atr
 * DESCRIPTION
 * This function decode ATR and show parsing result in screen  
 * PARAMETERS
 * atr_hex_str	[IN]	ATR hex string
 * RETURNS
 * void
******************************************************************************/
void decode_and_show_atr (atr_str_type *atr_hex_str){
	int i,j,k;
	/* eXpected TCK value */
	int X_TCK;
	/* eXpected historical bytes number K */
	int X_K;
	
	/* show ATR string */
	printf("ATR: ");
	for(i = 0; i < atr_hex_str->atr_len; i++) 
		printf("%X ", atr_hex_str->atr_str[i].value); 
	printf("\n");
	
	/* decode and show TS */
	if (atr_hex_str->atr_str[0].value == ATR_DIRECT_CONV){
		printf("0x%X -TS- Direct Convention\n", atr_hex_str->atr_str[0].value);
	}else if(atr_hex_str->atr_str[0].value == ATR_INVERSE_CONV)
	{
		printf("0x%X -TS- Inverse Convention\n", atr_hex_str->atr_str[0].value);
	}else{
		printf("0x%X -TS- Invalid TS byte\n", atr_hex_str->atr_str[0].value);
		return;
	}

	for(i = 1, j = 0; i < atr_hex_str->atr_len;){
		/* decode and show T0/TD[x] */
		if (i == 1){
			printf("0x%X -T0- Format Byte\n", atr_hex_str->atr_str[i].value);
			atr_hex_str->atr_str[i].type = ATR_BYTE_TYPE_T0;
			atr_hex_str->atr_k_num = atr_hex_str->atr_str[i].value & ATR_T0_K_NUM_MASK;
			/* ISO-IEC 7816-3 8.2.5 Check byte TCK
			 * If T=0 and T=15 are present and in all the other cases, TCK shall be present. 
			 * When TCK is present, exclusive-oring all the bytes T0 to TCK inclusive shall give '00'.
			 * Any other value is invalid. 
			 */
			X_TCK = atr_hex_str->atr_str[i].value;
		}
		/* decode Yj */
		if(atr_hex_str->atr_str[i].value & ATR_TD_EXIST_MASK){
			printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(ATR_TD_EXIST_MASK));
			printf("    TD[%d] is present\n", j+1);
		}
		if(atr_hex_str->atr_str[i].value & ATR_TC_EXIST_MASK){
			printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(ATR_TC_EXIST_MASK));
			printf("    TC[%d] is present\n", j+1);
		}
		if(atr_hex_str->atr_str[i].value & ATR_TB_EXIST_MASK){
			printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(ATR_TB_EXIST_MASK));
			printf("    TB[%d] is present\n", j+1);
		}
		if(atr_hex_str->atr_str[i].value & ATR_TA_EXIST_MASK){
			printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(ATR_TA_EXIST_MASK));
			printf("    TA[%d] is present\n", j+1);
		}
		
		/* decode K in T0 */
		if(i == 1){
			atr_hex_str->atr_k_num = atr_hex_str->atr_str[i].value & 0x0F;
			printf(" K is %d\n", atr_hex_str->atr_k_num);
		}

		/* temporarily store index value i of T0 and TD[x] in k */
		k = i;

		if(atr_hex_str->atr_str[k].value & ATR_TA_EXIST_MASK){
			printf("0x%X -TA[%d]- Interface Byte\n", atr_hex_str->atr_str[++i].value, j+1);
			X_TCK ^= atr_hex_str->atr_str[i].value;
			/* decode and show TA */
		}
		if(atr_hex_str->atr_str[k].value & ATR_TB_EXIST_MASK){
			printf("0x%X -TB[%d]- Interface Byte\n", atr_hex_str->atr_str[++i].value, j+1);
			X_TCK ^= atr_hex_str->atr_str[i].value;
			/* decode and show TB */
		}
		if(atr_hex_str->atr_str[k].value & ATR_TC_EXIST_MASK){
			printf("0x%X -TC[%d]- Interface Byte\n", atr_hex_str->atr_str[++i].value, j+1);
			X_TCK ^= atr_hex_str->atr_str[i].value;
			/* decode and show TC */
		}
		if(atr_hex_str->atr_str[k].value & ATR_TD_EXIST_MASK){
			printf("0x%X -TD[%d]- Interface Byte\n", atr_hex_str->atr_str[++i].value, j+1);
			X_TCK ^= atr_hex_str->atr_str[i].value;
			j++;
			continue;
		}else{
			break;
		}
		
	}

	/* move index to the first historical byte */
	i++;

	if(atr_hex_str->atr_k_num != (atr_hex_str->atr_len - i - 1)){
		printf("Rest ATR string character number does not match K value in T0!\n");
		return;
	}

	/* decode and show Ki */
	for (j=1; i < atr_hex_str->atr_len - 1; i++, j++) {
		printf("0x%X -K[%d]- Historical Byte\n", atr_hex_str->atr_str[i].value, j);
		X_TCK ^= atr_hex_str->atr_str[i].value;
	}
	
	if (X_TCK != atr_hex_str->atr_str[i].value){
		printf("0x%X -TCK- Check Byte - Wrong Checksum expected 0x%X\n", atr_hex_str->atr_str[i].value, X_TCK);
	}else {
		printf("0x%X -TCK- Check Byte - Correct Checksum\n", atr_hex_str->atr_str[i].value);
	}
	

}