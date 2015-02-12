TEMPLATE = app
TARGET = 
DEPENDPATH += .. ../src ../src/diff_match_patch
INCLUDEPATH += .. ../src/diff_match_patch ../src
QMAKE_LIBDIR += /usr/local/lib

# Input
HEADERS += ../src/command.h \
           ../src/diff_controller.h \
           ../src/diff_model_text.h \
           ../src/diff_selector_widget.h \
           ../src/diff_text.h \
           ../src/diff_text_edit.h \
           ../src/file_diffable.h \
           ../src/git_diffable.h \
           ../src/git/blob.h \
           ../src/git/diff_delta.h \
           ../src/git/diff_file.h \
           ../src/git/exception.h \
           ../src/git/oid.h \
           ../src/git/repository.h \
           ../src/git/status.h \
           ../src/git/status_entry.h \
           ../src/git/status_list.h \
           ../src/git/status_options.h \
           ../src/idiffable.h \
           ../src/range.h \
           ../src/strings.h \
           ../src/diff_match_patch/diff_match_patch.h
SOURCES += ../src/command.cpp \
           ../src/diff_controller.cpp \
           ../src/diff_model_text.cpp \
           ../src/diff_selector_widget.cpp \
           ../src/diff_text.cpp \
           ../src/diff_text_edit.cpp \
           ../src/file_diffable.cpp \
           ../src/git_diffable.cpp \
           ../src/git/blob.cpp \
           ../src/git/diff_delta.cpp \
           ../src/git/diff_file.cpp \
           ../src/git/exception.cpp \
           ../src/git/oid.cpp \
           ../src/git/repository.cpp \
           ../src/git/status.cpp \
           ../src/git/status_entry.cpp \
           ../src/git/status_list.cpp \
           ../src/git/status_options.cpp \
           ../src/main.cpp \
           ../src/strings.cpp \
           ../src/diff_match_patch/diff_match_patch.cpp
# LIBS += -lboost_program_options-mt
LIBS += -lboost_program_options
LIBS += -lgit2

release:DESTDIR = release
release:OBJECTS_DIR = $$DESTDIR/obj
release:MOC_DIR = $$DESTDIR/moc
release:RCC_DIR = $$DESTDIR/rcc
release:UI_DIR = $$DESTDIR/ui

debug:DESTDIR = debug
debug:OBJECTS_DIR = $$DESTDIR/obj
debug:MOC_DIR = $$DESTDIR/moc
debug:RCC_DIR = $$DESTDIR/rcc
debug:UI_DIR = $$DESTDIR/ui
debug:CONFIG += debug

