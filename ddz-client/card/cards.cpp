#include "cards.h"
#include "datamanager.h"
#include <QRandomGenerator>
#include <QDebug>

Cards::Cards()
{

}

Cards::Cards(const Card &card)
{
    add(card);
}

void Cards::add(const Card &card)
{
    m_cards.append(card);
}

void Cards::add(const Cards &cards)
{
    m_cards.append(cards.m_cards);
}

void Cards::add(const QVector<Cards> &cards)
{
    for(int i=0; i<cards.count(); ++i)
    {
        add(cards.at(i));
    }
}

Cards &Cards::operator <<(const Card &card)
{
    add(card);
    return *this;
}

Cards &Cards::operator <<(const Cards &cards)
{
    add(cards);
    return *this;
}

void Cards::remove(const Card &card)
{
    m_cards.removeOne(card);
}

void Cards::remove(const Cards &cards)
{
    // m_cards.subtract(cards.m_cards);
    for(auto& item :cards.m_cards)
    {
        for(auto& it : m_cards)
        {
            if(item == it)
            {
                remove(item);
                break;
            }
        }
    }
}

void Cards::remove(const QVector<Cards> &cards)
{
    for(int i=0; i<cards.size(); ++i)
    {
        remove(cards.at(i));
    }
}

int Cards::cardCount()
{
    return m_cards.size();
}

bool Cards::isEmpty()
{
    return m_cards.isEmpty();
}

bool Cards::isEmpty() const
{
    return m_cards.isEmpty();
}

void Cards::clear()
{
    m_cards.clear();
}

Card::CardPoint Cards::maxPoint()
{
    Card::CardPoint max = Card::Card_Begin;
    if(!m_cards.isEmpty())
    {
        for(auto it = m_cards.begin(); it!=m_cards.end(); ++it)
        {
            if(it->point() > max)
            {
                max = it->point();
            }
        }
    }
    return max;
}

Card::CardPoint Cards::minPoint()
{
    Card::CardPoint min = Card::Card_End;
    if(!m_cards.isEmpty())
    {
        for(auto it = m_cards.begin(); it!=m_cards.end(); ++it)
        {
            if(it->point() < min)
            {
                min = it->point();
            }
        }
    }
    return min;
}

int Cards::pointCount(Card::CardPoint point)
{
    int count = 0;
    for(auto it = m_cards.begin(); it!=m_cards.end(); ++it)
    {
        if(it->point() == point)
        {
            count++;
        }
    }
    return count;
}

bool Cards::contains(const Card &card)
{
    return m_cards.contains(card);
}

bool Cards::contains(const Cards &cards)
{
    //return m_cards.contains(cards.m_cards);
    bool flag = true;
    for(auto& item : cards.m_cards)
    {
        if(!m_cards.contains(item))
        {
            flag = false;
            break;
        }
    }
    return flag;
}

Card Cards::takeRandomCard()
{
    Card card;
    if(!DataManager::getInstance()->isNetworkMode())
    {
        // 生成一个随机数
        int num = QRandomGenerator::global()->bounded(m_cards.size());
        auto it = m_cards.begin();
        for(int i=0; i<num; ++i, ++it);
        card = *it;
        m_cards.erase(it);

    }
    else
    {
        card = m_cards.takeFirst();
    }
    return card;
}

CardList Cards::toCardList(SortType type)
{
    CardList list;
    for(auto it = m_cards.begin(); it != m_cards.end(); ++it)
    {
        list << *it;
    }
    if(type == Asc)
    {
        std::sort(list.begin(), list.end(), lessSort);
    }
    else if(type == Desc)
    {
        std::sort(list.begin(), list.end(), greaterSort);
    }
    return list;
}

void Cards::printAllCardInfo()
{
    QString text;
    char pts[] = "JQKA2";
    for(auto it = m_cards.begin(); it != m_cards.end(); ++it)
    {
        QString msg;
        Card::CardPoint pt = it->point();
        Card::CardSuit suit = it->suit();
        if(suit == Card::CardSuit::Club)
        {
            msg = "梅花";
        }
        else if(suit == Card::CardSuit::Diamond)
        {
            msg = "方片";
        }
        else if(suit == Card::CardSuit::Heart)
        {
            msg = "红桃";
        }
        else
        {
            msg = "黑桃";
        }
        if(pt>=Card::Card_3 && pt <= Card::Card_10)
        {
            msg = QString("%1%2").arg(msg).arg(pt+2);
        }
        else if(pt >= Card::Card_J && pt <= Card::Card_2)
        {
            msg = QString("%1%2").arg(msg).arg(pts[pt-Card::Card_J]);
        }
        if(pt == Card::Card_BJ)
        {
            msg = "Big Joker";
        }
        if(pt == Card::Card_SJ)
        {
            msg = "Small Joker";
        }
        msg += "  ";
        text += msg;
    }
    qDebug() << text;
}
