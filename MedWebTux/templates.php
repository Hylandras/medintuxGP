<?php

//les modeles de documents. Creez-en autant que vous voulez, mais ils doivent avoir la meme terminaison entre les crochets.
//Si le contenu entre crochets est identique a la specialite medicale, ce sera le modele par defaut en creant un nouveau document, comme une observation par exemple.
/*Vous pouvez introduire des variables, qui seront automatiquement reprises dans les documents créés, selon la liste suivante :
  $nom
  $prenom
  $naissance
  $adresse
  $secu
  $profession
  $sexe (M ou F)
  $genre[$sexe] (e au féminin)
  $titre
*/
$titre_default['kine']="kiné"; 
$obs_default['kine']="10 séances de rééducation de ";
$type_default['kine']="20020200"; 
$titre_default['certificat']="certificat"; 
$obs_default['certificat']="


Je, soussigné, certifie que ".stripslashes($titre)." ".stripslashes($nom)." ".stripslashes($prenom).", $profession, né$genre[$sexe] le $naissance, demeurant ".stripslashes($adresse).", nécessite



Certificat établi à la demande de l'intéressé$genre[$sexe] et remis en main propre pour faire valoir ce que de droit.";
$type_default['certificat']="20020300"; 
$titre_default['ald']="ALD"; 
$obs_default['ald']='<hr /><p style="text-align:center" >Prescriptions relatives au traitement de l\'affection de longue durée reconnue (liste ou hors liste)<br />(AFFECTION EXONÉRANTE)</p><hr /><p><br /></p><hr  /><p style="text-align:center" >Prescriptions SANS RAPPORT avec l\'affection de longue durée<br />(MALADIES INTERCURRENTES)</p><hr />';
$type_default['ald']="20020200"; 
$titre_default['fiche_adminisrative']='Fiche administrative';
$obs_default['fiche_adminisrative']='<span style="font-family:Arial;font-weight:600;text-decoration:underline;color:#0000ff">Complément administratif</span><span style="font-family:Arial;color:#0000ff"> :<br /></span><span style="font-family:Arial;font-weight:600;text-decoration:underline;color:#0000ff"><br />ALD SS</span><span style="font-family:Arial"> :  <br /></span><span style="font-family:Arial;font-weight:600;text-decoration:underline;color:#0000ff">Jusqu\'à </span><span style="font-family:Arial;font-weight:600;text-decoration:underline">:</span><span style="font-family:Arial">  <br /><br /></span><span style="color:#0000ff">Médecin traitant</span>  : <br /><span style="font-family:Arial;font-weight:600"><br /></span><span style="font-family:Arial;font-weight:600;text-decoration:underline;color:#0000ff">Bénéficiaire d\'une aide financière</span><span style="font-family:Arial"> :   <br /></span><span style="font-family:Arial;text-decoration:underline"><br /></span><span style="font-family:Arial;font-weight:600;
text-decoration:underline;color:#0000ff">Coordonnées d\'une éventuelle Mutuelle</span><span style="font-family:Arial"> :</span><span style="font-family:Arial;font-weight:600">  <br /><br /></span><span style="font-family:Arial;font-weight:600;text-decoration:underline;color:#0000ff">Mesure de protection</span><span style="font-family:Arial"> :  </span><span style="font-family:Arial;font-weight:600"><br /><br /></span><span style="font-family:Arial;font-weight:600;text-decoration:underline;color:#0000ff">Coordonnées des  personnes au courant de la situation</span><span style="font-family:Arial"> :   <br /><br /></span><span style="font-family:Arial;color:#0000ff">Lien avec le patient</span><span style="font-family:Arial"> :   <br /></span><span style="font-family:Arial;color:#0000ff">Fréquence des visites</span><span style="font-family:Arial"> :   <br /><br /></span><hr /> <span style="font-family:Arial;font-weight:600;text-decoration:underline;color:#0000ff">Evaluations médico-
sociales</span><span style="font-family:Arial">  :</span>';
$type_default['fiche_adminisrative']='20090000';
$titre_default['infirmiere_soins']="infirmiere_soins"; 
$obs_default['infirmiere_soins']="Le $date,


Soins infirmiers à domicile tous les jours, dimanches et jours fériés compris ";
$type_default['infirmiere_soins']="20020200"; 
$titre_default['Médecin généraliste']="MG";
$obs_default['Médecin généraliste']="
<span style=\"font-family:Arial;color:#0000ff\"><b>Motif </b></span>:</b> 

ANTECEDENTS:

