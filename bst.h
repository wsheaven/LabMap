/***********************************************************************
 * Header:
 *    BST
 * Summary:
 *    Our custom implementation of a BST for set and for map
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        BST                 : A class that represents a binary search tree
 *        BST::iterator       : An iterator through BST
 * Author
 *    Sam Heaven, Abram Hansen
 ************************************************************************/

#pragma once

#ifdef DEBUG
#define debug(x) x
#else // !DEBUG
#define debug(x)
#endif // !DEBUG

#include <cassert>
#include <utility>
#include <memory>     // for std::allocator
#include <functional> // for std::less
#include <utility>    // for std::pair

class TestBST; // forward declaration for unit tests
class TestMap;
class TestSet;

namespace custom
{

   template <class TT>
   class set;
   template <class KK, class VV>
   class map;

   /*****************************************************************
    * BINARY SEARCH TREE
    * Create a Binary Search Tree
    *****************************************************************/
   template <typename T>
   class BST
   {
      friend class ::TestBST; // give unit tests access to the privates
      friend class ::TestMap;
      friend class ::TestSet;

      template <class KK, class VV>
      friend class map;

      template <class TT>
      friend class set;

      template <class KK, class VV>
      friend void swap(map<KK, VV>& lhs, map<KK, VV>& rhs);
   public:
      //
      // Construct
      //

      BST();
      BST(const BST& rhs);
      BST(BST&& rhs);
      BST(const std::initializer_list<T>& il);
      ~BST();

      //
      // Assign
      //

      BST& operator = (const BST& rhs);
      BST& operator = (BST&& rhs);
      BST& operator = (const std::initializer_list<T>& il);
      void swap(BST& rhs);

      //
      // Iterator
      //

      class iterator;
      iterator   begin() const noexcept;
      iterator   end()   const noexcept { return iterator(nullptr); }

      //
      // Access
      //

      iterator find(const T& t);


      // 
      // Insert
      //

      std::pair<iterator, bool> insert(const T& t, bool keepUnique = false);
      std::pair<iterator, bool> insert(T&& t, bool keepUnique = false);

      //
      // Remove
      // 

      iterator erase(iterator& it);
      void   clear() noexcept;

      // 
      // Status
      //

      bool   empty() const noexcept { return size() == 0; }
      size_t size()  const noexcept { return numElements; }

      class BNode;

   private:






      void deleteBinaryTree(BNode*& pDelete) noexcept;
      void copyBinaryTree(const BNode* pSrc, BNode*& pDest);
      void deleteNode(BNode*& pDelete, bool toRight);


      BNode* root;              // root node of the binary search tree
      size_t numElements;        // number of elements currently in the tree
   };


   /*****************************************************************
    * BINARY NODE
    * A single node in a binary tree. Note that the node does not know
    * anything about the properties of the tree so no validation can be done.
    *****************************************************************/
   template <typename T>
   class BST <T> ::BNode
   {
   public:
      // 
      // Construct
      //
      BNode() : data(), pLeft(nullptr), pRight(nullptr), pParent(nullptr) {}

      BNode(const T& t) : data(t), pLeft(nullptr), pRight(nullptr), pParent(nullptr) {}

      BNode(T&& t) : data(std::move(t)), pLeft(nullptr), pRight(nullptr), pParent(nullptr) {}

      //
      // Insert
      //
      void addLeft(BNode* pNode);
      void addRight(BNode* pNode);
      void addLeft(const T& t);
      void addRight(const T& t);
      void addLeft(T&& t);
      void addRight(T&& t);

      // 
      // Status
      //
      bool isRightChild(BNode* pNode) const { return pRight == pNode; }
      bool isLeftChild(BNode* pNode) const { return pLeft == pNode; }

      //
      // Data
      //
      T data;                  // Actual data stored in the BNode
      BNode* pLeft;          // Left child - smaller
      BNode* pRight;         // Right child - larger
      BNode* pParent;        // Parent
      bool isRed;              // Red-black balancing stuff
   };

