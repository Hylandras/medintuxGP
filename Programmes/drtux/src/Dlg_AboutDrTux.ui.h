/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

void Dlg_AboutDrTux::init()
{QPixmap image = Theme::getIcon( "MedinTuxForDlg.png");
 textLabel_Image->setBackgroundPixmap ( image );
 textLabel_Image->adjustSize();
 int imgWidth       = image.width();
 int imgHeight      = image.height();
 textLabel_Image->setMinimumSize ( imgWidth, imgHeight );
 adjustSize();
}

