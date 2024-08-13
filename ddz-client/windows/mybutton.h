#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QPushButton>

class MyButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MyButton(QWidget *parent = nullptr);

    void setImage(QString normal, QString hover, QString pressed, QString disable=QString());
    void setBtnDisable(bool flag);

signals:

protected:
    // 鼠标按下
    void mousePressEvent(QMouseEvent* ev);
    // 鼠标释放
    void mouseReleaseEvent(QMouseEvent* ev);
    // 鼠标进入
    void enterEvent(QEvent* ev);
    // 鼠标离开
    void leaveEvent(QEvent* ev);
    // 绘图
    void paintEvent(QPaintEvent* ev);

private:
    QString m_normal;
    QString m_hover;
    QString m_pressed;
    QString m_disable;
    QPixmap m_pixmap;
    bool m_isDisable = false;

};

#endif // MYBUTTON_H
