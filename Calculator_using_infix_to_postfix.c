#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <conio.h>

struct node{
    char data;
    double num;
    struct node* link;
} *stack=NULL;

struct hell{
    char data;
    struct hell* link;
};

int isempty(){
    if (stack==NULL) return 1;
    else return 0;
}

void push(char data){
    struct node* p=(struct node*) malloc(sizeof(struct node));
    if (p==NULL){
        printf("Stack overflow\n");
        return;
    }
    else{
        p->data=data;
        p->link=stack;
        stack=p;
    }
}

char pop(){
    if (isempty()){
        printf("Stack underflow\n");
    }
    else{
        char data=stack->data;
        struct node* p=stack;
        stack=stack->link;
        free(p);
        return data;
    }
}

char peek(){
    if (isempty()){
        printf("Stack underflow\n");
    }
    else{
        return stack->data;
    }
}

void push_num(double data){
    struct node* p=(struct node*) malloc(sizeof(struct node));
    if (p==NULL){
        printf("Stack overflow\n");
        return;
    }
    else{
        p->num=data;
        p->link=stack;
        stack=p;
    }
}

double pop_num(){
    if (isempty()){
        printf("Stack underflow\n");
    }
    else{
        struct node* p=stack;
        double d=p->num;
        stack=stack->link;
        free(p);
        return d;
    }
}

void add(struct hell** tra, char data){
    struct hell* f=(struct hell*) malloc(sizeof(struct hell));
    f->data=data;
    if ((*tra)->link==NULL){
        (*tra)->link=f;
        f->link=NULL;
    }
    else{
        f->link=(*tra)->link;
        (*tra)->link=f;
    }
}

int pre(char data){
    switch (data){
        case '+':
        case '-':
            return 1;
        case '*':
        case 'x':
            return 2;
        case '/':
            return 3;
        case '^':
            return 4;
        default:
            return 0;
    }
}

double solve(double a, double b, char c){
    switch(c){
        case '+':
           return b+a;
        case '-':
           return b-a;
        case '*':
        case 'x':
           return b*a;
        case '/':
           return b/a;
        case '^':
           return pow(b,a);
    }
}

int main(){
    char a;
    printf("Enter expression:");
    int u=0,count=1;

    struct hell* list=NULL;
    
    a=' ';
    while (a!='\n'){
        a=getchar();
        if (!isspace(a)){
            struct hell* p=(struct hell *) malloc(sizeof(struct hell));
            p->data=a;
            p->link=NULL;
            if (list==NULL){
                list=p;
            }
            else{
                struct hell* p2=list;
                while (p2->link!=NULL){
                    p2=p2->link;
                }
                p2->link=p;
            }
            count++;
        }
    }

    struct hell* tra=list;
    while (tra!=NULL){
        if (tra->data=='-'){
            struct hell* n=(struct hell*) malloc(sizeof(struct hell));
            n->data='(';
            n->link=tra;
            list=n;
            add(&n,'0');
            while (tra->link!=NULL && isdigit(tra->link->data)){
                tra=tra->link;
            }
            add(&tra,')');
        }
        else if (pre(tra->data) && tra->link->data=='-'){
            add(&tra,'(');
            tra=tra->link;
            add(&tra,'0');
            tra=tra->link->link;
            while (tra->link!=NULL && isdigit(tra->link->data)){
                tra=tra->link;
            }
            add(&tra,')');
        }
        tra=tra->link;
    }

    char postfix[2*count];
    for (struct hell* q=list; q!=NULL; q=q->link){
        char sym=q->data;
        if (sym=='(' || sym=='{' || sym=='['){
            push(sym);
        }
        else if (sym==')' || sym==']' || sym=='}'){
            char c=pop();
            while (c!='(' && c!='{' && c!='['){
                postfix[u++]=c;
                postfix[u++]=' ';
                c=pop();
            }
        }
        else if (pre(sym)){
            while (!isempty() && (pre(sym) <= pre(peek()))){
                postfix[u++]=pop();
                postfix[u++]=' ';
            }
            push(sym);
        }
        else if (isdigit(sym) || sym=='.'){
            postfix[u++]=sym;
            if (q->link!=NULL && !isdigit(q->link->data) && q->link->data!='.'){
                postfix[u++]=' ';
            }
            if (q->link==NULL){
                postfix[u++]=' ';
            }
        }
    }

    while (!isempty()){
        postfix[u++]=pop();
        postfix[u++]=' ';
    }
    postfix[u++]='\0';
    printf("Postfix: %s\n",postfix);
    
    for (int j=0; j<strlen(postfix); j++){
        char sym=postfix[j];
        if (pre(sym)){
            double a=pop_num();
            double b=pop_num();
            push_num(solve(a,b,sym));
        }
        else if (isdigit(sym)){
            char k[50];
            int m;
            for(m=0; isdigit(sym) || sym=='.'; m++){
                k[m]=sym;
                sym=postfix[++j];
            }
            k[m]='\0';
            push_num(atof(k));
        }
    }

    printf("Solution: %.10g",pop_num());
    getch();
    return 0;
}