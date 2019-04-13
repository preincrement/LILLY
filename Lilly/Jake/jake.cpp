#if not defined(__linux__) 
    #error Jake mag bisher nur Tux basierte Systeme - Tut mir leid :/
#endif


/**
 * @file jake.cpp
 * @author Florian Sihler
 * @version 1.0.7
 * @warning Die Aktuelle Version ist WIP einige Funktionnen sind noch nicht implementiert
 * 
 * 
 * @brief Hilfsprogramm im Umgang mit LILLY
 * 
 * @note Zum Kompilieren dieser Datei sollte das beiliegende Makefile verwendet werdne
 * 
 * @mainpage
 * @section README README.md
 * @include README.md
 * 
 * Folgende Seiten sind unter Umständen interesssant
 * - @ref Changelog
 * - @ref jake.cpp
 * 
 * @page Changelog Changelog
 * @tableofcontents
 *  @section Version100 Version 1.0.0 -- Penguin
 *      * Command-Line-Parser
 *      * Grundlegende Funktionalität
 *      * Kommentar-Grundstruktur
 * 
 * 
 */

/* ================================================================================================================== */        /* ################# */
/* Include Direktiven                                                                                                 */        /* ################# */
/* ================================================================================================================== */        /* ################# */

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <iterator>
#include <cstdlib>

/* ================================================================================================================== */        /* ################# */
/* Preprocessor Makros                                                                                                */        /* ################# */
/* ================================================================================================================== */        /* ################# */

/// @brief Kurzbeschreibung der Aktuellen Jake-Version
#define PRG_BRIEF "Jake 1.0.7 - A Servant for Lilly"

/// @brief Design-Makro zum Zurücksetzen der Farbe
#define COL_RESET "\033[m"
/// @brief Design-Makro zum Setzen der Fehler-Farbe in der Konsole
#define COL_ERROR "\033[38;2;255;32;82m"
/// @brief Design-Makro zum Setzen der Erfolgs-Farbe in der Konsole
#define COL_SUCCESS "\033[38;2;102;250;0m"
/// @brief Design für Parameter
#define STY_PARAM "\033[2;3;51m"

/// @brief Signatur mit der ein single-Argument beginnt
#define ARG_PATTERN "-"
/// @brief Signatur einer Zuweisung
#define ASS_PATTERN "="
/// @brief Signatur einer Hinzufügenden Zuweisung
#define ASA_PATTERN "+="
/// @brief Signatur einer TeX/LaTeX-Datei
#define TEX_PATTERN ".tex"

/// @brief convenience-wrapper
#define TO_DATA(x) x

/* ================================================================================================================== */        /* ################# */
/* Zusätzliche Datentypen                                                                                             */        /* ################# */
/* ================================================================================================================== */        /* ################# */

/**
 * @struct fkt_descriptor
 * @brief Repräsentiert eine Methode der Signatur func_t mit Beschreibung
 * 
 * Wird in @ref functions_t benutzt
 */
struct fkt_descriptor;

/* ================================================================================================================== */        /* ################# */
/* usings und typedefs                                                                                                */        /* ################# */
/* ================================================================================================================== */        /* ################# */

/// @brief Datentyp für die Einstellungen
using data_t = std::string;

/** 
 * @brief Rückabewert für Funktionen 
 * @todo Funktionalität einbauen
 */
using status_t = uint8_t;

/// @brief Funktions-Signatur für erlaubte Operationen
typedef status_t (* func_t)(const std::string&);

/// @brief Mappings-Signatur für die Einstellungen
using settings_t = std::map<std::string, data_t>;

/// @brief Signatur fürs Funktionsmapping
using functions_t = std::map<std::string, fkt_descriptor>;

/* ================================================================================================================== */        /* ################# */
/* Globale Variablen                                                                                                  */        /* ################# */
/* ================================================================================================================== */        /* ################# */

/// @brief argv[0] - zur Einfachheit tihihihi
std::string program; 

/* ================================================================================================================== */        /* ################# */
/* Hilfsfunktionen                                                                                                    */        /* ################# */
/* ================================================================================================================== */        /* ################# */

/**
 * @brief Lesbarer Test ob char* in char*
 * 
 * @param str Der String in dem gesucht werden soll
 * @param seq Die zu suchende Sequenz
 * 
 * @returns true - wenn die Sequenz enthalten ist, sonst false
 */
