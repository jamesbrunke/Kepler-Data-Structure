//
//  main.cpp
//  Kepler
//
//  Created by James Brunke on 2019-10-12.
//  Copyright © 2019 James Brunke. All rights reserved.
//
//*************************************************************************
//ASSUMPTION:
//start values will be lower than the end value.
//ie user will not enter Add(5,-5)
//**************************************************************************

#include <iostream>
using namespace std;

  
// Structure to represent a range
struct Range
{
    int low, high;
};
  
// Structure to represent a node in Range Search Tree
struct RTNode
{
    Range *i;  // 'i' could also be a normal variable
    int max;
    RTNode *left, *right;
};

//FUNCTION PROTOTYPES
RTNode *Delete(RTNode *root, Range i);
RTNode * newNode(Range i);
bool doOverlap(Range i1, Range i2);
Range *overlapSearch(RTNode *root, Range i);
Range *FindAll(RTNode *root,Range i,Range *result, int *index);
bool isRange(RTNode *root,Range i);
RTNode *Add(RTNode *root, Range i);
void inorder(RTNode *root);
void adjnode(RTNode *root, Range i);
RTNode *findmin(RTNode *root);
RTNode *copydata(RTNode *root, RTNode *toCopy);
RTNode *rmnode(RTNode *root);
RTNode *Delete(RTNode *root, Range i);
Range *Get(RTNode *root, Range i);
  
// A utility function to create a new Range Search Tree Node
RTNode * newNode(Range i)
{
    RTNode *temp = new RTNode;
    temp->i = new Range(i);
    temp->max = i.high;
    temp->left = temp->right = NULL;
    return temp;
};

// A utility function to check if given two ranges overlap
bool doOverlap(Range i1, Range i2)
{
    if ((i1.low < i2.high && i1.high > i2.low))
        return true;
    return false;
}
  
// The main function that searches a given range i in a given
// Range Tree.
Range *overlapSearch(RTNode *root, Range i)
{
    // Base Case, tree is empty
    if (root == NULL) return NULL;
  
    // If given range overlaps with root
    if (doOverlap(*(root->i), i))
        return root->i;
  
    // If left child of root is present and max of left child is
    // greater than or equal to given range, then i may
    // overlap with a range in left subtree
    if (root->left != NULL && root->left->max > i.low)
        return overlapSearch(root->left, i);
  
    // Else range can only overlap with right subtree
    return overlapSearch(root->right, i);
}

//return all ranges in the structure root which are overlapped by range i.
Range *FindAll(RTNode *root,Range i,Range *result, int *index)
{
    
    
    //if we end up at a NULL branch of the interval tree
    //and we checked the right side branches, stop and return.
    if (root==NULL)
        return result;
    
    if (doOverlap(*(root->i), i))
    {
        result[*index]= *(root->i);
        (*index)++;
    }
    
    //left child exists and may also be overlapped
    if(root->left != NULL && i.low < root->left->max)
    {
        if(root->right != NULL && i.high > root->right->i->low) //right tree is also overlapped: hold root
        {
            result = FindAll(root->right,i,result,index);
        }
        return FindAll(root->left,i,result,index);
    }
    
    else
        return FindAll(root->right,i,result,index);
    
}

//see if range exists in the tree
//used for finding bounds for the pointer array returned by findall
bool isRange(RTNode *root,Range i)
{
    // Base Case, tree is empty
    if (root == NULL) return false;
    
    if(root->i->low == i.low && root->i->high == i.high) return true;
    
     
       // If left child of root is present and max of left child is
       // greater than or equal to given range, then i may
       // overlap with a range in left subtree
    if (root->left != NULL && i.low < root->left->max)
            return isRange(root->left, i);
        
     
       // Else range can only overlap with right subtree
    return isRange(root->right, i);
}


// Similar to BST insert but we use the low value to maintain BST propriety/order.
// usage: O(log(n))

