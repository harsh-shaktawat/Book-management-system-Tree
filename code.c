/*      ASSUMPTIONS
------> dates are considered within a single month from day1 to day15
------> return date = -1, indicates book is not returned till current date
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef enum{FAILURE,SUCCESS} status_code;
typedef enum{FALSE,TRUE} bool;
int CURRENT_DATE;
typedef struct bookNode_tag{
    int id;
    char title[30];
    char author[30];
    char subject[30];
    int copies_issued;
    int copies_available;
    struct bookNode_tag* next;
}bookNode;
typedef struct treeNode_tag {
    bookNode book;
    int height;
    struct treeNode_tag *left;
    struct treeNode_tag *right;
}treeNode;
typedef struct requestNode_tag{
    char name[30];
    char bookTitle[30];
}requestNode;
typedef struct requestQueue_tag{
    requestNode request;
    struct requestQueue_tag* next;
}requestQueue;
typedef struct borrowNode_tag{
    char name[30];
    char title[30];
    int issueDate;
    int returnDate;
}borrowNode;
typedef struct borrowList_tag{
    borrowNode borrow;
    struct borrowList_tag* next;
}borrowList;
typedef struct freqNode_tag{
    char name[30];
    char title[30];
    char author[30];
    int count;
    struct freqNode_tag* next;
}freqNode;
treeNode* BKL_root;
requestQueue* RQ_root;
borrowList* BRL_root;

int max(int a, int b)
{
    return (a > b)? a : b;
}
int height(treeNode* root){
    if(root==NULL) return 0;
    else return 1+max(height(root->left),height(root->right));
}
treeNode* rightRotate(treeNode* y){
    treeNode *x = y->left;
    treeNode *T2 = x->right;
 
    // Perform rotation
    x->right = y;
    y->left = T2;
 
    // Update heights
    y->height = height(y);
    x->height = height(x);
 
    // Return new root
    return x;
}
treeNode *leftRotate(treeNode *x)
{
    treeNode *y = x->right;
    treeNode *T2 = y->left;
 
    // Perform rotation
    y->left = x;
    x->right = T2;
 
    //  Update heights
    x->height = height(x);
    y->height = height(y);
 
    // Return new root
    return y;
}
int getBalance(treeNode *N){
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}
treeNode* newNode(bookNode book){
    treeNode* node = (treeNode*)malloc(sizeof(treeNode));
    node->book   = book;
    node->left   = NULL;
    node->right  = NULL;
    node->height = 0;  // new node is initially added at leaf
    return(node);
}
treeNode* insert(treeNode* node, bookNode key)
{
    /* 1.  Perform the normal BST insertion */
    if (node == NULL)
        return(newNode(key));
 
    if (key.id < node->book.id)
        node->left  = insert(node->left, key);
    else if (key.id > node->book.id)
        node->right = insert(node->right, key);
    else // Equal keys are not allowed in BST
        return node;
 
    /* 2. Update height of this ancestor node */
    node->height = height(node);
 
    /* 3. Get the balance factor of this ancestor
          node to check whether this node became
          unbalanced */
    int balance = getBalance(node);
 
    // If this node becomes unbalanced, then
    // there are 4 cases
 
    // Left Left Case
    if (balance > 1 && key.id <node->left->book.id)
        return rightRotate(node);
 
    // Right Right Case
    if (balance < -1 && key.id > node->right->book.id)
        return leftRotate(node);
 
    // Left Right Case
    if (balance > 1 && key.id > node->left->book.id)
    {
        node->left =  leftRotate(node->left);
        return rightRotate(node);
    }
 
    // Right Left Case
    if (balance < -1 && key.id < node->right->book.id)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
 
    /* return the (unchanged) node pointer */
    return node;
}

