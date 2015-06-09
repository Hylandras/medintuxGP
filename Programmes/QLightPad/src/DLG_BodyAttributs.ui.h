/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
void Dlg_BodyAttributs::init()
{m_PathImage              = "";
 m_DefaultTextColor       = "";
 m_DefaultBackgroundColor = "";
}

//---------------------------------- initDialog -------------------------------------------------
void Dlg_BodyAttributs::initDialog( MyEditText *edit)
{
 //  QString  m_Path;
 //  QString  m_PathImage;
 //  QString  m_DefaultTextColor;
 //  QString  m_DefaultBackgroundColor;
 m_Edit                   = edit;
 m_PathImage              = edit->m_PathImage;
 m_DefaultTextColor       = edit->m_DefaultTextColor;
 m_DefaultBackgroundColor = edit->m_DefaultBackgroundColor;
 m_DefaultFont            = edit->m_DefaultFont;
 updateInterface();
 
}

//---------------------------------- updateInterface -------------------------------------------------
void Dlg_BodyAttributs::updateInterface()
{if (m_PathImage.length() != 0 )
    {QPixmap image;
     image.load (m_Edit->m_Path + QDir::separator() + m_PathImage);
     textLabel_BackImg->setPixmap(image);
    }
 if (m_DefaultTextColor.length() != 0 )
    {
     pushButton_TextCol->setPaletteForegroundColor (QColor(m_DefaultTextColor));
     pushButton_TextCol->setPaletteBackgroundColor (QColor(m_DefaultTextColor)); 
    }  
 if (m_DefaultBackgroundColor.length() != 0 )
    {
     pushButton_TextBackCol->setPaletteForegroundColor (QColor(m_DefaultBackgroundColor));
     pushButton_TextBackCol->setPaletteBackgroundColor (QColor(m_DefaultBackgroundColor)); 
    }  

 CHtmlTools::setDefaultTextAttributs(    textEditTextEffect, 
					 m_DefaultBackgroundColor ,
					 m_PathImage.length()?m_Edit->m_Path + QDir::separator() + m_PathImage:0,
					 m_DefaultTextColor,
					 m_DefaultFont);

}


//---------------------------------- pushButton_TextCol_clicked -------------------------------------------------
void Dlg_BodyAttributs::pushButton_TextCol_clicked()
{   QColor col = QColorDialog::getColor( QColor(m_DefaultTextColor), this );
    if ( !col.isValid() )       return;
    m_DefaultTextColor = col.name();
    updateInterface();
}


//---------------------------------- pushButton_TextBackCol_clicked -------------------------------------------------
void Dlg_BodyAttributs::pushButton_TextBackCol_clicked()
{   QColor col = QColorDialog::getColor( QColor(m_DefaultBackgroundColor), this );
    if ( !col.isValid() )       return;
    m_DefaultBackgroundColor = col.name();
    m_PathImage              = "";
    textLabel_BackImg->setPixmap(QPixmap());
    updateInterface();
}

 
//---------------------------------- pushButton_BackImg_clicked -------------------------------------------------
void Dlg_BodyAttributs::pushButton_BackImg_clicked()
{//................. choisir le fichier image src à insérer .................................................
  QString pathDst = m_Edit->m_Path;
  QFileDialog *fd = new QFileDialog  ( pathDst ,
				       tr( "Fichiers images : (*.png *.xpm *.jpg *.gif *.bmp );;Tous fichiers (*)" ), 
				       this, 
				       "OuvrirDlg"
                                      ) ;
    				      
  if (fd==0)                                            return;
  CPreview* p = new CPreview;
  //fd->setMode(QFileDialog::ExistingFiles);   //preview mode ne fonctionne pas avec ce mode !!
  fd->setInfoPreviewEnabled( TRUE );
  fd->setInfoPreview( p, p );
  fd->setPreviewMode( QFileDialog::Info );
  if ( ! (fd->exec() == QDialog::Accepted) ) 
     {delete fd; 
     //if (p) delete p; 
      return;
     }
  QString pathSrc = fd->selectedFile(); 
  delete fd;
 
  if ( pathSrc.isEmpty() )                    return;
  if ( !QFile::exists( pathSrc ) )            return;  
  QString fileName = CHtmlTools::Copy_File(pathSrc, pathDst);
  if (fileName.length()==0)                   return;
  m_PathImage =    fileName ;
  updateInterface();
}


//---------------------------------- pushButtonOK_clicked -------------------------------------------------
void Dlg_BodyAttributs::pushButtonOK_clicked()
{m_Edit->m_PathImage              = m_PathImage;
 m_Edit->m_DefaultTextColor       = m_DefaultTextColor;
 m_Edit->m_DefaultBackgroundColor = m_DefaultBackgroundColor;
 m_Edit->m_DefaultFont            = m_DefaultFont;
 if (m_DefaultBackgroundColor.length()) 
    {m_Edit->setPaper (QBrush( QColor(m_DefaultBackgroundColor)));
    }
 if (m_PathImage.length())              
    {m_Edit->setPaper (QBrush( QColor(), QPixmap (m_Edit->m_Path + QDir::separator() + m_PathImage) ) );     
    }
 if (m_DefaultTextColor.length())              
    {m_Edit->setPaletteForegroundColor (QColor (m_DefaultTextColor) ); 
     QPalette palette(m_Edit->palette());
     palette.setColor ( QColorGroup::Text, QColor (m_DefaultTextColor) );
     m_Edit->setPalette (palette);
    }
 m_Edit->setFont(m_DefaultFont);
 accept();
}   

//---------------------------------- pushButton_TextFont_clicked -------------------------------------------------
void Dlg_BodyAttributs::pushButton_TextFont_clicked()
{   bool ok;
    QFont font = QFontDialog::getFont(&ok,  m_DefaultFont , this );
    if ( ok ) 
       {m_DefaultFont = font;
        updateInterface(); 
       }     
}