inline status_t in_str(const char* str,const char* seq){
    return (std::string(str).find(seq) != std::string::npos);
}

/**
 * @brief Lesbare Ausgabe einer unbekannten Einstellung
 * 
 * @param setting die Einstellung die unbekannt ist
 */
inline void er_unknown_setting(const char* setting){
    std::cerr << COL_ERROR << "Die Einstellung: \"" << setting << "\" ist so nicht gültig!" << std::endl << "Schreib \""
              << program << " help\"" << COL_RESET << std::endl;
}

/**
 * @brief Dekodiert Rückgabewerte Menschenlesbar
 * 
 * @param code GNU Fehlercode
 * 
 * @returns SUCCESS wenn == 0, sonst: ERROR (code)
 */
inline std::string er_decode(int code) {
    return ((code)?std::string(COL_ERROR) + "ERROR (" + std::to_string(code) +")"
                  :(std::string(COL_SUCCESS) + "SUCCESS")) + COL_RESET;
}
/**
 * @brief Fügt ähnlich iomanip's setw Leerfelder ein
 *
 * @param entry Eintrag der mit (left) pad ausgegeben werden soll
 * @param toWidth Breite auf die der Pad gesetzt werden soll
 *
 * @note Sollte toWidth <= der Länge von entry sein, so wird lediglich entry zurück gegeben!
 *
 * @returns formatierte Zeichenkette
 */
std::string padPrint(const std::string& entry, uint16_t toWidth=30);

/* ================================================================================================================== */        /* ################# */
/* Signaturen                                                                                                         */        /* ################# */
/* ================================================================================================================== */        /* ################# */

/**
 * @brief Gibt Hilfe zur Verwendung von lilly_jake
 * 
 * @note Die möglichen Optionen werden über die Funktionen generiert
 * @todo Implementiere Rückgabewert
 * 
 * @param cmd erfüllt bisher nur den Zweck der Signatur von func_t gerecht zu werden 
 * 
 * @returns Statuswert (bisher nichts)
 */
status_t fkt_help (const std::string& cmd) noexcept;

/**
 * @brief Gibt die aktuellen Settings aus
 * 
 * @todo Implementiere Rückgabewert
 *
 * @param cmd erfüllt bisher nur den Zweck der Signatur von func_t gerecht zu werden 
 * 
 * @returns Statuswert (bisher nichts)
 */
status_t fkt_dump(const std::string& cmd) noexcept;

/**
 * @brief Erstellt ein Makefile für die mit settings["file"] angegebene Datei
 * 
 * @todo Implementiere Funktion
 *
 * @param cmd erfüllt bisher nur den Zweck der Signatur von func_t gerecht zu werden 
 * 
 * @returns Statuswert (bisher nichts)
 */
status_t fkt_compile(const std::string& cmd);

/**
 * @brief Versucht LILLY zu installieren
 *
 * @param cmd erfüllt bisher nur den Zweck der Signatur von func_t gerecht zu werden 
 * 
 * @returns Statuswert (bisher nichts)
 */
status_t fkt_install(const std::string& cmd) noexcept;

/**
 * @brief Lädt die Einstellung auf Basis der Kommandozeilen argumente
 *
 * @todo Implementiere Rückgabewert
 *
 * @param n Anzahl der Argumente die Übergeben werden
 * @param argv Array der Kommandozeilenargumente (alternativ beliebiges array)
 * 
 * @returns Statuswert (bisher nichts)
 */
status_t ld_settings(int n /* = argc */, const char** argv);


/**
 * @brief Installationsregel für Linux-Basierte Systeme - nutzt konfigurationen der settings
 * 
 * @todo Implementiere Rückgabewert
 *
 * @returns Statuswert (bisher nichts)
 */
status_t ins_linux( void );

/* ################################################################################################################## */        /* ################# */
/* Implementationen                                                                                                   */        /* ################# */
/* ################################################################################################################## */        /* ################# */

/* ================================================================================================================== */        /* ################# */
/* Strukturen - Mappings                                                                                              */        /* ################# */
/* ================================================================================================================== */        /* ################# */

struct fkt_descriptor {
    /// @brief Die Funktion die Beschrieben wird
    func_t fkt;
    /// @brief Die Funktionsbeschreibung
    std::string brief_description;
};