void printBookNode(bookNode node){
    printf("%d\t%s\t%s\t%s\t%d\t%d\n", node.id,node.title,node.author,node.subject,node.copies_issued,node.copies_available);
}
void initializeBookList(){
    BKL_root=NULL;
}
bool isBookListEmpty(){
    return (BKL_root==NULL);
}
bookNode createBookNode(int id,char title[30],char author[30], char subject[30], int issued,int available){
    bookNode nptr;
    nptr.id=id;
    strcpy(nptr.title,title);
    strcpy(nptr.author,author);
    strcpy(nptr.subject,subject);
    nptr.copies_issued=issued;
    nptr.copies_available=available;
    return nptr;
}
void createBookList(){
    int tot;
    printf("How many books you want to add\n");
    scanf("%d", &tot);
    while(tot--){
        int id;
        char title[30];
        char author[30];
        char subject[30];
        int issued,available;
        printf("enter book-id\n");
        scanf("%d", &id);
        printf("Enter title of book\n");
        scanf("%s", title);
        printf("Enter author of book\n");
        scanf("%s", author);
        printf("Enter subject of book\n");
        scanf("%s", subject);
        printf("Enter no of issued copies\n");
        scanf("%d", &issued);
        printf("Enter no of books available\n");
        scanf("%d", &available);
        bookNode newBook=createBookNode(id,title,author,subject,issued,available);
        BKL_root=insert(BKL_root,newBook);
    }
}
//AVL tree traversal
void traversal(treeNode *root)
{
    if(root != NULL)
    {
        traversal(root->left);
        printBookNode(root->book);
        traversal(root->right);
    }
}

void initializeQueue(requestQueue* head){
    head=NULL;
}
bool isRequestQueueEmpty(requestQueue* head){
    return (head==NULL);
}
requestNode createRequestNode(char name[30],char title[30]){
    requestNode node;
    strcpy(node.name,name);
    strcpy(node.bookTitle,title);
    return node;
}
requestQueue* insertRequest(requestQueue* head,requestNode request){
    requestQueue* newNode=(requestQueue*)malloc(sizeof(requestQueue));
    newNode->request=request;
    newNode->next=NULL;
    if(isRequestQueueEmpty(head)){
        head=newNode;
    }
    else{
        requestQueue* tmp=head;
        while(tmp->next!=NULL){
            tmp=tmp->next;
        }
        tmp->next=newNode;
    }
    return head;        
}
void printRequestQueue(){
    printf("\n--------REQUEST QUEUE--------\n");
    requestQueue* head=RQ_root;
    while(head!=NULL){
        printf("%s\t%s\n", head->request.name,head->request.bookTitle);
        head=head->next;
    }
}
void createRequestQueue(){
    int tot;
    printf("How many request you want to add\n");
    scanf("%d", &tot);
    while(tot--){
        char name[30];
        char title[30];
        printf("Enter name of student\n");
        scanf("%s", name);
        printf("Enter title of book\n");
        scanf("%s", title);
        requestNode newRequest=createRequestNode(name,title);
        RQ_root=insertRequest(RQ_root,newRequest);
    }
}

void initializeBorrowList(){
    BRL_root=NULL;
}
bool isBorrowListEmpty(){
    return (BRL_root==NULL);
}
borrowNode createBorrowNode(char name[30],char title[30],int issuedate,int returndate){
    borrowNode bptr;
    strcpy(bptr.name,name);
    strcpy(bptr.title,title);
    bptr.issueDate=issuedate;
    bptr.returnDate=returndate;
    return bptr;
}
void insertBorrowNode(borrowNode borrow){
    borrowList* newNode=(borrowList*)malloc(sizeof(borrowList));
    newNode->borrow=borrow;
    newNode->next=NULL;
    if(isBorrowListEmpty()){
        BRL_root=newNode;
    }
    else{
        borrowList* tmp=BRL_root;
        while(tmp->next!=NULL){
            tmp=tmp->next;
        }
        tmp->next=newNode;
    }   
}
void printBorrowList(){
    printf("\n--------BORROW LIST--------\n");
    borrowList* head=BRL_root;
    while(head!=NULL){
        printf("%s\t%s\t",head->borrow.name,head->borrow.title);
        printf("%d\t%d\n",head->borrow.issueDate,head->borrow.returnDate);
        head=head->next;
    }
}
void incrementBook(treeNode* node,char title[30],bool ok){
    if(!node) return;
    if(strcmp(node->book.title,title)==0){
        if(ok){
            node->book.copies_available++;
            node->book.copies_issued--;
        }
        else{
            node->book.copies_available--;
            node->book.copies_issued++;
        }
        return;
    }
    incrementBook(node->left,title,ok);
    incrementBook(node->right,title,ok);
}
void returnBook(){
    int retDate;
    char name[30];
    char title[30];
    printf("enter name of student\n");
    scanf("%s", name);
    printf("enter title of book\n");
    scanf("%s", title);
    printf("enter return date\n");
    scanf("%d", &retDate);
    borrowList* ptr=BRL_root;
    bool found=FALSE;
    while(ptr!=NULL && found==FALSE){
        if(strcmp(ptr->borrow.name,name)==0 && strcmp(ptr->borrow.title,title)==0){
            found=TRUE;
        }
        else{
            ptr=ptr->next;
        }
    }
    if(found){
        ptr->borrow.returnDate=retDate;
        incrementBook(BKL_root,title,TRUE);
        printf("Return date updated\n");
    }
    else{
        printf("Invalid details\n");
    }
}

