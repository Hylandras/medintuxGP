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


/*! \class FormSelectionUser
 *  Ouvre une fenêtre de sélection des utilisateurs de la base de comptabilité
*/

#include "../icones.h"

void FormSelectionUser::init()
{ nbEssais = 0;
  selectedUsrId = 0;
  ul = 0;
  lkTux = 0;
  QImage img;
  img.loadFromData( icone, sizeof( icone ), "PNG" );
  setIcon( img );
}


void FormSelectionUser::destroy()
{ //delete ul;

}

/*! \fn void FormSelectionUser::setUsersList( UTILISATEURS_LISTE *usersl)
 * \brief Récupère la liste des utilisateurs passée en paramètre et met à jour la liste des utilisateurs disponibles.
*/
void FormSelectionUser::setUsersList( UTILISATEURS_LISTE *usersl)
{ ul = usersl;
  listUsers->clear();
  if (ul->count() > 0)
    if (ul->count() > 1)
    { UTILISATEURS_LISTE::iterator it;
      for (it = ul->begin(); it != ul->end(); ++it)
       { listUsers->insertItem( (*it).getNom() ); }
    } // Fin if + de 1 utilisateurs
    else
    { UTILISATEURS_LISTE::iterator it = ul->at(0);
       listUsers->insertItem( (*it).getNom() );
    } // Fin if au moins un utilisateur
  else { createNewUser(); }
}


/*! \fn void FormSelectionUser::setUsersList( UTILISATEURS_LISTE *usersl)
 * \brief Récupère la liste des utilisateurs passée en paramètre et met à jour la liste des utilisateurs disponibles.
*/
void FormSelectionUser::setComptaMoteurBase(ComptaMoteurBase* base)
{ if (base) m_CBase = base; }



/*! \fn void FormSelectionUser::createNewUser()
 *  \brief Lorsqu'il n'existe aucun utilisateur, il faut en créer un. Rôle de cette fonction.
 *  \todo A CODER.
*/
void FormSelectionUser::createNewUser()
{

}


/*! \fn void FormSelectionUser::but_Valider_clicked()
 *  \brief SLOT appelé lorsque l'utilisateur appui sur le boutton valide. Vérifie le login et mot de passe de l'utilisateur sélectionné. Sauvegarde l'Id (dans la listbox) de l'utilisateur dans la variable userSelected récupérable par le fonction FormSelectionUser::getUsrId().
 *  \sa ComptaMoteurBase, PassWordEncode , PassWordDecode.
 * \todo Après 3 essais bloquer le compte utilisateur erroné pdt 10 minutes.
*/
void FormSelectionUser::but_Valider_clicked()
{ // Vérifie que le password rentrée est le même que celui de la base de données
  QString pass = Password->text();
  // Récupérer l'utilisateur sélectionné
  UTILISATEURS_LISTE::iterator it = ul->at( selectedUsrId );
  m_CBase->PassWordEncode( pass );
  if ( pass == (*it).getPassword() && Login->text() == (*it).getLogin() )
  { // Login et Password sont vérifiés  ;   L'id de l'utilisateur est stockée dans selectedUsrId
    accept();
  }
  else
  { nbEssais++;
     Password->clear();
    if (nbEssais == 4)
     {reject();}
    else
     { 	QString msg = "";
	switch (nbEssais)
        { case 1 : { msg = TR("Premier essai erroné.\n\n"); break; }
	  case 2 : { msg = TR("Deuxième essai erroné.\n\n"); break; }
	  case 3 : { msg = TR("Troisième essai erroné.\n\nL'application va maintenant se fermer."); break; }
	}
	if (nbEssais == 3)
        { QMessageBox::information(this, TR("Identification erronée."),  msg ,"OK" ,"","",1,0);
 	  reject();
 	}
	else	QMessageBox::information(this, TR("Identification erronée."),  msg + TR("Le login et le mot de passe renseignés sont incorrects.\nVérifiez que la touche majuscule de votre clavier n'est pas activée."),"OK" ,"","",1,0);
      } // Fin if essais == 3
  } // Fin if pass and login
}


