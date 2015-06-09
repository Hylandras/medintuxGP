#include "C_DragQTreeWidget.h"
#include <QObject>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QApplication>
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>

//--------------------------------- C_DragQTreeWidget -------------------------------------------------------------
C_DragQTreeWidget::C_DragQTreeWidget(QWidget *parent) :
    QTreeWidget(parent)
{m_mimeType          ="";
 m_mimeTypeIconPath  ="";
}

//--------------------------------- setMimeType -------------------------------------------------------------
void C_DragQTreeWidget::setMimeType(const QString& mimeType)
{m_mimeType = mimeType;
}

//--------------------------------- setIconPathMimeType -------------------------------------------------------------
void C_DragQTreeWidget::setIconPathMimeType(const QString& path)
{m_mimeTypeIconPath = path;
}

//--------------------------------- mousePressEvent -------------------------------------------------------------
void C_DragQTreeWidget::mousePressEvent(QMouseEvent *event)
{   if (event->button() == Qt::LeftButton) m_startDragPos = event->pos();
    QTreeWidget::mousePressEvent(event);
}

//--------------------------------- getFirstSelectedItem -------------------------------------------------------------
QTreeWidgetItem * C_DragQTreeWidget::getFirstSelectedItem()
{QList<QTreeWidgetItem *> list = selectedItems ();
 if (list.count()) return list[0];
 return 0;
}
//--------------------------------- keyPressEvent -------------------------------------------------------------
void C_DragQTreeWidget::keyPressEvent ( QKeyEvent * event )
{   int call_StandardEvent_1 = C_DragQTreeWidget::AFTER_CALL_STD_EVENT;
    int call_StandardEvent_2 = C_DragQTreeWidget::AFTER_CALL_STD_EVENT;
    emit Sign_keyPressEvent(event, call_StandardEvent_1);
    emit Sign_keyPressEvent(event, call_StandardEvent_2, this);
    if ( call_StandardEvent_1==C_DragQTreeWidget::AFTER_CALL_STD_EVENT ||
         call_StandardEvent_2==C_DragQTreeWidget::AFTER_CALL_STD_EVENT
       ) QTreeWidget::keyPressEvent(event);
}
//--------------------------------- dropMimeData -------------------------------------------------------------
bool C_DragQTreeWidget::dropMimeData ( QTreeWidgetItem * parent, int index, const QMimeData * data, Qt::DropAction action )
{   bool ret = false;
    emit Sign_dropMimeData (ret,   parent,  index,   data, action );
    // if (ret) qDebug() << "true";
    // else     qDebug() << "false";
    return ret;
}

//--------------------------------- mouseMoveEvent -------------------------------------------------------------
void C_DragQTreeWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
       {
         int distance = (event->pos() - m_startDragPos).manhattanLength();
         if (distance >= QApplication::startDragDistance())
            { QTreeWidgetItem *item = getFirstSelectedItem();
              if (item)
                 { QString data;
                   emit Sign_giveMeDatas(data, item);
                   if (data.length())
                      {
                        QMimeData *mimeData = new QMimeData;
                        QDrag *drag         = new QDrag(this);
                        //if (m_mimeTypeIconPath.length())   drag->setDragCursor ( QPixmap (m_mimeTypeIconPath), Qt::CopyAction);  // marche po
                        mimeData->setData(m_mimeType, data.toLatin1());
                        drag->setMimeData(mimeData);
                        if (m_mimeTypeIconPath.length()) drag->setPixmap(QPixmap(m_mimeTypeIconPath));
                        if (drag->exec(Qt::CopyAction) == Qt::CopyAction)
                           { event->accept();
                            //return;
                           }
                      }
                 }
            }
       }
    QTreeWidget::mouseMoveEvent(event);
}