int countBorrowedBooks(char name[30]){
    int count=0;
    borrowList* ptr=BRL_root;
    while(ptr!=NULL){
        if(strcmp(ptr->borrow.name,name)==0 && ptr->borrow.returnDate==-1){//return date -1 indicates book is not returned till current date
            count++;
        }
        ptr=ptr->next;
    }
    return count;
}

void countAvailableCopies(treeNode* node,char title[30],int* cnt){
    if(!node) return;
    if(strcmp(node->book.title,title)==0){
        *cnt=node->book.copies_available;
    }
    countAvailableCopies(node->left,title,cnt);
    countAvailableCopies(node->right,title,cnt);
}

int daysGap(int day1,int day2){
    int count=0;
    count=day2-day1;
    return count;
}

bool isDefaulter(char name[30]){
    borrowList* ptr=BRL_root;
    bool ret_val=FALSE;   
    while(ptr!=NULL){
        if(strcmp(ptr->borrow.name,name)==0){
            int gap;
            if(ptr->borrow.returnDate!=-1){
                gap=daysGap(ptr->borrow.issueDate,ptr->borrow.returnDate);
            }
            else {
                gap=CURRENT_DATE-ptr->borrow.issueDate;
            }
            if(gap>15){
                ret_val=TRUE;
            }
        }        
        ptr=ptr->next;
    }
    return ret_val;
}

requestQueue* deleteAfter(requestQueue* p,requestQueue* q,requestQueue* lptr){
    if(q!=NULL){
        if(p!=NULL){
            if(p->next==q){
                p->next=q->next;
                free(q);
            }
        }
        else{
            if(q==lptr){
                lptr=lptr->next;
                free(q);
            }
        }
    }
    return lptr;
}

void q1(){
    requestQueue* ptr2=RQ_root;
    requestQueue* prev=NULL;
    borrowList* ptr3=BRL_root;
    requestQueue* newQueue=NULL;//new queue to store request who were shifted to last priortiy
    printf("\nEnter date of issue(current date, must be between 1 to 15)\n");
    scanf("%d", &CURRENT_DATE);//date on which books are issued
    if(isRequestQueueEmpty(RQ_root)){
        printf("Request queue is empty\n");
    }
    else{
        printf("\nProcessing request queue\n");
        while(ptr2!=NULL){
            printf("%s-----",ptr2->request.name);
            int cnt1=countBorrowedBooks(ptr2->request.name);
            int cnt2=0;
            countAvailableCopies(BKL_root,ptr2->request.bookTitle, &cnt2);
            if(cnt2==0){
                printf("PENDING: All copies are issued. Book is not avilable\n");
                prev=ptr2;//store current node in prev bcoz in future the copy might be available
                ptr2=ptr2->next;
            }
            else{
                if(cnt1==0){//cnt==0 indicates no book is issued by student
                    printf("SUCCESS\n");
                    borrowNode newNode=createBorrowNode(ptr2->request.name,ptr2->request.bookTitle,CURRENT_DATE,-1);
                    insertBorrowNode(newNode);//insert node in borrowlist
                    incrementBook(BKL_root,ptr2->request.bookTitle,FALSE);//decrement book from booklist,false is passed
                }
                else if(cnt1<3){
                    if(isDefaulter(ptr2->request.name)){
                        printf("FAILURE:Previous book not returned on time(DEFAULTER)\n");
                    }
                    else{
                        printf("already borrowed book. Moving the request to lowest priority\n");
                        requestNode newNode=createRequestNode(ptr2->request.name,ptr2->request.bookTitle);
                        newQueue=insertRequest(newQueue,newNode);//moving the request into new queue for later proccessing
                    }
                }
                else if(cnt1>=3){
                    printf("FAILURE: Already 3 books borrowed\n");
                }
                requestQueue* toDelete=ptr2;//current node is proccessed so delete it
                ptr2=ptr2->next;
                RQ_root=deleteAfter(prev,toDelete,RQ_root);
            }
        }
        ptr2=newQueue;
        if(ptr2!=NULL){
            printf("\nProcesssing request of students who were shifted to lowest priority\n");
            while(ptr2!=NULL){
                printf("%s-----",ptr2->request.name);
                int cnt1=countBorrowedBooks(ptr2->request.name);
                int cnt2;
                countAvailableCopies(BKL_root,ptr2->request.bookTitle,&cnt2);
                if(cnt1==0){
                    requestNode newNode=createRequestNode(ptr2->request.name,ptr2->request.bookTitle);
                    RQ_root=insertRequest(RQ_root,newNode);//insert request to original queue bcoz in future it might be available
                    printf("PENDING: All copies are issued. Book is not avilable\n");
                }
                else{
                    if(cnt1<3){
                        printf("SUCCESS\n");
                        borrowNode newNode=createBorrowNode(ptr2->request.name,ptr2->request.bookTitle,CURRENT_DATE,-1);
                        insertBorrowNode(newNode);
                        incrementBook(BKL_root,ptr2->request.bookTitle,FALSE);
                    }
                    else{
                        printf("FAILURE: Already 3 books borrowed\n");
                    }
                }
                ptr2=ptr2->next;
            }
        }
    }
}

