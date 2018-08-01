#include<stdio.h>
#include<conio.h>
#include<malloc.h>
#include<string.h>

//Binary Search Tree using Linked Lists [Dynamic Allocation]

struct node
{
    char *name;
    struct node *lchild;
    struct node *rchild;
}; //Create a pointer that points to the address of the Root node

// Find Method
void find(char *str,struct node **par,struct node **loc,struct node **root )
{
    struct node *ptr,*ptrsave=NULL;
    if(*root==NULL)
    {
        *loc=NULL;
        *par=NULL;
        return;
    }
    ptr=*root;
    while(ptr!=NULL)
    {
        if(!(strcmp(str,ptr->name)))
        {
            *loc=ptr;
            *par=ptrsave;
            return;
        }
        ptrsave=ptr;
        if(strcmp(str,ptr->name)<0)
            ptr=ptr->lchild;
        else
            ptr=ptr->rchild;
    }
    *loc=NULL;
    *par=ptrsave;
}

// Insertion Method
void insert(char *str,struct node **root)
{
    struct node *parent,*location,*temp;
    find(str,&parent,&location,root);
    if(location!=NULL)
    {
        printf("Identifier already Exists\n");
        return;
    }
    else
    {
        printf("Identifier");
    }
    //if the name is new , then a Node will be created in heap using malloc function in C and will be accessed through pointers
    temp=(struct node*)malloc(sizeof(struct node));
    temp->name=strdup(str);
    temp->lchild=NULL;
    temp->rchild=NULL;
    // if the tree is empty ( the root pointer points to null , then the structed node wil be the root
    if(*root==NULL)
    {
        *root=temp;

    }
    // if the tree is not empty, we have two cases
    // less value to the left
    //greater value to the right
    else
        //if the data to be inserted be less than the data in root ---> data will be added to the left subtree
        if(strcmp(str,parent->name)<0)
            parent->lchild=temp;
    //if the data to be inserted be greater than the data in root ---> data will be added to the right subtree
        else
            parent->rchild=temp;
}

//Displayin Method
void displayin(struct node *ptr)
{
    static int flag=1;
    if(ptr==NULL&&flag)
    {

        printf("Tree is empty");
        return;
    }
    else if(ptr == NULL) return;
    else
    {
        flag=0;
        displayin(ptr->lchild);
        printf("%s -> ",ptr->name);
        displayin(ptr->rchild);
    }
}




