#ifndef C_DRAGQTREEWIDGET_H
#define C_DRAGQTREEWIDGET_H

#include <QTreeWidget>
#include <QPoint>
class QTreeWidgetItem;
//==========================  C_DragQTreeWidget ===================================
class C_DragQTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:

    enum flags  { NOT_CALL_STD_EVENT     = 0,
                  AFTER_CALL_STD_EVENT   = 1,
                  BEFORE_CALL_STD_EVENT  = 2
                };
    explicit C_DragQTreeWidget(QWidget *parent = 0);
    void             setMimeType(const QString& mimeType);
    void             setIconPathMimeType(const QString& path);
    void             mousePressEvent(QMouseEvent *event);
    void             mouseMoveEvent(QMouseEvent *event);
    bool             dropMimeData ( QTreeWidgetItem * parent, int index, const QMimeData * data, Qt::DropAction action );
    QTreeWidgetItem *getFirstSelectedItem();
protected:
    void keyPressEvent ( QKeyEvent * event );

signals:
    void Sign_giveMeDatas(QString &data, QTreeWidgetItem *item);
    void Sign_keyPressEvent(QKeyEvent *, int &);
    void Sign_keyPressEvent(QKeyEvent *, int &, C_DragQTreeWidget*);
    void Sign_dropMimeData (bool &ret,  QTreeWidgetItem * parent, int index, const QMimeData * data, Qt::DropAction action );
public slots:

private:
  QString m_mimeType;
  QString m_mimeTypeIconPath;
  QPoint  m_startDragPos;

};

#endif // C_DRAGQTREEWIDGET_H