int countRequestBooks(requestQueue* head,char title[30]){
    int cnt=0;
    requestQueue* ptr2=head;
    while(ptr2!=NULL){
            if(strcmp(ptr2->request.bookTitle,title)==0){
                cnt++;
            }
            ptr2=ptr2->next;
    }
    return cnt;
} 
void bookInDemand(treeNode* node,int* mx,bool check){
    if(!node) return;
    int cnt1=countRequestBooks(RQ_root,node->book.title);
    int cnt2=node->book.copies_issued;
    if(cnt1+cnt2>*mx && check==TRUE){//check is true and max is less than count
        *mx=cnt1+cnt2;//update max
    }
    else if(check==FALSE && cnt1+cnt2==(*mx)){//check is false and cnt is equal to max
        printf("%s\n",node->book.title);//print book title
    }
    bookInDemand(node->left,mx,check);
    bookInDemand(node->right,mx,check);

}
void q2(){
    int mx=-1;
    bool check=1;//check is set to true
    bookInDemand(BKL_root,&mx,check);//function will update max value i.e. 
    check=0;
    printf("Book/Books in most demand\n");
    bookInDemand(BKL_root,&mx,check);
}

void countBooksQ3(treeNode* node,int stDay,int enDay){
    if(!node) return;
    int tot=node->book.copies_available+node->book.copies_issued;
    borrowList* ptr=BRL_root;
    while(ptr!=NULL){
        if(strcmp(ptr->borrow.title,node->book.title)==0){
            if(ptr->borrow.issueDate<stDay && ptr->borrow.returnDate==-1){//book was borrowed but not returned
                tot--;
            }
            else if(ptr->borrow.issueDate<stDay && ptr->borrow.returnDate>enDay){
                tot--;
            }
        }
        ptr=ptr->next;
    }
    printf("%d books of title-%s can be given\n", tot,node->book.title);
    countBooksQ3(node->left,stDay,enDay);
    countBooksQ3(node->right,stDay,enDay);
}
void q3(){
    int stDay;
    printf("\nEnter starting day(must be between 1 to 27)\n");
    scanf("%d", &stDay);
    int enDay=stDay+3;
    printf("Books which can be given from date %d to date %d\n", stDay,enDay);
    countBooksQ3(BKL_root,stDay,enDay);
}

