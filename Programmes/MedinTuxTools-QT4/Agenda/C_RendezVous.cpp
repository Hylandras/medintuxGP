/*********************************** CMoteurAgenda.cpp ********************
*  #include "CMoteurAgenda.h"           QT4-ready                         *
* ........................................................................*
*   Project   :  MedinTux  (typographie made in france)                   *
*   Copyright :  (C) 2005-2006-2007-2008-2009-2010 and for the eternity   *
*   by        :  Sevin Roland     from   MedinTux project                 *
*   E-Mail    :  roland-sevin@medintux.org                                *
*   Web site  :  www.medintux.org                                         *
* ........................................................................*
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/

#include "C_RendezVous.h"


//---------------------------- getQWhatsThisString ------------------------------------------------
QString C_RendezVous::getQWhatsThisString(   const QString &pathTheme, QString color)
{QString                                     mess  = "";
                                             mess += "<html><BODY BGCOLOR=\"#fff8dc\">";                                            
                                             mess += QString("<img src=\"") + pathTheme + "Agenda/AgendaToolTipHead.png\" /><br />";   // un bandeau pour elargir le tool tip
                                             mess += tr("<font color=\"#000000\"><b>Dated&nbsp;:&nbsp;</b></font><font color=\"#7f0000\"><b>\t%1 at <u>%2</u></b></font><font color=\"#000000\"><br />"
                                                        "Scheduled duration :<b>%3&nbsp; minutes</b><hr>"
                                                       ).arg(m_date.toString("ddd dd MMMM yyyy"),m_date.time().toString("hh:mm"),QString::number(m_Duree));
 if (m_Nom.length() || m_Prenom.length())    mess += ""+tr("Appointment  of:&nbsp;<font color=\"#7f0000\"><b>") + m_Nom + "&nbsp;" + m_Prenom+"</b></font>";
 else                                        mess += tr("<i>Anonymous appointment:&nbsp;<b>name - first name not filled&nbsp;</b></i>");
 if (m_Tel.length())                         mess += tr("<br>Phone&nbsp;:<b>")    + m_Tel;
                                             mess += "</b> <hr>";
                                             mess += tr("With:&nbsp;<b>")    + m_PrisAvec + tr("</b>Taken by:<b>")+m_PrisPar+"</b>&nbsp;<br />";
 if (m_Type.length())
                                            {mess += tr("<b>Cause: </b>")   + m_Type   ;
                                             mess += "<TABLE cellSpacing=\"2\"  cellpadding=\"0\" width=100 border=\"1\">"
                                                      "<TBODY>"
                                                      "<TR>"
                                                      "<TD width=100 align=\"center\" bgcolor = \""+color+"\" >"+m_Type+"</TD>"
                                                      "</TR>"
                                                      "</TBODY>"
                                                      "</TABLE><br>";
                                            }
                                             //mess += tr("<b>Dur\303\251e&nbsp;pr\303\251vue&nbsp;:&nbsp;</b>")    + QString::number(m_Duree) +tr("&nbsp; minutes<br>");
 if (m_Note.length())                        mess += tr("<hr><b>Note&nbsp;:&nbsp;</b><i>%1</i><br>").arg(m_Note);
 if (m_State.length())                       mess += tr("<b>Status: </b><i>%1</i><br></font>").arg(m_State);
 if (m_Where.length())                       mess += tr("<hr><b>Where: </b><i>%1</i><br></font>").arg(m_Where);
                                             mess += "</BODY></html>";
 return mess.replace("&NBSP;","&nbsp;");
}
//---------------------------- serialize ------------------------------------------------
QString C_RendezVous::serialize() const
{return serialize(this);
}
//---------------------------- serialize ------------------------------------------------
QString C_RendezVous::serialize(const C_RendezVous *pC_RendezVous)
{QString modeleXML =
 "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
 "<!-- Projet    : MedinTux                              -->\n"
 "<!-- Module    : Agenda                                -->\n"
 "<!-- Objet     : C_RendezVous                          -->\n"
 "<!-- guru mail : sevin-roland@medintux.org             -->\n"
 "<!-- Copyright : (C) 2004-to-2011 and for the eternity -->\n"
 "<C_RendezVous>\n"
 "   <version>1</version>\n";
      addXmlData("m_date",     pC_RendezVous->m_date.toString(Qt::ISODate), modeleXML,0);
      addXmlData("m_Duree",    QString::number(pC_RendezVous->m_Duree),     modeleXML,0);
      addXmlData("m_Nom",      pC_RendezVous->m_Nom,                        modeleXML,0);
      addXmlData("m_Prenom",   pC_RendezVous->m_Prenom,                     modeleXML,0);
      addXmlData("m_Type",     pC_RendezVous->m_Type,                       modeleXML,0);
      addXmlData("m_Tel",      pC_RendezVous->m_Tel,                        modeleXML,0);
      addXmlData("m_Note",     pC_RendezVous->m_Note,                       modeleXML,TRUE);
      addXmlData("m_GUID",     pC_RendezVous->m_GUID,                       modeleXML,0);
      addXmlData("m_PrisPar",  pC_RendezVous->m_PrisPar,                    modeleXML,0);
      addXmlData("m_PrisAvec", pC_RendezVous->m_PrisAvec,                   modeleXML,0);
      addXmlData("m_PrimKey",  pC_RendezVous->m_PrimKey,                    modeleXML,0);
      addXmlData("m_State",    pC_RendezVous->m_State,                      modeleXML,0);
      addXmlData("m_Where",    pC_RendezVous->m_Where,                      modeleXML,TRUE);
 modeleXML +=
 "</C_RendezVous>\n";
 return modeleXML;
}

//---------------------------- unSerialize ------------------------------------------------
C_RendezVous C_RendezVous::unSerialize(const QString & data)
{int nextPos=0;
 C_RendezVous rdv;
 rdv.m_date      = QDateTime::fromString(getXmlData("m_date", data, &nextPos),Qt::ISODate);
 rdv.m_Duree     = getXmlData("m_Duree",    data, &nextPos).toLong();
 rdv.m_Nom       = getXmlData("m_Nom",      data, &nextPos);
 rdv.m_Prenom    = getXmlData("m_Prenom",   data, &nextPos);
 rdv.m_Type      = getXmlData("m_Type",     data, &nextPos);
 rdv.m_Tel       = getXmlData("m_Tel",      data, &nextPos);
 rdv.m_Note      = getXmlData("m_Note",     data, &nextPos);
 rdv.m_GUID      = getXmlData("m_GUID",     data, &nextPos);
 rdv.m_PrisPar   = getXmlData("m_PrisPar",  data, &nextPos);
 rdv.m_PrisAvec  = getXmlData("m_PrisAvec", data, &nextPos);
 rdv.m_PrimKey   = getXmlData("m_PrimKey",  data, &nextPos);
 rdv.m_State     = getXmlData("m_State",    data, &nextPos);
 rdv.m_Where     = getXmlData("m_Where",    data, &nextPos);
 return rdv;
}

//------------------------ getRdvColor ---------------------------------------
QString C_RendezVous::getRdvColor(const C_RendezVous & rdv, MAP_COLOR *colorProfils)
{   QString background_color;
    if (rdv.m_Type.length())
       background_color = (*colorProfils)[rdv.m_Type].getColor();    // couleur du type rendez-vous.
    else
       background_color = "#FFFFFF";
    return background_color;
}

//---------------------------- getXmlData ------------------------------------------------
QString C_RendezVous::getXmlData(const QString& dataName, const QString& data, int *nextPos)
{bool mustBeB64Protected = FALSE;
 int posEnd  = -1;
 int posDeb  =  0;

 QString tag =  dataName+'>';
 if (nextPos) {posDeb  = data.indexOf(tag, *nextPos);*nextPos=0;}  // on  remet a zero au cas ou tag pas trouve (recommencer a zero)
 else         {posDeb  = data.indexOf(tag);}
 if (posDeb==-1)         return QString::null;
 if (data.at(posDeb-1)=='_') {posDeb += tag.length(); mustBeB64Protected = TRUE; tag = tag.prepend("</_");}
 else                        {posDeb += tag.length(); tag = tag.prepend("</");}
 posEnd  = data.indexOf(tag, posDeb);
 if (posEnd==-1)         return QString::null;
 if (nextPos) *nextPos = posEnd + tag.length();
 QString retour = "";
 if (mustBeB64Protected) retour = QString( QByteArray::fromBase64( data.mid(posDeb,posEnd-posDeb).toAscii() ) );
 else                    retour = data.mid(posDeb,posEnd-posDeb);
 retour.replace("&gt;",">");
 retour.replace("&lt;","<");
 return retour;
}
