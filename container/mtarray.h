#pragma once

template <class T>
class RefPOINTER
{
private:
  T            *Pointer;
public:
  RefPOINTER()
  {
    Pointer = NULL;
  }
  ~RefPOINTER() {}
};

// 只支持值类型操作.

template<typename T>
void ReversArray(T *pArray, int Num)
{
  int ExchangeNum = Num / 2;
  for (int i = 0; i < ExchangeNum; i++)
  {
    T Temp = pArray[i];
    pArray[i] = pArray[Num - 1 - i];
    pArray[Num - 1 - i] = Temp;
  }
}

template <class T>
class ArrayBASE
{
protected:
  T            *pItem;
  int           ItemBufNum;

  int           ItemNum;
  int           iAdd;
  int           XNum;
  int           YNum;
public:
  ArrayBASE()
  {
    pItem = NULL;
    ItemBufNum = 0;

    ItemNum = 0;
    iAdd = 0;
    XNum = 0;
    YNum = 0;
  }

  ~ArrayBASE()
  {
    if (pItem)
      delete[]pItem;
  }

  // copyto
  void          CopyTo(ArrayBASE *pTarget)
  {
    if (pTarget)
    {
      pTarget->Init(XNum, YNum);
      for (int iX = 0; iX < XNum; iX++)
      {
        for (int iY = 0; iY < YNum; iY++)
          pTarget->Get(iX, iY) = Get(iX, iY);
      }
    }
  }
  // copyfrom
  void          CopyFrom(ArrayBASE *pTarget)
  {
    if (pTarget)
      pTarget->CopyTo(this);
  }

  // 一维数组初始化
  void          Init(int Size)
  {
    Init(1, Size);
  }

  // 二维数组初始化
  void          Init(int X, int Y)
  {
    if (ItemBufNum < X * Y)
    {
      if (pItem)
        delete[]pItem;
      ItemBufNum = X * Y;
      pItem = new T[ItemBufNum];
    }

    XNum = X;
    YNum = Y;
    ItemNum = X * Y;
    iAdd = 0;
  }

  // 初始化为0
  void           Clear()
  {
    if (pItem)
      memset(pItem, 0, sizeof(T) * ItemNum);
  }
  void           MemSet(T V)
  {
    if (pItem)
    {
      for (int i = 0; i < ItemNum; i++)
        pItem[i] = V;
    }
  }

  int           GetNum() { return ItemNum; }
  int           GetAddNum() { return iAdd; }
  int           GetScale(int *pX, int *pY)
  {
    if(pX)
      *pX = XNum;
    if(pY)
      *pY = YNum;
    return ItemNum;
  }

  inline int    CheckIndex(int i)
  {
    return (i >= 0 && i < ItemNum) ? 1 : 0;
  }
  inline int    CheckIndex(int iX, int iY)
  {
    return (iX >= 0 && iX < XNum && iY >= 0 && iY < YNum) ? 1 : 0;
  }

  // 获取i下标的元素
  inline T           &operator[](int i)
  {
    return pItem[i];
  }

  // 获取i下标的元素地址
  inline T           *operator+(int i)
  {
    return pItem + i;
  }

  T            &Get(int i)
  {
    return pItem[i];
  }

  T            &Get(int iX, int iY)
  {
    return pItem[iX * YNum + iY];
  }

  T            *Gets() { return pItem; }
  T            *Gets(int iX) { return pItem + iX * YNum; }

  void          Set(int i, T Item)
  {
    if(i >= 0 && i < ItemNum)
      pItem[i] = Item;
  }
  void          Set(int iX, int iY, T Item)
  {
    Get(iX, iY) = Item;
  }

  void          Add(T Item)
  {
    if (iAdd < ItemNum)
    {
      pItem[iAdd] = Item;
      iAdd++;
    }
  }

  void          ResetAdd() { iAdd = 0; }
};

typedef ArrayBASE<int>      IntARRAY;
typedef ArrayBASE<int *>    PIntARRAY;
typedef ArrayBASE<BYTE>     ByteARRAY;
typedef ArrayBASE<BYTE *>   PByteARRAY;