/// @brief Liste aller unterstützer Operationen
functions_t functions {
    {"help", {fkt_help, "Zeigt diese Hilfe an"}},
    {"dump", {fkt_dump, "Zeigt alle settings und ihre Werte an" }},
    {"file_compile", {fkt_compile, "Erstellt ein makefile für settings[\"file\"]"}},
    {"install", {fkt_install, "Versucht LILLY zu installieren"}}
};

/// @brief Speicherstruktur für Einstellungen
settings_t settings {
    {"operation",           "help"},
    {"file",                "none.tex"},
    {"debug",               "false"},
    {"path",                "./"},
    {"install-path",        "\"${HOME}/texmf\""},
    {"lilly-path",          "\"${PWD}/../../Lilly\""},
    {"mk-name",             "makefile.lilly"},
    {"mk-path",             "./"},
    {"lilly-out",           "./$(BASENAME)-OUT/"}, // issues mkdir inside of makefile -- OS-Slave
    {"lilly-in",            "./"},
    {"lilly-nameprefix",    ""},
    {"lilly-boxes",         "DEFAULT"},
    {"lilly-cleans",        "log aux out ind bbl blg lof lot toc idx acn acr alg glg glo gls fls fdb_latexmk auxlock md5 SATZE ZSM UB TOP listing upa ilg TOPIC "},
    {"lilly-compiletimes",  "3"}, // wie oft soll pdflatex aufgerufen werden?
    {"jobcount",            "2"},
};


/* ================================================================================================================== */        /* ################# */
/* Installationsregeln                                                                                                */        /* ################# */
/* ================================================================================================================== */        /* ################# */

status_t ins_linux( void ) {
    int fb;
    std::cout << "  - Prüfe auf das Vorhandensein von pdflatex: "
              << er_decode(fb = system("which pdflatex"))
              << std::endl;
    
    if(fb) {
        char c_inp = '\0';
        std::cout << "  Jake kann 'pdflatex' nicht finden, dies ist für die Arbeit mit Lilly zwanghaft notwendig!" << std::endl
                  << "  Soll 'texlive-full' installiert werden?  [(y)es/(n)o]> ";
        while(c_inp != 'y' && c_inp != 'n')
            std::cin >> c_inp;
        if(c_inp == 'y') {
            std::cout << "Installiere 'texlive-full' via _apt_ : " << er_decode(system("sudo apt install texlive-full")) << std::endl;
        } else {
            std::cout << "  Jake installiert LILLY nun weiter, ohne 'pdflatex', da  du (n)o gewählt hast!" << std::endl;
        }
    }  

    std::cout << "  - Erstelle (" <<  settings["install-path"] << "/tex/latex): "
              << er_decode(system(("mkdir -p " + settings["install-path"] + "/tex/latex").c_str()))
              << std::endl;

    std::cout << "  - Verlinke (" << settings["lilly-path"] << ") nach (" << settings["install-path"] << "/tex/latex): "
              << er_decode(system(("ln -sf "+settings["lilly-path"]+" "+settings["install-path"]+"/tex/latex").c_str()))
              << std::endl;
    if(settings["lilly-path"]=="\"${PWD}/../../Lilly\"")
    std::cout << "    Information: es ist immer besser, wenn du den absoluten Pfad zu Lilly angibst. Nutze hierzu: " 
              << std::endl << "    jake -lilly-path=\"/pfad/zum/kuchen\" install" << std::endl;

    std::cout << "  - Informiere TEX über (" << settings["install-path"] << "): "
              << er_decode(system(("texhash " + settings["install-path"]).c_str()))
              << std::endl;
              
    std::cout << "Installationsprozess wurde abgeschlossen :D" << std::endl;
    
    return EXIT_SUCCESS;
}



/* ================================================================================================================== */        /* ################# */
/* Operationen                                                                                                        */        /* ################# */
/* ================================================================================================================== */        /* ################# */

std::string padPrint(const std::string& entry, uint16_t toWidth) {
    if (toWidth<=entry.length()) return entry;
    return entry + std::string(toWidth-entry.length(), ' ');
}


status_t fkt_dump(const std::string& cmd) noexcept {
    std::cout << "Settings Dump: " << std::endl;
    settings_t::iterator it = settings.begin();
    while(it != settings.end()){ // iterate over all settings 
        // simple padding without <iomanip> std::setw
        std::cout << it->first << ": " << std::string(20-it->first.length(),' ') << STY_PARAM << it->second << COL_RESET << std::endl; 
        ++it;
    }
    return EXIT_SUCCESS;
}

