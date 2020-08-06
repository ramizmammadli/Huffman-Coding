#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct n{
	char letter;
	int frequency;
	struct n* next;
	struct n* left;
	struct n* right;
};

typedef struct n node;


void printList(node* tmp);
void SortFunc(node **head_ref);
void sortedInsertion(node** head_ref, node* new_node);
void frontIns(node** head, char data);
void printLevel(node * root, int level);
int HeightOfTree(node* head);
void printTree(node * root);

//this function prints the list. head is sent from the main function.
void printList(node* tmp){
	int i;
	while (tmp != NULL){
		printf("%c - ", tmp->letter);
		printf("%d\n", tmp->frequency);
		tmp = tmp->next;
	}
}

//this function inserts the new node to the very begininning of the list. the reference of head is sent from main
void frontIns(node** head, char data) {
	node* newNode = (node*)malloc(sizeof(node)); 
	newNode->frequency = 1;
	newNode->letter = data;
	newNode->next = *head; 						//newNode->next shows the reference of head.
	*head = newNode; 							//new head is newNode now.
}

//Insertion sort functions start here
void insertIn(node * head, node * place, node * element){
	node * tmp;
	tmp = head;
	while(tmp->next != element){
		tmp = tmp->next;
	}
	tmp->next = element->next;
	tmp = head;
	while(tmp->next != place){
		tmp = tmp->next;
	}
	tmp->next = element;
	element->next = place;
}

node * insertFront(node * head, node * element){
	node * tmp;
	tmp = head;
	while(tmp->next != element){
		tmp = tmp->next;
	}
	tmp->next = element->next;
	element->next = head;
	head = element;
	return head;
}

//function is called from main and initiates insertion sort
node * sortList(node * head){
	node *tmp, *tmp2;
	tmp = head;			
	tmp2 = tmp->next;	//tmp first, tmp2 second node.
	while(tmp2 != NULL){//finds the place to insert the node
						
		while(tmp->next != tmp2 && tmp->frequency <= tmp2->frequency){
			tmp = tmp->next;
		}
		if(tmp->frequency > tmp2->frequency){
			if(tmp == head)
				head = insertFront(head, tmp2);	//if tmp is the first node, node is inserted front with insertFront function
			else
				insertIn(head, tmp, tmp2);		//otherwise, insertIn inserts the node after tmp
		}
		tmp = head;
		tmp2 = tmp2->next;
	}
	return head;
}


void printTree(node * root){		//prints the tree 
    int i, h;
	h = HeightOfTree(root) + 1;		//HeightOfTree returns one less than the height of tree, so 1 must be added to it
    for (i=1; i<=h; i++)
    {
        printLevel(root, i);		// for is looped for h times and printLevel is called in order to print the tree level by level			
        printf("\n");
    }
}
int usedLetter[100000];
void printLevel(node * root, int level){
    if (root->letter != NULL){			
        if(!usedLetter[root->letter]){		//this "if" prevents overprinting in the console by declared usedLetter global variable
            printf("%d", root->frequency);
            printf("%c", root->letter);
            printf(" ");
            usedLetter[root->letter] = 1;
        }
		return;
	}
    if (level == 1){				//additional  case for preventing underprinting. level = 1 must be additionally checked
    	printf("%d", root->frequency);
		printf(" ");
	}
    else if (level > 1) {		//these recursions printing both left and right parts of tree by decreasing the level by 1 and looping
        printLevel(root->left, level-1);
        printLevel(root->right, level-1);
    }
}


int HeightOfTree(node * root){		//finds Height of tree. it is used in the printing function of tree (printTree)
    int left, right;
    if(root->letter != NULL)		
        return 0;
	if (root == NULL)
        return 0;
    else {						//have recursions to both left and right leaves. And takes the maximum one as a height
        left = HeightOfTree(root->left);		
        right = HeightOfTree(root->right);
        if (left >= right)
            return left + 1;		//adds 1 because of the head of tree
        else
            return right + 1;				
    }
}


int main(void){
	char *str;
	int i=1, count=0;
	node* head;
	node* tmp;
	node* sum;
	node* tmp2;
	node* prev;
	node* head2;

	head = (node*)malloc(sizeof(node));
	head->next = NULL;
	head->right = NULL;
	head->left = NULL;
    node* zozo = head;
	tmp = head;
	str = (char*) malloc(200 * sizeof(char)); // 100 char-sized memory is allocated in the memory

	printf("Enter the text:\n");
	gets(str);					

	tmp->letter = *str; //first letter of string is put to the first node of linked list
	tmp->frequency = 1;	

	while (str[i] != '\0'){ 								// i starts from 1 because 1st node is used before this loop
															//briefly, this nested loops puts the string chars to linked list respectively
		while ( tmp != NULL && tmp->letter != str[i] ){  

			tmp = tmp->next;								   

		}

		if (tmp == NULL){ 							// if it's not the end of list, there's no this kind of char in the list, so new one will be added	
           
		    node* newNode = (node*)malloc(sizeof(node)); //allocating the nodememory
            newNode->frequency = 1;
            newNode->letter = str[i];
            newNode->next = head; 						//always inserting the new char to the beginning and making it "head"
            head = newNode;

		}

		else {											//if (tmp != NULL), it means char exist in the list
			tmp->frequency++;
		}

		tmp = head;
		i++;
	}

	printf("list before sorting\n");
	printList(head);					//printing the result before sorting


	head = sortList(head);				//sorting happens here
	
	printf("after sorting\n");			//printing after it
	printList(head);


	while (head->next->next != NULL && head->next != NULL){	//this loop is for turning linked list to tree

		tmp = head;				
		tmp2 = head->next;
		head = tmp2->next;
		tmp->next = NULL;
		tmp2->next = NULL;
		head2 = head;				
										//tmp is made 1st, tmp2 is made second node. head is shifted to 3rd, because every time first 2 nodes must be disconnected. 
		sum = (node*)malloc(sizeof(node));
		sum->letter = NULL;
		sum->frequency = tmp->frequency + tmp2->frequency;
		sum->left = tmp;
		sum->right = tmp2;					// sum is new node which is the head of new tree. sum->frequency equals to the sum of 1st and 2nd nodes' frequencies
								
		if (head->frequency > sum->frequency){		//if sum->frequency is the smallest among other frequencies in the list, add it to the beginning and make it head
			sum->next = head;					
			head = sum;
		}

		else {							//otherwise, find the node which has greater frequency than that of sum and add before it.
            head2 = head;
			while (head2->next != NULL && head2->next->frequency <= sum->frequency ){
				head2 = head2->next;
			}

			sum->next = head2->next;
			head2->next = sum;
		}
	}

	if (head->next->next == NULL){			// loop finishes. extra check for 2 remained nodes. If there are any 2 remained nodes, unite them and finish treemaking.
		sum = (node*)malloc(sizeof(node));
		sum->frequency = head->frequency + head->next->frequency;
		sum->next = NULL;
		sum->letter = NULL;
		sum->left = head;
		sum->right = head->next;
		head = sum;
	}
    printTree(head);			// prints the tree 
	return 0;
}