#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<iostream>
#include<unistd.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<wait.h>
#include<string.h>
#include<strings.h>
#include<sys/fcntl.h>
#include<dirent.h>
#include<sys/stat.h>

using namespace std;
#define MAXCOM 1000
#define MAX_BUF 200
char inputString[MAXCOM];
char *history[MAXCOM];
#define clear() printf("\033[H\033[J")
//function declaration for built in built in shell commands
int sh_cd(char *argc);
int sh_help(char *argc);
int sh_exit(char *argc);
int sh_yo(char *argc);
int sh_history(char *argc);
int sh_open(char *argc);
int sh_me(char *argc);
int sh_activetime(char *argc);
int sh_process(char *argc);
int sh_listall(char *argc);
int sh_mycd(char *argc);
int sh_write(char *argc);

int sh_write(char *argc)
{ int fd;
char sen[2048];
int i=0;
char c;
fd = open(argc, O_WRONLY|O_APPEND|O_CREAT, 0777);
if(fd<0){
perror("error");
exit(1);
}
while((c=getchar())!='`')
{

sen[i]=c;
i++;

}
write(fd,sen,strlen(sen));
close(fd);
return 1;

}
int sh_mycd(char *argv){
if(argv==NULL){
cerr<<"Argument not found"<<endl;



}
chdir(argv);
return 1;
}
int sh_listall(char *argc)
{
DIR *mydir;
struct dirent *myfile;
struct stat mystat;
char path[MAX_BUF];
getcwd(path, MAX_BUF);
char buf[512];

mydir = opendir(path);
while((myfile = readdir(mydir)) != NULL)
{
sprintf(buf, "%s/%s", path, myfile->d_name);
stat(buf, &mystat);
//printf("%zu",mystat.st_size);
printf(" %s\n", myfile->d_name);
}
closedir(mydir);
return 1;
}
int sh_process(char *argc){
system("ps -e");
return 1;
}
int sh_activetime(char *argc)
{
FILE * uptimefile;
char uptime_chr[28];
long uptime = 0;
if((uptimefile = fopen("/proc/uptime", "r")) == NULL){
perror("supt");return 1;}



fgets(uptime_chr, 12, uptimefile);
fclose(uptimefile);
uptime = strtol(uptime_chr, NULL, 10);
printf("System up for %ld seconds, %ld hours\n", uptime, uptime / 3600);
return 1;
}

int sh_me(char *argc)
{

char *username=getenv("USER");
printf("%s\n", username);
return 1;
}

int sh_help(char *argc){
if(argc==NULL){
cerr<<"Argument not provided"<<endl;
}
else{
puts("\n***WELCOME TO SHELL HELP***"

"\nList of Commands supported:"
"\n>cd"
"\n>ls"
"\n>history"
"\n>yo"
"\n>open (cat)"
"\n>write (cat > )"
"\n>me"
"\n>process"
"\n>listall"
//"\n>change"



"\n>exit");

//"\n>pipe handling"
//"\n>improper space handling"
//"\n>all other general commands available in UNIX shell");
}
return 1;

}
int sh_cd(char *argv){
if(argv==NULL){
cerr<<"Argument not found"<<endl;

}
chdir(argv);
return 1;
}
int sh_exit(char *argv){
if(argv==NULL){
cerr<<"Argument not provided"<<endl;

}
cout<<"Enjoy your day!!!!"<<endl;
return 1;

}
int sh_yo(char *argv){
if(argv==NULL){
cerr<<"Argument not provided"<<endl;

}
else{
char *username=getenv("USER");
cout<<"Hello "<<username<<"\nWelcome to linux world"<<endl;
cout<<"This is a simple implementation of linux shell!\nHopefully you find what you're looking for!"<<endl;



}
return 1;
}
int sh_history(char *argc){
HISTORY_STATE *hisd=history_get_history_state();
HIST_ENTRY **list=history_list();
int max;
if(argc==NULL){
max=hisd->length;
}
else{
max=atoi(argc);
}
for(int i=0;i<max;i++){
cout<<i+1<<":"<<list[i]->line<<endl;
}

}
int sh_open(char *argc){
int fd;
int i;
char buf[2];
fd=open(argc,O_RDONLY,0777);
if(fd<0){
perror("error");
exit(1);
}
while(read(fd,buf,1)>0){
printf("%c",buf[0]);
}
return 1;
}
void init(){
char *username=getenv("USER");
cout<<"Hello"<<username<<"\nWelcome to linux world"<<endl;


cout<<"This is a basic implementation of linux shell.Type help to see the commands that are supported!"<<endl;
sleep(1);
clear();

}

int takeinput(char *str){
char *buf;
buf=readline("\n>>");
//printf("%s",buf);
if(strlen(buf)!=0){
add_history(buf);
strcpy(str,buf);
return 0;
}
return 1;

}

