; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Verdant Sorrow"
#define MyAppVersion "1.0"
#define MyAppPublisher "My Company, Inc."
#define MyAppURL "https://www.example.com/"
#define MyAppExeName "VerdantSorrowRelease.exe"
#define MyAppAssocName MyAppName + " File"
#define MyAppAssocExt ".myp"
#define MyAppAssocKey StringChange(MyAppAssocName, " ", "") + MyAppAssocExt

[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{807F20E1-874C-4E9C-B264-E9D3DBFC7E25}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName}
ChangesAssociations=yes
DisableProgramGroupPage=yes
; Uncomment the following line to run in non administrative install mode (install for current user only.)
;PrivilegesRequired=lowest
OutputBaseFilename=VerdantSorrowInstaller
Compression=lzma
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "espanol"; MessagesFile: "compiler:Languages/Spanish.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "..\VerdantSorrow\bin\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\VerdantSorrow\VerdantSorrow\resources\*"; DestDir: "{app}\resources"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\VerdantSorrow\VerdantSorrow\libFLAC-8.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\VerdantSorrow\VerdantSorrow\libfreetype-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\VerdantSorrow\VerdantSorrow\libjpeg-9.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\VerdantSorrow\VerdantSorrow\libmodplug-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\VerdantSorrow\VerdantSorrow\libmpg123-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\VerdantSorrow\VerdantSorrow\libogg-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\VerdantSorrow\VerdantSorrow\libpng16-16.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\VerdantSorrow\VerdantSorrow\libtiff-5.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\VerdantSorrow\VerdantSorrow\libvorbis-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\VerdantSorrow\VerdantSorrow\libvorbisfile-3.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\VerdantSorrow\VerdantSorrow\libwebp-4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\VerdantSorrow\VerdantSorrow\resource.h"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\VerdantSorrow\VerdantSorrow\SDL2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\VerdantSorrow\VerdantSorrow\SDL2_image.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\VerdantSorrow\VerdantSorrow\SDL2_mixer.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\VerdantSorrow\VerdantSorrow\SDL2_net.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\VerdantSorrow\VerdantSorrow\SDL2_ttf.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\VerdantSorrow\VerdantSorrow\zlib1.dll"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Registry]
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocExt}\OpenWithProgids"; ValueType: string; ValueName: "{#MyAppAssocKey}"; ValueData: ""; Flags: uninsdeletevalue
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocKey}"; ValueType: string; ValueName: ""; ValueData: "{#MyAppAssocName}"; Flags: uninsdeletekey
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocKey}\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\{#MyAppExeName},0"
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocKey}\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\{#MyAppExeName}"" ""%1"""
Root: HKA; Subkey: "Software\Classes\Applications\{#MyAppExeName}\SupportedTypes"; ValueType: string; ValueName: ".myp"; ValueData: ""

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[Dirs]
Name: "{app}\resources"
