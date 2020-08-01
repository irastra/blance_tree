#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct t_type{
        int hight;
        int blance;
        int val;
        struct t_type * lchild;
        struct t_type * rchild;
}node;
node * clue[200];
int clue_index=0;
int max(int a,int b){
        return a > b ? a : b;
}
node* get_node(){
        node* newnode = (node*)malloc(sizeof(struct t_type));
        newnode->lchild = newnode->rchild = NULL;
        newnode->val = -1;
        newnode->blance = 0;
        newnode->hight = 1;
        return newnode;
}
void show(node* root){
        printf("val : %d  hight: %d blance:%d \n",(root)->val,(root)->hight,(root)->blance);
}
void order_visit(node* root){
        if(root != NULL){
                order_visit( root->lchild);
                show(root);
                order_visit( root->rchild);
        }
}
void pre_visit(node* root){
        if(root != NULL){
                printf("val:%d\n",root->val);
                pre_visit( root->lchild);
                pre_visit( root->rchild);
        }
}
void caculate(node ** root){
        if((*root) != NULL){
                if( (*root)->lchild && (*root)->rchild ){
                        (*root)->hight = max( (*root)->lchild->hight , (*root)->rchild->hight) + 1;
                        (*root)->blance = (*root)->lchild->hight - (*root)->rchild->hight;
                }else if((*root)->lchild){
                        (*root)->hight = (*root)->lchild->hight + 1;
                        (*root)->blance = (*root)->lchild->hight;
                }else if((*root)->rchild){
                        (*root)->hight = (*root)->rchild->hight + 1;
                        (*root)->blance = - (*root)->rchild->hight;
                }else{
                        (*root)->hight = 1;
                        (*root)->blance = 0;
                }
        }
}
void repair_tree(node** root){
        if(root){
                caculate(root);
                node* c = (*root);
                if( (*root)->blance == 2 && ((*root)->lchild->blance == 1 || (*root)->lchild->blance ==0)){ //ll
                        node* cl = (*root)->lchild;
                        node* cll = (*root)->lchild->lchild;
                        node* clr = (*root)->lchild->rchild;
                        cl->rchild = (*root);
                        c->lchild = clr;
                        (*root) = cl;
                        caculate(&c);
                        caculate(&cl);
                }else if( (*root)->blance == -2 && ((*root)->rchild->blance == -1 || (*root)->rchild->blance == 0)){//rr
                        node* cr = c->rchild;
                        node* crr = cr->rchild;
                        node* crl = cr->lchild;
                        cr->lchild = c;
                        c->rchild = crl;
                        (*root) = cr;
                        caculate(&c);
                        caculate(&cr);
                }else if( (*root)->blance == 2 && (*root)->lchild->blance ==-1){//lr
                        node* cl = (*root)->lchild;
                        node* clr = cl->rchild;
                        node* cll = cl->lchild;
                        cl->rchild = clr->lchild;
                        c->lchild = clr->rchild;
                        clr->lchild = cl;
                        clr->rchild = c;
                        (*root) = clr;
                        caculate(& (clr->lchild) );
                        caculate(& (clr->rchild) );
                        caculate(& clr);
                }else if( (*root)->blance == -2 && (*root)->rchild->blance == 1){
                        node* cr = (*root)->rchild;
                        node* crl = cr->lchild;
                        node* crr = cr->rchild;
                        c->rchild = crl->lchild;
                        cr->lchild = crl->rchild;
                        crl->lchild = c;
                        crl->rchild = cr;
                        (*root) = crl;
                        caculate(& (crl->lchild) );
                        caculate(& (crl->rchild) );
                        caculate(& crl);
                }
        }
}
void build_tree(node** root,int val){
        if((*root) == NULL ){
                (*root) = get_node();
                (*root)->val = val;
        }else{
                if((*root)->val > val){
                        build_tree( (&(*root)->lchild) ,val);
                }else{
                        build_tree( (&(*root)->rchild) ,val);
                }
        }
        repair_tree(root);
}
int find_node(node * root,int val,node* temporary){
        if(root){
                if( root->val == val){//finded!
                        *temporary = *root;
                        return 0;           
                }else if( val < root->val){
                        return find_node(root->lchild,val,temporary);
                }else{
                        return find_node(root->rchild,val,temporary);
                }
        }
        return -1;
}
int delete_node(node** rootx,int val,node** boss){
        int isdelete=0;
        node* root=(*rootx);
        if(root){
                if( root->val == val){//finded!
                        node* bottom;
                        if(root->lchild != NULL){
                                bottom = root->lchild;
                                clue[ clue_index ] = root;
                                clue[ ++clue_index ] = bottom;
                                while( bottom->rchild != NULL){
                                        bottom = bottom->rchild;
                                        clue[ ++clue_index ] = bottom;
                                }
                        }else if(root->rchild != NULL){
                                bottom = root->rchild;
                                clue[ clue_index ] = root;
                                clue[ ++clue_index ] = bottom;
                                //node * clue[200];
                                while( bottom->lchild != NULL){
                                        bottom = bottom->lchild;
                                        clue[ ++clue_index ] = bottom;
                                }
                        }else{
                                bottom = root;
                        }
                        //需要修改的节点直接指针的地址
                        if((bottom == root) && (bottom == *boss)){//最终是否是单一节点
                                *boss = NULL; //根直接指针等同于当前直接指针
                                return 0;
                        }else if(bottom == root){//叶子节点
                                *rootx = NULL;
                        }else{
                                root->val = bottom->val;
                                if( clue[ clue_index -1]->rchild == bottom ){//bottom此时只有一个孩子，再看给左，还是右
                                        clue[ clue_index -1]->rchild = bottom->lchild;
                                        clue[ clue_index -1]->rchild = bottom->rchild;
                                }else{
                                        clue[ clue_index -1]->lchild = bottom->lchild;
                                        clue[ clue_index -1]->lchild = bottom->rchild;
                                }
                                free(bottom);
                                clue_index--;
                                while(clue_index>0){
                                printf("repaire:%d\n",clue[clue_index]->val);
                                if( clue[ clue_index -1]->rchild == clue[ clue_index ]){
                                                repair_tree( & (clue[clue_index-1]->rchild) );
                                        }else{
                                                repair_tree( & (clue[clue_index-1]->lchild) );
                                        }
                                        clue_index--;
                                }
                                isdelete = 1;
                        }
                }else if( val < root->val){
                        isdelete = delete_node(&(root->lchild),val,boss);
                        printf("repair:%d\n",root->val);
                        repair_tree(rootx);
                }else{
                        isdelete = delete_node(&(root->rchild),val,boss);
                        printf("repair:%d\n",root->val);
                        repair_tree(rootx);
                }
                return isdelete;
        }
       return -1;
}
void remove_node(node ** rootx,int val,node** boss){   
        clue_index=0;
        memset(clue,-1,sizeof(clue));
        delete_node(rootx,val,boss);
        if(*rootx){
                repair_tree(rootx);
        }
}
int main(){
        node * root=NULL;
        int n,state=0;
        while(10){
                printf("state:\n");
                scanf("%d",&state);
                if(state==1){
                        printf("number add:\n");
                        scanf("%d",&n);       
                        build_tree(&root,n);
                }else if(state==2){
                        printf("number del:\n");
                        scanf("%d",&n);       
                        remove_node(&root,n,&root);
                }else if(state==3){
                        printf("number fin:\n");
                        node temp;
                        scanf("%d",&n);       
                        if(find_node(root,n, &temp)==0){
                                printf("find_number:%d\n",temp.val);
                        }else{
                                printf("no memmber:%d\n",n);
                        }
                }else if(state==4){
                        order_visit(root);
                        pre_visit(root);
                }
        }
        return 0;
}
