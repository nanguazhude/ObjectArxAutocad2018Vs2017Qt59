TEMPLATE = subdirs  

main_project.file                 = $$PWD/objectarx.pro
load_qt_library_project.file      = $$PWD/Project/LoadQtApplication/LoadQtDlls.pro
set_mtext_background_project.file = $$PWD/Project/SetMTextBG/SBGABOF.pro
shx_text_view.file                = $$PWD/Project/ShxTextView/ShxTextView.pro
raw_text_edit.file                = $$PWD/Project/Raw/Raw.pro
text_random_render.file           = $$PWD/Project/TextRandomRender/TextRandomRender.pro

SUBDIRS += main_project
SUBDIRS += load_qt_library_project
SUBDIRS += set_mtext_background_project
SUBDIRS += shx_text_view
SUBDIRS += raw_text_edit
SUBDIRS += text_random_render
