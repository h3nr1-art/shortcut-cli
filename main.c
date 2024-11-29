#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>

void help_sh(void)
{
  fprintf(stderr,"\nsh create [name_shortcut]\nsh show\nsh remove [name_shortcut]\n\n    -h --help : show this help");
}

void handle_error(const char* error_func)
{
  perror(error_func);
  exit(1);
}



int show_sh(char* sh_dir_path)
{
  int return_value;
  DIR* dir;
  struct dirent* dp;
  char buffer[256];
	char sh_path[128];
	size_t sym_path_size;

  return_value = 0;
  if ((dir = opendir(sh_dir_path)) != NULL)
  {
    while ((dp = readdir(dir)) != NULL)
    {
      if (!((strcmp(dp -> d_name, ".") == 0) 
        || (strcmp(dp -> d_name, "..") == 0)))
      {
				strcpy(sh_path, sh_dir_path);
				strcat(sh_path, dp -> d_name);

				sym_path_size = readlink(sh_path, buffer, sizeof(char) * 256);
        if (sym_path_size != -1)
        {
					buffer[sym_path_size] = 0;
        	printf("%s -> %s\n", dp -> d_name, buffer);
					buffer[0] = '\0';
        }
        else
        {
          return_value = -1;
        }
      }
    }
  }
  else 
  {
    closedir(dir);
    return_value = -1;
  }

  closedir(dir);
  return return_value;
}

int create_sh(char* shortcut_name, char* sh_dir_path)
{
  int sym;
  char* sym_path; 
  int return_value;
  char buffer[512];
  char* current_dir;
  size_t size_of_sym_path;


  current_dir = NULL;
  if (getcwd(buffer, sizeof(char) * 512) == NULL)
  {
    handle_error("current_dir");
    exit(1);
    
  }
  current_dir = malloc(sizeof(char) * (strlen(buffer) + 1));
  strcpy(current_dir, buffer);
  return_value = 0;

  size_of_sym_path = sizeof(char) * (strlen(sh_dir_path) + strlen(shortcut_name) + 1);

  sym_path = NULL;
  sym_path = malloc(size_of_sym_path);
  
  if (sym_path != NULL)
  {
    sprintf(sym_path,"%s%s",sh_dir_path, shortcut_name);
    sym = symlink(current_dir, sym_path);
    if (sym == -1)
    {
      handle_error("sym creation");
      return_value = -1;
    }
    else 
    {
      printf("shortcut created");
    }
  }
  else 
  {
    handle_error("symlink alloc");
    return_value = -1;
  }
	free(sym_path);
  return return_value; 
}

int remove_sh(char* name_sh, char* sh_dir_path)
{
  int remove_state;
  char* sh_path;
  size_t sh_path_size;

  sh_path_size = sizeof(char) * (strlen(name_sh) + strlen(sh_dir_path) + 1);
  sh_path = malloc(sh_path_size);
  if (sh_path != NULL)
  {

    strcpy(sh_path,sh_dir_path);
    strcat(sh_path,name_sh);
    remove_state = remove(sh_path);
		printf("Shortcut successfuly removed");
  }
  else 
  {
    remove_state = -1;
  }
	free(sh_path);
  return remove_state;
}




int main(int argc, char *argv[])
{
  char* sh_dir;
  int return_value;
  int nhelp;
 

  nhelp = 0;
  sh_dir = NULL;
  sh_dir =  malloc(sizeof(char) * (strlen(getenv("HOME")) + strlen("/.cache/shortcut/") + 1));
	return_value = 0;
  if (sh_dir == NULL)
  {
    handle_error("shortcut dir");
		free(sh_dir);
    exit(1);
  }

	if (argc < 2)
	{
		printf("Not enough argument");
		free(sh_dir);
		exit(1);
	}
  sprintf(sh_dir, "%s%s", getenv("HOME"), "/.cache/shortcut/\0");

  if (strcmp(argv[1],"create") == 0)
  {
    if (argc == 3)
    {
      if (create_sh(argv[2], sh_dir) == -1)
      {
				free(sh_dir);
        handle_error(NULL);
      }
    }
    else 
    {
      nhelp = 1;
    }
	}
	
  else if (strcmp(argv[1], "show") == 0)
  {
    if (argc == 2)
    {
      if (show_sh(sh_dir) == -1)
      {
				free(sh_dir);
        handle_error("show");
        return_value = -1;
      }
    }
    else 
    {
			free(sh_dir);
      handle_error(NULL);
      nhelp = 1;
      return_value = -1;
    }
  }
  else if (strcmp(argv[1], "remove") == 0)
  {
    if (argc == 3)
    {
      if (remove_sh(argv[2], sh_dir) == -1)
      {
				free(sh_dir);
        handle_error(NULL);
        return_value = -1;
      }
    }
  }
	else if (strcmp(argv[1], "help") == 0)
	{
		help_sh();
	}
  
  
 
   free(sh_dir);

  return return_value;
}