void FormSelectionUser::but_Quitter_clicked()
{ reject(); }


void FormSelectionUser::listUsers_highlighted( int id )
{ // Récupère le nom de l'utilisateur et mais le dans le login
  UTILISATEURS_LISTE::iterator it = ul->at( id );
  Login->setText ( (*it).getLogin() );
  // Setfocus dans la case password
  selectedUsrId = id;
  Password->setFocus();
}

/*! \fn int FormSelectionUser::getUserId()
 *  \brief Retourne l'Id validée de l'utilisateur sélectionné.
*/
Utilisateurs* FormSelectionUser::getUser()
{ UTILISATEURS_LISTE::iterator it = ul->at( selectedUsrId );
    return &(*it);
}


/*! \fn void FormSelectionUser::but_Creer_clicked()
 *  \brief SLOT appelé lorsque l'utilisateur clique sur le bouton "Créer un nouvel utilisateur".
*/
void FormSelectionUser::but_Creer_clicked()
{ // Ouvrir une fenêtre avec sélection utilisateur DrTux
  mQDialog_NewUser = new QDialog(this);
  lkTux = new FormLinkTux(mQDialog_NewUser);
  QString f, erreur;

 ComptaMoteurBase* m_BaseTux = new ComptaMoteurBase( G_pCApp->m_tuxdb_Driver, G_pCApp->m_tuxdb_Name, G_pCApp->m_tuxdb_User, G_pCApp->m_tuxdb_Pass,
                                                     G_pCApp->m_tuxdb_Host,   G_pCApp->m_tuxdb_Port,
                                                     f, G_pCApp->m_tuxdb_Name, &erreur);
  if (erreur != "") qWarning(erreur);

  // Récupérer les utilisateurs drtux
  tux_usr = new UTILISATEURS_LISTE;
  if (!m_BaseTux->getDrTuxUsers(tux_usr)) qWarning(m_BaseTux->m_messageErreur);

  // Créer widget de sélection avec demande du mot de passe
  lkTux->setUsersList(tux_usr);
  connect (lkTux, SIGNAL ( signal_FormLinkTux_butValider_clicked() ) ,
	   this,    SLOT   ( checkLinkTux() ) );
  mQDialog_NewUser->exec();
//  delete m_BaseTux;
//  delete mQDialog_NewUser;
//  delete lkTux;
}

/*! \fn void FormSelectionUser::checkLinkTux()
 *  \brief SLOT appelé lorsque l'utilisateur à sélectionné un nouvel utilisateur. Vérifie pass et login saisie si tout OK sauvegarde l'utilisateur dans la base et met à jour la fenêtre de sélection de l'utilisateur.
 *  \todo Gérer les essais erronés....
*/
void FormSelectionUser::checkLinkTux()
{ // Vérifie les paramètres de connection de l'utilisateur
  UTILISATEURS_LISTE::iterator it = tux_usr->at ( lkTux->getSelectedUsr() );
  QString pass = lkTux->getPassword();
  QString log  = lkTux->getLogin();
  m_CBase->PassWordEncode(pass);
  // Vérifie le login, le mot de passe et vérifie que l'utilisateur n'est pas déjà lié
  if ((pass == (*it).getPassword()) && (log == (*it).getLogin()) )
   { lkTux->valide();
     mQDialog_NewUser->close();
     // Sauvegarder dans la base de comptabilité l'utilisateur fraîchement créé
     m_CBase->saveUserToBase( &(*it) );
     // Met à jour la listbox de sélection d'utilisateurs
     ul->clear();
     m_CBase->getUsersForList( ul );
     setUsersList(ul);
   }
  else
   { qWarning("pass et log PAS OK"); }
  // Si tout OK Valide la fenêtre FormLinkTux
  //lnTux->valide();
}