   /**********************************************************
    * BINARY SEARCH TREE ITERATOR
    * Forward and reverse iterator through a BST
    *********************************************************/
   template <typename T>
   class BST <T> ::iterator
   {
      friend class ::TestBST; // give unit tests access to the privates
      friend class ::TestMap;
      friend class ::TestSet;

      template <class KK, class VV>
      friend class map;

      template <class TT>
      friend class set;
   public:
      // constructors and assignment
      iterator(BNode* p = nullptr)
      {
         pNode = p;
      }
      iterator(const iterator& rhs)
      {
         pNode = rhs.pNode;
      }
      iterator& operator = (const iterator& rhs)
      {
         pNode = rhs.pNode;
         return *this;
      }

      // compare
      bool operator == (const iterator& rhs) const
      {
         return rhs.pNode == pNode;
      }
      bool operator != (const iterator& rhs) const
      {
         return rhs.pNode != pNode;
      }

      // de-reference. Cannot change because it will invalidate the BST
      const T& operator * () const
      {
         return pNode->data;
      }

      // increment and decrement
      iterator& operator ++ ();
      iterator   operator ++ (int postfix)
      {
         iterator itReturn = *this;
         ++(*this);
         return itReturn;
      }
      iterator& operator -- ();
      iterator   operator -- (int postfix)
      {
         iterator itReturn = *this;
         --(*this);
         return itReturn;
      }

      // must give friend status to remove so it can call getNode() from it
      friend BST <T> ::iterator BST <T> ::erase(iterator& it);

   private:

      // the node
      BNode* pNode;
   };


   /*********************************************
    *********************************************
    *********************************************
    ******************** BST ********************
    *********************************************
    *********************************************
    *********************************************/


    /*********************************************
     * BST :: DEFAULT CONSTRUCTOR
     ********************************************/
   template <typename T>
   BST <T> ::BST() : root(nullptr), numElements(0)
   {
   }

   /*********************************************
    * BST :: COPY CONSTRUCTOR
    * Copy one tree to another
    ********************************************/
   template <typename T>
   BST <T> ::BST(const BST<T>& rhs) : root(nullptr), numElements(0)
   {
      *this = rhs;
   }

   /*********************************************
    * BST :: MOVE CONSTRUCTOR
    * Move one tree to another
    ********************************************/
   template <typename T>
   BST <T> ::BST(BST <T>&& rhs) : root(nullptr), numElements(0)
   {
      root = rhs.root;
      rhs.root = nullptr;

      numElements = rhs.numElements;
      rhs.numElements = 0;
   }

   /*********************************************
    * BST :: INITIALIZER LIST CONSTRUCTOR
    * Create a BST from an initializer list
    ********************************************/
   template <typename T>
   BST <T> ::BST(const std::initializer_list<T>& il)
   {
      numElements = 0;
      root = nullptr;
      *this = il;
   }

   /*********************************************
    * BST :: DESTRUCTOR
    ********************************************/
   template <typename T>
   BST <T> :: ~BST()
   {
      clear();
   }


   /*********************************************
    * BST :: ASSIGNMENT OPERATOR
    * Copy one tree to another
    ********************************************/
   template <typename T>
   BST <T>& BST <T> :: operator = (const BST <T>& rhs)
   {
      copyBinaryTree(rhs.root, this->root);
      this->numElements = rhs.numElements;

      return *this;
   }

   /*********************************************
    * BST :: ASSIGNMENT OPERATOR with INITIALIZATION LIST
    * Copy nodes onto a BTree
    ********************************************/
   template <typename T>
   BST <T>& BST <T> :: operator = (const std::initializer_list<T>& il)
   {

      deleteBinaryTree(root);
      numElements = 0;

      for (auto&& element : il)
      {
         insert(element);
      }
      return *this;
   }

   /*********************************************
    * BST :: ASSIGN-MOVE OPERATOR
    * Move one tree to another
    ********************************************/
   template <typename T>
   BST <T>& BST <T> :: operator = (BST <T>&& rhs)
   {

      clear();
      swap(rhs);

      return *this;
   }

   /*********************************************
    * BST :: SWAP
    * Swap two trees
    ********************************************/
   template <typename T>
   void BST <T> ::swap(BST <T>& rhs)
   {
      std::swap(rhs.root, root);
      std::swap(rhs.numElements, numElements);
   }

