#include <stdio.h>
#include <stdlib.h>
#include "bst.h"


typedef struct _Node {
    Node* left;
    Node* right;
    int data;
} Node;

//Helper function to allocate new node
Node * allocateNewNodeHelper(int value){
    Node* newNodePointer;
    newNodePointer = (Node *) malloc(sizeof(Node));
    if(newNodePointer == NULL){ //Check if new pointer is NULL
        exit(-1);
    }
    newNodePointer->left = NULL;
    newNodePointer->right = NULL;
    newNodePointer->data = value;
    return newNodePointer;
};

//Helper function to find right-most node in left subtree
Node * smallestNode(Node * pointerToSearchNode){ //Smallest node is the right-most in the left subtree (we pass in the left tree so just right-most)
    if (pointerToSearchNode == NULL){
        return NULL;
    } else if (pointerToSearchNode->right != NULL) {
        return smallestNode(pointerToSearchNode->right);
    }
    return pointerToSearchNode; //If pointer to right is null, the pointed to value is the smallest
}

//Helper function to search for a value and return the node that points to that value (value can be a left or right child)
Node * searchForPrev(Node* root, int value) {
    if (root == NULL) {
        return NULL;
    } else if (root->data == value) { //Shouldn't happen as this case is dealt with (case where value is the root itself)
        return NULL;
    } else {
        if (root->left->data == value) {
            return root;
        } else if (root->right->data == value) {
            return root;
        } else {
            if (value > root->data) { //Value is greater so go left
                root = root->left;
                searchForPrev(root, value);
            } else if (value < root->data) { //Value is greater so go left
                root = root->right;
                searchForPrev(root, value);
            }
            return root;
        }
    }
}

//Helper function for finding the node that the newly inserted value could be a child of
//Returns null if there is no tree
//Tried to use searchForPrev, but did not want to change it
Node * searchForPrevInsertion(Node* root, int value) {
    if (root == NULL) { //Value
        return NULL;
    }
    Node * returnNodePrev = NULL;

    while (root != NULL){
        if (value > root->data){ //If value is greater we go left
            returnNodePrev = root; //Assign root to prev (this is essentially the potential "parent" node of the value)
            root = root->left; //Go left
        } else if (value < root->data){//If value is smaller we go right
            returnNodePrev = root; //Assign root to prev (this is essentially the potential "parent" node of the value)
            root = root->right; //Go right
        } else{
            returnNodePrev = NULL;
            break;
        }
    }
    return returnNodePrev;
};

Node * addNode(Node * root, int value){
    Node * returnNodePointer; //The value we will eventually return
    Node * prevNode = searchForPrevInsertion(root, value);

    if (root == NULL) { //Making a new tree, just return
        return allocateNewNodeHelper(value);
    } else if (prevNode == NULL){//In case there is a duplicate added, change the return to NULL
        returnNodePointer = prevNode;
    } else {
        prevNode = searchForPrevInsertion(root, value);
        if (value > prevNode->data){ //If value is greater go left
            prevNode->left = allocateNewNodeHelper(value);
            returnNodePointer = prevNode->left;
        } else { //Value is smaller so go right
            prevNode->right = allocateNewNodeHelper(value);
            returnNodePointer = prevNode->right;
        }
    }
    return returnNodePointer;
}

Node * removeNode(Node * root, int value){
    if (root == NULL){ //If root is null, tree is empty & we return null OR a recursive call leads to it not being found ie Value not present
        return root;
    }else if (value > root->data) { //If value is greater go left
        root->left = removeNode(root->left, value); //Using left pointer as new root to recursively delete
        //the pointer to the left part of root is changed
    }else if (value < root->data){ //If value is smaller go right
        root->right = removeNode(root->right, value); //Using right pointer as new root to recursively delete
    }else if (value == root->data){ //We find the value, 3 cases: No children, one child & two children. Deal with them one by one
        //Case 1: No children
        if (root->left == NULL && root->right == NULL){
            free(root); //We free the root since there's no children
            root = NULL; //If we delete a leaf node we return NULL as that leaf is now null
        } //Case 2.1: One child (right or left dealt with separately for simplicity's sake)
        else if (root->left == NULL) { //Left child is null
            Node *toBeRemoved = root;
            root = root->right; //We store right
            free(toBeRemoved);
        } //Cases 2.2: One child
        else if (root->right == NULL){ //Right child is null
            Node * toBeRemoved = root;
            root = root->left; //We store left
            free(toBeRemoved);
        }else { //Case 3: Two Children (Last case)
            Node * smallestVal = smallestNode(root->left); //Find smallest in left subtree (right-most) with helper function
            root->data = smallestVal->data; //replace the root data
            removeNode(root->left, smallestVal->data); //delete the value we replaced the root by
        }
    }
    return root; //Return the root (Can be NULL or an actual pointer
};

