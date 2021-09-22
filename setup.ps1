param(
	[Alias("Visual-Studio-Code", "VSC")]
	[Parameter(HelpMessage="Download Visual Studio Code (defualt: False)")]
	[switch]$VSCode = $False,
	
	[Alias("Visual-Studio-Community", "VS")]
	[Parameter(HelpMessage="Download Visual Studio Community Edition (default: False)")]
	[switch]$VSCommunity = $False,
	
	[Alias("vcp")]
	[Parameter(HelpMessage="Download vcpkg (default: True)")]
	[switch]$vcpkg = $True,
	
	[Alias("NodeJS")]
	[Parameter(HelpMessage="Download Node.JS (default: True)")]
	[switch]$njs = $True,
	
	
	[Alias("vcpkgInstallPath", "vcpp")]
	[Parameter(HelpMessage="Path to install vcpkg to")]
	[string]$vcpkgPath = "",
	
	[Alias("F")]
	[Parameter(HelpMessage="Force Execution (default: False)")]
	[switch]$Force = $false
)

if (-not $Force){
	if ($VSCode){
		if ($VSCommunity){
			Write-Output "VSCode and VSCommunity cannot be true at the same time, pick one! If you want both pass the force flag.\n"
			Get-Help $MyInvocation.MyCommand.Definition
			exit
		}
	}

	if ($vcpkg){
		if ($vcpkgPath -eq ""){
			Write-Output "Parameter vcpkgPath needs to be specified, if vcpkg is true.\n"
			Get-Help $MyInvocation.MyCommand.Definition
			exit
		}
	}
}

if ($VSCODE){
	Write-Output "Getting VSCode"
	wget https://go.microsoft.com/fwlink/?Linkid=852157 -OutFile VSCode-Installer.exe
	$vscodeins = Start-Process "./VSCode-Installer.exe" -PassThru
	$vscodeins.WaitForExit()
} else {
	Write-Output "Skipping VSCode"
}

if ($VSCommunity){
	Write-Output "Getting Visual Studio Community Edition"
	wget https://download.visualstudio.microsoft.com/download/pr/22c17f05-944c-48dc-9f68-b1663f9df4cb/2492827d2bc58c5cd0a4e4ab40da86910d05ee96f71d69b561457885b3249a91/vs_Community.exe -OutFile VS-Community-Installer.exe
	$vscomins = Start-Process "./VS-Community-Installer.exe" -PassThru
	$vscomins.WaitForExit()
} else {
	Write-Output "Skipping Visual Studio Community Edition"
}

if ($vcpkg){
	Write-Output "Getting vcpkg and installing to $vcpkgPath"
	git clone https://github.com/microsoft/vcpkg $vcpkgPath
	[Environment]::SetEnvironmentVariable("Path", $env:Path + ";$vcpkgPath", "Machine")
	./$vcpkgPath/vcpkg.exe integrate install
}else {
	Write-Output "Skipping vcpkg"
}

if ($njs){
	Write-Output "Getting node.js"
	wget https://nodejs.org/dist/v14.17.6/node-v14.17.6-x64.msi -OutFile nodejs-installer.msi
	$njsins = Start-Process "./nodejs-installer.msi" -PassThru
	$njsins.WaitForExit()
} else {
	Write-Output "Skipping Node.JS"
}

git -C $(get-location) rev-parse
$RC = $LASTEXITCODE
if ($RC -eq 0){
	git checkout origin/dev
	git pull
} else {
	Write-Output "Not in git repo"
}


