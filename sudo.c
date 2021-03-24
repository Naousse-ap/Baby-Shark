#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <crypt.h>
#include <string.h>
#include <shadow.h>
#include <pwd.h>

//Donner les droit root a l'utilisateur
void tester_setreuid()
{ 
  uid_t uid_reel;
  uid_t uid_eff;
  uid_reel = getuid();
  uid_eff = geteuid();
  uid_eff, setreuid(uid_eff, -1); 
}

//Vérifier le password 
int password ()
{ 
  //on récupere le password depuis les variables d'environnement
  char *motDePasse = getspnam(getenv("USER"))->sp_pwdp; 
  char *mot = "";
  mot = getpass("Password: ");
  
  //on vérifie si le mdp rentré et le mdp du user sont identique
  if (strcmp(crypt(mot, motDePasse), motDePasse) == 0) { 
    return (1);
  } else {
    printf("Sorry, try again\n");
    return (0);
  }
  return (0);
}

//Vérifier si la command est éxecutable
int checkcommand(char *command)
{ 
  if (system(command) != 0 && system(command) != 512) {
    system("clear");
    printf("sudo: ");
    printf (command);
    printf(": command not found\n");
    return (1);
  } else if (system(command) == 512) {
    system("clear");
    printf (command);
    printf(": No such file or directory\n");
    return (2);
  } else {
    system("clear");
    return (3);
  }
}

//récuperer la commande dans une string
char *getcommand(char **command)
{
  int size = 0;
  char *retstr;
  
  //trouver la taille de la string finale    
  for (int i = 0; command[i] != 0; i++) {
    for (int j = 0; command[i][j] != 0; j++) {
      size++;
    }
    size++;
  }
  retstr = calloc(size - 1, sizeof(char));
  for (int i = 1; command[i] != 0; i++) {
    strcat(retstr, command[i]);
    strcat(retstr, " ");
  }
  return retstr;
}

//récuperer la command dans une string si l'option --user est appeller
char *getcommand_user(char **command)
{ 
  int size = 0; 
  char *retstr = "";
  
  for (int i = 0; command[i] != 0; i++) {
    for (int j = 0; command[i][j] != 0; j++) {
      size++;
    }
    size++;
  }
  retstr = malloc(sizeof(char) * size - 1);
  size = 0;
  
  for (int i = 3; command[i] != 0; i++) {
    for (int j = 0; command[i][j] != 0; j++) {
      retstr[size] = command[i][j];
      size++;
    }
    retstr[size] = ' ';
    size++;
  }
  retstr[size - 1] = 0;
  return (retstr);
}

//donner les droit du user demander pour l'option --user
int user_option(int ac, char **av)
{ 
  if (ac >= 3){
    if (strcmp(av[1], "--user") == 0) {
      // on crée une variable qui va contenir le nouveau user
      char *user = av[2]; 
      char cmd[1000];
      strcpy(cmd, "getent passwd ");
      strcat(cmd, user);
      strcat(cmd, " > /dev/null 2>&1");
      
      if (!system(cmd)) {
	struct passwd *pwd;
	pwd = getpwnam(user); 
	// on récupere l'uid du nouveau user 
	int uid = pwd->pw_uid; 
	uid_t uid_reel;
	uid_t uid_eff;
	uid_reel = getuid();
	uid_eff = geteuid();
	// on modifie l'uid du user courant avec le nouveau user                
	uid_eff, setreuid(uid, uid_reel); 
	return (1);
      } else {
	system("clear");
	printf("sudo: unknown user: %s \n", user);
	return (2);
      }
    } else {
      return (3);
    }
  }
  return (3);
}


int main(int argc, char *argv[])
{
  char *command = "";
  //on met la command dans une variable
  command = getcommand(argv);
  if (password() == 1){
    if (user_option(argc, argv) == 1) {
      command = getcommand_user(argv);
      //on vérifie si la command est éxecutable 
      if (checkcommand(command) == 3) {
	// si la commande est éxecutable on l'éxecute
	system(command);
	return (0); 
      }
    } else if (user_option(argc, argv) == 3) {
      tester_setreuid();
      command = getcommand(argv);
      if (checkcommand(command) == 3) {
	system(command);
      }
    }
  }
  return (0);
}
