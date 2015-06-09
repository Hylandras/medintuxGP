# Fichier g�n�r� par le module QMake de KDevelop. 
# -------------------------------------------------- 
# Sous dossier relatif au dossier principal du projet�: .
# Cible : un sous-dossier du projet

CONFIG += release \
          warn_on \
          qt \
          opengl \
          thread \
          x11 \
          exceptions 
TEMPLATE = subdirs 
SUBDIRS += src 
