<?php
if (file_exists (__DIR__.'/connexion_serveur.php'))
  include_once ('connexion_serveur.php');

$base='DrTuxTest';
$base_compta='comptabilite';
$version="6.2";
$fontsize="10";
//
$date_format='fr';//other possible value =en or nothing : $date_format=""
//pour preremplir les zones de saisie
$cp_default="75015";
$ville_default="Paris";
$rdv_default['delafond']="Consultation";
//status des rdv
//La valeur 3 (non défini) remplace automatiquement Non attribué en cas de modification de rendez-vous
//Doit etre en coherence avec MedinTux (Themes...)
$status_rdv[0]='Absent';
$status_rdv[1]="Présent en salle d'attente";
$status_rdv[2]="Rendez-vous terminé";
$status_rdv[3]="Statut non défini";
$status_rdv[4]="Annulé";
$status_rdv[5]="Pas venu";
$status_rdv[6]="Déplacé";
$status_rdv[7]="En consultation";
$status_rdv[8]="En retard";
$status_rdv[9]="Non attribué";
$color_status["Absent"]='#FFB4A7';
$color_status["Présent en salle d'attente"]='#FFEAA2';
$color_status["Rendez-vous terminé"]='#D6FFDC';
$color_status["Statut non défini"]='#CFFDFF';
$color_status["Annulé"]='#CDE7FF';
$color_status["Pas venu"]='#CCD5FF';
$color_status["Déplacé"]='#D7C9FF';
$color_status["En consultation"]='#F6C9FF';
$color_status["En retard"]='#F6C9F0';
$color_status["Non attribué"]='#FFFFFF';

//donner ici un identifiant valide, tel que donne par la fonction date_default_timezone_get())
date_default_timezone_set('Europe/Paris');

//Ajouter ici les types standard et personnalisés de documents tels que définis dans Manager.ini.
//La variable doit toujours être conforme à l'exemple fourni. Il n'y a pas de limite au nombre de types que vous pouvez ajouter.
//les rubriques standard
$rubrique_perso[] = "Observation|ob|20030000";
$rubrique_perso[] = "Prescription libre|or|20020200";
$rubrique_perso[] = "Ordonnance structurée|or|20020100";
$rubrique_perso[] = "Terrain|tv|20060000";
$rubrique_perso[] = "Document|do|20080000";
$rubrique_perso[] = "Certificat|ce|20020300";
$rubrique_perso[] = "Courrier|co|20020500";
$rubrique_perso[] = "Notes avancées|ie|20090000";
$rubrique_perso[] = "Classeur|cl|20050000";

//les rubriques perso
$rubrique_perso[] = "Alertes|al|90010000";
//Les types d'antecedents
$atcd_types=array(
"Médicaux" => array("Neurologique","Cardio-vasculaire", "Endocrinien","Gastro-entéroligique","Métabolique","Pneumologique","Psychiatrique"),
"Chirurgicaux" => array("Traumatologique","Vasculaire","Neurologique", "Orthopédique","Uro-génital","Viscéral"),
"Sociaux" => array("RMI", "SDF" ,"Tutelle","CMU"),
"Familiaux" => array("Coronarien", "Psychiatrique","Respiratoire","Carcinologique"),
"Allergique" => array("Iode", "Acariens","Pollen","Chat"),
"Cognitif" => array("DP", "DPTDC","TDC")
);


//Le pied de page des documents imprimes
$pied_de_page='Membre d\'une Association de gestion agréée, le règlement des honoraires par chèque est accepté.';

//La préférence de tri des medicaments par nom ou par prix, par utilisateur.

//syntaxe = $drug_order['nomdelogin']='n' (nom) ou 'p' (prix)

$drug_order['delafond']='p';
?>