void execArguments(char **parsedString){
pid_t p=fork();
//cout<<*parsedString[0]<<endl;

if(p==-1){
printf("hm\n");
return;
}
else if(p==0){
// cout<<"hi"<<endl;

if(execvp(parsedString[0],parsedString)<0);
{
printf("Cannot execute");
}



exit(0);
}
else{
wait(NULL);
return;
}
}
void execArgumentsPiped(char **parsedString,char **pipeparsed){
int pfd[2];
pid_t p1,p2;
if(pipe(pfd)<0){
cerr<<"Error"<<endl;
}
p1=fork();
if(p1<0){
printf("p1\n");
return;
}
if(p1==0){
//from stdout to pipe input
close(pfd[0]);
dup2(pfd[1],STDOUT_FILENO);
//cout<<parsedString<<endl;
close(pfd[1]);
//cout<<parsedString[0]<<endl;
if(execvp(parsedString[0],parsedString)<0){
cerr<<"Error"<<endl;
exit(0);
}

}
else{
p2=fork();
if(p2<0){
printf("Hm\n");
return;



}
if(p2==0){
close(pfd[1]);
//cout<<pipeparsed[0];
dup2(pfd[0],STDIN_FILENO);
close(pfd[0]);
if(execvp(pipeparsed[0],pipeparsed)<0){
cerr<<"Error"<<endl;
exit(0);
}
}
else{
wait(NULL);
wait(NULL);
}
}
}
int cmdhandler(char **parse){
int n,s;
n=12;
s=0;
char *listofcmds[n];
listofcmds[0]="cd";
listofcmds[1]="help";
listofcmds[2]="exit";
listofcmds[3]="yo";
listofcmds[4]="history";
listofcmds[5]="open";
listofcmds[6]="me";
listofcmds[7]="process";
listofcmds[8]="activetime";
listofcmds[9]="listall";
listofcmds[10]="change";
listofcmds[11]="write";



for(int i=0;i<n;i++){
if(strcmp(parse[0],listofcmds[i])==0)
{
s=i+1;
break;
}
}
switch (s)
{
case 1:sh_cd(parse[1]);return 1;
case 2:sh_help(parse[0]);return 1;
case 3:sh_exit(parse[0]);exit(0);
case 4:sh_yo("USER");return 1;
case 5:sh_history(parse[1]);return 1;
case 6:sh_open(parse[1]);return 1;
case 7:sh_me(parse[0]);return 1;
case 8:sh_process(parse[0]);return 1;
case 9:sh_activetime(parse[0]);return 1;
case 10:sh_listall(parse[0]);return 1;
case 11:sh_mycd(parse[1]);return 1;
case 12:sh_write(parse[1]);return 1;

default:
break;
}
return 0;

}
//find pipe
int checkpipe(char *str,char **strpipe){



for(int i=0;i<2;i++){
strpipe[i]=strtok_r(str,"|",&str);
//cout<<strpipe[i]<<endl;
}
if(strpipe[1]==NULL){
return 0;
}
//cout<<"Checkpipe:"<<strpipe[1]<<endl;
return 1;
}
//parse the cmd words
#define MAXLI 5
int parse(char *str,char **parsed){

//cout<<str<<endl;
for(int i=0;i<MAXLI;i++){
parsed[i]=strtok_r(str," ",&str);
//cout<<"IN parse"<<parsed[i]<<endl;
if(parsed[i]==NULL){
//cout<<"hey"<<endl;
break;}

if(strlen(parsed[i])==0){
i--;
}
}
return 1;

}
int processString(char* str, char** parsed, char** parsedpipe)
{

char* strpiped[2];
int piped = 0;



piped = checkpipe(str, strpiped);
//cout<<piped<<endl;
//cout<<"process string"<<strpiped[1]<<endl;

if (piped) {
parse(strpiped[0], parsed);
int x=parse(strpiped[1], parsedpipe);
// cout<<parsedpipe[0]<<" hey";
if(x){
//cout<<"Ecevuted"<<endl;
}

} else {

parse(str, parsed);
//cout<<parsed<<endl;
}

if (cmdhandler(parsed))
return 0;
else
return 1 + piped;
}
void printDir()
{
char cwd[1024];
getcwd(cwd, sizeof(cwd));
printf("\nDir: %s", cwd);
}
int main()
{
char *parsedArgs[MAXLI];
char* parsedArgsPiped[MAXLI];
int execFlag = 0;
init();



while (1) {
// print shell line
printDir();
// take input
if (takeinput(inputString))
continue;
// process
execFlag = processString(inputString,
parsedArgs, parsedArgsPiped);



if (execFlag == 1){
execArguments(parsedArgs);
//cout<<parsedArgs;
}}


return 0;
}
