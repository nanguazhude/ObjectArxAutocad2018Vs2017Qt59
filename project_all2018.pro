TEMPLATE = subdirs  

main_project.file =                 $$PWD/objectarx_2018.pro
load_qt_library_project.file =      $$PWD/Project/LoadQtApplication/LoadQtDlls2018.pro
set_mtext_background_project.file = $$PWD/Project/SetMTextBG/SBGABOF2018.pro
shx_text_view.file                = $$PWD/Project/ShxTextView/ShxTextView2018.pro

SUBDIRS += main_project
SUBDIRS += load_qt_library_project
SUBDIRS += set_mtext_background_project
SUBDIRS += shx_text_view