freqNode* DivideF(freqNode* lptr){
    freqNode* nptr,*fast,*slow;
    slow=lptr;
    fast=lptr->next->next;
    while(fast!=NULL){
        fast=fast->next;
        slow=slow->next;
        if(fast!=NULL){
            fast=fast->next;
        }
    }
    nptr=slow->next;
    slow->next=NULL;
    return nptr;
}
freqNode* MergeF(freqNode* lptr,freqNode* nptr){
    freqNode* ptr1,*ptr2,*result,*tail;
    ptr1=lptr;
    ptr2=nptr;
    if((ptr1->count)>ptr2->count){
        result=ptr1;
        ptr1=ptr1->next;
    }
    else{
        result=ptr2;
        ptr2=ptr2->next;
    }
    tail=result;
    while((ptr2!=NULL) && (ptr1!=NULL)){
        if((ptr1->count)>(ptr2->count)){
            tail=tail->next=ptr1;
            ptr1=ptr1->next;
        }
        else{
            tail=tail->next=ptr2;
            ptr2=ptr2->next;
        }
    }
    if(ptr1!=NULL) tail->next=ptr1;
    else tail->next=ptr2;
    return result;
}
freqNode* MergeSortF(freqNode* list_ptr){
    freqNode* lptr,*nptr;
    lptr=list_ptr;
    if((lptr!=NULL) && (lptr->next!=NULL)){
        nptr=DivideF(lptr);
        lptr=MergeSortF(lptr);
        nptr=MergeSortF(nptr);
        lptr=MergeF(lptr,nptr);
    }
    return lptr;
}
// int count(char name[30]){
//     requestNode* ptr=queueHead;
//     int count=1;
//     while(ptr!=NULL){
//         if(strcmp(ptr->name,name)==0){
//             count++;
//         }
//         ptr=ptr->next;
//     }
//     return count;    
// }
bool isPresent(freqNode* head,char name[30]){
    freqNode* ptr1=head;
    bool ret_val=FALSE;
    while(ptr1!=NULL && ret_val==FALSE){
        if(strcmp(ptr1->name,name)==0){
            ret_val=TRUE;
        }
        ptr1=ptr1->next;
    }
    return ret_val;
}
freqNode* createFreqNode(char name[30],int cnt,char title[30],char author[30] ){
    freqNode* newNode=(freqNode*)malloc(sizeof(freqNode));
    strcpy(newNode->name,name);
    strcpy(newNode->title,title);
    strcpy(newNode->author,author);
    newNode->count=cnt;
    newNode->next=NULL;
    return newNode;
}
freqNode* insertFreqNode(freqNode* lptr,freqNode* ptr){
    if(lptr==NULL){
        lptr=ptr;
    }
    else{
        freqNode* nptr=lptr;
        while(nptr->next!=NULL){
            nptr=nptr->next;
        }
        nptr->next=ptr;
    }
    return lptr;
}
void q4(){
    freqNode* head=NULL;
    borrowList* ptr=BRL_root;
    if(ptr==NULL){
        printf("Borrower list is empty\n");
    }
    else{
        char str[30];
        str[0]='\0';
        while(ptr!=NULL){
            int cnt=countBorrowedBooks(ptr->borrow.name);
            if(isPresent(head,ptr->borrow.name)==FALSE && cnt!=0){
                head=insertFreqNode(head,createFreqNode(ptr->borrow.name,cnt,str,str));
            }
            ptr=ptr->next;
        }
        head=MergeSortF(head);
        freqNode* ptr1=head;
        printf("\nSorted borrower_list according to the number of books issued in descending order\n");
        while(ptr1!=NULL){
            printf("NAME:%s\tCount of books borrowed:%d\n",ptr1->name,ptr1->count);
            ptr1=ptr1->next;
        }
    }
}

// void createList(struct btreeNode* myNode, freqNode** head,char sub[30]){
//     int i;
//     if (myNode) {
//         char str[30];
//         str[0]='\0';
//             for (i = 0; i < myNode->count; i++) {
//                     createList(myNode->link[i],head,sub);
//                     if(strcmp(myNode->val[i+1]->subject,sub)==0){
//                         int cnt=myNode->val[i+1]->copies_available;
//                         char title[30],author[30];
//                         //printf("found\n");
//                         strcpy(title,myNode->val[i+1]->title);
//                         strcpy(author,myNode->val[i+1]->author);
//                         freqNode* tempHead=*head;
//                         tempHead=insertFreqNode(tempHead,createFreqNode(str,cnt,title,author));
//                         *head=tempHead;
//                     }
//             }
//             createList(myNode->link[i],head,sub);
//     }
// }
// void q5(struct btreeNode* myNode ){
//     char sub[30];
//     printf("\nEnter the subject of books\n");
//     scanf("%s", sub);
//     freqNode* head=NULL;
//     if(myNode==NULL){
//         printf("Book List is empty\n");
//     }
//     else{
//         createList(myNode,&head,sub);
//         head=MergeSortF(head);
//         freqNode* ptr=head;
//         if(head==NULL){
//             printf("No book of subject %s\n",sub);
//         }
//         else{
//             printf("\nBooks of subject %s based on copies availbale\n\n",sub);
//             while(ptr!=NULL){
//                 printf("Title:%s\tAuthor:%s\tCopies Available:%d\n", ptr->title,ptr->author,ptr->count);
//                 ptr=ptr->next;
//             }
//             printf("\n");
//         }
        //     }
