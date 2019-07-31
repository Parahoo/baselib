#pragma once


template <class T>
class ListBASE;
template <class T>
class ListNODE
{
protected:
  ListNODE               *pPrev;
  ListNODE               *pNext;
  T                       t;
  friend class ListBASE<T>;
public:
  ListNODE()
  {
    pPrev = NULL;
    pNext = NULL;
  }

  ListNODE(const T &t1)
  {
    pPrev = NULL;
    pNext = NULL;
    t = t1;
  }

  virtual ~ListNODE() {}

  inline void                   SetT(const T &t1) { t = t1; }
  inline void                   SetT(const T *t1) 
  { 
    if(t1)
      t = *t1; 
  }

  inline T                      &GetT()  { return t; }
  inline const T                &GetT() const { return t; }
  inline T                      *GetpT()  { return &t; }
  inline const T                *GetpT() const { return &t; }

  inline ListNODE               *GetNext() { return pNext; }
  inline ListNODE               *GetPrev() { return pPrev; }

  inline const ListNODE         *GetNext() const { return pNext; }
  inline const ListNODE         *GetPrev() const { return pPrev; }
};


template <class T>
class ListBASE
{
protected:
  ListNODE<T>               *pHead;
  ListNODE<T>               *pTail;
  int                     Count;

  void                    Initialize()
  {
    pHead = NULL;
    pTail = NULL;
    Count = 0;
  }

  static inline ListNODE<T>               *CreateNode()
  {
    return new ListNODE<T>;
  }

  static inline ListNODE<T>               *CreateNode(const T &t)
  {
    return new ListNODE<T>(t);
  }

  static inline ListNODE<T>               *CreateNode(const T *t)
  {
    if (t)
      return new ListNODE<T>(*t);
    else
      return new ListNODE<T>;
  }

  static inline int           GetNodeSize()
  {
    return sizeof(ListNODE<T>);
  }

public:
  ListBASE() 
  { 
    Initialize(); 
  }
  ListBASE(const ListBASE &Src)
  {
    Initialize(); 
    CopyFromList(&Src);
  }
  virtual ~ListBASE() 
  { 
    ClearAll(); 
  }

  ListBASE &operator=(const ListBASE &Src)
  {
    CopyFromList(&Src);
    return *this;
  }

  int                     GetCount() const 
  { 
    return Count; 
  }

  int                    IsEmpty() const 
  { 
    return (Count == 0); 
  }

  ListNODE<T>               *GetHead()   //�ҵ�ͷ���
  { 
    return pHead; 
  }
  const ListNODE<T>         *GetHead() const   //�ҵ�ͷ���
  { 
    return pHead; 
  }

  T                         *GetHeadT()   //�ҵ�ͷ���
  { 
    return pHead->GetpT(); 
  }

  const T                   *GetHeadT() const   //�ҵ�ͷ���
  { 
    return pHead->GetpT(); 
  }

  ListNODE<T>               *GetTail()   //�ҵ�β���
  { 
    return pTail; 
  }
  const ListNODE<T>         *GetTail() const        //�ҵ�β���
  { 
    return pTail; 
  }
  T                         *GetTailT()   //�ҵ�β���
  { 
    return pTail->GetpT(); 
  }
  const T                   *GetTailT() const        //�ҵ�β���
  { 
    return pTail->GetpT(); 
  }
  ListNODE<T>               *GetAt(int Index)
  {
    if (Index < 0 || Index >= Count)
      return NULL;
    ListNODE<T> *Element = pHead;
    for (int i = 0; i < Index; i++)
      Element = Element->pNext;
    return Element;
  }

  const ListNODE<T>         *GetAt(int Index) const                   //�ҵ����ΪIndex�Ľ��
  {
    if (Index < 0 || Index >= Count)
      return NULL;
    ListNODE<T> *Element = pHead;
    for (int i = 0; i < Index; i++)
      Element = Element->pNext;
    return Element;
  }

  T               *GetpTAt(int Index)
  {
    ListNODE<T> *t = GetAt(Index);
    if (t)
      return t->GetpT();
    else
      return NULL;
  }

  const T               *GetpTAt(int Index) const
  {
    const ListNODE<T> *t = GetAt(Index);
    if (t)
      return t->GetpT();
    else
      return NULL;
  }
  T               &GetTAt(int Index)
  {
    ListNODE<T> *t = GetAt(Index);
    if (t)
      return t->GetT();
    else
      throw EXCEPTION_ARRAY_BOUNDS_EXCEEDED;
  }

  const T               &GetTAt(int Index) const
  {
    const ListNODE<T> *t = GetAt(Index);
    if (t)
      return t->GetT();
    else
        throw EXCEPTION_ARRAY_BOUNDS_EXCEEDED;
  }

