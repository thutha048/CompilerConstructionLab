#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<memory.h>

#define MAX_LINE 50
#define MAX_WORD 50
#define MAX_LENGTH 200

typedef void *bOfData;
struct BST;
typedef struct BST  bstNode, *node;
struct BST{
    bOfData data;
    bstNode *left, *right;
};

typedef enum
{
    BST_LVR,
    BST_LRV,
    BST_RLV,
    BST_RVL,
    BST_VRL,
    BST_VLR
}BST_TRAVERSE_MODE;

void bstInit(node *root); /*define root*/
node bstCreateNode(bOfData data, int size); 
void bstInsertNode(node *root, int (*func)(bOfData, void*), void *user,int size); /*insert node*/
node bstFindNode(node* root,int (*func)(bOfData, void*), void *user);
void bstTraverse(node root, BST_TRAVERSE_MODE mode, void (*func)(bOfData,void*), void *user);

void bstInit(node *root){
    *root= NULL;
}

node bstCreateNode(bOfData data, int size){
    bstNode *n=(bstNode*)malloc(sizeof(bstNode));
    n->data= malloc(size);
    memcpy(n->data, data, size);
    n->left = n->right = NULL;
    return n;
}

void bstInsertNode(node *root,  int (*func)(bOfData, void*), void *user,int size){
    if(*root==NULL){
        *root = bstCreateNode(user,size);
        return;
    }

    int bst_cmp = func((*root)->data, user);
    if(bst_cmp <=0)
        bstInsertNode(&((*root)->left),func,user,size);
    else if(bst_cmp >0)
        bstInsertNode(&((*root)->right),func,user,size);
}

node bstFindNode(node* root,int (*func)(bOfData, void*), void *user){
    if(*root == NULL)
        return NULL;
    int bst_cmp = func((*root)->data,user);
    if(bst_cmp==0){
        return *root;
    }
    else if (bst_cmp>0) return bstFindNode(&((*root)->right),func,user);
    else if (bst_cmp<0) return bstFindNode(&((*root)->left),func,user);
}

void bstTraverse(node root, BST_TRAVERSE_MODE mode, void (*func)(bOfData,void*), void *user){
    if(root==NULL)
        return;
    
    switch(mode){
        case BST_LVR:
            if(root->left !=NULL)
                bstTraverse(root->left,mode,func,user);
            
            if(root->right!= NULL)
                bstTraverse(root->right,mode,func,user);
            break;
        
        case BST_LRV:
            if(root->left !=NULL)
                bstTraverse(root->left,mode,func,user);
            if(root->right!= NULL)
                bstTraverse(root->right,mode,func,user);
            func(root->data,user);
            break;
        
        case BST_RLV:
            if(root->right !=NULL)
                bstTraverse(root->right,mode,func,user);
            if(root->left!= NULL)
                bstTraverse(root->left,mode,func,user);
            func(root->data,user);
            break;

        case BST_RVL:
            if(root->right !=NULL)
                bstTraverse(root->right,mode,func,user);
            func(root->data,user);
            if(root->left!= NULL)
                bstTraverse(root->left,mode,func,user);
            break;

        case BST_VLR:
            func(root->data,user);
            if(root->left !=NULL)
                bstTraverse(root->left,mode,func,user);
            if(root->right!= NULL)
                bstTraverse(root->right,mode,func,user);
            break;

         case BST_VRL:
            func(root->data,user);
       
            if(root->right!= NULL)
                bstTraverse(root->right,mode,func,user);
            if(root->left !=NULL)
                bstTraverse(root->left,mode,func,user);
            break;

    }
}

typedef struct{
    char word[MAX_WORD];
    int quantity; //so luong tu
    int line[MAX_LINE]; //mang luu dong hien thi
    int pos[MAX_LENGTH];
    int lineLength;// chieu dai cua mang line

}doc;

int compareStop(bOfData x, bOfData y);
int compareDoc(bOfData x, bOfData y);
void OUTPUT(bOfData x, bOfData y);

int main(){
    int tmp=0;
    char str[2] ="\0";
    char t[MAX_LINE] ="\0";
    int line =1;
    int checkNumLine=0;
    int firstName =0;
    int newWord=0;
    node tStop;
    node tDoc;
    bstInit(&tStop);
    bstInit(&tDoc);
    char s[MAX_WORD];
    FILE *f= fopen("stopw.txt","r");

    while(fgets(s,MAX_WORD,f)){
        while(!isalpha(s[strlen(s)-1]))
            s[strlen(s)-1]='\0';
        bstInsertNode(&tStop,compareStop,s,sizeof(s));
    }

    f= fopen("vanban.txt","r");

    while(fscanf(f,"%c",&str[0])!=EOF){
        if(isalpha(str[0])){
            if('A' <= str[0]  && str[0] <= 'Z'){
                if(checkNumLine==0 && newWord ==1)
                    firstName =1;
                str[0]= tolower(str[0]);
            }
            strcpy(t,strcat(t,str));
            checkNumLine=0;
            newWord=0; 
        }else{
            newWord=1;
            checkNumLine=0;

            if(strcmp(t,"\0")!=0){
                node nodeStop = bstFindNode(&tStop,compareStop,t);
                if(nodeStop==NULL & firstName==0){
                    doc *temp= (doc*)malloc(sizeof(doc));
                    strcpy(temp->word,t);
                    node nodeDoc= bstFindNode(&tDoc,compareDoc,t);
                    if(nodeDoc!=NULL ){
                        ((doc*)nodeDoc->data)->quantity++;
                        ((doc*)nodeDoc->data)->line[((doc*)nodeDoc->data)->lineLength]=line;
                        ((doc*)nodeDoc->data)->lineLength++;
                    }else{
                        temp->quantity=1;
                        temp->line[0]=line;
                        temp->lineLength=1;
                        bstInsertNode(&tDoc,compareDoc,temp,sizeof(doc));
                    }
                }
            }
            if(str[0]=='\n'){
                line++;
                checkNumLine=1;
            }
            firstName=0;
            t[0]='\0';
        }
    }
        printf("Result\n\n");
        bstTraverse(tDoc,BST_RVL,OUTPUT,NULL);
        printf("\n");
        printf("End\n");
        return 0;
}


int compareStop(bOfData x, bOfData y){
    return strcmp((char*)x,(char*)y);
}

int compareDoc(bOfData x, bOfData y){
    doc *a =(doc *)x;
    doc *b=(doc *)y;
    return strcmp(a->word, b->word);
}


void OUTPUT(bOfData x, bOfData y){
    int i;
    doc *a =(doc*)x;
    printf("%s , %d, %d,",a->word, a->quantity, a->line[0]);
    for(i=1; i<a->lineLength;i++){
        if(a->line[i]==a->line[i-1])
            continue;
        printf(" %d,", a->line[i]);
    }
    printf("\n");
}