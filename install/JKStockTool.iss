; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "JKStockTool"
#define MyAppVersion "0.0.1"
#define MyAppPublisher "JK, Inc."
#define MyAppExeName "JKStockTool.exe"

#define AppSrcDir "exe\"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{C41141A7-A96D-42BA-BD7D-1123C071EE96}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={pf}\JKStockTool
DefaultGroupName={#MyAppName}
DisableProgramGroupPage=yes
LicenseFile=license.txt
; InfoBeforeFile=D:\Program Files (x86)\Inno Setup 5\license.txt
; InfoAfterFile=D:\Program Files (x86)\Inno Setup 5\license.txt
OutputDir=.\
OutputBaseFilename={#MyAppName}
SetupIconFile=..\JKStockTool.ico
Compression=lzma
SolidCompression=yes

[Languages]
; Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "chinese"; MessagesFile: "compiler:Languages\Chinese.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: checkedonce

[Files]
; Source: "..\x64\Debug\JKStockTool.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#AppSrcDir}*"; DestDir: "{app}\"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\卸载"; Filename: "{app}\unins000.exe"
Name: "{commonprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent



[Code]
{ ================================================================
  功能: 打开一个XML文档, 返回文档的Nodes对象
  参数: XMLFile     文档路径, 可以是有效的URL路径
        XMLRoot     文档的根节点对象
  返回: 无
}
procedure OpenXML(XMLFile: String; var XMLRoot: Variant);
var
    XMLDocument: Variant;
begin
  try
    XMLDocument := CreateOleObject('MSXML2.DOMDocument');
    XMLDocument.async := False;
    XMLDocument.resolveExternals := False;
    XMLDocument.load(XMLFile);
    XMLRoot := XMLDocument.documentElement;
  except
    RaiseException(ExpandConstant('{cm:LOADXMLFAILED}'));
  end;
end;

{ ================================================================
  功能: 在打开的XML文档中, 查找指定的节点
  参数: XMLNode   使用OpenXML打开的XML文档的节点
        Key       节点名字
        AttrName  节点属性名, 可以为空
        AttrValue 节点属性值, 可以为空
        FoundNode 找到的节点
  返回: 成功返回True 否则返回False
}
function FindXMLNodeByKey(XMLNode: Variant; KeyName: String; AttrName: String; AttrVal: String; var FoundNode: Variant) :Boolean;
var
  NodeIndex: Integer;
  Node: Variant;
begin
  Result := False;
  try
    if '' = AttrName then begin
      FoundNode := XMLNode.selectSingleNode(KeyName)
      Result := True;
    end else
      for NodeIndex := 0 to XMLNode.childNodes.length()-1 do begin
        Node := XMLNode.childNodes.item(NodeIndex);
        if (KeyName = Node.nodeName) and ( AttrVal = Node.Attributes.GetNamedItem(AttrName).value) then begin
            FoundNode := XMLNode.childNodes.item(NodeIndex);
            Result := True;
            break;
        end;
      end;
  except
    Result := False;
  end;
end;
 
{ ================================================================
  功能: 获取版本号
  参数: String
  返回: 成功返回5D版本号
}
 function GetVerson(Param: String): String;
 var
  sFile: String;
  xmlRoot: Variant;
  oVersionNode: Variant;
begin
  sFile := ExpandConstant('{app}\bin\BIM5D.xml');
  if FileExists(sFile) then
  begin
    OpenXML(sFile, xmlRoot);
    if FindXMLNodeByKey(xmlRoot, 'Version', '', '', oVersionNode) then
    begin
        Result := oVersionNode.text;                                           
    end;
  end;
end;

//检测安装环境是否缺少补
procedure CheckInstallEnviroment();
var 
  sFile: String;
  nErrorCode: Integer;
begin
  sFile := ExpandConstant('{app}\vc_redist.x64_2015.exe');
  if FileExists(sFile) then
  begin
    if not RegValueExists(HKEY_LOCAL_MACHINE, 'SOFTWARE\Microsoft\VisualStudio\10.0\VC\Runtimes\x64', 'Version1') then
    begin
      ShellExec('', sFile, '/q', '', SW_HIDE, ewWaitUntilTerminated, nErrorCode);
    end; 
  end;
end;

//版本号写入注册表
procedure WriteVersionToReg();
var
  sFile: String;
  sVersion: String;
  xmlRoot: Variant;
  oVersionNode: Variant;
begin
  sFile := ExpandConstant('{app}\bin\BIM5D.xml');
  if FileExists(sFile) then
  begin
    OpenXML(sFile, xmlRoot);
    if FindXMLNodeByKey(xmlRoot, 'Version', '', '', oVersionNode) then
    begin
      sVersion := oVersionNode.text;                                           
      RegWriteStringValue(HKLM64, 'SOFTWARE\Glodon\GBIM',  'Version', sVersion);
    end;
  end;
end;


//安装的时候执行的步骤
procedure CurStepChanged(CurStep: TSetupStep);
var
  sfile: String;
  ErrorCode: Integer;
begin
  if CurStep = ssPostInstall then
  begin
     //判断是否需要安装运行环境
    CheckInstallEnviroment();
  
    //安装时记录安装时间
    //RegWriteDWordValue(HKLM64, 'SOFTWARE\JKStockTool',  'Install', Trunc(Date));

    //程序版本号写入注册表
    //WriteVersionToReg();
	
	//清除setup包
    //DelTree(ExpandConstant('{app}\vc_redist.x64_2015.exe'), True, True, True);
	
  end;
  if CurStep = ssDone then
  begin
    DeleteFile(ExpandConstant('{app}\vc_redist.x64_2015.exe'));
  end;
end;