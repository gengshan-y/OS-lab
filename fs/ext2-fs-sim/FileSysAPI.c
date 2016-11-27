#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Driver.h"
#include "defs.h"
#include "Queue_LinkedList.h"


#define NUM_INODE_BLOCKS 16
#define NUM_DIRECT 13
#define FILE_LIMIT 10
#define CHILD_LIMIT 10
#define FILE_NAME_LIMIT 100

typedef struct inode {
    int direct[NUM_DIRECT];
    int singl;
    int doubl;
    int tripl;
} inode_t;

typedef struct file {
    char *filename;     /* hash key */
    int size;
    int block;
    int auth;
} file;

typedef struct dir {
  char* dirName;
  file** files;
  int fileCount;
  struct dir* children;
  int childCount;
  int auth;
} dir;

dir dirSys;
queue freequeue = {NULL, NULL}; 

char* username = "root";
char* passwd = "xjtu";
int auth = 3;

int login(char* uname, char* pwd) {
  if (strcmp(uname, username)) {
    printf("username unmatched\n");
    return 0;
  }
  else if (strcmp(pwd, passwd)) {
    printf("password unmatched\n");
    return 0;
  }
  auth = 0;
  return 1;
}

void logout() {
  auth = 3;
  printf("successfully logged out\n");
}

void add2Dir(struct file* f) {
  dirSys.files[dirSys.fileCount++] = f;
  printf("added file %s, with block %d to file table\n", f->filename, f->block);
} 

void initDir(dir* dirPt, char* dirName) {
  dirPt->dirName = dirName;
  dirPt->fileCount = 0;
  dirPt->files = malloc(sizeof(file*) * FILE_LIMIT);
  dirPt->childCount = 0;
  dirPt->children = malloc(sizeof(dir) * CHILD_LIMIT);
  dirPt->auth = auth;
}

void delDir(int dirIndex) {
  int it;
  for (it = dirIndex + 1; it < dirSys.childCount; it++) {
    dirSys.children[it-1] = dirSys.children[it];
  }
  dirSys.childCount--;
}

void delfromSys(file* f) {
  int it, itt;
  for (it = 0; it < dirSys.fileCount; it++) {
    if (dirSys.files[it] == f) {
      for (itt = it+1; itt < dirSys.fileCount; itt++) {
        dirSys.files[itt-1] = dirSys.files[itt]; 
      }
      dirSys.fileCount--;
      free(f);
      break;
    }
  }
}

int dirLookUp(char* fileName) {
  int d;
  int it;
  for (it = 0; it < dirSys.childCount; it++) {
    if (!strcmp(dirSys.children[it].dirName, fileName))
      return it;
  }
  
  printf("cannot find dictionary\n");
  return -1;
}

struct file* fileLookUp(char* fileName) {
  struct file* f = NULL;
  int it;
  for (it = 0; it < dirSys.fileCount; it++) {
    if (!strcmp(dirSys.files[it]->filename, fileName)) {
      f = dirSys.files[it];
      printf("filename is %s, block is %d\n", f->filename, f->block);
      return f;
    }
  }
  printf("cannot find file\n");
  return f;
}

void del_block(int block){
    enqueue(&freequeue, block);
}

int del(int block, int layer){
    int block_buf[BYTES_PER_SECTOR];
    int i;

    if(!DevRead(block, (char *)&block_buf)){
        return 0;
    }
   
    for(i = 0; i < NUM_INODE_BLOCKS; i++){
        if(block_buf[i] != -1){
            if(layer == 1){
                del_block(block_buf[i]);
            } else{
                del(block_buf[i], layer - 1);
            }
        }
    }
    
    return 1;
}


int Delete(	char *Filename){
    struct file *f;
    inode_t inode;
    int i;

    f = fileLookUp(Filename);

    if(!f || f->auth < auth){
        return 0;
    }

    DevRead(f->block, (char *) &inode);

    for(i = 0; i < NUM_DIRECT; i++){
        if(inode.direct[i] != -1){
            del_block(inode.direct[i]);
        } else{
            break;
        }
    }

    if(inode.singl != -1){
        del(inode.singl, 1);
    }
    if(inode.doubl != -1){
        del(inode.doubl, 2);
    }
    if(inode.tripl != -1){
        del(inode.tripl, 3);
    }

    //delete hash from hashmap
    delfromSys(f);
  
    // free(f->filename);
    // free(f);
    return 1;
}

int Format (){
    struct file *f;
    struct file *tmp;
    int i;

    /* init dir system */
    initDir(&dirSys, "/");
    
    while(!empty(&freequeue)){
        dequeue(&freequeue);   
    }

    for(i = 0; i < SECTORS; i++){
        enqueue(&freequeue, i); 
    }

    DevFormat();
    return 1;
}


int write_block(char *buf, int *pos, int size, int *block){
    int free_block;
    char loc_buf[BYTES_PER_SECTOR];


    if(empty(&freequeue)){
        return 0;
    }

    free_block = front(&freequeue);

    bzero(loc_buf, BYTES_PER_SECTOR);

    if(*pos + BYTES_PER_SECTOR > size){
       memcpy(loc_buf, &buf[*pos], strlen(&buf[*pos])); 
       *pos += strlen(&buf[*pos]);
    } else{
       memcpy(loc_buf, &buf[*pos], BYTES_PER_SECTOR);
       *pos += BYTES_PER_SECTOR;
    }

    if (!DevWrite(free_block, loc_buf)){
        return 0;
    }
    dequeue(&freequeue);
    *block = free_block;
    return 1;
}

