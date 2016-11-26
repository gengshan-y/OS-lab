#include <stdio.h>
#include <stdlib.h>  
#include <string.h>
#ifndef _UNISTD_H
#define _UNISTD_H   
//#include <IO.H>   
//#include <PROCESS.H>   
#endif
#define TRUE 1
#define FALSE 0
#define INVALID -1
//#define NULL  0
#define  total_instruction  320    
#define  total_vp  32              
#define  clear_period  50          

typedef struct                     
{
    int pn,pfn,counter,time;
}pl_type;
pl_type pl[total_vp];              
struct pfc_struct{                 
    int pn,pfn;
    struct pfc_struct *next;
};

typedef struct pfc_struct pfc_type;

pfc_type pfc[total_vp],*freepf_head,*busypf_head,*busypf_tail;
int diseffect,  a[total_instruction];
int page[total_instruction],  offset[total_instruction];
int  initialize(int);
int  FIFO(int);
int  LRU(int);
int  LFU(int);
int  NUR(int);
int  OPT(int);

int main( )
{
    int s,i,j;
    srand(10*getpid());             
    s=(float)319*rand( )/32767/32767/2+1;  //
    for(i=0;i<total_instruction;i+=4)
    {
        if(s<0||s>319)
        {
            printf("When i==%d,Error,s==%d\n",i,s);
            exit(0);
        }
        a[i]=s;                           
        a[i+1]=a[i]+1;                    
        a[i+2]=(float)a[i]*rand( )/32767/32767/2;
        a[i+3]=a[i+2]+1;                  
          
        s=(float)(318-a[i+2])*rand( )/32767/32767/2+a[i+2]+2;
        if((a[i+2]>318)||(s>319))
            printf("a[%d+2],a number which is :%d and s==%d\n",i,a[i+2],s);     
    }
    for (i=0;i<total_instruction;i++)
    {
        page[i]=a[i]/10;
        offset[i]=a[i];
    }
    printf("Page\tFIFO\tLRU\tLFU\tNUR\tOPT\n");
    for(i=4;i<=32;i++)  
    {
        printf(" - \t",i);
        FIFO(i);
        LRU(i);
        LFU(i);
        NUR(i);
        OPT(i);
        printf("\n");
    }
    return 0;
}
int initialize(total_pf)             
int total_pf;                         
{int i;
diseffect=0;
for(i=0;i<total_vp;i++)
{
    pl[i].pn=i;
    pl[i].pfn=INVALID;       
    pl[i].counter=0;
    pl[i].time=-1;        
}
for(i=0;i<total_pf-1;i++)
{
    pfc[i].next=&pfc[i+1];
    pfc[i].pfn=i;
}  
pfc[total_pf-1].next=NULL;
pfc[total_pf-1].pfn=total_pf-1;
freepf_head=&pfc[0];        
return 0;
}
int FIFO(int total_pf)
{
    int i,j;
    pfc_type *p;
    initialize(total_pf);        
    busypf_head=busypf_tail=NULL;
    for(i=0;i<total_instruction;i++)
    {
        if(pl[page[i]].pfn==INVALID)  
        {
            diseffect+=1;                 
            if(freepf_head==NULL)        
            {
                p=busypf_head->next;
                pl[busypf_head->pn].pfn=INVALID;
                freepf_head=busypf_head; 
                freepf_head->next=NULL;
                busypf_head=p;
            }
            p=freepf_head->next;        
            freepf_head->next=NULL;
            freepf_head->pn=page[i];
            pl[page[i]].pfn=freepf_head->pfn;            
            if(busypf_tail==NULL)
                busypf_head=busypf_tail=freepf_head;
            else
            {
                busypf_tail->next=freepf_head; 
                busypf_tail=freepf_head;
            }
            freepf_head=p;
        }
    }
    printf("%6.4f\t",1-(float)diseffect/320);   
    return 0;
}
int LRU (total_pf)      
int total_pf;
{
    int min,minj,i,j,present_time;
    initialize(total_pf);
    present_time=0; 
    for(i=0;i<total_instruction;i++)
    {
        if(pl[page[i]].pfn==INVALID)            
        {
            diseffect++;
            if(freepf_head==NULL)             
            {
                min=32767;
                for(j=0;j<total_vp;j++)           
                    if(min>pl[j].time&&pl[j].pfn!=INVALID)
                    {
                        min=pl[j].time;
                        minj=j;
                    }
                    freepf_head=&pfc[pl[minj].pfn];   //Ì³ööª
                    pl[minj].pfn=INVALID;
                    pl[minj].time=-1;
                    freepf_head->next=NULL;
            }
            pl[page[i]].pfn=freepf_head->pfn;   //Ó¿ÕÐ³Ã,¸ÄªÓЧ
            pl[page[i]].time=present_time;
            freepf_head=freepf_head->next;      //¼õ»¸öe ҳÃ
        }
        else
            pl[page[i]].time=present_time;        //ÃÖÔÔ¼Ӹõ¥ԪµķÃʴÎý  
        present_time++;
    }
    printf("%6.4f\t",1-(float)diseffect/320);
    return 0;
}
int NUR(total_pf)                 
int  total_pf;
{ int i,j,dp,cont_flag,old_dp;
pfc_type *t;
initialize(total_pf);
dp=0;
for(i=0;i<total_instruction;i++)
{ if (pl[page[i]].pfn==INVALID)        
{diseffect++;
if(freepf_head==NULL)              
{ cont_flag=TRUE;
old_dp=dp;
while(cont_flag)
if(pl[dp].counter==0&&pl[dp].pfn!=INVALID)
cont_flag=FALSE;
else
{
    dp++;
    if(dp==total_vp)
        dp=0;
    if(dp==old_dp)
        for(j=0;j<total_vp;j++)
            pl[j].counter=0;
}
freepf_head=&pfc[pl[dp].pfn];
pl[dp].pfn=INVALID;
freepf_head->next=NULL;
}
pl[page[i]].pfn=freepf_head->pfn;
freepf_head=freepf_head->next;
}
else
pl[page[i]].counter=1;
if(i%clear_period==0)
for(j=0;j<total_vp;j++)
pl[j].counter=0;
}
printf("%6.4f\t",1-(float)diseffect/320);
return 0;
}
int OPT(total_pf)      
int total_pf;
{int i,j, max,maxpage,d,dist[total_vp];
pfc_type *t;
initialize(total_pf);
for(i=0;i<total_instruction;i++)
{ //printf("In OPT for 1,i=%d\n",i);  //i=86;i=176;206;250;220,221;192,193,194;258;274,275,276,277,278;
    if(pl[page[i]].pfn==INVALID)     
    {
        diseffect++;
        if(freepf_head==NULL)        
        {for(j=0;j<total_vp;j++)
        if(pl[j].pfn!=INVALID) dist[j]=32767; 
        else dist[j]=0;
        d=1;
        for(j=i+1;j<total_instruction;j++)
        {
            if(pl[page[j]].pfn!=INVALID)
                dist[page[j]]=d;
            d++;
        }
        max=-1;
        for(j=0;j<total_vp;j++)
            if(max<dist[j])
            {
                max=dist[j];
                maxpage=j;
            }
            freepf_head=&pfc[pl[maxpage].pfn];
            freepf_head->next=NULL;
            pl[maxpage].pfn=INVALID;
        }
        pl[page[i]].pfn=freepf_head->pfn;
        freepf_head=freepf_head->next;
    }
}
printf("%6.4f\t",1-(float)diseffect/320);
return 0;
}
int  LFU(total_pf)       
int total_pf;
{
    int i,j,min,minpage;
    pfc_type *t;
    initialize(total_pf);
    for(i=0;i<total_instruction;i++)
    {  if(pl[page[i]].pfn==INVALID)     
    { diseffect++;
    if(freepf_head==NULL)         
    { min=32767;
    for(j=0;j<total_vp;j++)
    {if(min>pl[j].counter&&pl[j].pfn!=INVALID)
    {
        min=pl[j].counter;
        minpage=j;
    }
    pl[j].counter=0;
    }
    freepf_head=&pfc[pl[minpage].pfn];
    pl[minpage].pfn=INVALID;
    freepf_head->next=NULL;
    }
    pl[page[i]].pfn=freepf_head->pfn;   //Ó¿ÕÐ³Ã,¸ÄªÓЧ
    pl[page[i]].counter++;
    freepf_head=freepf_head->next;      //¼õ»¸öe ҳÃ
    }
    else
        pl[page[i]].counter++;  
    }
    printf("%6.4f\t",1-(float)diseffect/320);   
    return 0;
}
