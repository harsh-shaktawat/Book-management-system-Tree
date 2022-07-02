/*      ASSUMPTIONS
------> dates are considered within a single month from day1 to day15
------> return date = -1, indicates book is not returned till current date
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef enum{FAILURE,SUCCESS} status_code;
typedef enum{FALSE,TRUE} bool;
#define MAX 4
#define MIN 2
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
struct btreeNode {
    bookNode* val[MAX + 1];
    int count;
    struct btreeNode *link[MAX + 1];
};
typedef struct requestNode_tag{
    char name[30];
    char bookTitle[30];
    struct requestNode_tag* next;
}requestNode;
typedef struct requestQueue_tag{
    requestNode* front;
    requestNode* rear;
}requestQueue;
typedef struct borrowNode_tag{
    char name[30];
    char title[30];
    int issueDate;
    int returnDate;
    struct borrowNode_tag* next; 
}borrowNode;
typedef struct borrowList_tag{
    borrowNode* front;
    borrowNode* rear;
}borrowList;
typedef struct freqNode_tag{
    char name[30];
    char title[30];
    char author[30];
    int count;
    struct freqNode_tag* next;
}freqNode;

struct btreeNode *root;
/* creating new btreeNode node */
struct btreeNode * createNode(bookNode* val, struct btreeNode *child) {
    struct btreeNode *newNode;
    newNode = (struct btreeNode *)malloc(sizeof(struct btreeNode));
    newNode->val[1] = val;
    newNode->count = 1;
    newNode->link[0] = root;
    newNode->link[1] = child;
    return newNode;
}

/* Places the value in appropriate position */
void addValToNode(bookNode* val, int pos, struct btreeNode *node, struct btreeNode *child) {
    int j = node->count;
    while (j > pos) {
            node->val[j + 1] = node->val[j];
            node->link[j + 1] = node->link[j];
            j--;
    }
    node->val[j + 1] = val;
    node->link[j + 1] = child;
    node->count++;
}

/* split the node */
void splitNode (bookNode* val, bookNode* *pval, int pos, struct btreeNode *node,struct btreeNode *child, struct btreeNode **newNode) {
    int median, j;

    if (pos > MIN)
            median = MIN + 1;
    else
            median = MIN;

    *newNode = (struct btreeNode *)malloc(sizeof(struct btreeNode));
    j = median + 1;
    while (j <= MAX) {
            (*newNode)->val[j - median] = node->val[j];
            (*newNode)->link[j - median] = node->link[j];
            j++;
    }
    node->count = median;
    (*newNode)->count = MAX - median;

    if (pos <= MIN) {
            addValToNode(val, pos, node, child);
    } else {
            addValToNode(val, pos - median, *newNode, child);
    }
    *pval = node->val[node->count];
    (*newNode)->link[0] = node->link[node->count];
    node->count--;
}

/* sets the value val in the node */
int setValueInNode(bookNode* val,bookNode* *pval,struct btreeNode *node, struct btreeNode **child) {

    int pos;
    if (!node) {
            *pval = val;
            *child = NULL;
            return 1;
    }

    if (val->id < node->val[1]->id) {
            pos = 0;
    } else {
            for (pos = node->count;(val->id < node->val[pos]->id && pos > 1); pos--);
            if (val->id == node->val[pos]->id) {
                    printf("Duplicates not allowed\n");
                    return 0;
            }
    }
    if (setValueInNode(val, pval, node->link[pos], child)) {
            if (node->count < MAX) {
                    addValToNode(*pval, pos, node, *child);
            } else {
                    splitNode(*pval, pval, pos, node, *child, child);
                    return 1;
            }
    }
    return 0;
}

/* insert val in B-Tree */
void insertion(bookNode* val) {
    int flag;
    bookNode* i;
    struct btreeNode *child;

    flag = setValueInNode(val, &i, root, &child);
    if (flag)
            root = createNode(i, child);
}

