/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/

#include <qmessagebox.h>

void FormLinkTux::init()
{ selectedUsrId = 0;
//   setWFlags(Qt::WDestructiveClose);
}

void FormLinkTux::destroy()
{ //delete tux_usr;
}


void FormLinkTux::setUsersList( UTILISATEURS_LISTE* ul )
{ UTILISATEURS_LISTE::iterator it;
   tux_usr = ul;
  for (it = ul->begin() ; it != ul->end(); ++it)
   { usersList->insertItem( (*it).getNom() ); } 
}


void FormLinkTux::usersList_selected( int id )
{ UTILISATEURS_LISTE::iterator it = tux_usr->at( id );
   selectedUsrId = id; 
   nomUsr->setText( (*it).getNom() );
   loginUsr->setText( (*it).getLogin() );
   passUsr->setFocus(); 
}


void FormLinkTux::butValider_clicked()
{ emit (signal_FormLinkTux_butValider_clicked() ); }


int FormLinkTux::getSelectedUsr( )
{ return selectedUsrId;;}

QString FormLinkTux::getPassword( )
{ return passUsr->text();}

QString FormLinkTux::getLogin( )
{ return loginUsr->text();}

UTILISATEURS_LISTE* FormLinkTux::getTuxUsr()
{ return tux_usr; }

void FormLinkTux::valide() { close(); }


