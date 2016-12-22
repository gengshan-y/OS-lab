/***
 * Filename:        FileSysAPI.h
 * Date:            Dec 21 2016
 * Last Edited by:  Gengshan Yang
 * Description:     Directory, file allocation, privilige 
 ***/

int Read(char *FileName,int MaxSize,char *Data);
int Delete(char *Filename);
int Write(char *FileName,int Size,char *Data);
int Format();
void List();
int Mkdir(char *Filename);
void Clean();
int login(char* uname, char* pwd);
void logout();
int Rmdir(char* dirName);