   /*****************************************************
    * BST :: INSERT
    * Insert a node at a given location in the tree
    ****************************************************/
   template <typename T>
   std::pair<typename BST <T> ::iterator, bool> BST <T> ::insert(const T& t, bool keepUnique)
   {

      std::pair<iterator, bool> pairReturn(end(), false);

      try
      {
         if (root == nullptr)
         {
            assert(numElements == 0);
            root = new BNode(t);
            numElements = 1;
            pairReturn.first = iterator(root);
            pairReturn.second = true;
            return pairReturn;
         }

         BNode* node = root;
         bool done = false;
         while (!done)
         {
            if (keepUnique && t == node->data)
            {
               pairReturn.first = iterator(node);
               pairReturn.second = false;
               return pairReturn;
            }

            if (t < node->data)
            {
               if (node->pLeft)
               {
                  node = node->pLeft;
               }
               else
               {
                  node->addLeft(t);
                  done = true;
                  pairReturn.first = iterator(node->pLeft);
                  pairReturn.second = true;
               }
            }
            else
            {
               if (node->pRight)
               {
                  node = node->pRight;
               }
               else
               {
                  node->addRight(t);
                  done = true;
                  pairReturn.first = iterator(node->pRight);
                  pairReturn.second = true;
               }
            }

         }
         assert(root != nullptr);
         numElements++;

         while (root->pParent != nullptr)
         {
            root = root->pParent;
         }

      }
      catch (const std::exception&)
      {
         throw "Error: Unable to allocate a node";
      }
      return pairReturn;
   }



   template <typename T>
   std::pair<typename BST <T> ::iterator, bool> BST <T> ::insert(T&& t, bool keepUnique)
   {
      std::pair<iterator, bool> pairReturn(end(), false);

      try
      {
         if (root == nullptr)
         {
            assert(numElements == 0);
            root = new BNode(t);
            numElements = 1;
            pairReturn.first = iterator(root);
            pairReturn.second = true;
            return pairReturn;
         }

         BNode* node = root;
         bool done = false;
         while (!done)
         {
            if (keepUnique && t == node->data)
            {
               pairReturn.first = iterator(node);
               pairReturn.second = false;
               return pairReturn;
            }

            if (t < node->data)
            {
               if (node->pLeft)
               {
                  node = node->pLeft;
               }
               else
               {
                  node->addLeft(t);
                  done = true;
                  pairReturn.first = iterator(node->pLeft);
                  pairReturn.second = true;
               }
            }
            else
            {
               if (node->pRight)
               {
                  node = node->pRight;
               }
               else
               {
                  node->addRight(t);
                  done = true;
                  pairReturn.first = iterator(node->pRight);
                  pairReturn.second = true;
               }
            }

         }
         assert(root != nullptr);
         numElements++;

         while (root->pParent != nullptr)
         {
            root = root->pParent;
         }

      }
      catch (const std::exception&)
      {
         throw "Error: Unable to allocate a node";
      }
      return pairReturn;
   }

   /*************************************************
    * BST :: ERASE
    * Remove a given node as specified by the iterator
    ************************************************/
   template <typename T>
   typename BST <T> ::iterator BST <T> ::erase(iterator& it)
   {
      if (it == end())
      {
         return end();
      }

      iterator itNext = it;
      BNode* pDelete = it.pNode;

      if (pDelete->pLeft == nullptr)
      {
         ++itNext;
         deleteNode(pDelete, true);
      }
      else if (pDelete->pRight == nullptr)
      {
         ++itNext;
         deleteNode(pDelete, false);
      }

      else
      {
         BNode* pIOS = pDelete->pRight;
         while (pIOS->pLeft != nullptr)
         {
            pIOS = pIOS->pLeft;
         }
         pIOS->pLeft = pDelete->pLeft;
         if (pDelete->pLeft)
         {
            pDelete->pLeft->pParent = pIOS;
         }

         if (pDelete->pRight != pIOS)
         {
            if (pIOS->pRight)
            {
               pIOS->pRight->pParent = pIOS->pParent;
            }
            pIOS->pParent->pLeft = pIOS->pRight;

            pIOS->pRight = pDelete->pRight;
            pDelete->pRight->pParent = pIOS;
         }

         pIOS->pParent = pDelete->pParent;
         if (pDelete->pParent && pDelete->pParent->pLeft == pDelete)
         {
            pDelete->pParent->pLeft = pIOS;
         }
         if (pDelete->pParent && pDelete->pParent->pRight == pDelete)
         {
            pDelete->pParent->pRight = pIOS;
         }

         if (root == pDelete)
         {
            root = pIOS;
         }
         itNext = iterator(pIOS);
      }

      numElements--;
      delete pDelete;
      return itNext;
   }

