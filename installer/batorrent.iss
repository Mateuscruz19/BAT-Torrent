[Setup]
AppName=BATorrent
AppVersion=1.0
AppPublisher=Mateuscruz19
AppPublisherURL=https://github.com/Mateuscruz19/BAT-Torrent
DefaultDirName={autopf}\BATorrent
DefaultGroupName=BATorrent
OutputDir=..\output
OutputBaseFilename=BATorrent-setup-x86_64
Compression=lzma2
SolidCompression=yes
SetupIconFile=..\src\images\logo1.ico
UninstallDisplayIcon={app}\BATorrent.exe
WizardStyle=modern
PrivilegesRequired=lowest

[Files]
Source: "..\release\*"; DestDir: "{app}"; Flags: recursesubdirs

[Icons]
Name: "{group}\BATorrent"; Filename: "{app}\BATorrent.exe"
Name: "{group}\Uninstall BATorrent"; Filename: "{uninstallexe}"
Name: "{autodesktop}\BATorrent"; Filename: "{app}\BATorrent.exe"; Tasks: desktopicon

[Tasks]
Name: "desktopicon"; Description: "Create a desktop shortcut"; GroupDescription: "Additional icons:"

[Run]
Filename: "{app}\BATorrent.exe"; Description: "Launch BATorrent"; Flags: nowait postinstall skipifsilent