//Node * removeNode(Node * root, int value){ //This one works but kinda also doesnt? It's very weird so will try and refactor (see above)
//    if (root == NULL) { //If root is null, tree is empty & we return null (Value not present)
//        return NULL;
//    } else if (value > root->data){ //If value is greater go left
//        if (root->left == NULL){//Value is not present
//            return root;
//        } else { //Otherwise we go left (value could be present)
//            root->left = removeNode(root->left, value); //Using left pointer as new root to recursively delete
//            //the pointer to the left part of root is changed
//        }
//    } else if (value < root->data){ //If value is smaller go right
//        if (root->right == NULL){//Value is not present
//            return root;
//        } else { //Otherwise we go right
//            root->right = removeNode(root->right, value); //Using right pointer as new root to recursively delete
//        }
//    } else { //This part actually deletes the node we're trying to delete
//        //root now holds the node we want to delete
//        //3 cases: No children, one child & two children. Deal with them one by one
//        //Case 1: No Children
//        if (root->left == NULL && root->right == NULL){
//            free(root); //We free the root since there's no children
//            return NULL; //If we delete a leaf node we return NULL as that leaf is now null
//        } else if (root->left == NULL || root->right == NULL) { //Case 2: One Child (Don't need to deal separately as tested for both above)
//            Node * tempChildStorage; //Actually 2 cases since either left is null OR right is null
//            if (root->left == NULL) { //First case that left is null
//                tempChildStorage = root->right; //We store right
//                free(root);
//                return tempChildStorage; //This return is okay because it will recurse upwards
//            } else if (root->right == NULL) { //Second case that right is null (Can be just else but for clarity's sake I left the if)
//                tempChildStorage = root->left; //We store left
//                free(root);
//                return tempChildStorage;
//            }
//        } else { //Case 3: Two Children (Last case)
//            Node * smallestVal = smallestNode(root->left); //Find smallest with helper function
//            root->data = smallestVal->data; //replace the root data
//            removeNode(root->left, smallestVal->data); //delete the value we replaced the root by
//        }
//    }
//    return root; //If tree isn’t empty, but we don't find it, just return the root
//};

void displaySubtree(Node * N){//Display in ascending order (right, data, left), one value per line (Done recursively)
    if (N == NULL){
        return;
    } else {
        displaySubtree(N->right); //Display the right tree
        printf("%d\n", N->data); //Print value and newline
        displaySubtree(N->left); //Display the right tree
    }
};


int numberLeaves(Node * N){
    int returnVal;
    if (N == NULL) { //Base case, no leaves for a null value
        returnVal = 0;
    } else if ((N->left == NULL) && (N->right == NULL)){ //We found a leaf!
        returnVal = 1;
    } else{
        returnVal = numberLeaves(N->left) + numberLeaves(N->right); //Recursively find number of leaves of left and right
    }
    return returnVal;
};

Node * removeSubtree(Node * root, int value){
    if (root == NULL) { //If root is null, tree is empty & we return null (Value not present)
        return NULL;
    } else if (root->right == NULL && root->left == NULL && root->data == value){//We found the node, and it's the root with no children
        removeNode(root, value);
        return NULL;
    } else {
        Node * prev = searchForPrev(root, value);
        if (prev == NULL){ // Value not found
            return root;
        }
        if (prev->left->data == value){
            removeSubtree(prev->left, value);
        } else if (prev->right->data == value){
            removeSubtree(prev->right, value);
        }
    }
    return root;
};


int globalDepthCounter = 0; //We use a global depth counter to keep track as this can be reset to 0 inside nodeDepth
int nodeDepth (Node * R, Node * N) {
    if (R == NULL || N == NULL){ //If either node is null, return -1
        return -1;
    } else if (N->data > R->data) { //Node has greater value than root, we go left

        ++globalDepthCounter; //Increment the depth counter as we go down a level
        nodeDepth(R->left, N); //We go left of the root and recursively call

    } else if (N->data < R->data) { //Node has smaller value than root, we go right
        ++globalDepthCounter; //Increment the depth counter as we go down a level
        nodeDepth(R->right, N); //We go right of the root and recursively call
    } else { //Base case
        int returnVal = globalDepthCounter; //Store depth in a temporary variable
        globalDepthCounter = 0; //Reset it to zero (as it's a global variable)
        return returnVal;
    }
};