   /*****************************************************
    * BST :: CLEAR
    * Removes all the BNodes from a tree
    ****************************************************/
   template <typename T>
   void BST <T> ::clear() noexcept
   {
      if (root)
      {
         deleteBinaryTree(root);
         numElements = 0;
      }
   }

   /*****************************************************
    * BST :: BEGIN
    * Return the first node (left-most) in a binary search tree
    ****************************************************/
   template <typename T>
   typename BST <T> ::iterator custom::BST <T> ::begin() const noexcept
   {
      if (root == nullptr)
      {
         return end();
      }

      BNode* p = root;
      while (p->pLeft)
      {
         p = p->pLeft;
      }
      return iterator(p);
   }


   /****************************************************
    * BST :: FIND
    * Return the node corresponding to a given value
    ****************************************************/
   template <typename T>
   typename BST <T> ::iterator BST<T> ::find(const T& t)
   {
      for (BNode* p = root; p != nullptr; p = (t < p->data ? p->pLeft : p->pRight))
      {
         if (p->data == t)
         {
            return iterator(p);
         }
      }


      return end();
   }

   /******************************************************
    ******************************************************
    ******************************************************
    *********************** B NODE ***********************
    ******************************************************
    ******************************************************
    ******************************************************/


    /******************************************************
     * BINARY NODE :: ADD LEFT
     * Add a node to the left of the current node
     ******************************************************/
   template <typename T>
   void BST <T> ::BNode::addLeft(BNode* pNode)
   {
      pLeft = pNode;
      if (pNode)
         pNode->pParent = this;
   }

   /******************************************************
    * BINARY NODE :: ADD RIGHT
    * Add a node to the right of the current node
    ******************************************************/
   template <typename T>
   void BST <T> ::BNode::addRight(BNode* pNode)
   {
      pRight = pNode;
      if (pNode)
         pNode->pParent = this;
   }

   /******************************************************
    * BINARY NODE :: ADD LEFT
    * Add a node to the left of the current node
    ******************************************************/
   template <typename T>
   void BST<T> ::BNode::addLeft(const T& t)
   {
      BNode* pNode = new BNode(t);
      addLeft(pNode);
   }

   /******************************************************
    * BINARY NODE :: ADD LEFT
    * Add a node to the left of the current node
    ******************************************************/
   template <typename T>
   void BST<T> ::BNode::addLeft(T&& t)
   {
      BNode* pNode = new BNode(t);
      addRight(pNode);
   }

   /******************************************************
    * BINARY NODE :: ADD RIGHT
    * Add a node to the right of the current node
    ******************************************************/
   template <typename T>
   void BST <T> ::BNode::addRight(const T& t)
   {
      BNode* pNode = new BNode(std::move(t));
      addLeft(pNode);
   }

   /******************************************************
    * BINARY NODE :: ADD RIGHT
    * Add a node to the right of the current node
    ******************************************************/
   template <typename T>
   void BST <T> ::BNode::addRight(T&& t)
   {
      BNode* pNode = new BNode(std::move(t));
      addRight(pNode);
   }



   /*************************************************
    *************************************************
    *************************************************
    ****************** ITERATOR *********************
    *************************************************
    *************************************************
    *************************************************/

