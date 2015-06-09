#include "C_PopupDial.h"
#include <QEvent>
#include <QPainter>
//======================== C_PopupDial ===========================================================================================================
C_PopupDial::C_PopupDial( QWidget *parent,
                          const QColor &backGround,
                          const QRect &initialRect,
                          const QRect &deployRect
                        )
  :QFrame(parent)
{   //setAttribute(Qt::WA_MouseTracking,true);
    m_alpha    = 180;
    m_duration = 700;
    setAttribute(Qt::WA_Hover,true);
    //setAttribute( Qt::WA_NoMousePropagation,true);
    m_backGround   = backGround;
    m_initialRect  = initialRect;
    m_deployRect   = deployRect;
    m_direction    = "";
    setGeometry(m_initialRect);
    //............ animer l'apparition du dialogue ..........................
    m_animation = new QPropertyAnimation(   this, "geometry", parent);
    connect( m_animation,       SIGNAL(finished ()) ,   this,   SLOT(Slot_finished( )) );
}

//------------------------------------------------------ changeInitialPosition ----------------------------------------------------------------------
void    C_PopupDial::changeInitialPosition(int x, int y, int max_x /* = -1 */, int max_y /* = -1 */)
{
  if ( max_x > 0 )   x     = qMin(x, max_x - m_deployRect.width());
  if ( max_y > 0 )   y     = qMin(y, max_y - m_deployRect.height());
  int dx = x -m_initialRect.x();
  int dy = y -m_initialRect.y();
  m_initialRect.translate(dx,dy);
  m_deployRect.translate(dx,dy);
}
//------------------------------------------------------ Slot_finished ----------------------------------------------------------------------
void  C_PopupDial::Slot_finished()
{
 emit Sign_finished(m_direction);
}

//------------------------------------------------------ setGeometry ----------------------------------------------------------------------
void  C_PopupDial::setGeometry(const QRect &rect)
{QFrame::setGeometry(rect);
}
//------------------------------------------------------ translateGeometry ----------------------------------------------------------------------
void  C_PopupDial::translateGeometry(int dx, int dy)
{
 bool status   = isDeploy();
 m_initialRect.translate(dx,dy);
 m_deployRect.translate(dx,dy);
 if (status)
    {QFrame::setGeometry(m_deployRect);
    }
 else
    {QFrame::setGeometry(m_initialRect);
    }
}
//------------------------------------------------------ setGeometry ----------------------------------------------------------------------
void  C_PopupDial::setGeometry(const QRect &initialRect, const QRect &deployRect)
{     bool status   = isDeploy();
      setRectanngles(initialRect, deployRect);
      if (status)
         {QFrame::setGeometry(m_deployRect);
         }
      else
         {QFrame::setGeometry(m_initialRect);
         }
}
//------------------------------------------------------ setRectanngles ----------------------------------------------------------------------
void  C_PopupDial::setRectanngles(const QRect &initialRect, const QRect &deployRect)
{ m_initialRect = initialRect;
  m_deployRect  = deployRect;
}

//------------------------------------------------------ event ----------------------------------------------------------------------
bool C_PopupDial::event ( QEvent * ev )
{
  if (ev->type()==QEvent::Enter)
    { if ( ! isDeploy() )
         {setToDeployRect();
         }
    }
  if (ev->type()==QEvent::Leave)
    {setToInitialRect();
    }
  return QFrame::event (ev);
}
//------------------------------------------------------ isDeploy ----------------------------------------------------------------------
bool C_PopupDial::isDeploy()
{return (this->width()>m_initialRect.width() && this->height()>m_initialRect.height());
}

//------------------------------------------------------ setToDeployRect ----------------------------------------------------------------------
void C_PopupDial::setToDeployRect(int fast /* -0 */)
{   int  ok = 1;
    emit Sign_deployed(ok);
    if (ok)
       { raise();
         if (fast)
            {setGeometry(m_deployRect);
            }
         else
            { m_animation->setDuration(m_duration);
              m_animation->setEasingCurve ( QEasingCurve ( QEasingCurve::OutCubic ));
              m_animation->setStartValue(m_initialRect);
              m_animation->setEndValue(m_deployRect);
              m_animation->start();
              m_direction = "TO_DEPLOY";
            }
       }
    emit Sign_deployed(this);
}

//------------------------------------------------------ setToInitialRect ----------------------------------------------------------------------
void C_PopupDial::setToInitialRect(int fast /*=0*/)
{   int  ok = 1;
    emit Sign_minimized(ok);
    if (ok)
       { if (fast)
            {this->setGeometry(m_initialRect);
            }
         else
            { m_animation->setDuration(m_duration);
              m_animation->setEasingCurve ( QEasingCurve ( QEasingCurve::OutCubic ));
              m_animation->setStartValue(geometry());
              m_animation->setEndValue(m_initialRect);
              m_animation->start();
              m_direction = "TO_INITIAL";
            }
       }
    emit Sign_minimized(this);
}

//------------------------------------------------------ paintEvent ----------------------------------------------------------------------
void C_PopupDial::paintEvent ( QPaintEvent * /*event*/ )
{   QPainter p(this);
    //........... cadre a bords ronds .................................
    QRect rect = this->rect();
    rect.setWidth  (this->rect().width() -1); // -1 car bord droit et bas masques par le widget
    rect.setHeight (this->rect().height()-1); // -1 car bord droit et bas masques par le widget

    QColor c(m_backGround);
    c.setAlpha( m_alpha );
    p.setPen (m_backGround.darker(200));
    QPainterPath path;
    path.addRoundedRect (rect, 6, 6);
    p.setBrush(c);
    p.drawPath(path);
}
