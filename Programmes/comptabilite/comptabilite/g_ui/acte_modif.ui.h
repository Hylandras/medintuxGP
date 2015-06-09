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


void Acte_Modif::but_Annuler_clicked()
{ reject(); }


void Acte_Modif::but_Sauv_clicked()
{ accept();  }

void Acte_Modif::setNom(const char* n) {nom->setText(n); }
void Acte_Modif::setType(const char* n) {type->setText(n); }
void Acte_Modif::setDesc(const char* n) {desc->setText(n); }
void Acte_Modif::setTotal(const char* n) {total->setText(n); }
void Acte_Modif::setTiers(const char* n) {tiers->setText(n); }

QString Acte_Modif::getNom()   { return nom->text(); }
QString Acte_Modif::getType()  { return type->text(); }
QString Acte_Modif::getDesc()  { return desc->text(); }
QString Acte_Modif::getTotal() { return total->text(); }
QString Acte_Modif::getTiers() { return tiers->text(); }