// }

// void q6(struct btreeNode* myNode,requestNode* queueHead){
//     requestNode* ptr=queueHead;
//     if(ptr==NULL){
//         printf("\nRequest queue is empty\n");
//     }
//     else{
//         printf("\nName(title) of requested books whose copies are available\n");
//         while(ptr!=NULL){
//             int cnt;
//             countAvailableCopies(myNode,ptr->bookTitle,&cnt);
//             if(cnt>0){
//                 printf("%s\n",ptr->bookTitle);
//             }
//             ptr=ptr->next;
//         }
//     }
    // }

// void bookissued0(struct btreeNode* myNode){
//     int i;
//     if (myNode) {
//             for (i = 0; i < myNode->count; i++) {
//                     bookissued0(myNode->link[i]);
//                     if(myNode->val[i+1]->copies_issued==0){
//                         printf("%s\n",myNode->val[i+1]->title);
//                     }
//             }
//             bookissued0(myNode->link[i]);
//     }
// }
// void q7(struct btreeNode* myNode){
//     printf("\nTitle of books which have not been issued by anyone\n");
//     bookissued0(myNode);
// }

// void idRange(struct btreeNode* myNode,int lo,int hi){
//     int i;
//     if (myNode) {
//             for (i = 0; i < myNode->count; i++) {
//                     idRange(myNode->link[i],lo,hi);
//                     if(lo<=myNode->val[i+1]->id && myNode->val[i+1]->id<=hi){
//                         printf("ID:%d\tTitle:%s\n",myNode->val[i+1]->id, myNode->val[i+1]->title);
//                     }
//             }
//             idRange(myNode->link[i],lo,hi);
//     }
// }

// void q8(struct btreeNode* myNode){
//     int From_Book_ID,To_Book_ID;
//     printf("Enter starting id:\n");
//     scanf("%d", &From_Book_ID);
//     printf("Enter ending id:\n");
//     scanf("%d", &To_Book_ID);
//     printf("Books with their id in range %d to %d\n",From_Book_ID,To_Book_ID);
//     idRange(myNode,From_Book_ID,To_Book_ID);
// }

void questions(){
    int opt;
    printf("\nPress\n1 for q1\n2 for q2\n3 for q3\n4 for q4\n");
    printf("5 for q5\n6 for q6\n7 for q7\n8 for q8\n");
    scanf("%d", &opt);
    if(opt==1){
        q1();
    }
    else if(opt==2){  
        q2();
    }
    else if(opt==3){
        q3();
    }
    else if(opt==4){
        q4();
    }
    // else if(opt==5){
    //     q5(root);
    // }
    // else if(opt==6){
    //     q6(root,myQueue->front);
    // }
    // else if(opt==7){
    //     q7(root);
    // }
    // else if(opt==8){
    //     q8(root);
    // }
    else{
        printf("INVALID OPTION\n");
    }
}

int main(){
    initializeBookList(); 
    initializeQueue(RQ_root); 
    initializeBorrowList();
    
    char choice='y';
    while(choice=='y'){
        int opt;
        printf("\nPress\n0 to exit\n1 to insert book\n2 to display book list\n");
        printf("3 to insert request\n4 to display request queue\n5 to return book\n");
        printf("6 to display borrower list\n7 to display assignment questions\n");
        scanf("%d", &opt);
        if(opt==0){
            printf("\nEXITED\n");
            choice='n';
        }
        else if(opt==1){  
            createBookList();
        }
        else if(opt==2){
            printf("Id\tTitle\tAuthor\tSubject\tCopiesIssued\tCopiesAvailable\n");
            traversal(BKL_root);
        }
        else if(opt==3){
            createRequestQueue();
        }
        else if(opt==4){
            printRequestQueue();
        }
        else if(opt==5){
            returnBook();
        }
        else if(opt==6){
            printBorrowList();
        }
        else if(opt==7){
            questions();
        }
        else{
            printf("Invalid option\nPress again\n");
        }
    }
    return 0;
}