RTNode *Add(RTNode *root, Range i)
{
    // Base case: Tree is empty, new node becomes root
    if (root == NULL)
        return newNode(i);
    
    // FindAll low and high values of range at root
    int l = root->i->low;
    int h = root->i->high;
  
    
    //check if the new range is a subset of an existing range
    if (i.low >= l && i.high <= h)
    {

        return root; //return the root of the unchanged Range Tree
    }
    
    //otherwise, check if the range is overlapping existing ranges
    //case 1: old range is entirely engulfed by new range
    //must check both left and right subtree to see if it overlaps those ranges
    if (i.low <= l && i.high >= h)
    {
        Range *temp =overlapSearch(root, i);
        temp->low = i.low;
        temp->high = i.high;
        root->max = i.high;
        i=*temp;
        
        //if left child exists and it's max value is greater than the low value of the range it could overlap this range
        if(root->left != NULL && root->left->max > temp->low)
            return Add(root->left,i);
        
        //otherwise check if it overlaps the right child if that child exists
        else if (root->right != NULL)
            return Add(root->right,i);
        
        //else return the Range tree root
        else
            return root;
        
    }
    
    //case 2: old range overlaps and extends over the upper bound
    //must also check the right subtree to see if it overlaps that range
    if (i.low >= l && i.low < h && i.high >= h)
    {
        Range *temp = overlapSearch(root,i);
        temp->high = i.high;
        i=*temp;
        if (root->right != NULL)
            return Add(root->right,i);
        else
            return Add(root,i);
        
        
    }
    
    // case 3: old range overlaps and extends over the lower bound
    // must also check the left subtree to see if it overlaps that range
    else if (i.low <= l && i.high > l && i.high <= h)
    {
        Range *temp = overlapSearch(root,i);
        temp->low = i.low;
        i=*temp;
        if (root->left != NULL)
            return Add(root->left,i);
        else
            return Add(root,i);
        
    }
    
    // If root's low value is smaller, then new range goes to
    // left subtree
    if (i.low < l)
        root->left = Add(root->left, i);
  
    // Else, new node goes to right subtree.
    else
        root->right = Add(root->right, i);
  
    // Update the max value of this ancestor if needed
    //if (root->max < i.high)
      //  root->max = i.high;
  
    return root;
}

  

  
void inorder(RTNode *root)
{
    if (root == NULL)
    {
        return;
    }
    
    inorder(root->left);
  
    cout << "(" << root->i->low << ", " << root->i->high << ")"
         << " max = " << root->max << endl;
  
    inorder(root->right);
}

 

//function when Delete only needs to adjust node->range values instead of delete node or split node.
void adjnode(RTNode *root, Range i)
{
    if (root == NULL)
    {
        //no ranges currently exist
        return;// NULL;
    }
    
    //Find high and low values of the range
    int l=root->i->low;
    int h=root->i->high;
    
    //Base case:
    //range to be deleted doesn't exist.
    //return the root unchanged.
    if (overlapSearch(root,i)==NULL)
    {
        cout<<"The range to be deleted doesnt exist.\n";
        return ;//NULL;
    }
    
    
    if (i.low > h)
        return adjnode(root->right,i);
    else if (i.high < l)
        return adjnode(root->left,i);
    
 
    //case 1:
    //range to be deleted partially overlaps and extends over the upper bound
    else if (i.low >= l && i.low <= h && i.high > h)
    {
        root->i->high=i.low; //changing upper bound
        root->max=i.low; //track max value
        
        if (i.high > h && root->right != NULL)
            {
                return adjnode(root->right,i);
            }
        
        else if (i.high >= l && i.high < h) //could just be else
            {
                root->i->low = i.high; //changing lower bound
                return adjnode(root,i);
            }
        
    }
    
        
    //case 2:
    //range to be deleted partially overlaps and extends below the lower bound
    else if (i.low < l && i.high < h && i.high > l)
    {
        
        root->i->low=i.high; //changing lower bound
        
        if (i.low < l && root->left != NULL)
            {
                return adjnode(root->left,i);
            }
    
        else if (i.low >= l && i.low < h) //could be just else
            {
                root->i->high = i.low; //changing lower bound
                return adjnode(root,i);
            }
        
    }
    
    return;
}

RTNode *findmin(RTNode *root) //find the node with the minimum value of range.
{
    if (root->left != NULL)
    {
        return findmin(root->left);
    }
    else return root;
}

RTNode *copydata(RTNode *root, RTNode *toCopy)
{
    root->i->low=toCopy->i->low;
    root->i->high=toCopy->i->high;
    root->max=toCopy->max;
    //root->left=toCopy->left;
    //root->right=toCopy->right;
    return root;
}