  int                     FindIndex(const ListNODE<T> *pNode) const        //�ҵ�pNode�ڱ��������
  {
    ListNODE<T> *Element = pHead;
    for (int i = 0; i < Count; i++, Element = Element->pNext)
    {
      if (pNode == Element)
        return i;
    }

    return -1;
  }

  int                     FindIndex(const T *pNode) const        //�ҵ�pNode�ڱ��������
  {
    ListNODE<T> *Element = pHead;
    for (int i = 0; i < Count; i++, Element = Element->pNext)
    {
      if (pNode && (*pNode == Element->GetT()))
        return i;
    }

    return -1;
  }

  int                     FindIndex(const T &pNode) const        //�ҵ�pNode�ڱ��������
  {
    ListNODE<T> *Element = pHead;
    for (int i = 0; i < Count; i++, Element = Element->pNext)
    {
      if (pNode == Element->GetT())
        return i;
    }

    return -1;
  }

  void                    InsertBefore(ListNODE<T> *Position, ListNODE<T> *Element) //pNodeΪ��ʱ,Insert��ǰ��
  {
    if (!Element)
      return;
    if (Position)
    {
      Element->pPrev = Position->pPrev;
      Element->pNext = Position;
      if (Element->pPrev)
        Element->pPrev->pNext = Element;
      else
        pHead = Element;
      Position->pPrev = Element;
    }
    else
    {
      Element->pPrev = NULL;
      Element->pNext = pHead;
      if (pHead)
        pHead->pPrev = Element;
      pHead = Element;
      if (!pTail)
        pTail = Element;
    }
    Count++;
  }

  void                    InsertAfter(ListNODE<T> *Position, ListNODE<T> *Element)  //pNodeΪ��ʱ,Insert�����
  {
    if (!Element)
      return;
    if (Position)
    {
      Element->pPrev = Position;
      Element->pNext = Position->pNext;
      if (Element->pNext)
        Element->pNext->pPrev = Element;
      else
        pTail = Element;
      Position->pNext = Element;
    }
    else
    {
      Element->pPrev = pTail;
      Element->pNext = NULL;
      if (pTail)
        pTail->pNext = Element;
      pTail = Element;
      if (!pHead)
        pHead = Element;
    }
    Count++;
  }

  void                    Insert(int i, ListNODE<T> *Element)  //��ĳһ�������һ���½��
  {
    ListNODE<T> *Position = GetAt(i);
    if (Position)
      InsertBefore(Position, Element);
    else
      InsertAfter(NULL, Element);
  }

  void                    Insert(int i, const T *Element)  //��ĳһ�������һ����Element��ʼ�����½����
  {
    ListNODE<T> *NewElement = CreateNode(Element);
    Insert(i, NewElement);
  }

  void                    AddTail(ListNODE<T> *Element)         //���β����һ���½��
  {
    InsertAfter(NULL, Element);
  }

  void                    AddTail(const T *Element)         //���β����һ���½��
  {
    ListNODE<T> *NewElement = CreateNode(Element);
    InsertAfter(NULL, NewElement);
  }

  void                    AddTail(const T &Element)         //���β����һ���½��
  {
    ListNODE<T> *NewElement = CreateNode(Element);
    InsertAfter(NULL, NewElement);
  }

  void                    AddHead(ListNODE<T> *Element)         //���ͷ����һ���½��
  {
    InsertBefore(NULL, Element);
  }

  void                    AddHead(const T *Element)         //���ͷ����һ���½��
  {
    ListNODE<T> *NewElement = CreateNode(Element);
    InsertBefore(NULL, NewElement);
  }

  void                    AddHead(const T &Element)         //���ͷ����һ���½��
  {
    ListNODE<T> *NewElement = CreateNode(Element);
    InsertBefore(NULL, NewElement);
  }

  void                    DetachNode(ListNODE<T> *pNode)       //�ӱ���ժȥһ�����, �����ȴӽ����ժ����Ȼ����ܽ��˽����뵽����������
  {
    if(Count == 0)
      return;

    if(pNode == pHead)
      pHead = pHead->GetNext();
    if(pNode == pTail)
      pTail = pTail->GetPrev();
    if(pNode->pNext != NULL)
      pNode->pNext->pPrev = pNode->pPrev;
    if(pNode->pPrev != NULL)
      pNode->pPrev->pNext = pNode->pNext;

    pNode->pPrev = NULL;
    pNode->pNext = NULL;

    Count--;
  }

  void                    DeleteNode(ListNODE<T> *Element)       //�ӱ������һ�����
  {
    DetachNode(Element);
    if(Element)
      delete Element;
  }

  void                    DeleteNode(int i)       //�ӱ������һ�����
  {
    ListNODE<T> *Element = GetAt(i);
    DetachNode(Element);
    if(Element)
      delete Element;
  }

