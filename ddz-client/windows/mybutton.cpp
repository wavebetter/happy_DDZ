#include "mybutton.h"

#include <QMouseEvent>
#include <QPainter>

MyButton::MyButton(QWidget *parent) : QPushButton(parent)
{

}

void MyButton::setImage(QString normal, QString hover, QString pressed, QString disable)
{
    m_normal = normal;
    m_hover = hover;
    m_pressed = pressed;
    m_disable = disable;
    m_pixmap.load(m_normal);
    update();
}

void MyButton::setBtnDisable(bool flag)
{
    m_isDisable = flag;
    setDisabled(flag);
    QString pic = flag ? m_disable : m_normal;
    m_pixmap.load(pic);
    update();
}

void MyButton::mousePressEvent(QMouseEvent *ev)
{
    if(m_isDisable)
    {
        return;
    }
    if(ev->button() == Qt::LeftButton)
    {
        m_pixmap.load(m_pressed);
        update();
    }
    QPushButton::mousePressEvent(ev);
}

void MyButton::mouseReleaseEvent(QMouseEvent *ev)
{
    if(m_isDisable)
    {
        return;
    }
    if(ev->button() == Qt::LeftButton)
    {
        m_pixmap.load(m_normal);
        update();
    }
    QPushButton::mouseReleaseEvent(ev);
}

void MyButton::enterEvent(QEvent *ev)
{
    if(m_isDisable)
    {
        return;
    }
    Q_UNUSED(ev)
    m_pixmap.load(m_hover);
    update();
}

void MyButton::leaveEvent(QEvent *ev)
{
    if(m_isDisable)
    {
        return;
    }
    Q_UNUSED(ev)
    m_pixmap.load(m_normal);
    update();
}

void MyButton::paintEvent(QPaintEvent *ev)
{
    Q_UNUSED(ev)
    QPainter p(this);
    p.drawPixmap(rect(), m_pixmap);
}