void printBookNode(bookNode* node){
    printf("%d\t%s\t%s\t%s\t%d\t%d\n", node->id,node->title,node->author,node->subject,node->copies_issued,node->copies_available);
}
void initializeBookList(){
    root=NULL;
}
bool isBookListEmpty(){
    return (root==NULL);
}
bookNode* createBookNode(int id,char title[30],char author[30], char subject[30], int issued,int available){
    bookNode *nptr=(bookNode*) malloc(sizeof(bookNode));
    nptr->id=id;
    strcpy(nptr->title,title);
    strcpy(nptr->author,author);
    strcpy(nptr->subject,subject);
    nptr->copies_issued=issued;
    nptr->copies_available=available;
    nptr->next=NULL;
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
        bookNode* newBook=createBookNode(id,title,author,subject,issued,available);
        insertion(newBook);
    }
}
/* B-Tree Traversal */
void traversal(struct btreeNode *myNode) {
    int i;
    if (myNode) {
            for (i = 0; i < myNode->count; i++) {
                    traversal(myNode->link[i]);
                    printBookNode(myNode->val[i + 1]);
            }
            traversal(myNode->link[i]);
    }
}

void initializeQueue(requestQueue* qptr){
    qptr->front=NULL;
    qptr->rear=NULL;
}
bool isRequestQueueEmpty(requestQueue* qptr){
    return (qptr->front==NULL && qptr->rear==NULL);
}
requestNode* createRequestNode(char name[30],char title[30]){
    requestNode* nptr=(requestNode*)malloc(sizeof(requestNode));
    strcpy(nptr->name,name);
    strcpy(nptr->bookTitle,title);
    nptr->next=NULL;
    return nptr;
}
status_code insertRequest(requestQueue* qptr,requestNode* request){
    status_code ret_val=SUCCESS;
    if(request==NULL){
        ret_val=FAILURE;
    }
    else{
        if(isRequestQueueEmpty(qptr)){
            qptr->front=qptr->rear=request;
        }
        else{
            (qptr->rear)->next=request;
            qptr->rear=request;
        }
        
    }
    return ret_val;
}
void printRequestQueue(requestNode* head){
    printf("\n--------REQUEST QUEUE--------\n");
    while(head!=NULL){
        printf("%s\t%s\n", head->name,head->bookTitle);
        head=head->next;
    }
}
void createRequestQueue(requestQueue* myQueue){
    int tot;
    printf("How many request you want to add\n");
    scanf("%d", &tot);
    requestQueue qptr=*myQueue;
    status_code ret_val;
    while(tot--){
        char name[30];
        char title[30];
        printf("Enter name of student\n");
        scanf("%s", name);
        printf("Enter title of book\n");
        scanf("%s", title);
        requestNode* newRequest=createRequestNode(name,title);
        ret_val=insertRequest(&qptr,newRequest);
    }
    
    *myQueue=qptr;
}

