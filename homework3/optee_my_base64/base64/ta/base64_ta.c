/*
 * Copyright (c) 2016, Linaro Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>
#include <base64_ta.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};

/*
 * Called when the instance of the TA is created. This is the first call in
 * the TA.
 */
TEE_Result TA_CreateEntryPoint(void)
{
	DMSG("has been called");

	return TEE_SUCCESS;
}

/*
 * Called when the instance of the TA is destroyed if the TA has not
 * crashed or panicked. This is the last call in the TA.
 */
void TA_DestroyEntryPoint(void)
{
	DMSG("has been called");
}

/*
 * Called when a new session is opened to the TA. *sess_ctx can be updated
 * with a value to be able to identify this session in subsequent calls to the
 * TA. In this function you will normally do the global initialization for the
 * TA.
 */
TEE_Result TA_OpenSessionEntryPoint(uint32_t param_types,
		TEE_Param __maybe_unused params[4],
		void __maybe_unused **sess_ctx)
{
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);

	DMSG("has been called");

	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	/* Unused parameters */
	(void)&params;
	(void)&sess_ctx;

	/*
	 * The DMSG() macro is non-standard, TEE Internal API doesn't
	 * specify any means to logging from a TA.
	 */
	IMSG("Hello World!\n");

	/* If return value != TEE_SUCCESS the session will not be created. */
	return TEE_SUCCESS;
}

/*
 * Called when a session is closed, sess_ctx hold the value that was
 * assigned by TA_OpenSessionEntryPoint().
 */
void TA_CloseSessionEntryPoint(void __maybe_unused *sess_ctx)
{
	(void)&sess_ctx; /* Unused parameter */
	IMSG("Goodbye!\n");
}


static TEE_Result encode(uint32_t param_types,
	TEE_Param params[4])
{
	uint32_t exp_param_types =
				TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_OUTPUT,
						TEE_PARAM_TYPE_NONE,
						TEE_PARAM_TYPE_NONE,
						TEE_PARAM_TYPE_NONE);
						
	IMSG("called!!!! \n");
	DMSG("has been called array!!!!");
	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	void *buf = NULL;
	buf = TEE_Malloc(params[0].memref.size, 0);
	TEE_MemMove(buf , params[0].memref.buffer, params[0].memref.size);
	
	char *cool =(char*) buf; 
	IMSG("name : %s\n",cool);
	IMSG("name : %d %d %d %d %d %d %d %d %d %d\n", cool[0] , cool[1] ,cool[2],cool[3],cool[4],cool[5],cool[6],cool[7],cool[8],cool[9]);

	//"陳俊昇";
	size_t len = strlen(cool);
	int num = len*8;
	char *binary = malloc( len*8+1);
	char *c = binary;
	//printf("%s\n",cool);
	for (size_t i = 0 ; i < len ; i++){
	//printf("%d\n",cool[i]);
	char ch = cool[i];
		for(int j = 7 ; 0 <= j ; --j){
		     if(ch & (1<<j)){
			*c='1';
		    } else{
			*c='0';
		    }
		    c++;
		    num--;
		}
	}
	*c='\0';
	size_t len_binary = strlen(binary);
	int times = len_binary/6;
	char *answer = malloc(times+1);
	char *d = answer;
	for (int i = 0 ; i < times ; i++){
		char *tmp2 = malloc(7);
		c=tmp2;
		//抓出前六個bits
		for (int j = 0+6*i ; j < 6+6*i  ; j++){
		    if(binary[j]=='1'){
			*c='1';
		    } else{
			*c='0';
		    }
		    c++;
		}
		//直接計算前面6bits的十位數，再去對照encode_table。
		//printf("%s\n",tmp2);
		size_t len_sixBits = strlen(tmp2);//取得字串的長度
		int countTen = 0 ;
		for(int i = 0 ; i<len ; i++){
		    if(tmp2[i]=='1'){
		    	int tmp=1;
		    	for(int j = 0 ; j < 5 - i ; j++){
		    		tmp*=2;
		    	}
			countTen += tmp;
		    }
		}
		//printf("%d\n",countTen);
		//printf("%c\n",encoding_table[countTen]);

		*d=encoding_table[countTen];
		d++;
	}
	*d='\0';
	//printf("%s\n",binary);
	//printf("%d\n",len_binary);
	//printf("%d\n",times);
	IMSG("%s\n",answer);

	
	void *ans = answer;
	TEE_MemMove(params[0].memref.buffer  , ans, 13);
	
	//IMSG("Generating random data over %u bytes.", params[0].memref.size);
	
	//buf = params[0].memref.buffer;	

	return TEE_SUCCESS;
}

