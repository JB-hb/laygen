#include <stdlib.h>
#include <stdio.h>
#include <cjson/cJSON.h>

int main(){

	const cJSON *sesion_name = NULL;
	const cJSON *num_windows = NULL;
	const cJSON *window = NULL;
	const cJSON *split = NULL;
	const cJSON *orientation = NULL;
	const cJSON *sub_panes = NULL;


	FILE *file = fopen("testData/testWindows.json", "rb");

	if(file == NULL){
		return 1;
	}
	
	//Get the size of the json file to store it in the buffer as a string
	fseek(file, 0, SEEK_END);
	long buff_size = ftell(file);
	rewind(file);

	printf("%d\n", buff_size);

	char *buffer = malloc(buff_size + 1);	

	if(buffer == NULL){
		perror("Error allocating memory");
	}

	//Saves String in buffer
	size_t bytes_read = fread(buffer, 1, buff_size, file);
	buffer[bytes_read] = '\0';

	fclose(file);

	printf("content:\n%s\n", buffer);

	//Parse the String to the cJSON stuct
	cJSON *layout = cJSON_Parse(buffer);

	sesion_name = cJSON_GetObjectItemCaseSensitive(layout, "Name");
	if(cJSON_IsString(sesion_name) && sesion_name->valuestring != NULL){
		printf("Sesion Name: %s\n", sesion_name->valuestring);	
	}

	num_windows = cJSON_GetObjectItemCaseSensitive(layout, "NumWindows");
	if(!cJSON_IsNumber(num_windows)){
		perror("Error reading Number Of Windows");
		return 1;
	}	
	printf("Number of Windows: %d\n", num_windows->valueint);

	return 0;
}