int write(int *block, char *buf, int *pos, int size, int layer){
    int i;
    int tmp = 0;

    if(layer == 0){
        if (!write_block(buf, pos, size, block)){
            return 0;
        }
    } else{     /*Need to use an indirect block */
        int ind[NUM_INODE_BLOCKS];
        for(i = 0; i < NUM_INODE_BLOCKS; i++){
            ind[i] = -1;
        }
        for(i = 0; i < NUM_INODE_BLOCKS; i++){
            if(*pos < size){
                if(!write(&ind[i], buf, pos, size, layer - 1)){
                    return 0;
                }
            } else{
                break;
            }
        }
        write_block((char *)&ind, &tmp, sizeof(ind), block); 
    }
    return 1; 
}


struct file* create_file(char *FileName, int Size){
    struct file *f = malloc(sizeof(struct file));
    inode_t inode;
    int i;
    int pos = 0;

    f->filename = FileName;
    f->size = Size;
    f->auth = auth;

    for(i = 0; i < NUM_DIRECT; i++){
        inode.direct[i] = -1;
    }
    inode.singl = -1;
    inode.doubl = -1;
    inode.tripl = -1;
    
    if(!write_block((char *) &inode, &pos, sizeof(inode), &(f->block))){
        perror("Couldn't write an inode to disk.");
    }
        
    add2Dir(f);  // add to file system
    return f;
}

int Write (	char *FileName, int Size, char *Data){
    struct file *f;
    int pos = 0;
    int i = 0;
    inode_t inode;

    f = fileLookUp(FileName);

    if(f){      /* Delete File if it exists */
        Delete(FileName);
    }

    //Create new file
    f = create_file(FileName, Size);
    DevRead(f->block, (char *)&inode);  // file inode name
    // printf("file inode name %s \n", (char *)&inode);

    for(i = 0; i < NUM_DIRECT; i++){
        if(pos < Size - 1){
            if(!write_block(Data, &pos, Size, &inode.direct[i])){
                return 0;
            }
        } else{
            break;
        }
    }

    if(pos < Size -1){
        if(!write(&inode.singl, Data, &pos, Size, 1)){
            return 0;
        }
    }
    if(pos < Size -1){
        if(!write(&inode.doubl, Data, &pos, Size, 2)){
            return 0;
        }
    }
    if(pos < Size -1){
        if(!write(&inode.tripl, Data, &pos, Size, 3)){
            return 0;
        }
    }
    DevWrite(f->block, (char *)&inode);
    return 1;
}

int read_block(int block, char *buf, int max, int *pos){
    char loc_buf[BYTES_PER_SECTOR];

    if(block != -1){
        if(!DevRead(block, loc_buf)){
            return 0;
        }
        
        if((*pos + BYTES_PER_SECTOR) <= max){
           memcpy(&buf[*pos], loc_buf, BYTES_PER_SECTOR); 
           *pos += BYTES_PER_SECTOR;
        }
        else {
           memcpy(&buf[*pos], loc_buf, max - *pos + 1);
           *pos += (max - *pos + 1);
        }
    }
    return 1;
}

int iread(int block, char *buf, int max, int *pos, int layer){
    int block_buf[BYTES_PER_SECTOR];
    int i;

    if(!DevRead(block,(char *)&block_buf)){
        return 0;
    }

    for(i = 0; i < NUM_INODE_BLOCKS; i++){
        if(block_buf[i] != -1){
            if(layer == 1){
                read_block(block_buf[i], buf, max, pos);
            } else{
                iread(block_buf[i], buf, max, pos, layer -1);
            }
        }
    }
    return 1; 
}


int Read (	char *FileName, int MaxSize, char *Data){
    struct file *f;
    inode_t inode;
    int num_read = 0;
    int i;

    f = fileLookUp(FileName);

    if (!f){
        return 0;
    }

    //get the inode data for the file and copy into the inode
    DevRead(f->block, (char *)&inode);

    //read direct blocks
    for(i = 0; i < NUM_DIRECT; i++){
        if(inode.direct[i] != -1){
            read_block(inode.direct[i], Data, MaxSize, &num_read);
        } else {
            break;
        }
    }
    
    if(inode.singl != -1){
        iread(inode.singl, Data, MaxSize, &num_read, 1);
    }
    if(inode.doubl != -1){
        iread(inode.doubl, Data, MaxSize, &num_read, 2);
    }
    if(inode.tripl != -1){
        iread(inode.tripl, Data, MaxSize, &num_read, 3);
    }
    
    Data[MaxSize] = 0;
    return num_read;
}

void List() {
  int it;
  for (it = 0; it < dirSys.fileCount; it++) {
    printf("%s ", dirSys.files[it]->filename);
  }
  for (it = 0; it < dirSys.childCount; it++) {
    printf("/%s ", dirSys.children[it].dirName);
  }
  printf("\n");
}

int Mkdir(char* dirName) {
  int d = dirLookUp(dirName);

  if (d >= 0) {
    return 0;
  }

  initDir(dirSys.children+dirSys.childCount++, dirName);

  return 1;
}

int Rmdir(char* dirName) {
  int d = dirLookUp(dirName);
  if (d == -1 || dirSys.children[d].auth < auth) {
    return 0; 
  }
  
  delDir(d);
}

void Clean() {
  int it;
  for (it = 0; it < dirSys.fileCount; it++) {
    free(dirSys.files[it]);
  }
  free(dirSys.files);
  free(dirSys.children);
}
