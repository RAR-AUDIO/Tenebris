[Setup]
AppName=Tenebris
AppCopyright=Copyright (C) 2021 RAR-AUDIO
AppPublisher=RAR-AUDIO
AppVersion=1.0.0
VersionInfoVersion=1.0.0
DefaultDirName={pf}\RAR-AUDIO\Tenebris
DefaultGroupName=Tenebris
Compression=lzma2
SolidCompression=yes
OutputDir=.\
ArchitecturesInstallIn64BitMode=x64
OutputBaseFilename=Tenebris Installer
LicenseFile=license.rtf
SetupLogging=yes
; WizardImageFile=installer_bg-win.bmp
; WizardSmallImageFile=installer_icon-win.bmp
DisableWelcomePage=False
AlwaysShowGroupOnReadyPage=True
AlwaysShowDirOnReadyPage=True
TimeStampsInUTC=True
UninstallDisplayName=Tenebris Uninstaller
UsePreviousSetupType=False
UsePreviousTasks=False
UsePreviousLanguage=False
ShowTasksTreeLines=True
AppId={{AD049DD5-43F4-4DE7-9154-B9B0C0F57960}

[Types]
Name: "full"; Description: "Full installation"
Name: "custom"; Description: "Custom installation"; Flags: iscustom

[Messages]
WelcomeLabel1=Welcome to the Tenebris installer
SetupWindowTitle=Tenebris installer
SelectDirLabel3=The standalone application and supporting files will be installed in the following folder.
SelectDirBrowseLabel=To continue, click Next. If you would like to select a different folder (not recommended), click Browse.

[Components]
;Name: "vst3_32"; Description: "32-bit VST3 Plugin (.vst3)"; Types: full custom
Name: "vst3_64"; Description: "64-bit VST3 Plugin (.vst3)"; Types: full custom; Check: Is64BitInstallMode
Name: "manual"; Description: "User guide"; Types: full custom; Flags: fixed

[Dirs]
;Name: "{cf32}\Avid\Audio\Plug-Ins\Tenebris.aaxplugin\"; Attribs: readonly; Components:aax_32;
;Name: "{cf32}\VST3\RAR-AUDIO\Tenebris.vst3\Tenebris.vst3"; Attribs: readonly; Components: vst3_32
Name: "{cf64}\VST3\RAR-AUDIO\Tenebris.vst3"; Attribs: readonly; Components: vst3_64; Check: Is64BitInstallMode

[Files]
;Source: "..\build-win\Tenebris.vst3\*.*"; DestDir: "{cf32}\VST3\RAR-AUDIO\Tenebris.vst3\"; Flags: ignoreversion recursesubdirs; Components: vst3_32; Excludes: "\Contents\x86_64\*,*.pdb,*.exp,*.lib,*.ilk,*.ico,*.ini"
;Source: "..\build-win\Tenebris.vst3\Desktop.ini"; DestDir: "{cf32}\VST3\RAR-AUDIO\Tenebris.vst3\"; Flags: overwritereadonly ignoreversion; Attribs: hidden system; Components: vst3_32
;Source: "..\build-win\Tenebris.vst3\PlugIn.ico"; DestDir: "{cf32}\VST3\RAR-AUDIO\Tenebris.vst3\"; Flags: overwritereadonly ignoreversion; Attribs: hidden system; Components: vst3_32

Source: "..\build-win\vst3\x64\Release\Tenebris.vst3"; DestDir: "{cf64}\VST3\RAR-AUDIO\Tenebris.vst3\"; Flags: ignoreversion recursesubdirs; Components: vst3_64; Excludes: "\Contents\x86\*,*.pdb,*.exp,*.lib,*.ilk,*.ico,*.ini"; Check: Is64BitInstallMode
Source: "..\build-win\Tenebris.vst3\Desktop.ini"; DestDir: "{cf64}\VST3\RAR-AUDIO\Tenebris.vst3\"; Flags: overwritereadonly ignoreversion; Attribs: hidden system; Components: vst3_64; Check: Is64BitInstallMode
Source: "..\build-win\Tenebris.vst3\PlugIn.ico"; DestDir: "{cf64}\VST3\RAR-AUDIO\Tenebris.vst3\"; Flags: overwritereadonly ignoreversion; Attribs: hidden system; Components: vst3_64; Check: Is64BitInstallMode

Source: "..\README.md"; DestDir: "{app}"; DestName: "README.md"; Flags: isreadme

[Icons]
Name: "{group}\Tenebris"; Filename: "{app}\Tenebris.exe"
;Name: "{group}\User guide"; Filename: "{app}\Tenebris manual.pdf"
Name: "{group}\Changelog"; Filename: "{app}\changelog.txt"
Name: "{group}\readme"; Filename: "{app}\readme.md"
Name: "{group}\Uninstall Tenebris"; Filename: "{app}\unins000.exe"

[Code]
var
  OkToCopyLog : Boolean;
  VST2DirPage_32: TInputDirWizardPage;
  VST2DirPage_64: TInputDirWizardPage;

procedure InitializeWizard;
begin
  if IsWin64 then begin
    VST2DirPage_64 := CreateInputDirPage(wpSelectDir,
    'Confirm 64-Bit VST2 Plugin Directory', '',
    'Select the folder in which setup should install the 64-bit VST2 Plugin, then click Next.',
    False, '');
    VST2DirPage_64.Add('');
    VST2DirPage_64.Values[0] := ExpandConstant('{reg:HKLM\SOFTWARE\VST,VSTPluginsPath|{pf}\Steinberg\VSTPlugins}\');

    VST2DirPage_32 := CreateInputDirPage(wpSelectDir,
      'Confirm 32-Bit VST2 Plugin Directory', '',
      'Select the folder in which setup should install the 32-bit VST2 Plugin, then click Next.',
      False, '');
    VST2DirPage_32.Add('');
    VST2DirPage_32.Values[0] := ExpandConstant('{reg:HKLM\SOFTWARE\WOW6432NODE\VST,VSTPluginsPath|{pf32}\Steinberg\VSTPlugins}\');
  end else begin
    VST2DirPage_32 := CreateInputDirPage(wpSelectDir,
      'Confirm 32-Bit VST2 Plugin Directory', '',
      'Select the folder in which setup should install the 32-bit VST2 Plugin, then click Next.',
      False, '');
    VST2DirPage_32.Add('');
    VST2DirPage_32.Values[0] := ExpandConstant('{reg:HKLM\SOFTWARE\VST,VSTPluginsPath|{pf}\Steinberg\VSTPlugins}\');
  end;
end;

function GetVST2Dir_32(Param: String): String;
begin
  Result := VST2DirPage_32.Values[0]
end;

function GetVST2Dir_64(Param: String): String;
begin
  Result := VST2DirPage_64.Values[0]
end;

procedure CurStepChanged(CurStep: TSetupStep);
begin
  if CurStep = ssDone then
    OkToCopyLog := True;
end;

procedure DeinitializeSetup();
begin
  if OkToCopyLog then
    FileCopy (ExpandConstant ('{log}'), ExpandConstant ('{app}\InstallationLogFile.log'), FALSE);
  RestartReplace (ExpandConstant ('{log}'), '');
end;

[UninstallDelete]
Type: files; Name: "{app}\InstallationLogFile.log"