if BUILD_ABBREVIATIONS
MAYBE_ABBREVIATIONS=abbreviations
endif

if BUILD_ASTYLE
MAYBE_ASTYLE=astyle
endif

if BUILD_AUTOSAVE
MAYBE_AUTOSAVE=autosave
endif

if BUILD_CLASSWIZARD
MAYBE_CLASSWIZARD=classwizard
endif

if BUILD_CODECOMPLETION
MAYBE_CODECOMPLETION=codecompletion
endif

if BUILD_COMPILER
MAYBE_COMPILER=compilergcc
endif

if BUILD_DEBUGGER
MAYBE_DEBUGGER=debuggergdb
endif

if BUILD_DISPLAYEVENTS
MAYBE_DISPLAYEVENTS=displayevents
endif

if BUILD_MANIFEST
MAYBE_MANIFEST=xpmanifest
endif

if BUILD_MIMEHANDLER
MAYBE_MIMEHANDLER=defaultmimehandler
endif

if BUILD_OCCURRENCESHIGHLIGHTING
MAYBE_OCCURRENCESHIGHLIGHTING=occurrenceshighlighting
endif

if BUILD_OPENFILESLIST
MAYBE_OPENFILESLIST=openfileslist
endif

if BUILD_PROJECTSIMPORTER
MAYBE_PROJECTSIMPORTER=projectsimporter
endif

if BUILD_SCRIPTEDWIZARD
MAYBE_SCRIPTEDWIZARD=scriptedwizard
endif

if BUILD_TODO
MAYBE_TODO=todo
endif

SUBDIRS=$(MAYBE_ABBREVIATIONS) \
		$(MAYBE_ASTYLE) \
		$(MAYBE_AUTOSAVE) \
		$(MAYBE_CLASSWIZARD) \
		$(MAYBE_CODECOMPLETION) \
		$(MAYBE_COMPILER) \
		$(MAYBE_DEBUGGER) \
		$(MAYBE_DISPLAYEVENTS) \
		$(MAYBE_MANIFEST) \
		$(MAYBE_MIMEHANDLER) \
		$(MAYBE_PROJECTSIMPORTER) \
		$(MAYBE_OCCURRENCESHIGHLIGHTING) \
		$(MAYBE_OPENFILESLIST) \
		$(MAYBE_SCRIPTEDWIZARD) \
		$(MAYBE_TODO) \
		contrib
# the 'contrib' subdir contains its own filtering

DIST_SUBDIRS = abbreviations \
		astyle \
		autosave \
		classwizard \
		codecompletion \
		compilergcc \
		debuggergdb \
		defaultmimehandler \
		displayevents \
		occurrenceshighlighting \
		openfileslist \
		projectsimporter \
		scriptedwizard \
		todo \
		xpmanifest \
		contrib

EXTRA_DIST = headerguard \
		loghacker \
		modpoller \
		tidycmt

