//
// C++ Interface: ThemePopup
//
// Description:
//
//
// Author: Eric MAEKER <eric.maeker@free.fr> and Roland Sevin <roland=sevin@medintux.org>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef THEMEPOPUP_H
#define THEMEPOPUP_H

#include <qpopupmenu.h>
#include <qobject.h>
#include <qfont.h>
#include "Theme.h"

/** \class ThemePopup
 *  \brief Reimplémentation du constructeur de QPopupMenu pour permettre la mise en place du thème de popup.
 *  Cette sera utile lors du passe à Qt4 car si toutes les applis utilisent ThemePopup à la place de QPopupMenu , une seule classe sera à réécrire.

        @author Eric MAEKER <eric.maeker@free.fr>
        @author Rolan Sevin <roland-sevinr@medintux.org>
*/
class ThemePopup : public QPopupMenu
{
    Q_OBJECT

public:
    ThemePopup(QWidget * parent = 0, const char * name = 0);
    ThemePopup(QStringList &list, QWidget * parent = 0, const char * name = 0 , const QString& sep = "|", int toMask = 0);
    QString DoPopupList(int *pID_to_Ret = 0);
    int GetPopupItemID ();
protected slots:
    void memorisePopupItem (int id);
    void OnSelectPopupItem ();
protected:
QString      m_CurrentPopupItem;
QString      m_RetPopupItem;
int          m_CurrentPopupID;
int          m_RetPopupID;
QStringList *m_pListItem;
};

#endif