void initializeBorrowList(borrowList* bptr){
    bptr->front=NULL;
    bptr->rear=NULL;
}
bool isBorrowListEmpty(borrowList* bptr){
    return (bptr->front==NULL && bptr->rear==NULL);
}
borrowNode* createBorrowNode(char name[30],char title[30],int issuedate,int returndate){
    borrowNode* bptr=(borrowNode*)malloc(sizeof(borrowNode));
    strcpy(bptr->name,name);
    strcpy(bptr->title,title);
    bptr->issueDate=issuedate;
    bptr->returnDate=returndate;
    bptr->next=NULL;
    return bptr;
}
status_code insertBorrowNode(borrowList* bptr,borrowNode* borrow){
    status_code ret_val=SUCCESS;
    if(borrow==NULL){
        ret_val=FAILURE;
    }
    else{
        if(isBorrowListEmpty(bptr)){
            bptr->front=bptr->rear=borrow;
        }
        else{
            (bptr->rear)->next=borrow;
            bptr->rear=borrow;
        }
    }
    return ret_val;
}
void printBorrowList(borrowNode* head){
    printf("\n--------BORROW LIST--------\n");
    while(head!=NULL){
        printf("%s\t%s\t",head->name,head->title);
        printf("%d\t%d\n",head->issueDate,head->returnDate);
        head=head->next;
    }
}
void incrementBook(struct btreeNode* myNode,char title[30]){
    int i;
    if (myNode) {
            for (i = 0; i < myNode->count; i++) {
                    incrementBook(myNode->link[i],title);
                    if(strcmp(myNode->val[i+1]->title,title)==0){
                        myNode->val[i+1]->copies_available++;
                        myNode->val[i+1]->copies_issued--;
                        return;
                    }
            }
            incrementBook(myNode->link[i],title);
    }
}
void returnBook(struct btreeNode* myNode,borrowList myBorrowList){
    borrowNode* ptr=myBorrowList.front;
    int retDate;
    char name[30];
    char title[30];
    printf("enter name of student\n");
    scanf("%s", name);
    printf("enter title of book\n");
    scanf("%s", title);
    printf("enter return date\n");
    scanf("%d", &retDate);
    bool found=FALSE;
    while(ptr!=NULL && found==FALSE){
        if(strcmp(ptr->name,name)==0 && strcmp(ptr->title,title)==0){
            found=TRUE;
        }
        else{
            ptr=ptr->next;
        }
    }
    if(found){
        ptr->returnDate=retDate;
        incrementBook(myNode,title);
        printf("Return date updated\n");
    }
    else{
        printf("Invalid details\n");
    }
}