RTNode *rmnode(RTNode *root) //delete the passed node but fix all links
{
   
    if (root->left == NULL && root->right == NULL) //no child
    {
        delete root;
        root = NULL;
    }
    else if (root->right != NULL && root->left == NULL)//one right child
    {
        RTNode *temp = root;
        root=root->right;
        delete temp;
       
    }
    else if (root->left !=NULL && root->right == NULL)//one left child
    {
        RTNode *temp = root;
        root=root->left;
        delete temp;
       
    }
    else //two children
    {
        RTNode *temp = findmin(root->right);//min value range node in temp
        root=copydata(root,temp);
        root->right = Delete(root->right,*(temp->i));
      
    }
    
    return root;
}

//Usage: log(n) pretty rough.
RTNode *Delete(RTNode *root, Range i)
{
    if (root == NULL)
      {
          return root;
      }
      
      //Find high and low values of the range
      int l=root->i->low;
      int h=root->i->high;
    
    //Base Case:
    if (overlapSearch(root,i)==NULL)
       {
           cout<<"Finished Deleting.\n";
           return root;//NULL;
       }
    
    if (i.low >= l && i.low <= h && i.high > h) //only need to adjust range values of node
    {
        adjnode(root,i);
        return Delete(root,i);
    }
    
    else if (i.low < l && i.high < h && i.high > l) //only need to adjust range values of node
    {
        adjnode(root,i);
        return Delete(root,i);
    }
    
    
     //case 1:
     //range to be deleted is contained in an existing range
     //adjust the range of the node and re-Add the other truncated part of
     //the range.
     if (i.low >= l && i.high <= h)
     {
         Range goober = *(root->i); // hard copy of range
         root->i->low = i.high; // change the lower bound
         goober.high = i.low; //change the upper bound of copied range
         return Add(root,goober);
     }
    
    //case 2:
    // range to be deleted completely overlaps an existing range.
    else
    {
        // need to check if the range to be deleted overlaps the parent root
        if (i.low <= l && i.high >= h)
        {
            root = rmnode(root);
            return Delete(root,i);
        }
        
        if(root->left != NULL && i.low < root->left->i->high)
            root->left = Delete(root->left,i);
        else if(root->right != NULL && i.high > root->right->i->low)
            root->right = Delete(root->right,i);
        
    }
    
    return root;
    
}




Range *Get(RTNode *root, Range i)
{
    Range *result = new Range; //cannot initialize result as nullptr
    int *index = new int; //pointer to an index so it can be manipulated recursively
     
     cout<<"\nSearching for range ["<<i.low<<","<<i.high<<"]\n";
    
     result=FindAll(root,i,result,index);
     
     if (!isRange(root,result[0]))
         cout<<"\nno overlapped ranges\n";
     else
     {
         cout<<"\nThe Overlapped Ranges are: ";
         int j=0;
         do
         {
            cout << "\n[" << result[j].low << ", " << result[j].high << "]";
             j++;
         }
         while(isRange(root,result[j])); //make sure the next element being accessed is an actual range in the Range tree otherwise we're outside the bounds.
         //this is super bad and increase the time complexity by stupid amounts
         //learn to use the vector library in STD becasue it would allow
         //the array size to be calculated at runtime whereas right now you
         //can only determine the array size at compile time and this information will not
         //allow you to calculate the size of the array at run-time so you need to do this hacky stuff.
         
         //alternatively, you coulda when you end up at a NULL in the findall function you could append some range that signals the end of the array maybe like [-1,-1], when you find this range you'd know you're at the end of the array. This is still non-ideal cause you might need some of these ranges but it would still be better than this nonsense.
     }
    
    return result;
}







// Driver program to test above functions
int main()
{
    // create the Range tree by inserting the following ranges...
    Range ints[] = {{1, 3}, {6,10},{20,30}
    };
    
    int n = sizeof(ints)/sizeof(ints[0]);
    RTNode *root = NULL;
    for (int i = 0; i < n; i++)
        root = Add(root, ints[i]);
    
    root=Add(root,{-5,2});
    Range rm= {25,28};
    root = Delete(root,rm);

    
    Range x={0,100};
    Range *Overlapped = Get(root,x);
     
    cout << "\n \nIn-order traversal of constructed Range Tree is\n";
    inorder(root);
  
    return 0;
}
