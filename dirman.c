// Directory management
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h>
#define s_size 169

typedef struct Class_File {
	int folders, folder_count;
	char perm[4], name[2][s_size], folder_name[s_size];
} classfile;

int fc;
classfile* folders[1379];

void srch_file_extn(int index,char *directory_ext) {
	char *abs_path = folders[index]->folder_name;
	if(strstr(abs_path,directory_ext)!=NULL) printf("%s\n",abs_path);
}

void read_only(int index,char *folder_name) {
	if(strcmp(folders[index]->perm,"r--") == 0) {
		char *abs_path = folders[index]->folder_name;
		if(strcmp(folder_name,"")) {
			if(strstr(abs_path,folder_name) != NULL)
			 printf("%s\n",abs_path);
		}
		else printf("%s\n",abs_path);
	}
}

void list_files(char *folder_name) {
	for(int i=0; i<fc; i++) {
		char *abs_path = folders[i]->folder_name;
		if(strcmp(folder_name, "") != 0)
			if(strstr(abs_path,folder_name) == NULL) continue;
		printf("%s\n",abs_path);
		fflush(stdout);
	}
}

void piped_ls_func(char *folder_name,int fdescriptors[]) {
	for(int i=0; i<fc; i++) {
		char *abs_path = folders[i]->folder_name;
		if(strcmp(folder_name,""))
			if(strstr(abs_path,folder_name) == NULL) continue;
		char index[s_size];
		sprintf(index,"%d",i);
		write(fdescriptors[1], index, s_size);
	}
}

void add_file(char *folder_name) {
	classfile *file = (classfile *)(malloc(sizeof(classfile)));
	strcpy(file->perm, "r--");
	file->folder_count = 0;
	strcpy(file->folder_name,folder_name);
	folders[fc++] = file;
}

void mod_perm(char *folder_name, int val) {
	for(int i=0; i<fc; i++) {
		char *abs_path = folders[i]->folder_name;
		if(strstr(abs_path, folder_name) != NULL) {
			switch(val) {
				case 0 : {
					strcpy(folders[i]->perm,"---");
					break;
				}
				case 1 : {
					strcpy(folders[i]->perm,"--x");
					break;
				}
				case 2 : {
					strcpy(folders[i]->perm,"-w-");
					break;
				}
				case 3 : {
					strcpy(folders[i]->perm,"-wx");
					break;
				}
				case 4 : {
					strcpy(folders[i]->perm,"r--");
					break;
				}
				case 5 : {
					strcpy(folders[i]->perm,"r-x");
					break;
				}
				case 6 : {
					strcpy(folders[i]->perm,"rw-");
					break;
				}
				case 7 : {
					strcpy(folders[i]->perm,"rwx");
					break;
				}
				default : break;
			}
		}
	}
}

int main(void) {
	int N; scanf("%d ", &N);
	char dummy; char s[s_size];
	for(int i=0; i<N; i++) {
		classfile* file = (classfile*)malloc(sizeof(classfile));
		file->folder_count = 0;
		scanf("%s ", s);
		strcpy(file->perm, s);
		scanf("%s ", file->folder_name);
		folders[fc] = file;
		++fc;
	}
	int ops; scanf("%d ", &ops);
	for(int op=0; op<ops; op++) {
		int fdescriptors[2];
		if(pipe(fdescriptors) == -1) {
			perror("Wrror in creating pipe\n");
			exit(1);
		}
		int cnt = 0;
		char cmd[s_size];
		fgets(cmd,s_size,stdin);
		if(cmd[strlen(cmd)-1] == '\n') cmd[strlen(cmd)-1] = '\0';
		char *token = strtok(cmd," ");
		char cmd_type[s_size],folder_name[s_size],value[s_size];
		strcpy(value, "");
		strcpy(folder_name, "");
		strcpy(cmd_type, "");
		while(token != NULL) {
			if(cnt == 1) {
				if(strcmp(cmd_type,"chmod") != 0) strcpy(folder_name,token);
				else strcpy(value,token);
				cnt = 2;
			}
			else if(cnt == 0) {
				strcpy(cmd_type,token);
				cnt = 1;
			}
			else strcpy(folder_name,token);
			token = strtok(NULL," ");
		}
		int val = 0;
		if(strcmp(value, "") != 0) val = atoi(value);
		if(strcmp(cmd_type, "ls") == 0) {
			close(fdescriptors[1]);
			close(fdescriptors[0]);
			list_files(folder_name);
		}
		else if(strcmp(cmd_type,"ronly") == 0){
			if(fork() != 0) {				
				wait(NULL);
				close(fdescriptors[1]);
				char buf[s_size];
				while(read(fdescriptors[0],buf,s_size) != 0) {
					int index = atoi(buf);
					read_only(index,folder_name);
					fflush(stdout);    
				}
				close(fdescriptors[0]);
			}
			else {
				close(fdescriptors[0]);
				piped_ls_func(folder_name,fdescriptors);
				close(fdescriptors[1]);
				exit(0);
			}
		}
		else if(strcmp(cmd_type, "search") == 0) {
			if(fork() != 0) {
				wait(NULL);
				close(fdescriptors[1]);
				char buf[s_size];
				while(read(fdescriptors[0],buf,s_size) != 0) {
					int index = atoi(buf);
					srch_file_extn(index,folder_name);    
					fflush(stdout);
				}
				close(fdescriptors[0]);
			}
			else {
				close(fdescriptors[0]);
				piped_ls_func(folder_name, fdescriptors);
				close(fdescriptors[1]);
				exit(0);
			}
		}
		else if(strcmp(cmd_type,"chmod") == 0) {
			close(fdescriptors[1]);
			close(fdescriptors[0]);
			mod_perm(folder_name,val);
		}
		else {
			close(fdescriptors[1]);
			close(fdescriptors[0]);
			add_file(folder_name);
		}
	}
	return 0;
}