status_t fkt_help(const std::string& cmd) noexcept {
    std::cerr << "Benutzung:" << std::endl << std::endl;
    std::cerr << program << " [options=help] [file]" << std::endl << std::endl;
    std::cerr << "[options]:" << std::endl;
    functions_t::iterator it = functions.begin();
    while(it != functions.end()){ // iterate over all functions 
        std::cout << "  " << it->first << " " << std::string(15-it->first.length(),' ')<< it->second.brief_description 
                  << std::endl;                                 // simple padding without <iomanip> std::setw
        ++it;
    }
    std::cerr << std::endl << "[file]:" << std::endl
              << "Angabe genäß \"xxx.tex\". Dies setzt die Operation automatisch auf file_compile und generiert damit \
                  ein generelles makefile für \"xxx.tex\"."
              << std::endl;

    std::cerr << std::endl << "note:" << std::endl
              << "Allgemeine Einstellungen können über \"-key=value\" gesetzt werden (\"-key\" für boolesche). So "
              << "setzt: \"-path=/es/gibt/kuchen\" die Einstellung settings[\"path\"] auf besagten Wert: "
              << "\"/es/gibt/kuchen\". Weiter ist es möglich mit '+=' values hinzuzufügen." 
              << std::endl;
    return EXIT_SUCCESS;
}

status_t ld_settings(int n /* = argc */, const char** argv) {
    for(; n > 0; n--) {
        if(!in_str(argv[n],ARG_PATTERN)){                       // not a single argument
            if(!in_str(argv[n],TEX_PATTERN)) {                  // not a .tex file
                settings["operation"] = TO_DATA(argv[n]);
            } else {                                            // is a .tex file
                settings["operation"] = "file_compile";
                settings["file"] = TO_DATA(argv[n]);
            }
        } else {                                                // is a single argument
            if(!in_str(argv[n], ASS_PATTERN)) {                 // not an assignment
                if(settings.find(argv[n] + 1 /* offset for '-' */) != settings.end()
                    && settings[argv[n]+1] == "false") {        // valid bool-setting
                    settings[argv[n]+1] = "true";
                } else {                                        // non valid bool-setting
                    er_unknown_setting(argv[n]+1);
                }
            } else {                                            // is an assignment
                std::string s{argv[n]+1};
                if(in_str(argv[n], ASA_PATTERN)) {              // is an addition statement
                    settings[s.substr(0,s.find(ASA_PATTERN))] += s.substr(s.find(ASA_PATTERN)+std::string(ASA_PATTERN).length());
                } else {                                        // normal statement
                    settings[s.substr(0,s.find(ASS_PATTERN))]  = s.substr(s.find(ASS_PATTERN)+std::string(ASS_PATTERN).length());
                }
            }
        }
    }
    return EXIT_SUCCESS;
}

status_t fkt_install(const std::string& cmd) noexcept { 
//Determine Operating System
#if defined(__linux__)
    std::cout << "Betriebsystem wurde als Linux-Basiert identifiziert - versuche ins_linux()" << std::endl;
    ins_linux();
#else 
    std::cerr << "Es existiert keine Installationsregel für dein Betriebssystem :/ - Bitte melde dich beim Maintainer dieses Pakets!" << std::endl;
#endif
   return EXIT_SUCCESS; 
}

