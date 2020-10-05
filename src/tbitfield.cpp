// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
    if (len<0) throw - 1;
    BitLen = len;
    MemLen = (BitLen /sizeof(TELEM)*8) +1;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    this->BitLen = bf.BitLen;
    this->MemLen = bf.MemLen;
    this->pMem = new TELEM[this->MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    if (pMem != NULL)
    {
        delete[] pMem;
        pMem = NULL;
    }
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    int tmp = floor( n / sizeof (TELEM));
    return tmp;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    TELEM m = 1 << (n % 8);
    return m;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if ((n < BitLen) && (n >= 0))
        pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] | GetMemMask(n);
    else throw - 1;
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if ((n < BitLen) && (n >= 0))
        pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] & ~GetMemMask(n);
    else throw - 1;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if ((n < BitLen) && (n >= 0))
    {
        int tmp = pMem[GetMemIndex(n)] & GetMemMask(n);
        return tmp;
    }
    else throw - 1;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{

    if (*this == bf)
        return *this;
    if (pMem!=NULL)
        delete[] pMem;
    this->BitLen = bf.BitLen;
    this->MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    if (pMem != NULL)
    {
        for (int i = 0; i < MemLen; i++)
            pMem[i] = bf.pMem[i];
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen == bf.BitLen)
    {
        for (int i = 0; i < MemLen; i++)
            if (pMem[i] != bf.pMem[i])
                return 0;
        return 1;
    }
    else
        return 0;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    if (BitLen == bf.BitLen)
    {
        for (int i = 0; i < MemLen; i++)
            if (pMem[i] != bf.pMem[i])
                return 1;
        return 0;
    }
    return 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int Len = (this->BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField tmp(Len);
    for (int i = 0; i < Len; i++)
    {
        tmp.pMem[i] = pMem[i] | bf.pMem[i];
    }
    return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int Len = (this->BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField tmp(Len);
    for (int i = 0; i < Len; i++)
    {
        tmp.pMem[i] = pMem[i] & bf.pMem[i];
    }
    return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField tmp(BitLen);
    for (int i = 0; i < BitLen; i++)
    {
        tmp.SetBit(i);
    }

    for (int i = 0; i < MemLen; i++)
    {
        pMem[i] = ~pMem[i];
        pMem[i] = pMem[i] & tmp.pMem[i];
    }
    return *this;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    char ch;
    int i = 0;
    istr >> ch;
    while (ch=='0' || ch=='1')
    {
        if (ch == '0')
            bf.ClrBit(i);
        if (ch == '1')
            bf.SetBit(i);
        i++;
        istr >> ch;
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.GetLength(); i++)
        if (bf.GetBit(i))
            ostr << '1';
        else 
            ostr << '0';
    return ostr;
}