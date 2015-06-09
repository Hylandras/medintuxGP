#ifndef C_LEVELITEMDELEGATE_H
#define C_LEVELITEMDELEGATE_H
#include <QStyledItemDelegate>

#define INDIC_COL_ICONE    0
#define INDIC_COL_LIBELLE  0
#define INDIC_COL_SMR      1
#define INDIC_COL_DATE     2
#define INDIC_COL_COMMENT  3
#define INDIC_COL_CODE     4
#define INDIC_COL_TYPE     5
#define INDIC_COL_REFUNDED 6
#define INDIC_COL_CIP      7

//================================ C_LevelItemDelegate =====================================================
    class C_LevelItemDelegate : public QStyledItemDelegate
    {
        Q_OBJECT

    public:
        C_LevelItemDelegate(QObject *parent = 0);
        /*
        QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;

        void setEditorData(QWidget *editor, const QModelIndex &index) const;
        void setModelData(QWidget *editor, QAbstractItemModel *model,
                          const QModelIndex &index) const;

        void updateEditorGeometry(QWidget *editor,
            const QStyleOptionViewItem &option, const QModelIndex &index) const;
        */
        void paint(QPainter *painter, const QStyleOptionViewItem &option,  const QModelIndex &index) const;

    };
#endif // C_LEVELITEMDELEGATE_H
