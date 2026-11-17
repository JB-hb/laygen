#include <stdlib.h>
#include <stdio.h>
#include <cjson/cJSON.h>

int main(){

	const cJSON *sesion_name = NULL;
	const cJSON *num_windows = NULL;
	const cJSON *window = NULL;
	const cJSON *name = NULL;
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

	//Get Data For Windows
	
	char win[8];
	for (int i = 0; i < num_windows->valueint; i++){
		int length = sprintf(win, "window%d", i+1);
		if(length < 0){
			printf("Error writing window %d in string", i+1);
			return 1;
		}

		//Get Window Name

		window = cJSON_GetObjectItemCaseSensitive(layout, win);
		name = cJSON_GetObjectItemCaseSensitive(window, "name");
		if(!cJSON_IsString(name) || name->valuestring == NULL){
			printf("Error in name in window %d", i+1);
			return 1;
		}
		printf("Window %d name %s :\n\t",i+1, name->valuestring);

		//Get Split Value

		split = cJSON_GetObjectItemCaseSensitive(window, "split");			
		if(!cJSON_IsBool(split)){
			printf("Error in split of window %d", i+1);
			return 1;
		}
		if(cJSON_IsTrue(split)){
			printf("split -> true\n\t");

			//Get Orientation Value
			orientation = cJSON_GetObjectItemCaseSensitive(window, "orientation"); 
			if(!cJSON_IsString(orientation) || orientation->valuestring == NULL){
				printf("Error in orientation of split window %d", i+1);
				return 1;
			}
			printf("orientation -> %s\n\t", orientation->valuestring);

			//Get Pane Mode
			sub_panes = cJSON_GetObjectItemCaseSensitive(window, "subPanes");
			if(!cJSON_IsNumber(sub_panes) || (sub_panes->valueint < 0 || sub_panes->valueint > 3)){
				printf("Error in subPanes value of window %d", i+1);
				return 1;
			}
			printf("subPanes -> %d\n", sub_panes->valueint);
		}else{
			printf("split -> false\n");
		}

	}

	return 0;
}
