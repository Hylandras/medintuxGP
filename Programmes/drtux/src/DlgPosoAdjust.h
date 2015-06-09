/****************************************************************************
** Form interface generated from reading ui file 'DlgPosoAdjust.ui'
**
** Created: ven fév 11 22:11:40 2005
**      by: The User Interface Compiler ($Id: DlgPosoAdjust.h,v 1.4 2008-03-08 09:27:11 ericmaeker2 Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef DLGPOSOADJUST_H
#define DLGPOSOADJUST_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>
#include "CMoteur_Base.h"
#include "CMDI_Terrain.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QLineEdit;
class QPushButton;
class QGroupBox;
class QSlider;
class QRadioButton;
class QComboBox;
class QTextBrowser;
class QTabWidget;
class QWidget;
class QListView;
class QListViewItem;

class DlgPosoAdjust : public QDialog
{
    Q_OBJECT

public:
    DlgPosoAdjust( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~DlgPosoAdjust();

    QLabel* textLabelUP_Jour;
    QLabel* textLabelPoids;
    QLabel* textLabelTaille;
    QLineEdit* lineEdit_Poids;
    QLineEdit* lineEdit_Taille;
    QLabel* textLabelPdsUnit;
    QLabel* textLabelTailleUnit;
    QPushButton* pushButtonOK;
    QGroupBox* groupBox1;
    QLabel* lineEdit_a_UP1;
    QLineEdit* lineEdit_QMax_UP1;
    QLabel* lineEdit_a_UP2;
    QLineEdit* lineEdit_QMax_UP2;
    QLineEdit* lineEdit_QMin_UP1;
    QSlider* slider_VarPoso;
    QLineEdit* lineEdit_QMin_UP2;
    QLabel* textLabelUnite_UP1;
    QLabel* textLabelUnite_UP2;
    QRadioButton* radioButtonSecable1;
    QRadioButton* radioButtonSecable2;
    QRadioButton* radioButtonSecable4;
    QLabel* textLabel_Moment;
    QComboBox* comboBoxMoment;
    QLabel* textLabel_Pendant;
    QComboBox* comboBoxPendant;
    QPushButton* pushButton_NextPoso;
    QTextBrowser* textBrowser_Result1;
    QGroupBox* Infos_Techniques;
    QLineEdit* lineEdit_Value;
    QTabWidget* tabWidgetPosoAdjust;
    QWidget* tab;
    QListView* listView_Coef;
    QWidget* tab_2;
    QTextBrowser* textBrowser_Result2;

    CMoteurBase* m_pCMoteurBase;
    Type_Posologie m_Poso;
    float m_UnitByCps_Max;
    float m_UT;
    float m_DoseByUT_Max;
    float m_DoseByUT_Min;
    int m_Poids;
    int m_ParPoidsUT;
    int m_NumInOrdo;
    int m_Secabilite;
    float m_RatioMin;
    float m_RatioMax;
    CMDI_Terrain *m_pCMDI_Terrain;

    virtual Type_Posologie CalculeAdequationPoso( Type_Posologie & posoOri, const QString & qsPoids, const QString & qsTaille );
    virtual float ArondirUniteDePrise( float value, int secabilite );
    virtual float ArondirUniteDePrise( QString value, int secabilite );
    virtual QString ComboMomentGetValue( float & nb_fois );
    virtual float comboBoxPendantGetValue();

public slots:
    virtual void initDialog( CMoteurBase * pCmoteurBase, Type_Posologie type_Posologie, CMDI_Terrain * pCMDI_Terrain );
    virtual void GetAdequationPosoDec( QListView * pQlistView, Type_PosoDec & posoDec );
    virtual void lineEdit_Poids_textChanged( const QString & qsPoids );
    virtual void lineEdit_Taille_textChanged( const QString & qsTaille );
    virtual void comboBoxMoment_activated( const QString & );
    virtual void slider_VarPoso_valueChanged( int );
    virtual void pushButtonOK_clicked();
    virtual void radioButtonSecable1_clicked();
    virtual void radioButtonSecable2_clicked();
    virtual void radioButtonSecable4_clicked();
    virtual void comboBoxPendant_activated( const QString & );
    virtual void comboBoxPendant_textChanged( const QString & );
    virtual void pushButton_NextPoso_clicked();

protected:
    QGridLayout* DlgPosoAdjustLayout;
    QHBoxLayout* layout15;
    QHBoxLayout* layout13;
    QVBoxLayout* layout10;
    QVBoxLayout* layout11;
    QVBoxLayout* layout12;
    QGridLayout* groupBox1Layout;
    QHBoxLayout* layout2;
    QVBoxLayout* layout11_2;
    QHBoxLayout* layout10_2;
    QVBoxLayout* layout8;
    QVBoxLayout* layout6;
    QVBoxLayout* layout9;
    QGridLayout* Infos_TechniquesLayout;
    QGridLayout* tabLayout;
    QGridLayout* tabLayout_2;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;
    QPixmap image1;

    void init();

};

#endif // DLGPOSOADJUST_H