static TEE_Result decode(uint32_t param_types,
	TEE_Param params[4])
{
	uint32_t exp_param_types =
				TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_OUTPUT,
						TEE_PARAM_TYPE_NONE,
						TEE_PARAM_TYPE_NONE,
						TEE_PARAM_TYPE_NONE);
						
	IMSG("called!!!! \n");

	
	DMSG("has been called array!!!!");
	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;
	
	void *buf = NULL;
	buf = TEE_Malloc(params[0].memref.size, 0);
	TEE_MemMove(buf , params[0].memref.buffer, params[0].memref.size);
	
	char *cool =(char*) buf; 
	IMSG("name : %s\n",cool);
	IMSG("name : %d %d %d %d %d %d %d %d %d %d\n", cool[0] , cool[1] ,cool[2],cool[3],cool[4],cool[5],cool[6],cool[7],cool[8],cool[9]);	

	//char *cool ="6Km55YGJ57+U";
	size_t len = strlen(cool);
	char *pointer_cool = cool;
	//printf("%d\n",len);
	//把cool轉成十進位
	char *Ten = malloc(sizeof(char)*12+1);
	char *pointer_Ten = Ten;
	for(int i = 0 ; i < len ;i++){
	for(int j = 0 ; j < 64 ; j++){
	    if(*pointer_cool==encoding_table[j]){
		*pointer_Ten=j;
		//printf("%d ",*pointer_Ten);
		break;
	    }
	}
	pointer_cool++;
	pointer_Ten++;
	}
	*pointer_Ten='\0';
	//printf("\n");
	// 把十進位轉成二進位
	char *binary = malloc(sizeof(char)*len*8+1);
	char *c = binary;
	for(int i = 0 ; i < len ;i++){
	char ch = Ten[i];
	for(int j = 7 ; 0 <= j ; --j){
	     if(ch & (1<<j)){
		*c='1';
	    } else{
		*c='0';
	    }
	    c++;
	}
	}
	*c='\0';
	//printf("%s\n",binary);

	//把二進位的96碼轉成二進位的72碼(中文的 9bytes)
	char* binary_9byte = malloc(sizeof(char)*9*8+1);
	char *pointer_9bytes = binary_9byte ;
	c = binary;
	for(int i = 0 ; i < 96; i++){
	if(i%8!=0 && i%8!=1){
	    *pointer_9bytes=*c;
	    //printf("%d ",i);
	    pointer_9bytes++;
	}
	c++;
	}
	*pointer_9bytes='\0';
	//printf("\n");
	//printf("%s\n",binary_9byte);

	//把二進位的72碼轉成十進位(9bytes)

	char *answer=malloc(sizeof(char)*9+1);
	char *ans=answer;
	int countTen ;
	for (int i = 0 ; i < 9 ; i++){
	countTen = 0 ;
	for(int k = 0 ; k<8 ; k++){
	    if(binary_9byte[k+8*i]=='1'){
		int tmp =1 ;
		for(int j = 0 ; j < 7-k ; j++){
		    tmp*=2;
		}
		countTen += tmp;
	    }
	}
	//printf ("%d ",countTen);
	*ans=countTen;
	ans++;
	}
	//printf ("%s ",answer);
	
	void *s = answer;
	TEE_MemMove(params[0].memref.buffer  , s, 10);
		
	return TEE_SUCCESS;
}

/*
 * Called when a TA is invoked. sess_ctx hold that value that was
 * assigned by TA_OpenSessionEntryPoint(). The rest of the paramters
 * comes from normal world.
 */
TEE_Result TA_InvokeCommandEntryPoint(void __maybe_unused *sess_ctx,
			uint32_t cmd_id,
			uint32_t param_types, TEE_Param params[4])
{
	(void)&sess_ctx; /* Unused parameter */

	switch (cmd_id) {
	case TA_BASE64_ENCODE:
		return encode(param_types,params);	
	case TA_BASE64_DECODE:
		return decode(param_types,params);
	default:
		return TEE_ERROR_BAD_PARAMETERS;
	}
}
