# Microsoft Developer Studio Generated NMAKE File, Based on PIC_EXTRACT.dsp
!IF "$(CFG)" == ""
CFG=PIC_EXTRACT - Win32 Debug
!MESSAGE No configuration specified. Defaulting to PIC_EXTRACT - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "PIC_EXTRACT - Win32 Release" && "$(CFG)" != "PIC_EXTRACT - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PIC_EXTRACT.mak" CFG="PIC_EXTRACT - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PIC_EXTRACT - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "PIC_EXTRACT - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "PIC_EXTRACT - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\PIC_EXTRACT.exe"


CLEAN :
	-@erase "$(INTDIR)\File.obj"
	-@erase "$(INTDIR)\FileTidy.obj"
	-@erase "$(INTDIR)\FileTinyXml.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\tinystr.obj"
	-@erase "$(INTDIR)\tinyxml.obj"
	-@erase "$(INTDIR)\tinyxmlerror.obj"
	-@erase "$(INTDIR)\tinyxmlparser.obj"
	-@erase "$(INTDIR)\TitleExtract.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\PIC_EXTRACT.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\PIC_EXTRACT.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\PIC_EXTRACT.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\PIC_EXTRACT.pdb" /machine:I386 /out:"$(OUTDIR)\PIC_EXTRACT.exe" 
LINK32_OBJS= \
	"$(INTDIR)\File.obj" \
	"$(INTDIR)\FileTidy.obj" \
	"$(INTDIR)\FileTinyXml.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\tinystr.obj" \
	"$(INTDIR)\tinyxml.obj" \
	"$(INTDIR)\tinyxmlerror.obj" \
	"$(INTDIR)\tinyxmlparser.obj" \
	"$(INTDIR)\TitleExtract.obj" \
	".\tidy.lib"

"$(OUTDIR)\PIC_EXTRACT.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "PIC_EXTRACT - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\PIC_EXTRACT.exe" "$(OUTDIR)\PIC_EXTRACT.bsc"


CLEAN :
	-@erase "$(INTDIR)\File.obj"
	-@erase "$(INTDIR)\File.sbr"
	-@erase "$(INTDIR)\FileTidy.obj"
	-@erase "$(INTDIR)\FileTidy.sbr"
	-@erase "$(INTDIR)\FileTinyXml.obj"
	-@erase "$(INTDIR)\FileTinyXml.sbr"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\main.sbr"
	-@erase "$(INTDIR)\tinystr.obj"
	-@erase "$(INTDIR)\tinystr.sbr"
	-@erase "$(INTDIR)\tinyxml.obj"
	-@erase "$(INTDIR)\tinyxml.sbr"
	-@erase "$(INTDIR)\tinyxmlerror.obj"
	-@erase "$(INTDIR)\tinyxmlerror.sbr"
	-@erase "$(INTDIR)\tinyxmlparser.obj"
	-@erase "$(INTDIR)\tinyxmlparser.sbr"
	-@erase "$(INTDIR)\TitleExtract.obj"
	-@erase "$(INTDIR)\TitleExtract.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\PIC_EXTRACT.bsc"
	-@erase "$(OUTDIR)\PIC_EXTRACT.exe"
	-@erase "$(OUTDIR)\PIC_EXTRACT.ilk"
	-@erase "$(OUTDIR)\PIC_EXTRACT.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\PIC_EXTRACT.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\PIC_EXTRACT.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\File.sbr" \
	"$(INTDIR)\FileTidy.sbr" \
	"$(INTDIR)\FileTinyXml.sbr" \
	"$(INTDIR)\main.sbr" \
	"$(INTDIR)\tinystr.sbr" \
	"$(INTDIR)\tinyxml.sbr" \
	"$(INTDIR)\tinyxmlerror.sbr" \
	"$(INTDIR)\tinyxmlparser.sbr" \
	"$(INTDIR)\TitleExtract.sbr"

"$(OUTDIR)\PIC_EXTRACT.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tidy.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\PIC_EXTRACT.pdb" /debug /machine:I386 /out:"$(OUTDIR)\PIC_EXTRACT.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\File.obj" \
	"$(INTDIR)\FileTidy.obj" \
	"$(INTDIR)\FileTinyXml.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\tinystr.obj" \
	"$(INTDIR)\tinyxml.obj" \
	"$(INTDIR)\tinyxmlerror.obj" \
	"$(INTDIR)\tinyxmlparser.obj" \
	"$(INTDIR)\TitleExtract.obj" \
	".\tidy.lib"

"$(OUTDIR)\PIC_EXTRACT.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("PIC_EXTRACT.dep")
!INCLUDE "PIC_EXTRACT.dep"
!ELSE 
!MESSAGE Warning: cannot find "PIC_EXTRACT.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "PIC_EXTRACT - Win32 Release" || "$(CFG)" == "PIC_EXTRACT - Win32 Debug"
SOURCE=.\File.cpp

!IF  "$(CFG)" == "PIC_EXTRACT - Win32 Release"


"$(INTDIR)\File.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PIC_EXTRACT - Win32 Debug"


"$(INTDIR)\File.obj"	"$(INTDIR)\File.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\FileTidy.cpp

!IF  "$(CFG)" == "PIC_EXTRACT - Win32 Release"


"$(INTDIR)\FileTidy.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PIC_EXTRACT - Win32 Debug"


"$(INTDIR)\FileTidy.obj"	"$(INTDIR)\FileTidy.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\FileTinyXml.cpp

!IF  "$(CFG)" == "PIC_EXTRACT - Win32 Release"


"$(INTDIR)\FileTinyXml.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PIC_EXTRACT - Win32 Debug"


"$(INTDIR)\FileTinyXml.obj"	"$(INTDIR)\FileTinyXml.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\main.cpp

!IF  "$(CFG)" == "PIC_EXTRACT - Win32 Release"


"$(INTDIR)\main.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PIC_EXTRACT - Win32 Debug"


"$(INTDIR)\main.obj"	"$(INTDIR)\main.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\tinystr.cpp

!IF  "$(CFG)" == "PIC_EXTRACT - Win32 Release"


"$(INTDIR)\tinystr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PIC_EXTRACT - Win32 Debug"


"$(INTDIR)\tinystr.obj"	"$(INTDIR)\tinystr.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\tinyxml.cpp

!IF  "$(CFG)" == "PIC_EXTRACT - Win32 Release"


"$(INTDIR)\tinyxml.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PIC_EXTRACT - Win32 Debug"


"$(INTDIR)\tinyxml.obj"	"$(INTDIR)\tinyxml.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\tinyxmlerror.cpp

!IF  "$(CFG)" == "PIC_EXTRACT - Win32 Release"


"$(INTDIR)\tinyxmlerror.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PIC_EXTRACT - Win32 Debug"


"$(INTDIR)\tinyxmlerror.obj"	"$(INTDIR)\tinyxmlerror.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\tinyxmlparser.cpp

!IF  "$(CFG)" == "PIC_EXTRACT - Win32 Release"


"$(INTDIR)\tinyxmlparser.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PIC_EXTRACT - Win32 Debug"


"$(INTDIR)\tinyxmlparser.obj"	"$(INTDIR)\tinyxmlparser.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\TitleExtract.cpp

!IF  "$(CFG)" == "PIC_EXTRACT - Win32 Release"


"$(INTDIR)\TitleExtract.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "PIC_EXTRACT - Win32 Debug"


"$(INTDIR)\TitleExtract.obj"	"$(INTDIR)\TitleExtract.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