int countBorrowedBooks(borrowNode* borrowHead,char name[30]){
    int count=0;
    
    borrowNode* ptr=borrowHead;
    
    while(ptr!=NULL){
        if(strcmp(ptr->name,name)==0 && ptr->returnDate==-1){//return date -1 indicates book is not returned till current date
            count++;
        }
        ptr=ptr->next;
    }
    
    return count;
}
void countAvailableCopies(struct btreeNode *myNode,char title[30],int* cnt){
    int i;
    if (myNode) {
            for (i = 0; i < myNode->count; i++) {
                countAvailableCopies(myNode->link[i],title,cnt);
                if(strcmp(myNode->val[i+1]->title,title)==0){
                    *cnt=myNode->val[i+1]->copies_available;
                    return ;
                }
            }
            countAvailableCopies(myNode->link[i],title,cnt);
    }
}
int daysGap(int day1,int day2){
    int count=0;
    count=day2-day1;
    return count;
}
bool isDefaulter(borrowNode* borrowHead,char name[30]){
    borrowNode* ptr=borrowHead;
    bool ret_val=FALSE;
    
    while(ptr!=NULL){
        if(strcmp(ptr->name,name)==0){
            int gap;
            if(ptr->returnDate!=-1){
                gap=daysGap(ptr->issueDate,ptr->returnDate);
            }
            else {
                gap=CURRENT_DATE-ptr->issueDate;
            }
            if(gap>15){
                ret_val=TRUE;
            }
        }
        
        ptr=ptr->next;
    }
    return ret_val;
}
void decrementBook(struct btreeNode *myNode,char title[30]){
    int i;
    if (myNode) {
            for (i = 0; i < myNode->count; i++) {
                decrementBook(myNode->link[i],title);
                if(strcmp(myNode->val[i+1]->title,title)==0){
                    myNode->val[i+1]->copies_available--;
                    myNode->val[i+1]->copies_issued++;
                    return;
                }
            }
            decrementBook(myNode->link[i],title);
    } 
}
requestNode* deleteAfter(requestNode* p,requestNode* q,requestNode* lptr){
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
void q1(struct btreeNode* root,requestQueue* myQueue,borrowList* myBorrowList){
    
    requestNode* ptr2=myQueue->front;
    requestNode* prev=NULL;
    borrowNode* ptr3=myBorrowList->front;
    requestQueue newQueue;//new queue to store request who were shifted to last priortiy
    initializeQueue(&newQueue);
    printf("\nEnter date of issue(current date, must be between 1 to 15)\n");
    scanf("%d", &CURRENT_DATE);//date on which books are issued
    if(isRequestQueueEmpty(myQueue)){
        printf("Request queue is empty\n");
    }
    else{
        printf("\nProcessing request queue\n");
        while(ptr2!=NULL){
            printf("%s-----",ptr2->name);
            int cnt1=countBorrowedBooks(myBorrowList->front,ptr2->name);
            int cnt2=0;
            countAvailableCopies(root,ptr2->bookTitle, &cnt2);
            if(cnt2==0){
                printf("PENDING: All copies are issued. Book is not avilable\n");
                prev=ptr2;//store current node in prev bcoz in future the copy might be available
                ptr2=ptr2->next;
            }
            else{
                if(cnt1==0){//cnt==0 indicates no book is issued by student
                    printf("SUCCESS\n");
                    borrowNode* newNode=createBorrowNode(ptr2->name,ptr2->bookTitle,CURRENT_DATE,-1);
                    status_code val= insertBorrowNode(myBorrowList,newNode);//insert node in borrowlist
                    decrementBook(root,ptr2->bookTitle);//decrement book from booklist
                }
                else if(cnt1<3){
                    if(isDefaulter(myBorrowList->front,ptr2->name)){
                        printf("FAILURE:Previous book not returned on time(DEFAULTER)\n");
                    }
                    else{
                        printf("already borrowed book. Moving the request to lowest priority\n");
                        requestNode* newNode=createRequestNode(ptr2->name,ptr2->bookTitle);
                        status_code val=insertRequest(&newQueue,newNode);//moving the request into new queue for later proccessing
                    }
                }
                else if(cnt1>=3){
                    printf("FAILURE: Already 3 books borrowed\n");
                }
                requestNode* toDelete=ptr2;//current node is proccessed so delete it
                ptr2=ptr2->next;
                myQueue->front=deleteAfter(prev,toDelete,myQueue->front);
            }
            
        }
        ptr2=newQueue.front;
        if(ptr2!=NULL){
            printf("\nProcesssing request of students who were shifted to lowest priority\n");
            while(ptr2!=NULL){
                printf("%s-----",ptr2->name);
                int cnt1=countBorrowedBooks(myBorrowList->front,ptr2->name);
                int cnt2;
                countAvailableCopies(root,ptr2->bookTitle,&cnt2);
                if(cnt1==0){
                    requestNode* newNode=createRequestNode(ptr2->name,ptr2->bookTitle);
                    status_code ret_val=insertRequest(myQueue,newNode);//insert request to original queue bcoz in future it might be available
                    printf("PENDING: All copies are issued. Book is not avilable\n");
                }
                else{
                    if(cnt1<3){
                        printf("SUCCESS\n");
                        borrowNode* newNode=createBorrowNode(ptr2->name,ptr2->bookTitle,CURRENT_DATE,-1);
                        status_code val= insertBorrowNode(myBorrowList,newNode);
                        decrementBook(root,ptr2->bookTitle);
                    }
                    else{
                        printf("FAILURE: Already 3 books borrowed\n");
                    }
                    
                }
                ptr2=ptr2->next;
            }
        }
        if(myQueue->front==NULL){
            initializeQueue(myQueue);
        }
    }
}

int countRequestBooks(requestNode* queueHead,char title[30]){
    int cnt=0;
    requestNode* ptr2=queueHead;
    while(ptr2!=NULL){
            if(strcmp(ptr2->bookTitle,title)==0){
                cnt++;
            }
            ptr2=ptr2->next;
    }
    return cnt;
}
void bookInDemand(struct btreeNode* myNode,int* mx,requestNode* queueHead,bool check){
    int i;
    if (myNode) {
            for (i = 0; i < myNode->count; i++) {
                bookInDemand(myNode->link[i],mx,queueHead,check);
                int cnt1=countRequestBooks(queueHead,myNode->val[i+1]->title);
                int cnt2=myNode->val[i+1]->copies_issued;
                if(cnt1+cnt2>*mx && check==TRUE){//check is true and max is less than count
                    *mx=cnt1+cnt2;//update max
                }
                else if(check==FALSE && cnt1+cnt2==(*mx)){//check is false and cnt is equal to max
                    printf("%s\n",myNode->val[i+1]->title);//print book title
                }
            }
           bookInDemand(myNode->link[i],mx,queueHead,check);
    }
}
void q2(struct btreeNode* root,requestNode* queueHead){
    int mx=-1;
    bool check=1;//check is set to true
    bookInDemand(root,&mx,queueHead,check);//function will update max value i.e. 
    check=0;
    printf("Book/Books in most demand\n");
    bookInDemand(root,&mx,queueHead,check);
}

void countBooksQ3(struct btreeNode* myNode,borrowNode* borrowHead,int stDay,int enDay){
    int i;
    if (myNode) {
            for (i = 0; i < myNode->count; i++) {
                countBooksQ3(myNode->link[i],borrowHead,stDay,enDay);
                int tot=myNode->val[i+1]->copies_available+myNode->val[i+1]->copies_issued;
                borrowNode* ptr=borrowHead;
                while(ptr!=NULL){
                    if(strcmp(ptr->title,myNode->val[i+1]->title)==0){
                        if(ptr->issueDate<stDay && ptr->returnDate==-1){//book was borrowed but not returned
                            tot--;
                        }
                        else if(ptr->issueDate<stDay && ptr->returnDate>enDay){
                            tot--;
                        }
                    }
                    ptr=ptr->next;
                }
                printf("%d books of title-%s can be given\n", tot,myNode->val[i+1]->title);
            }
            countBooksQ3(myNode->link[i],borrowHead,stDay,enDay);
    }
}
void q3(struct btreeNode* myNode,borrowNode* borrowHead){
    int stDay;
    printf("\nEnter starting day(must be between 1 to 27)\n");
    scanf("%d", &stDay);
    int enDay=stDay+3;
    printf("Books which can be given from date %d to date %d\n", stDay,enDay);
    countBooksQ3(myNode,borrowHead,stDay,enDay);
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
int count(requestNode* queueHead,char name[30]){
    requestNode* ptr=queueHead;
    int count=1;
    while(ptr!=NULL){
        if(strcmp(ptr->name,name)==0){
            count++;
        }
        ptr=ptr->next;
    }
    return count;    
}
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
void q4(borrowNode* borrowHead){
    freqNode* head=NULL;
    borrowNode* ptr=borrowHead;
    if(ptr==NULL){
        printf("Borrower list is empty\n");
    }
    else{
        char str[30];
        str[0]='\0';
        while(ptr!=NULL){
            int cnt=countBorrowedBooks(borrowHead,ptr->name);
            if(isPresent(head,ptr->name)==FALSE && cnt!=0){
                head=insertFreqNode(head,createFreqNode(ptr->name,cnt,str,str));
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

void createList(struct btreeNode* myNode, freqNode** head,char sub[30]){
    int i;
    if (myNode) {
        char str[30];
        str[0]='\0';
            for (i = 0; i < myNode->count; i++) {
                    createList(myNode->link[i],head,sub);
                    if(strcmp(myNode->val[i+1]->subject,sub)==0){
                        int cnt=myNode->val[i+1]->copies_available;
                        char title[30],author[30];
                        //printf("found\n");
                        strcpy(title,myNode->val[i+1]->title);
                        strcpy(author,myNode->val[i+1]->author);

                        freqNode* tempHead=*head;
                        tempHead=insertFreqNode(tempHead,createFreqNode(str,cnt,title,author));
                        *head=tempHead;
                    }
            }
            createList(myNode->link[i],head,sub);
    }

}
void q5(struct btreeNode* myNode ){
    char sub[30];
    printf("\nEnter the subject of books\n");
    scanf("%s", sub);
    freqNode* head=NULL;
    if(myNode==NULL){
        printf("Book List is empty\n");
    }
    else{
        createList(myNode,&head,sub);
        head=MergeSortF(head);
        freqNode* ptr=head;
        if(head==NULL){
            printf("No book of subject %s\n",sub);
        }
        else{
            printf("\nBooks of subject %s based on copies availbale\n\n",sub);
            while(ptr!=NULL){
                printf("Title:%s\tAuthor:%s\tCopies Available:%d\n", ptr->title,ptr->author,ptr->count);
                ptr=ptr->next;
            }
            printf("\n");
        }
        
    }
}

void q6(struct btreeNode* myNode,requestNode* queueHead){
    requestNode* ptr=queueHead;
    if(ptr==NULL){
        printf("\nRequest queue is empty\n");
    }
    else{
        printf("\nName(title) of requested books whose copies are available\n");
        while(ptr!=NULL){
            int cnt;
            countAvailableCopies(myNode,ptr->bookTitle,&cnt);
            if(cnt>0){
                printf("%s\n",ptr->bookTitle);
            }
            ptr=ptr->next;
        }
    }
    
}

void bookissued0(struct btreeNode* myNode){
    int i;
    if (myNode) {
            for (i = 0; i < myNode->count; i++) {
                    bookissued0(myNode->link[i]);
                    if(myNode->val[i+1]->copies_issued==0){
                        printf("%s\n",myNode->val[i+1]->title);
                    }
            }
            bookissued0(myNode->link[i]);
    }
}
void q7(struct btreeNode* myNode){
    printf("\nTitle of books which have not been issued by anyone\n");
    bookissued0(myNode);
}

void idRange(struct btreeNode* myNode,int lo,int hi){
    int i;
    if (myNode) {
            for (i = 0; i < myNode->count; i++) {
                    idRange(myNode->link[i],lo,hi);
                    if(lo<=myNode->val[i+1]->id && myNode->val[i+1]->id<=hi){
                        printf("ID:%d\tTitle:%s\n",myNode->val[i+1]->id, myNode->val[i+1]->title);
                    }
            }
            idRange(myNode->link[i],lo,hi);
    }
}
void q8(struct btreeNode* myNode){
    int From_Book_ID,To_Book_ID;
    printf("Enter starting id:\n");
    scanf("%d", &From_Book_ID);
    printf("Enter ending id:\n");
    scanf("%d", &To_Book_ID);
    printf("Books with their id in range %d to %d\n",From_Book_ID,To_Book_ID);
    idRange(myNode,From_Book_ID,To_Book_ID);
}

void questions(requestQueue* myQueue,borrowList* myBorrowList){
    int opt;
    printf("\nPress\n1 for q1\n2 for q2\n3 for q3\n4 for q4\n");
    printf("5 for q5\n6 for q6\n7 for q7\n8 for q8\n");
    scanf("%d", &opt);
    if(opt==1){
        q1(root,myQueue,myBorrowList);
    }
    else if(opt==2){  
        q2(root,myQueue->front);
    }
    else if(opt==3){
        q3(root,myBorrowList->front);
    }
    else if(opt==4){
        q4(myBorrowList->front);
    }
    else if(opt==5){
        q5(root);
    }
    else if(opt==6){
        q6(root,myQueue->front);
    }
    else if(opt==7){
        q7(root);
    }
    else if(opt==8){
        q8(root);
    }
    else{
        printf("INVALID OPTION\n");
    }
}

int main(){
    initializeBookList();
    
    requestQueue myQueue; 
    initializeQueue(&myQueue); 
    
    borrowList myBorrowList; 
    initializeBorrowList(&myBorrowList); 
    
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
            traversal(root);
        }
        else if(opt==3){
            createRequestQueue(&myQueue);
        }
        else if(opt==4){
            printRequestQueue(myQueue.front);
        }
        else if(opt==5){
            returnBook(root,myBorrowList);
        }
        else if(opt==6){
            printBorrowList(myBorrowList.front);
        }
        else if(opt==7){
            questions(&myQueue,&myBorrowList);
        }
        else{
            printf("Invalid option\nPress again\n");
        }
    }
    return 0;
}