<span style=\"font-family:Arial;color:#0000ff\"><b>Maladies chroniques </b></span>:
<span style=\"font-family:Arial;color:#0000ff\"><b>Traitements de fond </b></span>:
<span style=\"font-family:Arial;color:#0000ff\"><b>Vaccins </b></span>:

Habitus :
	<span style=\"font-family:Arial;color:#0000ff\"><b>Tabac </b></span>: 
	<span style=\"font-family:Arial;color:#0000ff\"><b>Alcool </b></span>: 
	<span style=\"font-family:Arial;color:#0000ff\"><b>Autres toxiques </b></span>: 
	<span style=\"font-family:Arial;color:#0000ff\"><b>Activité physique </b></span>:

_____________ Constantes ______________________
Pouls :	TA : 	T°:   	Poids:  Peak Flow :
_______________________________________________

OBSERVATION:	

Subjectif : 
	<span style=\"font-family:Arial;color:#0000ff\"><b>Symptômes </b></span>: 
	<span style=\"font-family:Arial;color:#0000ff\"><b>Depuis </b></span>: 
	<span style=\"font-family:Arial;color:#0000ff\"><b>Contexte </b></span>:
	<span style=\"font-family:Arial;color:#0000ff\"><b>Traitements essayés </b></span>: 

Objectif : 

<span style=\"font-family:Arial;color:#0000ff\"><b>PARACLINIQUE</b></span>: 

<span style=\"font-family:Arial;color:#0000ff\"><b>Hypothèses diagnostiques </b></span>:

<span style=\"font-family:Arial;color:#0000ff\"><b>TRAITEMENT</b></span>:

CONCLUSION:
<span style=\"font-family:Arial;color:#0000ff\"><b>Certificat </b></span>            	:
<span style=\"font-family:Arial;color:#0000ff\"><b>Arrêt Travail (ou scolaire)</b></span> :    
<span style=\"font-family:Arial;color:#0000ff\"><b>Soins  </b></span>                             :
<span style=\"font-family:Arial;color:#0000ff\"><b>Arrêt Sport  </b></span>   		:    
ITT                 		: 

Fact agravants	</b></span>:     
<span style=\"font-family:Arial;color:#0000ff\"><b>Codage CCAM:";
$type_default['Médecin généraliste']="20030000";
$titre_default[1]="Courrier";
$obs_default[1]="

Cher confrère,

Je vous adresse ".stripslashes($titre)." ".stripslashes($nom)." ".stripslashes($prenom).", ".stripslashes($profession).", qui présente

Bien confraternellement";
$type_default[1]="20020500";
$titre_default['labo']="Labo";
$obs_default['labo']="
<b>Prise de sang</b>
NFS
Glycémie
Créatinine
Cholestérol - LDL et HDL
Triglycérides
Uricémie
Transaminases
Gamma GT
CRP
TSH ultrasensible
Ferritinémie
HIV";
$type_default['labo']="20020200";
$titre_default['vide']="Vide";
$obs_default['vide']="";
$type_default['vide']="20020200";
//le terrain par defaut
$terrain_modele="[Antécédents]
[Obstétrique]
Grossesse = non
Allaitement = non
[VAR Constantes de Base]
Modèle = Date, Pouls, 31536000, 100, 50, pps, Tachycardie, Bradycardie
Modèle = Date, SAO2, 31536000, 0, 92, %, , Hypoxie
Modèle = Date, Temp, 31536000, 40.5, 36, °, Hyperthermie, Hypothermie
Modèle = Date, Maxima, 31536000, 14, 9, cmHg, Hypertension, Hypotension
Modèle = Date, Minima, 31536000, 9.5, 5, cmHg, Hypertension, Hypotension
[VAR Poids]
Modèle = Date, Poids, 31536000, 200, 50, kg, Obésité, Amaigrissement
Modèle = Date, Taille, 31536000, 200, 50, cm, Gigantisme, Nanisme
Modèle = Date, Créatinine, 31536000, 10, 5, mg, Insuffisance rénale , 
Modèle = Date, Tour de taille, 31536000, 250, 70, cm, 30, cm(s)
Modèle = Date, Tour de hanches, 31536000, 250, 70, cm, 30, cm(s)
[VAR Pneumo]
Modèle = Date, Peak Flow, 31536000, 500, 400, L/min, , Obstruction
Modèle = Date, CV, 31536000, 5, 1, L, ,
Modèle = Date, VR, 31536000, 1, 0, L, ,
Modèle = Date, VEMS, 31536000, 4, 1, L, ,
[VAR Obstétrique]
Modèle = Date, Hauteur utérine, 31536000, 40, 0, cm, ,
Modèle = Date, Périmètre ombilical, 31536000, 120, 60, cm, ,
[Propriété Ordonnance]
[Traitement]
"
?>