  void                    DeleteHead()                    //ɾ��ͷ���
  {
    if (!IsEmpty())
      DeleteNode(GetHead());
  }

  void                    DeleteTail()                    //ɾ��β���
  {
    if (!IsEmpty())
      DeleteNode(GetTail());
  }

  void                    MovePrev(ListNODE<T> *Element)  //��ǰ�ƶ����
  {
    ListNODE<T> *pPrev = Element->pPrev;
    if (!pPrev)
      return;

    DetachNode(Element);
    InsertBefore(pPrev, Element);
  }

  void                    MovePrev(int i)  //��ǰ�ƶ����
  {
    MovePrev(GetAt(i));
  }

  void                    MoveNext(ListNODE<T> *Element)  //����ƶ����
  {
    ListNODE<T> *pNext = Element->pNext;
    if (!pNext)
      return;

    DetachNode(Element);
    InsertAfter(pNext, Element);
  }

  void                    MoveNext(int i)  //����ƶ����
  {
    MoveNext(GetAt(i));
  }
  void                    MoveHead(ListNODE<T> *Element)  //�ƶ���㵽��ͷ
  {
    if (Element)
    {
      DetachNode(Element);
      InsertBefore(NULL, Element);
    }
  }
  void                    MoveHead(int i)  //�ƶ���㵽��ͷ
  {
    MoveHead(GetAt(i));
  }
  void                    MoveTail(ListNODE<T> *Element)  //�ƶ���㵽��β
  {
    if (Element)
    {
      DetachNode(Element);
      InsertAfter(NULL, Element);
    }
  }
  void                    MoveTail(int i)  //�ƶ���㵽��β
  {
    MoveTail(GetAt(i));
  }

  void                    ClearAll()                      //����������еĽ��
  {
    ListNODE<T> *pTempNode = NULL;
    ListNODE<T> *pNode = pHead;
    while (pNode != NULL)
    {
      pTempNode = pNode->GetNext();
      delete pNode;
      pNode = pTempNode;
    }
    Initialize();
  }

  void                    DeleteAll()   //����������еĽ��
  {
    ClearAll();
  }

  void                    DeleteAt(int i)   //�������ĳ���Ľ��
  {
    DeleteNode(i);
  }

  ListNODE<T>            *AddNew()
  {
    ListNODE<T> *Element = CreateNode();
    if (Element)
      InsertAfter(NULL, Element);
    return Element;
  }

  //���β����һ���½��
  void                    AddNew(ListNODE<T> *pInsertedNode) 
  { 
    AddTail(pInsertedNode); 
  }

  //���β����һ���½�Ԫ��
  void                    AddNew(const T &Element)   
  {
    ListNODE<T> *NewElement = CreateNode(Element);
    AddTail(NewElement);
  }

  //���β����һ���½�Ԫ��
  void                    AddNew(const T *Element)
  {
    ListNODE<T> *NewElement = CreateNode(Element);
    AddTail(NewElement);
  }

  ListNODE<T>               *DupNode(ListNODE<T> *Element)
  {
    ListNODE<T> *New = Duplicate(Element);
    AddTail(New);
    return New;
  }

  static ListNODE<T>       *Duplicate(ListNODE<T> *Element)
  {
    ListNODE<T> *New = CreateNode();
    *New = *Element;
    return New;
  }

  static ListNODE<T>       *Duplicate(const T *Element)
  {
    ListNODE<T> *New = CreateNode(Element);
    return New;
  }

  static ListNODE<T>       *Duplicate(const T &Element)
  {
    ListNODE<T> *New = CreateNode(Element);
    return New;
  }

  //��Ŀ��List��������ǰ�б�
  void            CopyFromList(const ListBASE *List)
  {
    ClearAll();
    ListNODE<T> *Node = List->pHead;
    while (Node)
    {
      DupNode(Node);
      Node = Node->pNext;
    }
  }

  //����ǰ�б�����Ŀ��List
  void            DupToList(ListBASE *List)
  {
    List->ClearAll();
    ListNODE<T> *Node = pHead;
    while (Node)
    {
      List->DupNode(Node);
      Node = Node->pNext;
    }
  }

  //����ǰ�б������½�List
  ListBASE *DupNewList()
  {
    ListBASE *pNew = new ListBASE();
    DupList(pNew);
    return pNew;
  }

  //��Ŀ��List�Ľڵ����ӵ���ǰ�б�
  void                    AppendList(ListBASE *pList)
  {
    ListNODE<T> *pNode = pList->GetHead();
    while (pNode)
    {
      ListNODE<T> *pNext = pNode->GetNext();
      pList->DetachNode(pNode);
      AddTail(pNode);
      pNode = pNext;
    }
  }
};

typedef ListBASE<int> IntLIST;