    /**************************************************
     * BST ITERATOR :: INCREMENT PREFIX
     * advance by one
     *************************************************/
   template <typename T>
   typename BST <T> ::iterator& BST <T> ::iterator :: operator ++ ()
   {
      if (this->pNode == nullptr)
      {
         return *this;
      }
      BST::BNode* pCurrent = this->pNode;
      if (pCurrent->pRight)
      {
         pCurrent = pCurrent->pRight;
         while (pCurrent->pLeft)
         {
            pCurrent = pCurrent->pLeft;
         }
         this->pNode = pCurrent;
         return *this;

      }

      if (pCurrent->pRight == nullptr && pCurrent->pParent->pLeft == pCurrent)
      {
         pCurrent = pCurrent->pParent;
         this->pNode = pCurrent;
         return *this;
      }

      if (pCurrent->pRight == nullptr && pCurrent->pParent->pRight == pCurrent)
      {
         while (pCurrent->pParent && pCurrent->pParent->pRight == pCurrent)
         {
            pCurrent = pCurrent->pParent;
         }
         pCurrent = pCurrent->pParent;
         this->pNode = pCurrent;
         return *this;
      }
      pCurrent = nullptr;
      this->pNode = pCurrent;
      return *this;

   }


   /**************************************************
    * BST ITERATOR :: DECREMENT PREFIX
    * advance by one
    *************************************************/
   template <typename T>
   typename BST <T> ::iterator& BST <T> ::iterator :: operator -- ()
   {

      if (this->pNode == nullptr)
      {
         return *this;
      }
      BST::BNode* pCurrent = this->pNode;
      if (pCurrent->pLeft)
      {
         pCurrent = pCurrent->pLeft;
         while (pCurrent->pRight)
         {
            pCurrent = pCurrent->pRight;
         }
         this->pNode = pCurrent;
         return *this;

      }

      if (pCurrent->pLeft == nullptr && pCurrent->pParent->pRight == pCurrent)
      {
         pCurrent = pCurrent->pParent;
         this->pNode = pCurrent;
         return *this;
      }

      if (pCurrent->pLeft == nullptr && pCurrent->pParent->pLeft == pCurrent)
      {
         while (pCurrent->pParent && pCurrent->pParent->pLeft == pCurrent)
         {
            pCurrent = pCurrent->pParent;
         }
         pCurrent = pCurrent->pParent;
         this->pNode = pCurrent;
         return *this;
      }
      return *this;

   }


   template <typename T>
   void BST <T> ::deleteNode(BNode*& pDelete, bool toRight)
   {
      BNode* pNext = (toRight ? pDelete->pRight : pDelete->pLeft);

      if (pDelete != root)
      {
         if (pDelete->pParent->pLeft == pDelete)
         {
            pDelete->pParent->pLeft = nullptr;
            pDelete->pParent->addLeft(pNext);
         }
         else
         {
            pDelete->pParent->pRight = nullptr;
            pDelete->pParent->addRight(pNext);
         }
      }

      else
      {
         root = pNext;
         pNext->pParent = nullptr;
      }
   }

   template <typename T>
   void BST <T> ::deleteBinaryTree(BNode*& pDelete) noexcept
   {
      if (pDelete == nullptr)
         return;
      deleteBinaryTree(pDelete->pLeft);
      deleteBinaryTree(pDelete->pRight);

      delete pDelete;
      pDelete = nullptr;
   }

   template <typename T>
   void BST <T> ::copyBinaryTree(const BNode* pSrc, BNode*& pDest)
   {
      if (nullptr == pSrc)
      {
         deleteBinaryTree(pDest);
         return;
      }

      assert(pSrc);

      try
      {
         if (nullptr == pDest)
         {
            pDest = new BST::BNode(pSrc->data);
         }
         else
         {
            pDest->data = pSrc->data;
         }
      }
      catch (...)
      {
         throw "ERROR: Unable to allocate a node";
      }
      assert(pDest != nullptr);

      copyBinaryTree(pSrc->pLeft, pDest->pLeft);
      if (pSrc->pLeft)
      {
         pDest->pLeft->pParent = pDest;
      }

      copyBinaryTree(pSrc->pRight, pDest->pRight);
      if (pSrc->pRight)
      {
         pDest->pRight->pParent = pDest;
      }

   }




} // namespace custom

