TEMPLATE = subdirs  

main_project.file =                 $$PWD/objectarx.pro
load_qt_library_project.file =      $$PWD/Project/LoadQtApplication/LoadQtDlls.pro
set_mtext_background_project.file = $$PWD/Project/SetMTextBG/SBGABOF.pro

SUBDIRS += main_project
SUBDIRS += load_qt_library_project
SUBDIRS += set_mtext_background_project