status_t fkt_compile(const std::string& cmd) {
    std::cout << "Generiere Makefile für Datei: " << settings["file"] << std::endl;
    std::ofstream out_makefile(settings["path"]+"/"+settings["mk-name"], std::ofstream::out); 
   
    // Generiere Notwendige Ordnerstruktur für Ein- und Ausgabedateien!
    std::cout << "    Erstelle Ordner settings[\"mk-path\"] (" << settings["mk-path"] << ") " 
              << er_decode(system(("mkdir -p " + settings["mk-path"]).c_str())) << std::endl; // OS - BARRIER
    
    
    out_makefile << "# " << PRG_BRIEF << "     (compiled on: " << __DATE__ << " at: " << __TIME__ << ")" << std::endl << std::endl;
    
    // Einfügen der Variablen des Makefiles, es erhält eine andere Struktur wie das, welches von lilly_compile zur Verfügung gestellt wird!!
    out_makefile << "TEXFILE      := " << settings["file"]                                                                      << std::endl    
                 << "BASENAME     := $(basename $(TEXFILE))"                                                                    << std::endl
                 << "PDFFILE      := $(addsuffix .pdf,$(basename $(TEXFILE)))"                                                  << std::endl
                 << "LATEXARGS    := -shell-escape -enable-write18 -interaction=batchmode"                                      << std::endl 
                 // lilly- settings 
                 << "## Directories used for INPUT and OUTPUT Files "                                                           << std::endl
                 << "OUTPUTDIR    := " << settings["lilly-out"]                                                                 << std::endl
                 << "INPUTDIR     := " << settings["lilly-in"]                                                                  << std::endl
                 << "BOXMODES     := " << padPrint(settings["lilly-boxes"])                 << "## Seperator: ' '"              << std::endl
                 << "CLEANTARGETS := " << settings["lilly-cleans"]                                                              << std::endl
                 // lilly- names
                 << "NAMEPREFIX   := " << padPrint(settings["lilly-nameprefix"])            << "## Immer"                       << std::endl  
                 << std::endl
                 //Generals
                 << "## Makefile/General settings"                                                                              << std::endl
                 << R"(_LILLYARGS   :=  \\\providecommand{\\\LILLYxDOCUMENTNAME{$(TEXFILE)}} $(DEBUG) \\\providecommand{\\\LILLYxPATH}{${INPUTDIR}} \\input{$(INPUTDIR)$(TEXFILE)} )" << std::endl << std::endl //DONT'T Change
                 << "JOBCOUNT     := " << padPrint(settings["jobcount"])                    << "## <= CPU_COUNT! "              << std::endl
                 << std::endl << std::endl;
    //Compile-Regel
    out_makefile << "define LILLYxCompile = "                                                                                   << std::endl;
    //clean log
    out_makefile << R"( @echo LOGFILE: $(date +'%d.%m.%Y %H:%M:%S') > $(OUTPUTDIR)LILLY_COMPILE.log 2>&1)"                      << std::endl;
    for(int i = 0; i < std::stoi(settings["lilly-compiletimes"]); i++) {
        out_makefile << "   @pdflatex -jobname $(BASENAME) $(LATEXARGS) $(_LILLYARGS) ${2}" << R"(\\\providecommand{\\\LILLYxPDFNAME}{${1}} >> $(OUTPUTDIR)LILLY_COMPILE.log 2>&1)" << std::endl;
    }
    
    // call clean routine if clean is enabled :D
    //TODO: 
    out_makefile << "endef" << std::endl;



    out_makefile.close(); // PREFIXES FOR DIFFERENT RULES DEFINE WITH: --rule=name:specials or --rule:name:specials
   
    // TODO:
    // add cleantarget, jobcount, debug, general: options for remote compile, etc.
    // jake prepare document: expandable macros
    return EXIT_SUCCESS;
}

/* ================================================================================================================== */        /* ################# */
/* Main                                                                                                               */        /* ################# */
/* ================================================================================================================== */        /* ################# */

/**
 * @brief startet Jake - jippie
 * 
 * @param argc Kommandozeilenargumentanzahl
 * @param argv Kommandozeilenargumentearray
 * 
 * @returns status
 */

int main(int argc, const char** argv) {
    program = argv[0];
    
    ld_settings(argc-1, argv);
    //fkt_help(argv[0]);
    if(functions.find(settings["operation"]) != functions.end()) {                   // Operation ist valide
            functions[settings["operation"]].fkt(argv[0]);                           // Führe Operation aus
    } else {
        er_unknown_setting(("operation (=" + settings["operation"] + ")").c_str());  // Diese Operation kenne ich nicht
    }
    
    return EXIT_SUCCESS;
}

/* ================================================================================================================== */        /* ################# */
/* Makefiledump                                                                                                       */        /* ################# */
/* ================================================================================================================== */        /* ################# */

/*
## Makefile for jake.cpp

define WRITE_RC =
	@if grep -q "#LILLY_CODE" "$(1)"; then\
		echo "Already prepped $(1)";\
	else\
		echo "#LILLY_CODE\nexport PATH=\044PATH:$(shell pwd)"  >> $(1);\
	fi
endef

default: no_link
	@#Write for zshell
	$(call WRITE_RC,"${HOME}/.zshrc")
	@#Write for bash
	$(call WRITE_RC,"${HOME}/.bashrc")
	
	@export PATH=${PATH}:$(shell pwd)
	@chmod +x ./lilly_jake

no_link: jake.cpp
	g++ -std=c++11 -I './' jake.cpp -o lilly_jake
*/

// TODO:Prepare for github-launch
